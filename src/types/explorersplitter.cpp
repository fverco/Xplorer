#include "explorersplitter.h"
#include "explorersplitterhandle.h"

/*!
 * \brief The constructor.
 * \param orientation = The orientation of the splitter
 * \param parent = The widget to which the splitter is bound
 */
ExplorerSplitter::ExplorerSplitter(Qt::Orientation orientation, QWidget *parent) :
    QSplitter(orientation, parent)
{
}

/*!
 * \brief Creates a splitter handle and returns it.
 * \return A QSplitterHandle
 */
QSplitterHandle *ExplorerSplitter::createHandle()
{
    return new ExplorerSplitterHandle(orientation(), this);
}
