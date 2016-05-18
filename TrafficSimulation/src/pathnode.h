#ifndef PATHNODE_H
#define PATHNODE_H

#include <QtCore>
#include "trafficlight.h"

class PathNode
{
public:
    PathNode(int id, QString location, QString description, int xpos, int ypos, int xvar, int yvar, double inSpeed, double outSpeed, bool inTele, bool outTele, bool isPlatform, TrafficLight *t = 0);

    int getId() const;
    QString getDescription() const;
    QString getLocation() const;
    QPointF getPos() const;
    QPointF getVariance() const;
    double getIn_speed() const;
    double getOut_speed() const;
    bool getIn_teleport() const;
    bool getOut_teleport() const;
    bool getIs_platform() const;
    QMap<int, QList<PathNode *> *> *getPath_to_platforms() const;
    void setPath_to_platforms(QMap<int, QList<PathNode *> *> *value); //
    int getDensity() const;
    void subDensity();
    void addDensity();
    TrafficLight *getTraffic_light() const;
    void setTrafficLight();
    bool operator==(const PathNode &node);



private:
    int id;
    QString location;
    QString description;
    QPointF pos;
    QPointF variance;
    double in_speed;
    double out_speed;
    bool in_teleport;
    bool out_teleport;
    bool is_platform;
    TrafficLight *traffic_light;
    QMap<int, QList<PathNode *> *> *path_to_platforms; // holds columns 14 to 20
    int density;

};

#endif // PATHNODE_H
