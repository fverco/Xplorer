#ifndef OPENDIRCOMMAND_H
#define OPENDIRCOMMAND_H

#include <QUndoCommand>
#include <QFileSystemModel>
#include <QSharedPointer>

/*!
 * \brief The command class for opening a directory path.
 */
class OpenDirCommand : public QUndoCommand
{
public:
    enum {Id = 1122};

    OpenDirCommand(const QString &nDir, const QString &oDir, QSharedPointer<QFileSystemModel> fileSystemModel, QUndoCommand *parent = nullptr);
    ~OpenDirCommand();

    void undo() override;
    void redo() override;
    int id() const override;

private:
    QString newDir; ///< The new directory path to be opened.
    QString oldDir; ///< The old directory path before opening the new one.
    QSharedPointer<QFileSystemModel> fileModel; ///< The file system model on which the command should be performed.
};

#endif // OPENDIRCOMMAND_H
