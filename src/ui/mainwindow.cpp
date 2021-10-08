#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../types/explorersplitter.h"

#include <QSplitter>
#include <QtDebug>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QGroupBox>

/*!
 * \brief The constructor of the main window.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      dirModel(new QFileSystemModel(this)),
      explorerMan1(),
      explorerMan2(),
      actionMan()
{
    ui->setupUi(this);
    initializeExplorerUi();
    setActiveExplorer(Explorer::Explorer1, ui->gbExplorer1);
}

/*!
 * \brief The desctructor of the main window.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Closes the application.
 */
void MainWindow::closeApp()
{
    this->close();
}

/*!
 * \brief Catches the key press events of both explorers' QListViews and calls functions.
 * \param watched = The list view being watched
 * \param event = The event that was caught
 * \return A Boolean that is true if the event is stopped from being passed on to the watched object, and false if it is not.
 */
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    switch(event->type()) {
    case QEvent::KeyPress:
        if (watched == ui->lvExplorer1) {
            QKeyEvent *keyEvent(static_cast<QKeyEvent*>(event));
            catchExplorerKeyEvent(explorerMan1, ui->lvExplorer1, keyEvent);
        }
        else {
            if (watched == ui->lvExplorer2) {
                QKeyEvent *keyEvent(static_cast<QKeyEvent*>(event));
                catchExplorerKeyEvent(explorerMan2, ui->lvExplorer2, keyEvent);
            } else {
                if (watched == ui->tvFileSys) {
                    QKeyEvent *keyEvent(static_cast<QKeyEvent*>(event));
                    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                        openDirectoryInExplorer(getTreeDirPath(ui->tvFileSys->currentIndex()));
                    }
                }
            }
        }
        break;

    case QEvent::FocusIn:
        if (ui->gbExplorer1 == watched || ui->gbExplorer1->children().contains(watched)) {
            setActiveExplorer(Explorer::Explorer1, ui->gbExplorer1);
        } else {
            if (ui->gbExplorer2 == watched || ui->gbExplorer2->children().contains(watched)) {
                setActiveExplorer(Explorer::Explorer2, ui->gbExplorer2);
            }
        }
        break;

    default:
        return false;
    }

    return false;
}

/*!
 * \brief Initializes the core UI components for the file explorers.
 */
void MainWindow::initializeExplorerUi()
{
    // Add splitter between the file views.
    ExplorerSplitter *viewSplitter(new ExplorerSplitter(Qt::Horizontal, this));
    ui->hLayoutFiles->addWidget(viewSplitter);
    viewSplitter->addWidget(ui->tvFileSys);
    viewSplitter->addWidget(ui->gbExplorer1);
    viewSplitter->addWidget(ui->gbExplorer2);

    // Add file models to the file views.
    dirModel->setRootPath(QDir::homePath());
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->tvFileSys->setModel(dirModel);
    ui->tvFileSys->setRootIndex(dirModel->index(QDir::homePath()));

    ui->lvExplorer1->setModel(explorerMan1.getFileSystemModel().data());
    ui->lvExplorer1->setRootIndex(explorerMan1.currentPathIndex());

    ui->lvExplorer2->setModel(explorerMan2.getFileSystemModel().data());
    ui->lvExplorer2->setRootIndex(explorerMan2.currentPathIndex());

    // Assign current directory of each explorer to their combo boxes.
    ui->cbPathExplorer1->addItem(explorerMan1.currentPath());
    connect(explorerMan1.getFileSystemModel().data(), &QFileSystemModel::rootPathChanged, ui->cbPathExplorer1, [this](){
        ui->cbPathExplorer1->setCurrentText(explorerMan1.currentPath());
    });

    ui->cbPathExplorer2->addItem(explorerMan2.currentPath());
    connect(explorerMan2.getFileSystemModel().data(), &QFileSystemModel::rootPathChanged, ui->cbPathExplorer2, [this](){
        ui->cbPathExplorer2->setCurrentText(explorerMan2.currentPath());
    });

    // Assign a file icon provider to each model.
    QFileIconProvider *iconProvider(new QFileIconProvider());
    dirModel->setIconProvider(iconProvider);
    explorerMan1.getFileSystemModel()->setIconProvider(iconProvider);
    explorerMan1.getFileSystemModel()->setIconProvider(iconProvider);

    // Set fixed width of the back and forward buttons.
    ui->btnBackExplorer1->setFixedWidth(50);
    ui->btnForwardExplorer1->setFixedWidth(50);
    ui->btnBackExplorer2->setFixedWidth(50);
    ui->btnForwardExplorer2->setFixedWidth(50);

    // Connect buttons and actions to their functions.
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::closeApp);
    connect(&explorerMan1, &ExplorerManager::pathChanged, this, &MainWindow::refreshExplorer1);
    connect(&explorerMan2, &ExplorerManager::pathChanged, this, &MainWindow::refreshExplorer2);
    connect(&explorerMan1, &ExplorerManager::pathChanged, this, [this](){
        refreshBackAndForwardButtons(explorerMan1, ui->btnBackExplorer1, ui->btnForwardExplorer1);
    });
    connect(&explorerMan2, &ExplorerManager::pathChanged, this, [this](){
        refreshBackAndForwardButtons(explorerMan2, ui->btnBackExplorer2, ui->btnForwardExplorer2);
    });

    // Add event filter to the explorer group boxes and everything in it for handling key presses.
    ui->gbExplorer1->installEventFilter(this);

    for (int i(0); i < ui->gbExplorer1->children().count(); ++i) {
        ui->gbExplorer1->children().at(i)->installEventFilter(this);
    }

    ui->gbExplorer2->installEventFilter(this);

    for (int i(0); i < ui->gbExplorer2->children().count(); ++i) {
        ui->gbExplorer2->children().at(i)->installEventFilter(this);
    }

    ui->tvFileSys->installEventFilter(this);
}

