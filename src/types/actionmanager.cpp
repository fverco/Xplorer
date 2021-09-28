#include "actionmanager.h"

#include <QProcess>
#include <QUrl>
#include <QDesktopServices>

/*!
 * \brief The constructor
 */
ActionManager::ActionManager()
{
}

/*!
 * \brief Opens a given file.
 * \param file = A QFileInfo variable with the path to the file
 * \return A boolean value stating if the function was successful or not.
 * \note This function will try to open the file with the correct application, but if there is no assigned default application, then this function will still return true and the file will not be opened.
 */
bool ActionManager::openFile(const QFileInfo &file)
{
    if (file.isExecutable()) {
        return openExecutableFile(file);
    } else
        return openReadableFile(file);
}

/*!
 * \brief Opens a readable file (aka a non-executable file like jpg, png, txt etc.).
 * \param file = A QFileInfo variable with the path to the file
 * \return A boolean value stating if the function was successful or not.
 * \note If there is no assosiated application for opening this specific file, then the function will still return true but the file will not be opened.
 */
bool ActionManager::openReadableFile(const QFileInfo &file)
{
    return QDesktopServices::openUrl(QUrl::fromLocalFile(file.path() + "/" + file.fileName()));
}

/*!
 * \brief Opens an executable file (such as exe, run etc.).
 * \param file = A QFileInfo variable with the path to the file
 * \return A boolean value stating if the function was successful or not.
 * \note If the executable cannot be started, then this function will try and open it as a non-executable.
 */
bool ActionManager::openExecutableFile(const QFileInfo &file)
{
    if (!QProcess::startDetached(file.path() + "/" + file.fileName())) {
        return openReadableFile(file);
    }

    return true;
}
