#include "person.h"
#include "businessman.h"
#include "tourist.h"
#include <QDebug>
#include <QTime>

Person::Person(PathNode *start, int goal_platform, Globals &g, Train *t,
               int loc, bool isExperiment, QGraphicsItem *parent) :
    QGraphicsEllipseItem(-4, -4, 7, 7, parent),
    stopped(false),
    waiting(false),
    colour(Qt::green),
    speed(2.0),
    density(0),
    velocity(0.0,0.0),
    goal_distance(0.0),
    final_destination(goal_platform),
    current_destination(start),
    previous_destination(start),
    score_check_counter(0),
    globals(g),
    train(t),
    exiting(false),
    experiment(isExperiment),
    location(loc),
    step(0),
    steadyOnPlatform(false),
    lookedAtPhone(false)
{
    setPos(start->getPos() + (QPointF(start->getVariance().x()*(((qrand()%200)-100.0)/100.0), start->getVariance().y()*(((qrand()%200)-100.0)/100.0))));
    goal_pos = this->pos();
    speed_variation = ((qrand()%20 - 10)/100.0);
    start->addDensity();
    start->addDensity();
    finalDestinationString = "Platform " + QString::number(final_destination);

    score_check_frequency = globals.score_check_frequency;
}

Person::~Person()
{
}

void Person::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    this->setBrush(QBrush(colour));
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsEllipseItem::paint(painter, option, widget);

    if(globals.DEBUG_DISPLAY_ALTERNATIVE_PATHS) { // paint alternative path nodes
        foreach(PathNode *node, alternative_path_nodes ){
            if(node == current_destination && !steadyOnPlatform){
                QPen pen;
                pen.setColor(Qt::green);
                QPainterPath path(QPointF(0,0));
                QPointF finish(mapFromScene(node->getPos()));
                path.lineTo(finish);

                pen.setWidth(1);
                painter->setPen(pen);
                painter->setBrush(Qt::NoBrush);
                painter->drawPath(path);
            }
            //else pen.setColor(Qt::black);
        }
    }
}

void Person::advance(int phase) {
    if(!stopped) {
        if(!phase) { // update current Destination in memory
            if(checkPlatformNode()) {
                return; // once we reach a node in front of a train door, the person stops painting
            }
            if(checkGoalDistance()) {
                return;
            }
            if(score_check_counter % score_check_frequency == 0) {
                checkScores();
            }
            score_check_counter++;
        }
        else {
            this->setPos(this->pos() + velocity); // Move current Position
        }
    }
    else if(exiting) {
        getCurrent_destination()->subDensity();
        getPrevious_destination()->subDensity();
        scene()->removeItem(this);
        delete this;
    }
}

QRectF Person::boundingRect() const
{
    return QRectF(-25,-25,50,50);
}

Person *Person::spawn(PathNode *start, int goal_platform, Globals &g, int loc, Train *t, QGraphicsItem *parent)
{
    int roll = qrand()%100;
    if(roll < g.businessmanDemographic) return new Businessman(start, goal_platform, g, t, loc, parent);
    else if(roll - g.businessmanDemographic < g.touristDemographic) return new Tourist(start, goal_platform, g, t, loc, parent);
    else return new Person(start, goal_platform, g, t, loc, parent);
}

/*
 * Performs checking that is necessary when our current destination is a platform node
 * */
bool Person::checkPlatformNode() {
    if(!exiting && current_destination->getIs_platform()) {   // is the person going to a platform node?
        if(train->getState() == Train::State::Arrived) {  // has the train arrived?
            waiting = false;
            goal_pos = current_destination->getPos();
            goal_distance = ( goal_pos - this->pos() ).manhattanLength();
            if(goal_distance < 15.0) {
                score_check_counter = 0;
                alternative_path_nodes = *current_destination->getPath_to_platforms()->value(final_destination);
            }
            return false;
        }
        else if(!waiting) { // is the passenger on the way to a platform node, but the train has not arrived?
            waiting = true;
            goal_pos = current_destination->getPos() +
                    (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0)*4.5,
                             current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0)));
        }
        if((goal_pos-this->pos()).manhattanLength() <= 5.0) { // stop moving once you are "close enough" to the platform node
            steadyOnPlatform = true;
            velocity = QPointF(0.0,0.0);
            return true;
        }
        setSpeed();
        return true;
    }
    else return false;
}

bool Person::checkGoalDistance()
{
    goal_distance = ( goal_pos - this->pos() ).manhattanLength();
    if(goal_distance <= 4.0) {
        if(current_destination->getPath_to_platforms()->value(final_destination)->size() < 1) {
            stopped = true;
            return true;
        }
        score_check_counter = 0;
        alternative_path_nodes = *current_destination->getPath_to_platforms()->value(final_destination);
    }
    return false;
}
/*
 *  Calculates scores of immediately next alternate nodes with regards to manhatten distance and traffic value
 * */
