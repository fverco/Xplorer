#include "opendircommand.h"

/*!
 * \brief The constructor
 * \param nDir = The new directory path
 * \param oDir = The previous directory path
 * \param fileSystemModel = The file system model to which the changes will be applied
 * \param parent = The QUndoCommand to which this command is bound
 */
OpenDirCommand::OpenDirCommand(const QString &nDir,
                               const QString &oDir,
                               QSharedPointer<QFileSystemModel> fileSystemModel,
                               QUndoCommand *parent) :
    QUndoCommand(parent),
    newDir(nDir),
    oldDir(oDir),
    fileModel(fileSystemModel)
{
}

/*!
 * \brief The destructor
 */
OpenDirCommand::~OpenDirCommand()
{
}

/*!
 * \brief Reverts the directory path back to before this command was performed.
 */
void OpenDirCommand::undo()
{
    fileModel->setRootPath(oldDir);
}

/*!
 * \brief Performs this command again.
 */
void OpenDirCommand::redo()
{
    fileModel->setRootPath(newDir);
}

/*!
 * \brief Provides the command type ID of this command.
 * \return This always returns the integer '1122'.
 */
int OpenDirCommand::id() const
{
    return Id;
}
