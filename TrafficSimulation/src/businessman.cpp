#include "businessman.h"

Businessman::Businessman(PathNode *start, int goal_platform, Globals &g, Train *t, int loc, QGraphicsItem *parent) :
    Person(start, goal_platform, g, t, loc, parent)
{
    colour = Qt::cyan;
}

void Businessman::setSpeed()
{
    Person::setSpeed();
    this->velocity *= 1.5;
}
