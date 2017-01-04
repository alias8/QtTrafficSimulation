#include "tourist.h"

Tourist::Tourist(PathNode *start, int goal_platform, Globals &g, Train *t, int loc, QGraphicsItem *parent) :
    Person(start, goal_platform, g, t, loc, parent),
    loitering(false)
{
    colour = Qt::yellow;
    score_check_frequency = 50;
}

void Tourist::advance(int phase)
{
//    Person::advance(phase);
    if(!phase && !loitering) {
        if(qrand()%100 == 0) {
            loitering = true;
        }
        else {
            Person::advance(phase);
        }
    }
    else if(!phase && loitering) {
        if(qrand()%50 == 0) {
            loitering = false;
        }
    }
    else if(phase && !loitering) {
        Person::advance(phase);
    }

}

double Tourist::calculateScoreFixedMode(int i)
{
    return Person::calculateScoreFixedMode(i) * (1 + (qrand()%200 - 100)/100.0);
}

void Tourist::setSpeed()
{
    Person::setSpeed();
    this->velocity *= 0.7;
}
