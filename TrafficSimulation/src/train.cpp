#include "train.h"
#include "person.h"
#include <QDebug>
#include "simulationcontroller.h"

Train::Train(QPointF position, int dir, Globals &g, SimulationController *sc, int loc, QGraphicsItem *parent) :
    QGraphicsPixmapItem(QPixmap(":/resources/pics/train.png"), parent),
    advance_counter(0),
    stopped(true),
    start_pos(position),
    direction(dir),
    off_screen_wait(0),
    on_screen_wait(0),
    acceleration(0.993),
    velocity(21.0),
    state(State::Arriving),
    passengers_to_spawn(200),
    globals(g),
    location(loc),
    sc(sc)
{
    this->setPos(start_pos);
}

void Train::advance(int phase)
{
    if(stopped){
        return;
    }

    if(!phase) {
        advance_counter++;
        bool check = false;
        if(state == State::Arriving) { // Arriving
            if(velocity < 0.5) {
                state = State::Arrived;
                velocity = 0.0;
                spawnPassengers();
                timer = advance_counter + on_screen_wait*20;
                return;
            }
            velocity *= acceleration;
        } else if(state == State::Arrived) { // On screen
            if(advance_counter > timer) {
                state = State::DoorsClosing;
                timer = advance_counter + 6*20;
            }
        } else if(state == State::DoorsClosing) { // Doors Closing
            if(advance_counter > timer) {
                foreach(QGraphicsItem *item, this->collidingItems()) {
                    if (dynamic_cast<Person*>(item) != NULL)
                    {
                        dynamic_cast<Person*>(item)->getCurrent_destination()->subDensity();
                        dynamic_cast<Person*>(item)->getPrevious_destination()->subDensity();
                        scene()->removeItem(item);
                        delete item;
                    }
                }
                check = true;
                velocity = 0.5;
                state = State::Leaving;
                timer = advance_counter + 3*20;
            }
        } else if(state == State::Leaving) { // Leaving
            if(velocity > 20) {
                state = State::OffScreen;
                velocity = 0.0;
                this->setPos(start_pos);
                timer = advance_counter + off_screen_wait*20;
                return;
            }
            if(check && advance_counter > timer) {
                check = false;
                foreach(QGraphicsItem *item, this->collidingItems()) {
                    if (dynamic_cast<Person*>(item) != NULL)
                    {
                        dynamic_cast<Person*>(item)->getCurrent_destination()->subDensity();
                        dynamic_cast<Person*>(item)->getPrevious_destination()->subDensity();
                        scene()->removeItem(item);
                        delete item;
                    }
                }
            }

            velocity *= 1 + (1 - acceleration);
        } else { // Off screen
            if(advance_counter > timer) {
                state = State::Arriving;
                velocity = 21.0;
            }
        }
    }
    else this->setPos(this->pos().x() + (velocity * direction), this->pos().y());
}

void Train::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(pos() == start_pos) return;
    QGraphicsPixmapItem::paint(painter, option, widget);
}

void Train::setOn_screen_wait(int value)
{
    on_screen_wait = value;
}

void Train::setOff_screen_wait(int value)
{
    off_screen_wait = value;
}

void Train::setStopped(bool value)
{
    stopped = value;
}

bool Train::getStopped() const
{
    return stopped;
}

Train::State Train::getState() const
{
    return state;
}

QList<PathNode *> Train::getSpawners() const
{
    return spawners;
}

void Train::setSpawners(const QList<PathNode *> &value)
{
    spawners = value;
}


void Train::spawnPassengers()
{
    int spawned = 1;
    int spawner_number = 0;
    foreach(PathNode *spawner, spawners) { // "spawners" member is a QList of nodes inside the carriages for pushing passengers out of the train eg. 220, 406
        while((double)spawned/passengers_to_spawn <= distribution_numbers.at(spawner_number)) {
            if(this->scene()->items().size() - globals.initial_scene_items < globals.max_people) {
                Person *temp = Person::spawn(spawner, 0, globals, location);
                temp->setExiting(true);
                this->scene()->addItem(temp);
            }
            spawned++;
        }
        spawner_number++;
    }
    foreach(GridSquare *square, grid_squares) {
        square->updateDensity();
    }
}

void Train::setPassengers_to_spawn(int value)
{
    passengers_to_spawn = value;
    updatePlatform_lights();
}

void Train::reset()
{
    foreach(QGraphicsItem *item, this->collidingItems()) {
        if (dynamic_cast<Person*>(item) != NULL)
        {
            dynamic_cast<Person*>(item)->getCurrent_destination()->subDensity();
            dynamic_cast<Person*>(item)->getPrevious_destination()->subDensity();
            scene()->removeItem(item);
            delete item;
        }
    }
    state = State::Arriving;
    this->setPos(start_pos);
    velocity = 21.0;
}

void Train::setDistribution_state(int value)
{
    distribution_numbers.clear();
    switch(value) {
    case 0: { distribution_numbers << 0.18140 << 0.35721 << 0.51730 << 0.65422 << 0.76425 << 0.84730 << 0.90619 << 0.94542 << 0.96997 << 0.98440 << 0.99237 << 0.99651 << 0.99852 << 0.99944 << 0.99984 << 1.00000; break; }
    case 1: { distribution_numbers << 0.00036 << 0.00221 << 0.00952 << 0.03203 << 0.08602 << 0.18690 << 0.33368 << 0.50000 << 0.66632 << 0.81310 << 0.91398 << 0.96797 << 0.99048 << 0.99779 << 0.99964 << 1.00000; break; }
    case 2: { distribution_numbers << 0.00016 << 0.00056 << 0.00148 << 0.00349 << 0.00763 << 0.01560 << 0.03003 << 0.05458 << 0.09381 << 0.15270 << 0.23575 << 0.34578 << 0.48270 << 0.64279 << 0.81860 << 1.00000; break; }
    case 3: { distribution_numbers << 0.16632 << 0.31310 << 0.41398 << 0.46797 << 0.49048 << 0.49779 << 0.49964 << 0.50000 << 0.50036 << 0.50221 << 0.50952 << 0.53203 << 0.58602 << 0.68690 << 0.83368 << 1.00000; break; }
    case 4: { distribution_numbers << 0.00158 << 0.02089 << 0.10739 << 0.25000 << 0.39261 << 0.47911 << 0.49842 << 0.50000 << 0.50158 << 0.52089 << 0.60739 << 0.75000 << 0.89261 << 0.97911 << 0.99842 << 1.00000; break; }
    case 5: { distribution_numbers << 0.06250 << 0.12500 << 0.18750 << 0.25000 << 0.31250 << 0.37500 << 0.43750 << 0.50000 << 0.56250 << 0.62500 << 0.68750 << 0.75000 << 0.81250 << 0.87500 << 0.93750 << 1.00000; break; }
    }
    updatePlatform_lights();
}

void Train::setPlatform_lights(const QList<TrafficLight *> &value)
{
    platform_lights = value;
}

void Train::updatePlatform_lights()
{
    if(platform_lights.at(0)->getState() == TrafficLight::State::Distribution) {
        platform_lights.at(0)->setTraffic_value((distribution_numbers.at(0) * passengers_to_spawn)/40.0);
        for(int i=1; i<16; i++) {
            platform_lights.at(i)->setTraffic_value(((distribution_numbers.at(i) - distribution_numbers.at(i-1)) * passengers_to_spawn)/40.0);
        }
    }
}

void Train::setGrid_squares(const QList<GridSquare *> &value)
{
    grid_squares = value;
}