/*!
 * \brief Gives the explorer the appearance of having active focus.
 * \param explorerGroupBox = The group box that contains all the components of the explorer.
 * \note You should only use a QGroupBox that encapsulates the components of the explorer.
 */
void MainWindow::setActiveExplorer(const Explorer &explorer, QGroupBox *explorerGroupBox)
{
    if (activeExplorer.second != nullptr) {
        // Make the current active explorer inactive.
        activeExplorer.second->setStyleSheet("");
    }

    // Set the style of the new active explorer.
    explorerGroupBox->setStyleSheet("QGroupBox { "
                                    "    border: 2px solid gray; "
                                    "    border-radius: 3px; "
                                    "    margin-top: 1em; "
                                    "} "

                                    "QGroupBox::title { "
                                    "   background-color: transparent; "
                                    "   subcontrol-position: top left; "
                                    "   padding:2 13px; "
                                    "   padding-top: -36px; "
                                    "}");

    // Assign the new active explorer.
    activeExplorer.first = explorer;
    activeExplorer.second = explorerGroupBox;
}

/*!
 * \brief Function executed when a file/folder in Explorer 1 is double clicked. This will call the openFileIndex() function.
 * \param index = The index of the file/folder
 * \see openFileIndex()
 */
void MainWindow::on_lvExplorer1_doubleClicked(const QModelIndex &index)
{
    openFileIndex(explorerMan1, index);
}

/*!
 * \brief Function executed when a file/folder in Explorer 2 is double clicked. This will call the openFileIndex() function.
 * \param index = The index of the file/folder
 * \see openFileIndex()
 */
void MainWindow::on_lvExplorer2_doubleClicked(const QModelIndex &index)
{
    openFileIndex(explorerMan2, index);
}

/*!
 * \brief Performs an undo action when the back button in Explorer 1 is clicked.
 */
void MainWindow::on_btnBackExplorer1_clicked()
{
    explorerMan1.undoPath();
}

/*!
 * \brief Performs a redo action when the forward button in Explorer 1 is clicked.
 */
void MainWindow::on_btnForwardExplorer1_clicked()
{
    explorerMan1.redoPath();
}

/*!
 * \brief Performs an undo action when the back button in Explorer 2 is clicked.
 */
void MainWindow::on_btnBackExplorer2_clicked()
{
    explorerMan2.undoPath();
}

