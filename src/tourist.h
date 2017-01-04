#ifndef TOURIST_H
#define TOURIST_H

#include "person.h"

class Tourist : public Person
{
public:
    Tourist(PathNode *start, int goal_platform, Globals &g, Train *t,
            int loc = 0, QGraphicsItem * parent = 0 );
    void advance(int phase);

private:
    bool loitering;

    virtual double calculateScoreFixedMode(int i);
    void setSpeed();

};

#endif // TOURIST_H