//void Person::checkScores()
//{
//    score_check_counter = 0;
//    double min_score = std::numeric_limits<double>::max();
//    PathNode * new_destination;
//    for(int i=0; i<alternative_path_nodes.size(); ++i) {
//        double val = calculateScoreFixedMode(i);
//        if(val < min_score) {
//            min_score = val;
//            new_destination = alternative_path_nodes.at(i); // important: here is where we have reached the current destination node and we decide on the next node to move to
//        }
//    }
//    if(current_destination != new_destination) {
//        setNewDestination(new_destination);
//        if((!exiting && current_destination->getIn_teleport()) || (exiting && current_destination->getOut_teleport())) {
//            setPos(current_destination->getPos() + (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0), current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0))));
//            goal_pos = this->pos();
//            velocity = QPointF(0.0,0.0);
//            if(this->pos().y()<680) this->setLocation(0);
//            else if(this->pos().y()<1300) this->setLocation(1);
//            else if(this->pos().y()<1900) this->setLocation(2);
//            return;
//        }
//    }
//    setSpeed();
//}



/*
 * Calculates scores of nodes on target platform with regards to manhatten distance and traffic value
 * */
void Person::checkScores() {
    bool newDestinationAssigned = false;
    score_check_counter = 0;
    double min_score = std::numeric_limits<double>::max();
    PathNode * new_destination;
    bool beforeConcourse = current_destination->getDescription() == "Out Gate" || current_destination->getDescription() == "In Gate" || current_destination->getDescription() == "Spawner";
    bool movingToTrain = alternative_path_nodes.at(0)->getDescription() == "Door Train";
    bool insideTrain = alternative_path_nodes.at(0)->getDescription().contains("Car");
    bool allPathCheck = alternative_path_nodes.at(0)->getDescription() == "Helper" && globals.MOBILE;
    if(lookedAtPhone && alternative_path_nodes.at(0)->getDescription() == "Helper") {
        lookedAtPhone = false; // let the passenger look again once on the platform
    }
    if(allPathCheck && !lookedAtPhone) {
        for(int i = 0; i < alternative_path_nodes.size() ; i++) {
            for(int j = 0 ; j < SimulationController::possiblePaths->size() ; j++) {
                QList<PathNode *> *path = SimulationController::possiblePaths->at(j);
                PathNode* alternativeNode = alternative_path_nodes.at(i);
                bool a = path->contains(alternativeNode);
                bool b = path->last()->getLocation() == finalDestinationString;
                bool c = alternativeNode->getDescription() == "Door Train";
                if(a && b && !c) {
                    double val = calculateScoreMobileMode(path);
                    if(val <= min_score) {
                        min_score = val;
                        new_destination = alternativeNode;
                        newDestinationAssigned = true;
                        if(globals.MOBILE_CHECK_ONCE) {
                            lookedAtPhone = true; // only set when on the concourse
                        }

                    }
                }
            }
        }
    } else if(allPathCheck && lookedAtPhone) {
        for(int i = 0 ; i < alternative_path_nodes.size() ; i++) {
            if(alternative_path_nodes.at(i)->getId() == current_destination->getId()){
                new_destination = alternative_path_nodes.at(i);
                newDestinationAssigned = true;
            }
        }
    }
    else {
        for(int i=0; i<alternative_path_nodes.size(); i++) {
            double val = calculateScoreFixedMode(i);
            if(val < min_score) {
                min_score = val;
                new_destination = alternative_path_nodes.at(i); // important: here is where we have reached the current destination node and we decide on the next node to move to
                newDestinationAssigned = true;
            }
        }
    }

    if(!newDestinationAssigned && allPathCheck) {
        qDebug() << "about to fail, repeat check";
        qFatal("PathNode * new_destination is a null pointer"); // need to watch that a new_destination node is found when searching through SimulationController::possiblePaths
    }

    if(current_destination != new_destination) {
        setNewDestination(new_destination);
        if(!beforeConcourse && step < SimulationController::possiblePaths->at(0)->size()-1){ // step must be a maximum of 6
            step++;
        }
        if((!exiting && current_destination->getIn_teleport()) || (exiting && current_destination->getOut_teleport())) {
            setPos(current_destination->getPos() + (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0), current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0))));
            goal_pos = this->pos();
            velocity = QPointF(0.0,0.0);
            if(this->pos().y()<680) this->setLocation(0);
            else if(this->pos().y()<1300) this->setLocation(1);
            else if(this->pos().y()<1900) this->setLocation(2);
            return;
        }
    }
    setSpeed();
}

/*
 * Used by Person::checkScores1()
 */
