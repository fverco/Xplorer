#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "../types/explorersplitter.h"

#include <QSplitter>
#include <QtDebug>
#include <QFileIconProvider>
#include <QMessageBox>
#include <QKeyEvent>
#include <QGroupBox>
#include <QStorageInfo>

/*!
 * \brief The constructor of the main window.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      treeViewMan(),
      explorerMan1(),
      explorerMan2(),
      actionMan(),
      viewSplitter(new ExplorerSplitter(Qt::Horizontal, this)),
      iconProvider(new QFileIconProvider())
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
    viewSplitter.clear();
    iconProvider.clear();
    aboutDialog.clear();
    delete ui;
}

/*!
 * \brief Closes the application.
 */
void MainWindow::closeApp()
{
    viewSplitter.clear();
    iconProvider.clear();
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
                        openDirectoryInExplorer(TreeViewManager::getTreeDirPath(ui->tvFileSys->currentIndex()));
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
 * \brief Opens an instance of the about dialog.
 * \note This also destroys any existing instance of the about dialog.
 */
void MainWindow::openAboutDialog()
{
    aboutDialog.clear();
    aboutDialog = QSharedPointer<AboutDialog>::create(this);
    aboutDialog->show();
}

/*!
 * \brief Initializes the core UI components for the file explorers.
 */
void MainWindow::initializeExplorerUi()
{
    // Add splitter between the file views.
    ui->hLayoutFiles->addWidget(viewSplitter.data());
    viewSplitter->addWidget(ui->gbTreeView);
    viewSplitter->addWidget(ui->gbExplorer1);
    viewSplitter->addWidget(ui->gbExplorer2);

    // Add file models to the file views.
    ui->tvFileSys->setModel(treeViewMan.getFileSystemModel().data());
    ui->tvFileSys->setRootIndex(treeViewMan.currentPathIndex());

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
    treeViewMan.getFileSystemModel()->setIconProvider(iconProvider.data());
    explorerMan1.getFileSystemModel()->setIconProvider(iconProvider.data());
    explorerMan1.getFileSystemModel()->setIconProvider(iconProvider.data());

    // Set fixed width of the back and forward buttons.
    ui->btnBackExplorer1->setFixedWidth(50);
    ui->btnForwardExplorer1->setFixedWidth(50);
    ui->btnBackExplorer2->setFixedWidth(50);
    ui->btnForwardExplorer2->setFixedWidth(50);

    // Connect buttons and actions to their functions.
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::closeApp);
    connect(&explorerMan1, &ExplorerManager::pathChanged, this, &MainWindow::refreshExplorer1);
    connect(&explorerMan2, &ExplorerManager::pathChanged, this, &MainWindow::refreshExplorer2);
    connect(ui->cbDrives, &QComboBox::currentTextChanged, this, &MainWindow::setTreeViewPath);
    connect(&explorerMan1, &ExplorerManager::pathChanged, this, [this](){
        refreshBackAndForwardButtons(explorerMan1, ui->btnBackExplorer1, ui->btnForwardExplorer1);
    });
    connect(&explorerMan2, &ExplorerManager::pathChanged, this, [this](){
        refreshBackAndForwardButtons(explorerMan2, ui->btnBackExplorer2, ui->btnForwardExplorer2);
    });
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openAboutDialog);

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

    // Add important directories to the tree view.
    refreshDriveList();

    // Add icons to buttons.
    ui->btnRefreshDrives->setIcon(QIcon(":/images/refresh.png"));
    ui->btnBackExplorer1->setIcon(QIcon(":/images/left_arrow.png"));
    ui->btnBackExplorer2->setIcon(QIcon(":/images/left_arrow.png"));
    ui->btnForwardExplorer1->setIcon(QIcon(":/images/right_arrow.png"));
    ui->btnForwardExplorer2->setIcon(QIcon(":/images/right_arrow.png"));
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
 * \brief Assigns a new path for the tree view.
 * \param newPath = The new path
 */
void MainWindow::setTreeViewPath(const QString &newPath)
{
    QDir path(newPath);

    if (path.exists()) {
        treeViewMan.setCurrentPath(newPath);
        ui->tvFileSys->setRootIndex(treeViewMan.currentPathIndex());
    } else {
        QMessageBox::warning(this, "Directory not found", "The selected directory no longer exists. Please ensure that the drive is still connected.");
        refreshDriveList();
    }
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
 * \brief Refreshes the list of drives for the tree view.
 * \note This contains preprocessor directives for detecting the OS at compile time.
 */
void MainWindow::refreshDriveList()
{
#if defined (Q_OS_LINUX)
    // Clean up the list.
    ui->cbDrives->clear();

    // Add the common directories.
    ui->cbDrives->addItem("/");
    ui->cbDrives->addItem("/home");
    ui->cbDrives->addItem(QDir::homePath());

    // Add the connected external storage devices.
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            QString path(storage.rootPath());

            if (!storage.isReadOnly() && (path.startsWith("/run/media") || path.startsWith("/run/mnt"))) {
                ui->cbDrives->addItem(path);
            }
        }
    }
#endif
}

/*!
 * \brief When the file system tree view is double clicked, it opens the currently selected folder in the active explorer.
 * \param index = The index of the tree view
 */
void MainWindow::on_tvFileSys_doubleClicked(const QModelIndex &index)
{
    openDirectoryInExplorer(TreeViewManager::getTreeDirPath(index));
}

/*!
 * \brief Refreshes the drive list.
 */
void MainWindow::on_btnRefreshDrives_clicked()
{
    refreshDriveList();
}

