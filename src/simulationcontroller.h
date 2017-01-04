#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H
#include <QGraphicsScene>
#include "person.h"
#include "globals.h"
#include "gridsquare.h"
#include "qlcdnumber.h"

class Person;

class SimulationController : public QGraphicsScene
{
    Q_OBJECT

public:
    SimulationController(QGraphicsPixmapItem *b, QLCDNumber *frameRate, QLCDNumber *t, QLCDNumber *pc, QObject * parent = 0);
    Globals globals;

    void removeItem ( QGraphicsItem * item );

    void setRandom_person_spawner_frequency(double value);
    void setRandom_person_spawner(bool value);
    void setFrequency(int value);

    void spawnPerson(int start_node, int goal_platform, int loc, QGraphicsItem * parent = 0);
    void spawnExperimentPerson(int start_node, int goal_platform, int loc, QGraphicsItem * parent = 0);

    QList<TrafficLight *> getTraffic_lights() const;
    QList<Train *> getTrains() const;
    void setTimer_pause(bool value);
    void resetTimer();
    void setDebug(bool value);
    void setCurrent_experiment(int value);
    void updateTrafficLightDensities();
    QMap<int, PathNode *> getPath_nodes() const;
    static QList<QList<PathNode *> *> *possiblePaths;
    void removeDebugItems();
    void addDebugItems();
    void setTrafficLightDependants();

    //static QList<QList<PathNode *> > getPossiblePaths() { return possiblePaths;}

private:
    QGraphicsPixmapItem *background_item;
    unsigned long advance_counter;
    int advance_frequency;

    QList<TrafficLight *> traffic_lights;
    QMap<int, PathNode *> path_nodes;
    QList<Train *> trains;
    QLCDNumber *frame_rate_lcd;
    QLCDNumber *timer_lcd;
    QLCDNumber *person_counter_lcd;
    QTime timer_time;
    bool timer_pause;
    QTime last_advance;
    QQueue<int> times;
    int person_count;
    QFile data_file;
    QTextStream data_file_write_stream;
    bool fast_forward;
    QMap <int, QGraphicsSimpleTextItem *> node_densities;
    QList <Person *> experiment_people;
    int current_experiment;

    bool random_person_spawner;
    double random_person_spawner_frequency;
    void spawn_random_person();

    void updateTimer();
    void updateFrameRate();
    void updatePersonCount();
    void recordData();
    void spawnTrafficLights();
    void readNodeData();
    void addTrains();
    void spawnGridSquares(int width);
    void checkExperiment();

    // DEBUG
    void initialiseDebugNumbers();
    void showDensities();
    bool shouldNotAppear(PathNode *node);

    //QList<QList<PathNode *> > possiblePaths;
    void buildList(PathNode *next);
    QList<QGraphicsItem *> debugItems;
    int experiment3PeopleAdded;


public slots:
    void advance();
};

#endif // SIMULATIONCONTROLLER_H
