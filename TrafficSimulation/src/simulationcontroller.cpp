#include "simulationcontroller.h"
#include <QDebug>
#include <QTimer>
#include "math.h"
#include <QMessageBox>

QList<QList<PathNode *> *> *SimulationController::possiblePaths = NULL;

SimulationController::SimulationController(QGraphicsPixmapItem *b, QLCDNumber *frameRate, QLCDNumber *t, QLCDNumber *pc, QObject *parent) :
    QGraphicsScene(b->boundingRect(), parent),
    advance_counter(0),
    advance_frequency(0),
    frame_rate_lcd(frameRate),
    timer_lcd(t),
    person_counter_lcd(pc),
    timer_time(0,0,0),
    timer_pause(true),
    last_advance(QTime::currentTime()),
    person_count(0),
    data_file_write_stream(&data_file),
    fast_forward(false),
    current_experiment(0),
    random_person_spawner(false),
    random_person_spawner_frequency(0),
    experiment3PeopleAdded(0)
{
    this->addItem(b);
    spawnTrafficLights();
    readNodeData();
    addTrains();
    spawnGridSquares(72);
    initialiseDebugNumbers();
    timer_lcd->display(timer_time.toString("hh:mm:ss"));

    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("ddMMMyyyyHHmmss");
    dateTimeString.insert(9, "_");
    dateTimeString.append(".txt");
    data_file.setFileName(dateTimeString);
    data_file.open(QIODevice::WriteOnly | QIODevice::Text);


    data_file_write_stream << "time\t";
    for(int i=0 ; i<1 ; i++) { // platforms 1
        for(int j=0 ; j<16 ; j++) { // the 16 traffic lights on that platform
            int val = 214+(12*j)+i;
            data_file_write_stream << val << "\t"; // record density of each platform
        }
    }
    data_file_write_stream << "total" << "\t";
    data_file_write_stream << "\n";
}

void SimulationController::removeItem(QGraphicsItem *item) {
    if (dynamic_cast<Person*>(item) != NULL && dynamic_cast<Person*>(item)->getExperiment())
    {
        experiment_people.removeOne(dynamic_cast<Person*>(item));
        qDebug() << experiment_people.size();
    }
    QGraphicsScene::removeItem(item);
}

void SimulationController::advance() {
    if(current_experiment != 0) checkExperiment();
    updateTimer();
    updateFrameRate();
    updatePersonCount();
    if(advance_counter%20 == 0) recordData();
    if(random_person_spawner && fmod(advance_counter, round(20.0/random_person_spawner_frequency)) == 0) spawn_random_person();
    QGraphicsScene::advance();
    advance_counter++;

    // DEBUG
    if(globals.DEBUG_DISPLAY_NODES) showDensities();

}

