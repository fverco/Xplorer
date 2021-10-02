#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../types/actionmanager.h"
#include "../types/explorermanager.h"

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
    void on_btnBackExplorer1_clicked();
    void on_btnForwardExplorer1_clicked();
    void on_btnBackExplorer2_clicked();
    void on_btnForwardExplorer2_clicked();
    void refreshExplorer1();
    void refreshExplorer2();

private:
    Ui::MainWindow *ui;              ///< The object containing all the ui elements.
    QFileSystemModel *dirModel;      ///< The tree view's directory model.
    ExplorerManager explorerMan1;    ///< The object responsible for managing explorer 1.
    ExplorerManager explorerMan2;    ///< The object responsible for managing explorer 2.
    ActionManager actionMan;         ///< The object responsible for performing actions on files.

    void initializeExplorerUi();
};
#endif // MAINWINDOW_H
