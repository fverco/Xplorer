#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../types/explorersplitter.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QtDebug>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QMessageBox>

/*!
 * \brief The constructor of the main window.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      dirModel(new QFileSystemModel(this)),
      fileModel1(new QFileSystemModel(this)),
      fileModel2(new QFileSystemModel(this)),
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

    fileModel1->setRootPath(QDir::homePath());
    fileModel1->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->lvExplorer1->setModel(fileModel1);
    ui->lvExplorer1->setRootIndex(fileModel1->index(QDir::homePath()));

    fileModel2->setRootPath(QDir::homePath());
    fileModel2->setFilter(QDir::AllEntries | QDir::NoDot);
    ui->lvExplorer2->setModel(fileModel2);
    ui->lvExplorer2->setRootIndex(fileModel2->index(QDir::homePath()));

    // Assign current directory of each explorer to their combo boxes.
    ui->cbPathExplorer1->addItem(fileModel1->rootPath());
    connect(fileModel1, &QFileSystemModel::rootPathChanged, ui->cbPathExplorer1, [this](){
        ui->cbPathExplorer1->setCurrentText(fileModel1->rootPath());
    });

    ui->cbPathExplorer2->addItem(fileModel2->rootPath());
    connect(fileModel2, &QFileSystemModel::rootPathChanged, ui->cbPathExplorer2, [this](){
        ui->cbPathExplorer2->setCurrentText(fileModel2->rootPath());
    });

    // Assign a file icon provider to each model.
    QFileIconProvider *iconProvider(new QFileIconProvider());
    dirModel->setIconProvider(iconProvider);
    fileModel1->setIconProvider(iconProvider);
    fileModel2->setIconProvider(iconProvider);

    // Set fixed width of the back and forward buttons.
    ui->btnBackExplorer1->setFixedWidth(50);
    ui->btnForwardExplorer1->setFixedWidth(50);
    ui->btnBackExplorer2->setFixedWidth(50);
    ui->btnForwardExplorer2->setFixedWidth(50);

    // Connect standard buttons to their functions.
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::closeApp);
}

/*!
 * \brief Function executed when a file/folder in Explorer 1 is double clicked.
 * \param index = The index of the file/folder
 */
void MainWindow::on_lvExplorer1_doubleClicked(const QModelIndex &index)
{
    QFileInfo newDir(fileModel1->rootPath() + "/" + index.data().toString());

    if (newDir.isDir()) {
        fileModel1->setRootPath(newDir.path() + "/" + newDir.fileName());
        ui->lvExplorer1->setRootIndex(fileModel1->index(fileModel1->rootPath()));
    } else {
        if (!actionMan.openFile(newDir)) {
            QMessageBox::critical(this, "File Error", "An unknown error occurred while trying to open the file.");
        }
    }
}

/*!
 * \brief Function executed when a file/folder in Explorer 2 is double clicked.
 * \param index = The index of the file/folder
 */
void MainWindow::on_lvExplorer2_doubleClicked(const QModelIndex &index)
{
    QFileInfo newDir(fileModel2->rootPath() + "/" + index.data().toString());

    if (newDir.isDir()) {
        fileModel2->setRootPath(newDir.path() + "/" + newDir.fileName());
        ui->lvExplorer2->setRootIndex(fileModel2->index(fileModel2->rootPath()));
    } else {
        if (!actionMan.openFile(newDir)) {
            QMessageBox::critical(this, "File Error", "An unknown error occurred while trying to open the file.");
        }
    }
}