double Person::calculateScoreMobileMode(QList<PathNode *> *path)
{

    if(step > path->size()) {
        qFatal("must keep steps in the range of SimulationController::possiblePaths->size()");
    }
    double distanceScore = 0;
    double densityScore = 0;
    double trafficScore = 0;

    PathNode * _nextnode = path->at(step);
    if(!_nextnode->getIn_teleport()) distanceScore += globals.distanceWeight * (_nextnode->getPos() - this->pos()).manhattanLength();
    if(_nextnode == current_destination) densityScore += globals.densityWeight * (_nextnode->getDensity()-1);
    else densityScore += globals.densityWeight * _nextnode->getDensity();
    if(!exiting && _nextnode->getTraffic_light() != 0) trafficScore += globals.trafficLightWeight * _nextnode->getTraffic_light()->getTraffic_value();

    // all remaining nodes in chain
    for(int i = step+1 ; i < path->size() ; i++) { // i < 7
        PathNode * _node = path->at(i);
        PathNode * _prevnode = path->at(i-1);
        if(!_node->getIn_teleport()) distanceScore += globals.distanceWeight * (_node->getPos() - _prevnode->getPos()).manhattanLength(); // skip the length for the stair node
        if(_node == current_destination) densityScore += globals.densityWeight * (_node->getDensity()-1);
        else densityScore += globals.densityWeight * _node->getDensity();
        if(!exiting && _node->getTraffic_light() != 0) trafficScore += globals.trafficLightWeight * _node->getTraffic_light()->getTraffic_value();
    }
    return distanceScore + densityScore + trafficScore;
}

bool Person::getSteadyOnPlatform() const
{
    return steadyOnPlatform;
}

/*
 * Used by Person::checkScores()
 */
double Person::calculateScoreFixedMode(int i)
{
    double new_score = 0.0;
    new_score += globals.distanceWeight * ( alternative_path_nodes.at(i)->getPos() - this->pos() ).manhattanLength(); // how far away is each node
    if(alternative_path_nodes.at(i) == current_destination)
        new_score += globals.densityWeight * (alternative_path_nodes.at(i)->getDensity()-1);
    else
        new_score += globals.densityWeight * alternative_path_nodes.at(i)->getDensity();
    if(!exiting && alternative_path_nodes.at(i)->getTraffic_light() != 0) {
        if(alternative_path_nodes.at(i)->getTraffic_light()->getTraffic_value() > 0.2) {
            qDebug() << "stop here";
        }
        new_score += globals.trafficLightWeight * alternative_path_nodes.at(i)->getTraffic_light()->getTraffic_value();
    }
    return new_score;
}

void Person::setSpeed()
{
    if(!exiting)
    {
        if(density > 20)
        {
            speed = current_destination->getIn_speed()*0.3;
        }
        else speed = current_destination->getIn_speed()*((1-(0.035*density)));
    }
    else
    {
        if(density > 20)
        {
            speed = current_destination->getOut_speed()*0.3;
        }
        else speed = current_destination->getOut_speed()*((1-(0.035*density)));
    }
    speed *= 1+speed_variation;
    if((goal_pos-this->pos()).manhattanLength() < 1.0) { velocity = QPointF(0.0, 0.0); return; }
    velocity = (((goal_pos-this->pos())/(goal_pos-this->pos()).manhattanLength())*speed);
}
int Person::getFinal_destination() const
{
    return final_destination;
}
bool Person::getWaiting() const
{
    return waiting;
}


void Person::setNewDestination(PathNode *new_destination)
{
    previous_destination->subDensity();
    previous_destination = current_destination;
    current_destination = new_destination;
    goal_distance = (current_destination->getPos()-this->pos()).manhattanLength();
    goal_pos = current_destination->getPos() + (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0), current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0)));
    if(!exiting && current_destination->getIs_platform()) {
        goal_pos = current_destination->getPos() + (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0)*4.5, current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0)));
    }
    if((!exiting && current_destination->getIn_teleport()) || (exiting && current_destination->getOut_teleport())) {
        setPos(current_destination->getPos() + (QPointF(current_destination->getVariance().x()*(((qrand()%200)-100.0)/100.0), current_destination->getVariance().y()*(((qrand()%200)-100.0)/100.0))));
        goal_pos = this->pos();
    }
    current_destination->addDensity();
}

/*
 * The density member of Person class is the number of other Persons in the same GridSquare
 * */
void Person::setDensity(int value)
{
    density = value;
}

PathNode *Person::getCurrent_destination() const
{
    return current_destination;
}

PathNode *Person::getPrevious_destination() const
{
    return previous_destination;
}

void Person::setExiting(bool value)
{
    exiting = value;
}

bool Person::getExperiment() const
{
    return experiment;
}

int Person::getLocation() const
{
    return location;
}

void Person::setLocation(int value)
{
    location = value;
}
