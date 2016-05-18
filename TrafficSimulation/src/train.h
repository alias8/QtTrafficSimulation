#ifndef TRAIN_H
#define TRAIN_H

#include <QGraphicsPixmapItem>
#include <QDateTime>
#include "pathnode.h"
#include "globals.h"
#include "gridsquare.h"

class SimulationController;

class Train : public QGraphicsPixmapItem
{
public:
    enum class State {Arriving = 0, Arrived = 1, DoorsClosing = 2, Leaving = 3, OffScreen = 4};
    explicit Train(QPointF position, int dir, Globals &g, SimulationController *sc, int loc, QGraphicsItem *parent = 0);
    void advance(int phase);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option,
                 QWidget * widget = 0 );

    void setOff_screen_wait(int value);
    void setOn_screen_wait(int value);
    bool getStopped() const;
    void setStopped(bool value);
    State getState() const;
    void setPassengers_to_spawn(int value);
    void reset();
    void setDistribution_state(int value);
    void setPlatform_lights(const QList<TrafficLight *> &value);
    void updatePlatform_lights();

    QList<PathNode *> getSpawners() const;
    void setSpawners(const QList<PathNode *> &value);
    void setGrid_squares(const QList<GridSquare *> &value);

private:
    long advance_counter;
    bool stopped;
    QPointF start_pos;
    int direction;
    int off_screen_wait;
    int on_screen_wait;
    double acceleration;
    double velocity;
    State state;
    int timer;
    int passengers_to_spawn;
    QList<double> distribution_numbers;
    QList<PathNode *> spawners;
    QList<GridSquare *> grid_squares;
    QList<TrafficLight *> platform_lights;
    Globals &globals;
    int location;


    void spawnPassengers();
    SimulationController *sc;
    
signals:
    
public slots:
    
};

#endif // TRAIN_H
