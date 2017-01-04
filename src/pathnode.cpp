#include "pathnode.h"

PathNode::PathNode(int id, QString location, QString description, int xpos, int ypos, int xvar, int yvar, double inSpeed, double outSpeed, bool inTele, bool outTele, bool isPlatform, TrafficLight *t) :
    id(id),
    location(location),
    description(description),
    pos(xpos, ypos),
    variance(xvar, yvar),
    in_speed(inSpeed),
    out_speed(outSpeed),
    in_teleport(inTele),
    out_teleport(outTele),
    is_platform(isPlatform),
    traffic_light(t),
    density(0)
{
}

int PathNode::getId() const { return id; }
QString PathNode::getDescription() const { return description; }
QString PathNode::getLocation() const { return location; }
QPointF PathNode::getPos() const { return pos; }
QPointF PathNode::getVariance() const { return variance; }
bool PathNode::getOut_teleport() const { return out_teleport; }
bool PathNode::getIn_teleport() const { return in_teleport; }
double PathNode::getOut_speed() const { return out_speed; }
double PathNode::getIn_speed() const { return in_speed; }
bool PathNode::getIs_platform() const { return is_platform; }
QMap<int, QList<PathNode *> *> *PathNode::getPath_to_platforms() const { return path_to_platforms; }
void PathNode::setPath_to_platforms(QMap<int, QList<PathNode *> *> *value) { path_to_platforms = value; }// this map will have 6 keys, and each value is the associated PathNode objects
int PathNode::getDensity() const { return density; }
void PathNode::subDensity() { density--; setTrafficLight(); }
void PathNode::addDensity() { density++; setTrafficLight(); }
TrafficLight *PathNode::getTraffic_light() const { return traffic_light; }
bool PathNode::operator==(const PathNode &node) {return id == node.getId(); }


/*
 * the nodes with "attached" traffic lights call setTrafficLight()
 * whenever a node is set as a "currentDestination" for a Person
 * The traffic_value will be a number between 0 and 1, and is how the traffic light colour is decided
*/
void PathNode::setTrafficLight()
{

    if(traffic_light != 0 && traffic_light->getState() == TrafficLight::State::Density) {
        traffic_light->setTraffic_value(density/40.0);
    }
}








