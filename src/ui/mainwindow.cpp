#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSplitter *viewSplitter(new QSplitter(this));
    ui->hLayoutFiles->addWidget(viewSplitter);
    viewSplitter->addWidget(ui->tvFileSys);
    viewSplitter->addWidget(ui->lvCurFolder);
}

MainWindow::~MainWindow()
{
    delete ui;
}

