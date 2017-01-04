#ifndef PERSON_H
#define PERSON_H
#include <QGraphicsEllipseItem>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QGraphicsScene>
#include <QPainter>
#include "pathnode.h"
#include "globals.h"
#include "train.h"
#include "simulationcontroller.h"

class SimulationController;

class Person : public QGraphicsEllipseItem
{
public:
    Person(PathNode *start, int goal_platform, Globals &g, Train *t,
           int loc = 0, bool isExperiment = false, QGraphicsItem * parent = 0);
    ~Person();
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option,
                 QWidget * widget = 0 );
    void advance(int phase);
    QRectF boundingRect() const;

    static Person *spawn(PathNode *start, int goal_platform, Globals &g,
                         int loc = 0, Train *t = NULL, QGraphicsItem * parent = 0);
    void setDensity(int value);
    PathNode *getCurrent_destination() const;
    PathNode *getPrevious_destination() const;
    void setExiting(bool value);
    bool getExperiment() const;

    bool operator==(const Person & other) const;

    int getLocation() const;
    void setLocation(int value);
    virtual void setSpeed();

    int getFinal_destination() const;

    bool getWaiting() const;

    bool getSteadyOnPlatform() const;

protected:
    bool stopped;
    bool waiting;
    double speed;
    double speed_variation;
    double goal_distance;
    int final_destination;
    QString finalDestinationString;

    QColor colour;
    int density;
    QPointF velocity;

    PathNode *previous_destination;
    QPointF goal_pos;
    QList<PathNode *> alternative_path_nodes;

    QList<QPair<PathNode *, double> > targetPlatform;
    QList<QList<PathNode *> > possiblePaths;

    QSet<PathNode*> platformSet;
    int score_check_counter;
    int score_check_frequency;
    Globals &globals;
    Train *train;
    bool experiment;
    int location;

    bool checkPlatformNode();
    bool exiting;
    PathNode *current_destination;
    bool checkGoalDistance();
    //void checkScores();
    virtual double calculateScoreFixedMode(int i);
    void setNewDestination(PathNode *new_destination);

    void checkScores();
    virtual double calculateScoreMobileMode(QList<PathNode *> *path);
    int step;
    bool steadyOnPlatform;
    bool lookedAtPhone;

};

#endif // PERSON_H
