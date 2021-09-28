#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../types/actionmanager.h"

#include <QMainWindow>

class QFileSystemModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The main window of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeApp();

private slots:
    void on_lvExplorer1_doubleClicked(const QModelIndex &index);
    void on_lvExplorer2_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;              ///< The object containing all the ui elements.
    QFileSystemModel *dirModel;      ///< The tree view's directory model.
    QFileSystemModel *fileModel1;    ///< Explorer 1's file model.
    QFileSystemModel *fileModel2;    ///< Explorer 2's file model.
    ActionManager actionMan;         ///< The object responsible for performing actions on files.

    void initializeExplorerUi();
};
#endif // MAINWINDOW_H
