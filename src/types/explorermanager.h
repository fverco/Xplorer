#ifndef EXPLORERMANAGER_H
#define EXPLORERMANAGER_H

#include <QFileSystemModel>
#include <QSharedPointer>
#include <QObject>
#include <QUndoStack>

/*!
 * \brief The class responsible for managing data shown in the file explorer.
 */
class ExplorerManager : public QObject
{
    Q_OBJECT

public:
    explicit ExplorerManager(QObject *parent = nullptr);
    ~ExplorerManager();
    QSharedPointer<QFileSystemModel> getFileSystemModel() const;
    QString currentPath() const;
    QModelIndex currentPathIndex() const;
    void setCurrentPath(const QString &newPath);
    void undoPath() const;
    void redoPath() const;
    bool canUndoPath() const;
    bool canRedoPath() const;

signals:
    void pathChanged(); ///< Emitted when the directory path is changed.

private:
    QSharedPointer<QFileSystemModel> fileModel;     ///< The explorer's file model.
    QSharedPointer<QUndoStack> dirHistoryStack;     ///< The explorer's directory access history.
    QAction *undoPathAction;    ///< Triggers the explorer to undo its last path change.
    QAction *redoPathAction;    ///< Triggers the explorer to redo its previously undone path change.
};

#endif // EXPLORERMANAGER_H
