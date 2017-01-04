#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include <QGraphicsRectItem>
#include "globals.h"

class GridSquare : public QGraphicsRectItem
{
public:
//
    GridSquare( qreal x, qreal y, qreal width, int counter, Globals &g, QGraphicsItem * parent = 0 );
    void advance(int phase);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option,
                 QWidget * widget = 0 );

    void updateDensity();

private:
    unsigned long skip_counter;
    Globals &globals;
};

#endif // GRIDSQUARE_H
