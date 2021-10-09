#ifndef TREEVIEWMANAGER_H
#define TREEVIEWMANAGER_H

#include <QFileSystemModel>
#include <QSharedPointer>

/*!
 * \brief The class responsible for managing data shown in the tree view.
 */
class TreeViewManager
{
public:
    TreeViewManager();
    ~TreeViewManager();
    QSharedPointer<QFileSystemModel> getFileSystemModel() const;
    QString currentPath() const;
    QModelIndex currentPathIndex() const;
    void setCurrentPath(const QString &newPath);
    static QString getTreeDirPath(const QModelIndex &dirIndex);

private:
    QSharedPointer<QFileSystemModel> dirModel;  ///< The tree view's file model.
};

#endif // TREEVIEWMANAGER_H
