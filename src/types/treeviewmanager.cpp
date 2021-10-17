#include "treeviewmanager.h"

/*!
 * \brief The constructor
 * \note This will make the dirModel only display folders.
 */
TreeViewManager::TreeViewManager() :
    dirModel(new QFileSystemModel())
{
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
}

/*!
 * \brief The desctructor
 * \note This function will call clear() on the dirModel.
 * \see QSharedPointer::clear()
 */
TreeViewManager::~TreeViewManager()
{
    dirModel.clear();
}

/*!
 * \brief Provides a QSharedPointer of the tree view's file model.
 * \return A QSharedPointer of a QFileSystemModel
 */
QSharedPointer<QFileSystemModel> TreeViewManager::getFileSystemModel() const
{
    return dirModel;
}

/*!
 * \brief Provides the current path the dirModel is showing.
 * \return A QString value of the current path
 */
QString TreeViewManager::currentPath() const
{
    return dirModel->rootPath();
}

/*!
 * \brief Provides the current path index of the dirModel.
 * \return A QModelIndex of the current path index.
 */
QModelIndex TreeViewManager::currentPathIndex() const
{
    return dirModel->index(dirModel->rootPath());
}

/*!
 * \brief Sets a new path of the dirModel.
 * \param newPath = The new path
 */
void TreeViewManager::setCurrentPath(const QString &newPath)
{
    dirModel->setRootPath(newPath);
}

/*!
 * \brief Retrieves the full path to the given directory index.
 * \param dirIndex = The directory index from the tree view
 * \return A QString with the full path
 * \note This contains preprocessor directives for detecting the OS at compile time.
 */
QString TreeViewManager::getTreeDirPath(const QModelIndex &dirIndex)
{
#if defined (Q_OS_WIN)
    QModelIndex parent(dirIndex.parent()), nextParent(parent.parent());
    QString path(dirIndex.data().toString()), lastParentString("");

    while (nextParent.isValid()) {
        path.prepend(parent.data().toString() + "/");
        parent = parent.parent();
        nextParent = parent.parent();
    }

    lastParentString = parent.data().toString();

    QStringView driveLetter(lastParentString);
    driveLetter = driveLetter.sliced(driveLetter.lastIndexOf(':') - 1, 2);
    path.prepend(driveLetter.toString() + "/");

    return path;
#endif

#if defined (Q_OS_LINUX)
    QModelIndex parent(dirIndex.parent());
    QString path(dirIndex.data().toString());

    while(parent.isValid()) {
        path.prepend(parent.data().toString() + "/");
        parent = parent.parent();
    }

    path = path.remove(0, 1);

    return path;
#endif
}
