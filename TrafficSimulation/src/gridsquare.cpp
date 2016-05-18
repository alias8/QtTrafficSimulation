#include "gridsquare.h"
#include "person.h"
#include <QDebug>
#include <QPainter>

GridSquare::GridSquare(qreal x, qreal y, qreal width, int counter, Globals &g, QGraphicsItem *parent) :
    QGraphicsRectItem (  0,  0,  width,  width, parent ),
    globals(g)
{
    this->setPos(x, y);
    skip_counter = counter;
}

void GridSquare::advance(int phase)
{
    if(!phase) {
        if(skip_counter % globals.density_check == 0) { // every 20th grid square is tested for colliding items, then its neighbour is checked
            updateDensity();
        }
        skip_counter++;
    }
}

void GridSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(globals.DEBUG_DISPLAY_GRIDSQUARES) QGraphicsRectItem::paint(painter, option, widget);
}

void GridSquare::updateDensity()
{
    QList<QGraphicsItem *> list = this->collidingItems();
    int size = list.size();
    foreach(QGraphicsItem *item, list) {
        if (dynamic_cast<Person*>(item) != NULL)
        {
            dynamic_cast<Person*>(item)->setDensity(size);
            dynamic_cast<Person*>(item)->setSpeed();
        }
    }
}