/*!
 * \brief Performs a redo action when the forward button in Explorer 2 is clicked.
 */
void MainWindow::on_btnForwardExplorer2_clicked()
{
    explorerMan2.redoPath();
}

/*!
 * \brief Refreshes the file view of Explorer 1.
 */
void MainWindow::refreshExplorer1()
{
    ui->lvExplorer1->setRootIndex(explorerMan1.currentPathIndex());
}

/*!
 * \brief Refreshes the file view of Explorer 2.
 */
void MainWindow::refreshExplorer2()
{
    ui->lvExplorer2->setRootIndex(explorerMan2.currentPathIndex());
}

/*!
 * \brief Enables/disables the back and forward buttons if there is a possibility to undo and/or redo.
 * \param explMan = The explorer manager from which the information will be obtained
 * \param backButton = The back button in the explorer
 * \param forwardButton = The forward button in the explorer
 */
void MainWindow::refreshBackAndForwardButtons(const ExplorerManager &explMan, QPushButton *backButton, QPushButton *forwardButton)
{
    if (explMan.canUndoPath() && !backButton->isEnabled()) {
        backButton->setEnabled(true);
    } else {
        if (!explMan.canUndoPath() && backButton->isEnabled()) {
            backButton->setEnabled(false);
        }
    }

    if (explMan.canRedoPath() && !forwardButton->isEnabled()) {
        forwardButton->setEnabled(true);
    } else {
        if (!explMan.canRedoPath() && forwardButton->isEnabled()) {
            forwardButton->setEnabled(false);
        }
    }
}

/*!
 * \brief Opens the file/folder currently selected by the given explorer.
 * \param explMan = The explorer
 * \param fileIndex = The index of the selected file/folder
 */
void MainWindow::openFileIndex(ExplorerManager &explMan, const QModelIndex &fileIndex)
{
    QFileInfo newDir(explMan.currentPath() + "/" + fileIndex.data().toString());

    if (newDir.isDir()) {
        explMan.setCurrentPath(newDir.path() + "/" + newDir.fileName());
    } else {
        if (!actionMan.openFile(newDir)) {
            QMessageBox::critical(this, "File Error", "An unknown error occurred while trying to open the file.");
        }
    }
}

/*!
 * \brief Opens the given path in the active explorer.
 * \param path = The directory path
 */
void MainWindow::openDirectoryInExplorer(const QString &path)
{
    QFileInfo newDir(path);

    if (newDir.isDir()) {
        switch(activeExplorer.first) {
        case Explorer::Explorer1:
            explorerMan1.setCurrentPath(path);
            break;

        case Explorer::Explorer2:
            explorerMan2.setCurrentPath(path);
            break;

        default:
            break;
        }
    }
}

/*!
 * \brief Retrieves a full path to the given directory index.
 * \param dirIndex = The directory index from the tree view
 * \return A QString with the full path
 */
QString MainWindow::getTreeDirPath(const QModelIndex &dirIndex)
{
    QModelIndex parent(dirIndex.parent());
    QString path(dirIndex.data().toString());

    while(parent.isValid()) {
        path.prepend(parent.data().toString() + "/");
        parent = parent.parent();
    }

    path = path.remove(0, 1);

    return path;
}

/*!
 * \brief Catches a key event for an explorer and performs their appropriate actions.
 * \param explMan = The explorer manager
 * \param explView = The list view of the explorer
 * \param keyEvent = The event containing the key pressed
 */
void MainWindow::catchExplorerKeyEvent(ExplorerManager &explMan, QListView *explView, QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
        openFileIndex(explMan, explView->currentIndex());
    } else {
        if (keyEvent->key() == Qt::Key_Backspace && explMan.canUndoPath()) {
            explMan.undoPath();
        }
    }
}

/*!
 * \brief When the file system tree view is double clicked, it opens the currently selected folder in the active explorer.
 * \param index = The index of the tree view
 */
void MainWindow::on_tvFileSys_doubleClicked(const QModelIndex &index)
{
    openDirectoryInExplorer(getTreeDirPath(index));
}

