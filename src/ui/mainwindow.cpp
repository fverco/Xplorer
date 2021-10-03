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
    if (watched == ui->lvExplorer1 && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent(static_cast<QKeyEvent*>(event));
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            openFileIndex(explorerMan1, ui->lvExplorer1->currentIndex());
        } else {
            if (keyEvent->key() == Qt::Key_Backspace && explorerMan1.canUndoPath()) {
                explorerMan1.undoPath();
            }
        }
    }
    else {
        if (watched == ui->lvExplorer2 && event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent(static_cast<QKeyEvent*>(event));
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                openFileIndex(explorerMan2, ui->lvExplorer2->currentIndex());
            } else {
                if (keyEvent->key() == Qt::Key_Backspace && explorerMan2.canUndoPath()) {
                    explorerMan2.undoPath();
                }
            }
        }
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

    // Add event filter to the explorers for handling key presses.
    ui->lvExplorer1->installEventFilter(this);
    ui->lvExplorer2->installEventFilter(this);
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
