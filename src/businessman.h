#ifndef BUSINESSMAN_H
#define BUSINESSMAN_H

#include "person.h"

class Businessman : public Person
{
public:
    Businessman(PathNode *start, int goal_platform, Globals &g, Train *t,
                int loc = 0, QGraphicsItem * parent = 0 );

private:
    void setSpeed();

};

#endif // BUSINESSMAN_H
