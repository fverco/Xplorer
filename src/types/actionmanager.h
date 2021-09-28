#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QString>
#include <QFileInfo>

/*!
 * \brief The class responsible for all actions applied to files.
 */
class ActionManager
{
public:
    ActionManager();
    bool openFile(const QFileInfo &file);

private:
    bool openReadableFile(const QFileInfo &file);
    bool openExecutableFile(const QFileInfo &file);
};

#endif // ACTIONMANAGER_H
