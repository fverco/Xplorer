#include "explorermanager.h"

/*!
 * \brief The constructor
 * \note This will initialize fileModel with the home path as its current path and will display all files and folders as well as the "DotDot" entry.
 */
ExplorerManager::ExplorerManager() :
    fileModel(new QFileSystemModel()),
    dirHistory()
{
    fileModel->setRootPath(QDir::homePath());
    fileModel->setFilter(QDir::AllEntries | QDir::NoDot);
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
    fileModel->setRootPath(newPath);
}
