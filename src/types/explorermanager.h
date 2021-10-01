#ifndef EXPLORERMANAGER_H
#define EXPLORERMANAGER_H

#include <QFileSystemModel>
#include <QSharedPointer>

class ExplorerManager
{
public:
    ExplorerManager();
    ~ExplorerManager();
    QSharedPointer<QFileSystemModel> getFileSystemModel() const;
    QString currentPath() const;
    QModelIndex currentPathIndex() const;
    void setCurrentPath(const QString &newPath);

private:
    QSharedPointer<QFileSystemModel> fileModel;    ///< The explorer's file model.
    QList<QString> dirHistory;                     ///< The explorer's directory access history.
};

#endif // EXPLORERMANAGER_H
