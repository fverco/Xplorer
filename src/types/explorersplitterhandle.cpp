#include "explorersplitterhandle.h"

#include <QPainter>
#include <QGradient>
#include <QPaintEvent>

/*!
 * \brief The constructor.
 * \param orientation = The orientation of the handle
 * \param parent = The splitter to which this handle is bound
 */
ExplorerSplitterHandle::ExplorerSplitterHandle(Qt::Orientation orientation, QSplitter *parent) :
    QSplitterHandle(orientation, parent)
{
}

/*!
 * \brief Gives the handle its look and feel.
 * \param event = Contains event parameters.
 */
void ExplorerSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient gradient;

    if (orientation() == Qt::Horizontal) {
        gradient.setStart(rect().left(), rect().height()/2);
        gradient.setFinalStop(rect().right(), rect().height()/2);
    } else {
        gradient.setStart(rect().width()/2, rect().top());
        gradient.setFinalStop(rect().width()/2, rect().bottom());
    }

    painter.fillRect(event->rect(), QBrush(gradient));
}
