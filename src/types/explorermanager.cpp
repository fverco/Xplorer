#include "explorermanager.h"
#include "opendircommand.h"

#include <QAction>

/*!
 * \brief The constructor
 * \param parent = The QObject to which this object is bound
 * \note This will initialize fileModel with the home path as its current path and will display all files and folders as well as the "DotDot" entry.
 */
ExplorerManager::ExplorerManager(QObject *parent) :
    QObject(parent),
    fileModel(new QFileSystemModel()),
    dirHistoryStack(new QUndoStack())
{
    fileModel->setRootPath(QDir::homePath());
    fileModel->setFilter(QDir::AllEntries | QDir::NoDot);

    undoPathAction = dirHistoryStack->createUndoAction(this, "&Undo");
    redoPathAction = dirHistoryStack->createRedoAction(this, "&Redo");

    connect(undoPathAction, &QAction::triggered, this, [this](){
        emit pathChanged();
    });
    connect(redoPathAction, &QAction::triggered, this, [this](){
        emit pathChanged();
    });
}

/*!
 * \brief The destructor
 * \note This function will call clear() on the fileModel.
 * \see QSharedPointer::clear()
 */
ExplorerManager::~ExplorerManager()
{
    fileModel.clear();
}

/*!
 * \brief Provides a QSharedPointer of the explorer's file model.
 * \return A QSharedPointer of a QFileSystemModel
 */
QSharedPointer<QFileSystemModel> ExplorerManager::getFileSystemModel() const
{
    return fileModel;
}

/*!
 * \brief Provides the current path the fileModel is showing.
 * \return A QString value of the current path
 */
QString ExplorerManager::currentPath() const
{
    return fileModel->rootPath();
}

/*!
 * \brief Provides the current path index of the fileModel.
 * \return A QModelIndex of the current path index.
 */
QModelIndex ExplorerManager::currentPathIndex() const
{
    return fileModel->index(fileModel->rootPath());
}

/*!
 * \brief Sets a new path of the fileModel.
 * \param newPath = The new path
 */
void ExplorerManager::setCurrentPath(const QString &newPath)
{
    dirHistoryStack->push(new OpenDirCommand(newPath, currentPath(), fileModel));
    emit pathChanged();
}

/*!
 * \brief This function will undo the last setCurrentPath() call.
 * \see setCurrentPath()
 */
void ExplorerManager::undoPath()
{
    undoPathAction->trigger();
}

/*!
 * \brief This function will redo the setCurrentPath() call that was previously undone by undoPath().
 * \see setCurrentPath()
 * \see undoPath()
 */
void ExplorerManager::redoPath()
{
    redoPathAction->trigger();
}
