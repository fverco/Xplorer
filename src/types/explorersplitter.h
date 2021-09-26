#ifndef EXPLORERSPLITTER_H
#define EXPLORERSPLITTER_H

#include <QSplitter>

/*!
 * \brief Used to split the explorer views.
 */
class ExplorerSplitter : public QSplitter
{
public:
    ExplorerSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    QSplitterHandle *createHandle() override;
};

#endif // EXPLORERSPLITTER_H
