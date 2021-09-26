#ifndef EXPLORERSPLITTERHANDLE_H
#define EXPLORERSPLITTERHANDLE_H

#include <QSplitterHandle>

/*!
 * \brief The handle used to adjust the size of the views next to the splitter.
 */
class ExplorerSplitterHandle : public QSplitterHandle
{
public:
    ExplorerSplitterHandle(Qt::Orientation orientation, QSplitter *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // EXPLORERSPLITTERHANDLE_H