void SimulationController::spawnTrafficLights() {
    int i = 0;
    int j = 0;
    int k = 0;
    for(i=0;i<3;i++){ //concourse traffic lights
        for(j=0;j<2;j++){
            for(k=0;k<2;k++){
                traffic_lights.append(new TrafficLight(669+(i*230)+(j*1227), 325+(k*84)));
            }
        }
    }

    for(i=0;i<8;i++) {
        for(j=0;j<2;j++) {
            // 16 lights on each platform:
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 815)); // platform 1
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 938)); // platform 2
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 1153)); // platform 3
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 1431)); // platform 4
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 1623)); // platform 5
            traffic_lights.append(new TrafficLight(236+(i*335)+(j*205), 1754)); // platform 6
        }
    }

    foreach(TrafficLight *temp, traffic_lights) {
        this->addItem(temp); // add TrafficLights to the scene
    }


    if(globals.FIXED) {
        QList<TrafficLight *> temp;
        for(i=0;i<4;i++) { // 4 columns
            for(j=0;j<2;j++) { // 2 rows
                // 12 to 30 on platform 1
                // 13 to 31 on platform 2
                int t = 12+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(0)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) { // 5 columns
            for(j=0;j<2;j++) { // 2 rows
                // 36 to 60 platform 1
                // 37 to 61 platform 2
                int t = 36+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(8)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            for(j=0;j<2;j++) {
                // 54 to 78 platform 1
                // 55 to 79 platform 2
                int t = 54+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(2)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            for(j=0;j<2;j++) {
                // 84 to 102 platform 1
                // 85 to 103 platform 2
                int t = 84+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(10)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            // 14 to 22 platform 3
            int t = 14+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(1)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            // 38 to 62 platfrom 3
            int t = 38+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(9)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            // 56 to 80 platfrom 3
            int t = 56+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(3)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            // 86 to 104 platfrom 3
            int t = 86+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(11)->setDependants(temp);
        temp.clear();

        // TEMP: Cropped for platforms 1, 2 and 3
        traffic_lights.at(0)->setState(TrafficLight::State::Off);
        traffic_lights.at(1)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(2)->setState(TrafficLight::State::Off);
        traffic_lights.at(3)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(4)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(5)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(6)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(7)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(8)->setState(TrafficLight::State::Off);
        traffic_lights.at(9)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(10)->setState(TrafficLight::State::Off);
        traffic_lights.at(11)->setState(TrafficLight::State::Invalid);
    } else if(globals.MOBILE) {
        QList<TrafficLight *> temp;
        for(i=0;i<4;i++) { // 4 columns
            for(j=0;j<2;j++) { // 2 rows
                // 12 to 30 on platform 1
                // 13 to 31 on platform 2
                int t = 12+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(0)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) { // 5 columns
            for(j=0;j<2;j++) { // 2 rows
                // 36 to 60 platform 1
                // 37 to 61 platform 2
                int t = 36+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(8)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            for(j=0;j<2;j++) {
                // 54 to 78 platform 1
                // 55 to 79 platform 2
                int t = 54+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(2)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            for(j=0;j<2;j++) {
                // 84 to 102 platform 1
                // 85 to 103 platform 2
                int t = 84+j+(6*i);
                temp.append(traffic_lights.at(t));
            }
        }
        traffic_lights.at(10)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            // 14 to 22 platform 3
            int t = 14+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(1)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            // 38 to 62 platfrom 3
            int t = 38+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(9)->setDependants(temp);
        temp.clear();
        for(i=0;i<5;i++) {
            // 56 to 80 platfrom 3
            int t = 56+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(3)->setDependants(temp);
        temp.clear();
        for(i=0;i<4;i++) {
            // 86 to 104 platfrom 3
            int t = 86+(6*i);
            temp.append(traffic_lights.at(t));
        }
        traffic_lights.at(11)->setDependants(temp);
        temp.clear();

        // TEMP: Cropped for platforms 1, 2 and 3
        traffic_lights.at(0)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(1)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(2)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(3)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(4)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(5)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(6)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(7)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(8)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(9)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(10)->setState(TrafficLight::State::Invalid);
        traffic_lights.at(11)->setState(TrafficLight::State::Invalid);
    }

    for(i=0;i<16;i++) {
        for(j=0;j<3;j++) {
            traffic_lights.at(15+j+(6*i))->setState(TrafficLight::State::Invalid);
        }
    }
}

/*
 * Open the node_data.xlsx
 * This function initialises the node data.
 * Basically storing each row of the excel document.
 * The SimulationController only "knows" about columns 1 to 13
 * The PathNode object can access columns 14 to 20
 */
void SimulationController::readNodeData() {
    QFile inputFile(":/resources/node_data_modified_txt.txt");
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        QString line = in.readLine();

        int id = -1, xpos, ypos, xvar, yvar;
        double inSpeed, outSpeed;
        bool inTele, outTele, isPlatform;
        QString location, description;
        QMap<int, QList<int> > platforms_id; // for collecting columns 14 to 20
        QMap<int, QMap<int, QList<int> > > path_list; // this is QMap<node_id,

        while ( !in.atEnd() ) { // collect data in each row
            TrafficLight *light = 0; // null pointer
            line = in.readLine();
            QStringList line_list = line.split("\t"); // each column is separated by a "tab" character

            //handling data from columns 1 to 13, turning from strings into integers
            id = line_list.at(0).toInt();
            location = line_list.at(1);
            description = line_list.at(2);
            xpos = line_list.at(3).toInt();
            ypos = line_list.at(4).toInt();
            xvar = line_list.at(5).toInt();
            yvar = line_list.at(6).toInt();
            inSpeed = line_list.at(7).toDouble();
            outSpeed = line_list.at(8).toDouble();
            (line_list.at(9) == "TRUE") ? (inTele = true) : (inTele = false);
            (line_list.at(10) == "TRUE") ? (outTele = true) : (outTele = false);
            (line_list.at(11) == "TRUE") ? (isPlatform = true) : (isPlatform = false);
            if(line_list.at(12).toInt() != -1) light = traffic_lights.at(line_list.at(12).toInt());
            // insert collected data into member of class SimulationController
            path_nodes.insert(id, new PathNode(id, location, description, xpos, ypos, xvar, yvar, inSpeed, outSpeed, inTele, outTele, isPlatform, light)); // associating node number with columns 1 to 13

            // handling data from columns 14 to 20:
            // the 14th column to the 20th column sometimes have more than one entry
            // platforms_id collects the contents of each column as a QList<int>
            platforms_id = QMap<int, QList<int> >();
            for(int i=0; i<7; i++) { // for each column from 14 to 20, do the following:
                QList<int> tempNodeList;
                QStringList tempStringList = line_list.at(i+13).split(","); // collect entries in a QStringList, splitting where commas occur
                foreach (const QString &str, tempStringList) { // organise QStringList into an integer QList
                    QString s(str);
                    s.remove(QChar('\"')); // there are leading and trailing quotes in the text file ""84,110,136,162""
                    if(s.toInt() != -1 && s != "000") tempNodeList.append(s.toInt());
                }
                // the 14th column to the 20th column are QList<int> objects, the 1st to 13th columns are integers
                // insert(column number, QList<int> in that column)
                platforms_id.insert(i, tempNodeList);
            }
            path_list.insert(id, platforms_id); // associating node number with columns 14 to 20

            //            // DEBUG
            //            QGraphicsSimpleTextItem *temp_text = new QGraphicsSimpleTextItem();
            //            temp_text->setPos(xpos-34, ypos-8);
            //            node_densities.insert(id, temp_text);
            //            this->addItem(temp_text);
        }

        foreach(PathNode *node, path_nodes) // for each of the 453 nodes (see node_data.xlsx)
        {
            // we are now linking the next available nodes for each node
            // there are 6 platforms. the next available nodes depend on which platform the person is aiming for
            QMap<int, QList<PathNode *> *> *platforms = new QMap<int, QList<PathNode *> *>(); // this map will have 6 keys, and each value is the associated PathNode objects
            // each key is a column
            for(int i=0; i<7; i++) // for each column of given node
            {
                // for the table entry 8,20,32,44,56,68 which is row "node 0" , column "platform 1"
                // we will create a PathNode*
                QList<PathNode *> *temp = new QList<PathNode *>(); // this will hold the
                QMap<int, QList<int> > columns14to20 = path_list.value(node->getId()); // get the collection of columns
                QList<int> column_contents = columns14to20.value(i); //isolate one column
                foreach(int val, column_contents) //cycle through all contents of one column
                {
                    temp->append(path_nodes.value(val)); // add existing rows in the pathnode table to a list
                }
                platforms->insert(i, temp); // associate each of the 6 platform columns with the
            }
            node->setPath_to_platforms(platforms); // now each node has a a container with the information about all nodes for each of its 6 platform destinations
        }

        // remove all non used nodes (since platform 4,5,6 are not used)
        foreach(PathNode *node, path_nodes) {
            //QList<PathNode *> test = *node->getPath_to_platforms()->value(1);
            if(shouldNotAppear(node)) {
                path_nodes.remove(node->getId());
            }
        }

        inputFile.close();
    }

    // build list of helpers
    QList<PathNode *> helperList; // 16 entries, platforms 1, 2, and 3 in one list
    QList<PathNode *> platformNodes; // 48 entries, platforms 1, 2, and 3 in one list
    possiblePaths = new QList<QList<PathNode *> *>();
    QMapIterator<int, PathNode *> i(path_nodes);
    while(i.hasNext()) {
        i.next();
        if(i.value()->getDescription() == "Helper" && i.value()->getLocation() == "Platform 1") {
            PathNode *node; // make node
            node = i.value();
            helperList.append(node);
        } else if(i.value()->getDescription() == "Helper" && i.value()->getLocation() == "Platform 2") {
            PathNode *node; // make node
            node = i.value();
            helperList.append(node);
        } else if(i.value()->getDescription() == "Helper" && i.value()->getLocation() == "Platform 3") {
            PathNode *node; // make node
            node = i.value();
            helperList.append(node);
        }
    }
    i.toFront();
    while(i.hasNext()) {
        i.next();
        if(i.value()->getIs_platform() == true && i.value()->getLocation() == "Platform 1") {
            PathNode *node; // make node
            node = i.value();
            platformNodes.append(node);
        }
    }
    i.toFront();
    while(i.hasNext()) {
        i.next();
        if(i.value()->getIs_platform() == true && i.value()->getLocation() == "Platform 2") {
            PathNode *node; // make node
            node = i.value();
            platformNodes.append(node);
        }
    }
    i.toFront();
    while(i.hasNext()) {
        i.next();
        if(i.value()->getIs_platform() == true && i.value()->getLocation() == "Platform 3") {
            PathNode *node; // make node
            node = i.value();
            platformNodes.append(node);
        }
    }

    qDebug() << "helperList complete, length = " << helperList.size();
    qDebug() << "platformNodes list complete, length = " << platformNodes.size();

    for(int i = 0 ; i < platformNodes.size() ; i++) {
        double min_score = std::numeric_limits<double>::max();
        PathNode * closestHelper;
        foreach(PathNode *helperNode, helperList) {
            double val = (platformNodes.at(i)->getPos() - helperNode->getPos()).manhattanLength();
            if(val < min_score) {
                min_score = val;
                closestHelper = helperNode;
            }
        }
        QList<PathNode *> *newPath = new QList<PathNode *>();
        newPath->prepend(platformNodes.at(i));
        newPath->prepend(closestHelper);
        possiblePaths->append(newPath);
    }
    qDebug() << "possiblePaths building of first two nodes complete, length = " << possiblePaths->size();

    for(int i = 0 ; i < possiblePaths->size() ; i++) {
        buildList(possiblePaths->at(i)->at(0));
    }

    qDebug() << "possiblePaths after buildList complete, length = " << possiblePaths->size();
}

void SimulationController::buildList(PathNode *next) {
    static int counter = 0;
    if(next->getLocation() == "Concourse" && next->getDescription() == "Helper") {
        // append final node
        //possiblePaths[counter].append(next);
        QList<PathNode *> *currentList = possiblePaths->at(counter); // comment out when not debugging
        qDebug() << "printing list entry " << counter + 1;
        foreach(PathNode *n, *currentList) {
            qDebug() << "" << n->getId();
        }
        // reached final node, close this list and start new one
        counter++;
    } else {
        PathNode *nextNode;
        nextNode = next->getPath_to_platforms()->value(0)->at(0); // exit node
        QList<PathNode *> *currentList = (*possiblePaths)[counter]; // comment out when not debugging
        (*possiblePaths)[counter]->prepend(nextNode);
        currentList = possiblePaths->value(counter); // comment out when not debugging
        //possiblePaths[counter].append(nextNode);
        buildList(nextNode);
    }
}

void SimulationController::addTrains() {
    trains.append(new Train(QPointF(-2790, 710), 1, globals, this, 1));
    trains.append(new Train(QPointF(3070, 955), -1, globals, this, 1));
    trains.append(new Train(QPointF(3070, 1170), -1, globals, this, 2));
    foreach(Train *t, trains) {
        this->addItem(t);
    }
    for(int i=0;i<3;i++) {
        QList<PathNode *> spawners;
        QList<TrafficLight *> lights;
        for(int j=0;j<16;j++) {
            int s = 220+(j*12)+i;
            int l = 12+i+(j*6);
            spawners.append(path_nodes.value(s)); // nodes inside the carriages for pushing passengers out of the train
            lights.append(traffic_lights.at(l)); // trafficlight nodes just outside train doors
        }
        trains.at(0+i)->setSpawners(spawners);
        trains.at(0+i)->setPlatform_lights(lights);
    }
}

void SimulationController::spawnGridSquares(int width) {
    int s_height = this->height()-120;
    int s_width = this->width()-140;
    int x = 140;
    int y = 140;
    int counter = 0;
    while(y < s_height) {
        while(x < s_width) {
            this->addItem(new GridSquare(x, y, width, counter, globals));
            counter++;
            x += width + 1;
        }
        x = 140;
        y += width + 1;
    }

    foreach(Train *train, trains) {
        QList<GridSquare *> grid_squares;
        foreach(PathNode *node, train->getSpawners()) {
            //            dynamic_cast<GridSquare*>(this->itemAt(node->getPos(), QTransform()))->setSkip_frequency(0);
            grid_squares.append(dynamic_cast<GridSquare*>(this->itemAt(node->getPos(), QTransform())));
        }
        train->setGrid_squares(grid_squares);
    }
}

void SimulationController::checkExperiment() {
    if(current_experiment != 0 && experiment_people.size() == 0) {
        QString mode;
        if(globals.FIXED) mode = " mode = fixed";
        else mode = " mode = mobile";
        QMessageBox msgBox;
        msgBox.setText("Experiment Complete!");
        msgBox.setInformativeText("All the people got on to the train at Platform 1 in " + timer_time.toString("hh:mm:ss") + mode);
        msgBox.exec();
        current_experiment = 0;
        experiment_people.clear();
        QString modeName;
        if(globals.FIXED) modeName = "fixed";
        else modeName = "mobile";
        data_file_write_stream << "pathing mode = " << modeName << "\n";
        data_file_write_stream << "distance multipler = " << QString::number(globals.distanceWeight) << "\n";
        data_file_write_stream << "traffic light multipler = " << QString::number(globals.trafficLightWeight) << "\n";
        data_file_write_stream << "density multipler = " << QString::number(globals.densityWeight) << "\n";
        data_file_write_stream << "score check freq = " << QString::number(globals.score_check_frequency) << "\n";
        return;
    }
    foreach(Person *person, experiment_people) {
        if(current_experiment == 3 && person->getSteadyOnPlatform()) { // experiemtn 3 will not have people board a train
             experiment_people.removeOne(person);
        } else if(trains.at(0)->collidesWithItem(person)) {
            experiment_people.removeOne(person);
        }
    }
    //    switch(current_experiment) {
    //    case 1: {
    //        foreach(Person *person, experiment_people) {
    //            if(trains.at(0)->collidesWithItem(person)) {
    //                experiment_people.removeOne(person);
    //            }
    //        }
    //    }
    //    case 2: {
    //        foreach(Person *person, experiment_people) {
    //            if(trains.at(0)->collidesWithItem(person)) {
    //                experiment_people.removeOne(person);
    //            }
    //        }
    //    }
    //    }
}

void SimulationController::spawnPerson(int start_node, int goal_platform, int loc, QGraphicsItem *parent) {
    if(this->items().size() - globals.initial_scene_items < globals.max_people) {
        this->addItem(Person::spawn(path_nodes[start_node], goal_platform, globals, loc, trains.at(goal_platform-1), parent));
    }
}

void SimulationController::spawnExperimentPerson(int start_node, int goal_platform, int loc, QGraphicsItem *parent) {
    Person *temp = new Person(path_nodes[start_node], goal_platform, globals, trains.at(goal_platform-1), loc, true, parent);
    experiment_people.append(temp);
    this->addItem(temp);
    experiment3PeopleAdded++;
}

void SimulationController::spawn_random_person() {
    if(experiment3PeopleAdded > 400) {
        random_person_spawner = false;
    }
    if(current_experiment == 3) {
        int start_node = qrand()%8;
        int destination_platform = 1;
        spawnExperimentPerson(start_node, destination_platform, 0);
    } else {
        int start_node = qrand()%8;
        int destination_platform = (qrand()%3) + 1;
        spawnPerson(start_node, destination_platform, 0);
    }

}

void SimulationController::updateTimer() {
    if(!timer_pause && advance_counter%20 == 0) {
        timer_time = timer_time.addSecs(1);
        timer_lcd->display(timer_time.toString("hh:mm:ss"));
    }
}

void SimulationController::updateFrameRate() {
    int time_difference = last_advance.elapsed();
    last_advance.start();
    times.enqueue(time_difference);
    while(times.size() > 20) {
        times.dequeue();
    }
    int sum = 0;
    foreach(int i, times) {
        sum += i;
    }
    frame_rate_lcd->display(1000.0/(sum/20.0));
}

void SimulationController::updatePersonCount() {
    person_count = this->items().size() - globals.initial_scene_items;
    if(globals.DEBUG_DISPLAY_NODES) person_count -= debugItems.size();
    if(person_count >= globals.max_people - 5) person_counter_lcd->setPalette(Qt::red);
    else person_counter_lcd->setPalette(Qt::white);
    person_counter_lcd->display(person_count);

}

void SimulationController::recordData() {
    bool recorded = false;
    if(!timer_pause && person_count > 0) {
        data_file_write_stream << timer_time.toString() << "\t" ;
        for(int i=0 ; i<1 ; i++) { // platforms 1, 2, and 3
            for(int j=0 ; j<16 ; j++) { // the 16 traffic lights on that platform
                int val = 214+(12*j)+i;
                int sum = path_nodes.value(val)->getDensity(); //
                data_file_write_stream << sum << "\t"; // record density of each platform
            }
        }
        data_file_write_stream << person_count << "\n";
        recorded = true;
    }

}

void SimulationController::setRandom_person_spawner_frequency(double value)
{
    random_person_spawner_frequency = value;
}

void SimulationController::setRandom_person_spawner(bool value)
{
    random_person_spawner = value;
}

void SimulationController::setFrequency(int value)
{
    advance_frequency = value;
}

QList<Train *> SimulationController::getTrains() const
{
    return trains;
}

QList<TrafficLight *> SimulationController::getTraffic_lights() const
{
    return traffic_lights;
}

void SimulationController::setTimer_pause(bool value)
{
    timer_pause = value;
}

void SimulationController::resetTimer()
{
    timer_time = QTime(0,0,0);
    timer_lcd->display(timer_time.toString("hh:mm:ss"));
}

void SimulationController::initialiseDebugNumbers()
{
    foreach (PathNode *node, path_nodes) {
        QColor brushColour;
        if(node->getDescription() == "Helper") brushColour = Qt::cyan;
        else brushColour = Qt::yellow;
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(node->getPos().x()-15, node->getPos().y()-13, 26, 26);
        ellipse->setBrush(QBrush(brushColour));
        //this->addItem(ellipse);
        QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(node->getId()));
        text->setPos(node->getPos().x()-14, node->getPos().y()-8);
        //this->addItem(text);
        debugItems.append(ellipse);
        debugItems.append(text);
    }

    foreach (TrafficLight *node, traffic_lights) {
        if(node->getState() != TrafficLight::State::Invalid) {
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(node->pos().x()+15, node->pos().y()-13, 26, 26);
            ellipse->setBrush(QBrush(Qt::green));
            //this->addItem(ellipse);
            QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(node->getId()));
            text->setPos(node->pos().x()+16, node->pos().y()-8);
            //this->addItem(text);
            debugItems.append(ellipse);
            debugItems.append(text);
        }
    }
    showDensities();
}

bool SimulationController::shouldNotAppear(PathNode *node) {
    bool a = node->getDescription() == "Stairs";
    bool b = node->getDescription() == "Helper";
    bool c = node->getPath_to_platforms()->value(1)->isEmpty();
    bool d = node->getPath_to_platforms()->value(2)->isEmpty();
    bool e = node->getPath_to_platforms()->value(3)->isEmpty();
    bool f = node->getLocation() == "Platform 4" || node->getLocation() == "Platform 5" || node->getLocation() == "Platform 6";
    if(f) {
        qDebug() << "";
    }
    bool result = ((a||b) && c && d && e) || f ;
    return result;
}

void SimulationController::showDensities()
{
    if(node_densities.isEmpty()) { // initialise list when starting
        foreach(PathNode *node, path_nodes) {
            QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(node->getDensity()));
            QFont font;
            font.setPixelSize(30);
            text->setFont(font);
            text->setPos(node->getPos().x()-14, node->getPos().y());
            //this->addItem(text);
            //debugItems.append(text);
            node_densities.insert(node->getId(), text);
        }
    } else { // called when scene advances
        foreach(PathNode *node, path_nodes) {
            node_densities.value(node->getId())->setText(QString::number(node->getDensity()));
        }
    }
}

void SimulationController::setCurrent_experiment(int value)
{
    current_experiment = value;
}

void SimulationController::updateTrafficLightDensities()
{
    foreach(PathNode *node, path_nodes) {
        node->setTrafficLight();
    }
}
QMap<int, PathNode *> SimulationController::getPath_nodes() const
{
    return path_nodes;
}

void SimulationController::removeDebugItems()
{
    if(debugItems.isEmpty()) return;
    foreach(QGraphicsItem *item, debugItems) {
        this->removeItem(item);
    }
}

void SimulationController::addDebugItems()
{
    if(debugItems.isEmpty()) return;
    foreach(QGraphicsItem *item, debugItems) {
        this->addItem(item);
    }
}

void SimulationController::setTrafficLightDependants() {
    if(globals.MOBILE) {
        traffic_lights.at(0)->setState(TrafficLight::State::Off);
        traffic_lights.at(2)->setState(TrafficLight::State::Off);
        traffic_lights.at(8)->setState(TrafficLight::State::Off);
        traffic_lights.at(10)->setState(TrafficLight::State::Off);
    } else if(globals.FIXED) {
        traffic_lights.at(0)->setState(TrafficLight::State::Density);
        traffic_lights.at(2)->setState(TrafficLight::State::Density);
        traffic_lights.at(8)->setState(TrafficLight::State::Density);
        traffic_lights.at(10)->setState(TrafficLight::State::Density);
    }

}


