#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../types/actionmanager.h"
#include "../types/explorermanager.h"
#include "../types/treeviewmanager.h"
#include "../types/explorersplitter.h"

#include <QMainWindow>
#include <QPair>

class QFileSystemModel;
class QPushButton;
class QGroupBox;
class QListView;
class QFileIconProvider;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void setTreeViewPath(const QString &newPath);
    void on_tvFileSys_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;              ///< The object containing all the ui elements.
    TreeViewManager treeViewMan;     ///< The manager for the tree view.
    ExplorerManager explorerMan1;    ///< The object responsible for managing explorer 1.
    ExplorerManager explorerMan2;    ///< The object responsible for managing explorer 2.
    ActionManager actionMan;         ///< The object responsible for performing actions on files.
    QPair<Explorer, QGroupBox*> activeExplorer;     ///< The explorer currently in focus. The first value is an enum containing the explorer number and the second value is a pointer to the explorer's groupbox.
    QSharedPointer<ExplorerSplitter> viewSplitter;  ///< The line that splits the explorers and tree view.
    QSharedPointer<QFileIconProvider> iconProvider; ///< The object that provides icons to the explorers and tree view.

    void initializeExplorerUi();
    void setActiveExplorer(const Explorer &explorer, QGroupBox *explorerGroupBox);
    void refreshBackAndForwardButtons(const ExplorerManager &explMan, QPushButton *backButton, QPushButton *forwardButton);
    void openFileIndex(ExplorerManager &explMan, const QModelIndex &fileIndex);
    void openDirectoryInExplorer(const QString &path);
    void catchExplorerKeyEvent(ExplorerManager &explMan, QListView *explView, QKeyEvent *keyEvent);
    void refreshDriveList();
};
#endif // MAINWINDOW_H
