#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSplitter>
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QtDebug>

/*!
 * \brief The constructor of the main window.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      fileModel(new QFileSystemModel(this))
{
    ui->setupUi(this);

    // Add splitter between the file views.
    QSplitter *viewSplitter(new QSplitter(this));
    ui->hLayoutFiles->addWidget(viewSplitter);
    viewSplitter->addWidget(ui->tvFileSys);
    viewSplitter->addWidget(ui->gbExplorer1);
    viewSplitter->addWidget(ui->gbExplorer2);

    // Add file model to the file views.
    fileModel->setRootPath(QDir::homePath());
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->tvFileSys->setModel(fileModel);
    ui->tvFileSys->setRootIndex(fileModel->index(QDir::homePath()));
    ui->lvExplorer1->setModel(fileModel);
    ui->lvExplorer1->setRootIndex(fileModel->index(QDir::homePath()));
    ui->lvExplorer2->setModel(fileModel);
    ui->lvExplorer2->setRootIndex(fileModel->index(QDir::homePath()));

}

/*!
 * \brief The desctructor of the main window.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

