#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../types/actionmanager.h"
#include "../types/explorermanager.h"

#include <QMainWindow>
#include <QPair>

class QFileSystemModel;
class QPushButton;
class QGroupBox;
class QListView;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum OS {
    Unknown,    ///< 0
    Windows,    ///< 1
    Linux       ///< 2
};

enum Explorer {
    Explorer1,  ///< 0
    Explorer2   ///< 1
};

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
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_lvExplorer1_doubleClicked(const QModelIndex &index);
    void on_lvExplorer2_doubleClicked(const QModelIndex &index);
    void on_btnBackExplorer1_clicked();
    void on_btnForwardExplorer1_clicked();
    void on_btnBackExplorer2_clicked();
    void on_btnForwardExplorer2_clicked();
    void refreshExplorer1();
    void refreshExplorer2();
    void on_tvFileSys_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;              ///< The object containing all the ui elements.
    QFileSystemModel *dirModel;      ///< The tree view's directory model.
    ExplorerManager explorerMan1;    ///< The object responsible for managing explorer 1.
    ExplorerManager explorerMan2;    ///< The object responsible for managing explorer 2.
    ActionManager actionMan;         ///< The object responsible for performing actions on files.
    OS systemOs;
    QPair<Explorer, QGroupBox*> activeExplorer; ///< The explorer currently in focus. The first value is an enum containing the explorer number and the second value is a pointer to the explorer's groupbox.

    void initializeExplorerUi();
    void setActiveExplorer(const Explorer &explorer, QGroupBox *explorerGroupBox);
    void refreshBackAndForwardButtons(const ExplorerManager &explMan, QPushButton *backButton, QPushButton *forwardButton);
    void openFileIndex(ExplorerManager &explMan, const QModelIndex &fileIndex);
    void openDirectoryInExplorer(const QString &path);
    QString getTreeDirPath(const QModelIndex &dirIndex);
    QString getWindowsDirPath(const QModelIndex &dirIndex);
    QString getLinuxDirPath(const QModelIndex &dirIndex);
    void catchExplorerKeyEvent(ExplorerManager &explMan, QListView *explView, QKeyEvent *keyEvent);
    OS getSystemOs();
};
#endif // MAINWINDOW_H
