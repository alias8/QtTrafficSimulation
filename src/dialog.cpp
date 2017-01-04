#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemAnimation>
#include <QGraphicsEllipseItem>
#include "person.h"
#include "businessman.h"
#include "gridsquare.h"
#include <QObject>
#include <iostream>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    random_spawner(false)
{
    qsrand(QTime::currentTime().msec());
    ui->setupUi(this);
    this->setWindowTitle("Town Hall Simulation");

    scene = new SimulationController(new QGraphicsPixmapItem(QPixmap(":/resources/pics/Town-Hall-Station-Old.png")), ui->frameRate_lcdNumber, ui->timer_lcdNumber, ui->personCount_lcdNumber, this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    scene->globals.initial_scene_items = scene->items().size();
    scene->globals.max_people = ui->maxPeopleSlider->value();
    scene->globals.businessmanDemographic = ui->businessmanDemographicSlider->value();
    scene->globals.touristDemographic = ui->touristDemographicSlider->value();
    scene->globals.normalDemographic = ui->normalDemographicSlider->value();
    scene->globals.density_check = ui->densityCheckSlider->value();
    scene->globals.DEBUG_DISPLAY_ALTERNATIVE_PATHS = ui->debugDisplayAlternativePathsCheckBox->checkState();
    scene->globals.DEBUG_DISPLAY_GRIDSQUARES = ui->debugDisplayGridSquaresCheckBox->checkState();
    scene->globals.DEBUG_DISPLAY_NODES = ui->debugDisplayNodesCheckBox->checkState();
    scene->globals.MOBILE = ui->mobile_radioButton->isChecked();
    scene->globals.FIXED = ui->fixed_radioButton->isChecked();

    //Initialise
    ui->distribution_p1_even_radioButton->click();
    ui->distribution_p2_even_radioButton->click();
    ui->distribution_p3_even_radioButton->click();
    ui->distributionRadioButton->click();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    scene->setFrequency(50);
    timer->start(50); // 50msec

//    spawner_timer = new QTimer(this);
//    connect(spawner_timer, SIGNAL(timeout()), this, SLOT(timer_spawner()));
}

Dialog::~Dialog()
{
    delete scene;
    delete ui;
}


void Dialog::on_maxPeopleSlider_valueChanged(int value)
{
    scene->globals.max_people = value;
    int difference = scene->items().size() - value - scene->globals.initial_scene_items;
    if(difference > 0) {
        int i=0;
        foreach(QGraphicsItem *item, scene->items()) {
            if(i >= difference) return;
            if (dynamic_cast<Person*>(item) != NULL)
            {
                dynamic_cast<Person*>(item)->getCurrent_destination()->subDensity();
                dynamic_cast<Person*>(item)->getPrevious_destination()->subDensity();
                scene->removeItem(item);
                delete item;
                i++;
            }
        }
    }
}

void Dialog::on_zoomHorizontalSlider_valueChanged(int value)
{
    ui->graphicsView->setTransform(QTransform::fromScale(value/10.0, value/10.0));
}

void Dialog::on_speedHorizontalSlider_valueChanged(int value)
{
    if(value == 0) timer->stop();
    else {
        scene->setFrequency(50*(1.0/(value/10.0)));
        timer->start(50*(1.0/(value/10.0)));
    }
}

void Dialog::on_start_timer_pushButton_toggled(bool checked)
{
    if(checked) {
        scene->setTimer_pause(false);
    }
    else {
        scene->setTimer_pause(true);
    }
}

void Dialog::on_reset_timer_pushButton_clicked()
{
    scene->resetTimer();
}

void Dialog::on_about_pushButton_clicked()
{
    QMessageBox msgBox(this);

    // Header Text
    msgBox.setText("Town Hall Simulation v1.05 - University of Technology Sydney 2013");

    // Body Text
    msgBox.setInformativeText("Lore feum quisciliqui ting eugait ullandignim zzrit iriustrud doluptat volum il il iustin utet, sum dolore tat volobor autpat alisim quipis nit iure vendrerit eugait ing et ad magnim amconse min ulla corper in heniat accum am dipit lutatuero od minismo orercipis ad tie veraesectet ipit lum incil dolor augiam dolorpe ciliquis at.");

    msgBox.exec();
}

void Dialog::on_fastForward_pushButton_toggled(bool checked)
{
    if(checked) {
        timer->start(1);
        ui->graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    } else {
        timer->start(50*(1.0/(ui->speedHorizontalSlider->value()/10.0)));
        ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }
}

void Dialog::on_exp1_start_pushButton_clicked()
{
    for(int j=0; j<20; j++) {
        scene->spawnExperimentPerson(137, 1, 1);
        scene->spawnExperimentPerson(86, 1, 1);
        scene->spawnExperimentPerson(163, 1, 1);
        scene->spawnExperimentPerson(112, 1, 1);
    }
    scene->getTrains().at(0)->setStopped(true);
    ui->trains_p1_pushButton->click();
    scene->getTrains().at(0)->reset();
    ui->start_timer_pushButton->setChecked(true);
    ui->reset_timer_pushButton->click();
    scene->setCurrent_experiment(1);
}

void Dialog::on_exp2_start_pushButton_clicked()
{
    for(int j=0; j<70; j++) {
        scene->spawnExperimentPerson(0, 1, 0);
        scene->spawnExperimentPerson(1, 1, 0);
    }
    for(int j=0; j<10; j++) {
        scene->spawnExperimentPerson(2, 1, 0);
        scene->spawnExperimentPerson(3, 1, 0);
        scene->spawnExperimentPerson(4, 1, 0);
        scene->spawnExperimentPerson(5, 1, 0);
        scene->spawnExperimentPerson(6, 1, 0);
        scene->spawnExperimentPerson(7, 1, 0);
    }
    scene->getTrains().at(0)->setStopped(true);
    ui->trains_p1_pushButton->click();
    ui->start_timer_pushButton->setChecked(true);
    ui->reset_timer_pushButton->click();
    scene->setCurrent_experiment(2);
}

void Dialog::on_exp3_start_pushButton_clicked()
{
    scene->spawnExperimentPerson(0, 1, 0);
//    for(int j=0; j<70; j++) {
//        scene->spawnExperimentPerson(0, 1, 0);
//        scene->spawnExperimentPerson(1, 1, 0);
//    }
//    for(int j=0; j<10; j++) {
//        scene->spawnExperimentPerson(2, 1, 0);
//        scene->spawnExperimentPerson(3, 1, 0);
//        scene->spawnExperimentPerson(4, 1, 0);
//        scene->spawnExperimentPerson(5, 1, 0);
//        scene->spawnExperimentPerson(6, 1, 0);
//        scene->spawnExperimentPerson(7, 1, 0);
//    }
    scene->getTrains().at(0)->setStopped(true);
    //ui->trains_p1_pushButton->click();
    ui->start_timer_pushButton->setChecked(true);
    ui->reset_timer_pushButton->click();
    scene->setCurrent_experiment(3);
    on_random_spawn_button_clicked();
}


void Dialog::on_s1_button_clicked()
{
    for(int i=0; i<ui->s1_quantity_spinBox->value(); i++) {
        scene->spawnPerson(0, (ui->s1_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s2_button_clicked()
{
    for(int i=0; i<ui->s2_quantity_spinBox->value(); i++) {
        scene->spawnPerson(1, (ui->s2_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s3_button_clicked()
{
    for(int i=0; i<ui->s3_quantity_spinBox->value(); i++) {
        scene->spawnPerson(2, (ui->s3_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s4_button_clicked()
{
    for(int i=0; i<ui->s4_quantity_spinBox->value(); i++) {
        scene->spawnPerson(3, (ui->s4_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s5_button_clicked()
{
    for(int i=0; i<ui->s5_quantity_spinBox->value(); i++) {
        scene->spawnPerson(4, (ui->s5_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s6_button_clicked()
{
    for(int i=0; i<ui->s6_quantity_spinBox->value(); i++) {
        scene->spawnPerson(5, (ui->s6_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s7_button_clicked()
{
    for(int i=0; i<ui->s7_quantity_spinBox->value(); i++) {
        scene->spawnPerson(6, (ui->s7_destination_spinBox->value()), 0);
    }
}

void Dialog::on_s8_button_clicked()
{
    for(int i=0; i<ui->s8_quantity_spinBox->value(); i++) {
        scene->spawnPerson(7, (ui->s8_destination_spinBox->value()), 0);
    }
}

void Dialog::on_despawn_button_clicked()
{
    foreach(QGraphicsItem *item, scene->items()) {
        if (dynamic_cast<Person*>(item) != NULL)
        {
            dynamic_cast<Person*>(item)->getCurrent_destination()->subDensity();
            dynamic_cast<Person*>(item)->getPrevious_destination()->subDensity();
            scene->removeItem(item);
            delete item;
        }
    }
}

void Dialog::on_spawnAll_button_clicked()
{
    for(int i=0; i<ui->s1_quantity_spinBox->value(); i++) {
        scene->spawnPerson(0, (ui->s1_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s2_quantity_spinBox->value(); i++) {
        scene->spawnPerson(1, (ui->s2_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s3_quantity_spinBox->value(); i++) {
        scene->spawnPerson(2, (ui->s3_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s4_quantity_spinBox->value(); i++) {
        scene->spawnPerson(3, (ui->s4_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s5_quantity_spinBox->value(); i++) {
        scene->spawnPerson(4, (ui->s5_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s6_quantity_spinBox->value(); i++) {
        scene->spawnPerson(5, (ui->s6_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s7_quantity_spinBox->value(); i++) {
        scene->spawnPerson(6, (ui->s7_destination_spinBox->value()), 0); }
    for(int i=0; i<ui->s8_quantity_spinBox->value(); i++) {
        scene->spawnPerson(7, (ui->s8_destination_spinBox->value()), 0); }
}


void Dialog::on_random_spawn_button_clicked()
{
    if(!random_spawner) {
        random_spawner = true;
        ui->random_spawn_button->setText("Stop");
        scene->setRandom_person_spawner_frequency(ui->random_spawn_spinBox->value());
        scene->setRandom_person_spawner(true);
    }
    else {
        random_spawner = false;
        ui->random_spawn_button->setText("Start");
        scene->setRandom_person_spawner(false);
    }
}

void Dialog::on_normalDemographicSlider_valueChanged(int value)
{
    scene->globals.normalDemographic = value;
    int businessman = scene->globals.businessmanDemographic;
    int tourist = scene->globals.touristDemographic;
    while(businessman + tourist + value > 100) {
        if(businessman > tourist) businessman--;
        else tourist--;
    }
    while(businessman + tourist + value < 98) {
        if(businessman < tourist) businessman++;
        else tourist++;
    }
    scene->globals.businessmanDemographic = businessman;
    scene->globals.touristDemographic = tourist;
    ui->businessmanDemographicSlider->setValue(businessman);
    ui->touristDemographicSlider->setValue(tourist);
}

void Dialog::on_businessmanDemographicSlider_valueChanged(int value)
{
    scene->globals.businessmanDemographic = value;
    int normal = scene->globals.normalDemographic;
    int tourist = scene->globals.touristDemographic;
    while(normal + tourist + value > 100) {
        if(normal > tourist) normal--;
        else tourist--;
    }
    while(normal + tourist + value < 98) {
        if(normal < tourist) normal++;
        else tourist++;
    }
    scene->globals.normalDemographic = normal;
    scene->globals.touristDemographic = tourist;
    ui->normalDemographicSlider->setValue(normal);
    ui->touristDemographicSlider->setValue(tourist);
}

void Dialog::on_touristDemographicSlider_valueChanged(int value)
{
    scene->globals.touristDemographic = value;
    int businessman = scene->globals.businessmanDemographic;
    int normal = scene->globals.normalDemographic;
    while(businessman + normal + value > 100) {
        if(businessman > normal) businessman--;
        else normal--;
    }
    while(businessman + normal + value < 98) {
        if(businessman < normal) businessman++;
        else normal++;
    }
    scene->globals.businessmanDemographic = businessman;
    scene->globals.normalDemographic = normal;
    ui->businessmanDemographicSlider->setValue(businessman);
    ui->normalDemographicSlider->setValue(normal);
}

void Dialog::on_trains_p1_pushButton_clicked()
{
    if(scene->getTrains().at(0)->getStopped()) {
        ui->trains_p1_pushButton->setText("Stop");
        scene->getTrains().at(0)->setOff_screen_wait(ui->trains_p1_off_spinBox->value());
        scene->getTrains().at(0)->setOn_screen_wait(ui->trains_p1_on_spinBox->value());
        scene->getTrains().at(0)->setPassengers_to_spawn(ui->trains_p1_passengers_spinBox->value());
        scene->getTrains().at(0)->setStopped(false);
    }
    else {
        ui->trains_p1_pushButton->setText("Start");
        scene->getTrains().at(0)->setStopped(true);
    }
}

void Dialog::on_trains_p2_pushButton_clicked()
{
    if(scene->getTrains().at(1)->getStopped()) {
        ui->trains_p2_pushButton->setText("Stop");
        scene->getTrains().at(1)->setOff_screen_wait(ui->trains_p2_off_spinBox->value());
        scene->getTrains().at(1)->setOn_screen_wait(ui->trains_p2_on_spinBox->value());
        scene->getTrains().at(1)->setPassengers_to_spawn(ui->trains_p2_passengers_spinBox->value());
        scene->getTrains().at(1)->setStopped(false);
    }
    else {
        ui->trains_p2_pushButton->setText("Start");
        scene->getTrains().at(1)->setStopped(true);
    }
}

void Dialog::on_trains_p3_pushButton_clicked()
{
    if(scene->getTrains().at(2)->getStopped()) {
        ui->trains_p3_pushButton->setText("Stop");
        scene->getTrains().at(2)->setOff_screen_wait(ui->trains_p3_off_spinBox->value());
        scene->getTrains().at(2)->setOn_screen_wait(ui->trains_p3_on_spinBox->value());
        scene->getTrains().at(2)->setPassengers_to_spawn(ui->trains_p3_passengers_spinBox->value());
        scene->getTrains().at(2)->setStopped(false);
    }
    else {
        ui->trains_p3_pushButton->setText("Start");
        scene->getTrains().at(2)->setStopped(true);
    }
}

void Dialog::on_hide_sideBar_pushButton_toggled(bool checked)
{
    if(checked) {
        ui->controls_sidebar->setMinimumWidth(300);
        ui->controls_sidebar->setMaximumWidth(300);
    }
    else {
        ui->controls_sidebar->setMinimumWidth(0);
        ui->controls_sidebar->setMaximumWidth(0);
    }
}

void Dialog::on_offRadioButton_clicked()
{
    foreach(TrafficLight *light, scene->getTraffic_lights()){
        light->setState(TrafficLight::State::Off);
        light->setTraffic_value(0.0);
    }
}

void Dialog::on_manualRadioButton_clicked()
{
    foreach(TrafficLight *light, scene->getTraffic_lights()){
        light->setState(TrafficLight::State::Manual);
    }
}

void Dialog::on_randomRadioButton_clicked()
{
    foreach(TrafficLight *light, scene->getTraffic_lights()){
        light->setState(TrafficLight::State::Random);
        light->setTraffic_value((qrand()%100)/100.0);
    }
}

void Dialog::on_densityRadioButton_clicked()
{
    foreach(TrafficLight *light, scene->getTraffic_lights()){
        light->setState(TrafficLight::State::Density);
    }
    scene->updateTrafficLightDensities();
}

void Dialog::on_distributionRadioButton_clicked()
{
    foreach(TrafficLight *light, scene->getTraffic_lights()){
        light->setState(TrafficLight::State::Distribution);
    }
    foreach (Train *train, scene->getTrains()) {
        train->updatePlatform_lights();
    }
}


void Dialog::on_random_spawn_spinBox_valueChanged(double arg1)
{
    if(random_spawner) {
        scene->setRandom_person_spawner_frequency(arg1);
    }
}

void Dialog::on_densityCheckSlider_valueChanged(int value) { scene->globals.density_check = value; }
void Dialog::on_trains_p1_passengers_spinBox_valueChanged(int arg1) { scene->getTrains().at(0)->setPassengers_to_spawn(arg1); }
void Dialog::on_trains_p2_passengers_spinBox_valueChanged(int arg1) { scene->getTrains().at(1)->setPassengers_to_spawn(arg1); }
void Dialog::on_trains_p3_passengers_spinBox_valueChanged(int arg1) { scene->getTrains().at(2)->setPassengers_to_spawn(arg1); }
void Dialog::on_trains_p1_off_spinBox_valueChanged(int arg1) { scene->getTrains().at(0)->setOff_screen_wait(arg1); }
void Dialog::on_trains_p2_off_spinBox_valueChanged(int arg1) { scene->getTrains().at(1)->setOff_screen_wait(arg1); }
void Dialog::on_trains_p3_off_spinBox_valueChanged(int arg1) { scene->getTrains().at(2)->setOff_screen_wait(arg1); }
void Dialog::on_trains_p1_on_spinBox_valueChanged(int arg1) { scene->getTrains().at(0)->setOn_screen_wait(arg1); }
void Dialog::on_trains_p2_on_spinBox_valueChanged(int arg1) { scene->getTrains().at(1)->setOn_screen_wait(arg1); }
void Dialog::on_trains_p3_on_spinBox_valueChanged(int arg1) { scene->getTrains().at(2)->setOn_screen_wait(arg1); }
void Dialog::on_distribution_p1_left_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(0); }
void Dialog::on_distribution_p1_middle_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(1); }
void Dialog::on_distribution_p1_right_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(2); }
void Dialog::on_distribution_p1_ends_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(3); }
void Dialog::on_distribution_p1_double_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(4); }
void Dialog::on_distribution_p1_even_radioButton_clicked() { scene->getTrains().at(0)->setDistribution_state(5); }
void Dialog::on_distribution_p2_left_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(0); }
void Dialog::on_distribution_p2_middle_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(1); }
void Dialog::on_distribution_p2_right_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(2); }
void Dialog::on_distribution_p2_ends_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(3); }
void Dialog::on_distribution_p2_double_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(4); }
void Dialog::on_distribution_p2_even_radioButton_clicked() { scene->getTrains().at(1)->setDistribution_state(5); }
void Dialog::on_distribution_p3_left_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(0); }
void Dialog::on_distribution_p3_middle_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(1); }
void Dialog::on_distribution_p3_right_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(2); }
void Dialog::on_distribution_p3_ends_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(3); }
void Dialog::on_distribution_p3_double_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(4); }
void Dialog::on_distribution_p3_even_radioButton_clicked() { scene->getTrains().at(2)->setDistribution_state(5); }
void Dialog::on_p1_horizontalSlider_1_valueChanged(int value) { scene->getTraffic_lights().at(12)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_2_valueChanged(int value) { scene->getTraffic_lights().at(18)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_3_valueChanged(int value) { scene->getTraffic_lights().at(24)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_4_valueChanged(int value) { scene->getTraffic_lights().at(30)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_5_valueChanged(int value) { scene->getTraffic_lights().at(36)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_6_valueChanged(int value) { scene->getTraffic_lights().at(42)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_7_valueChanged(int value) { scene->getTraffic_lights().at(48)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_8_valueChanged(int value) { scene->getTraffic_lights().at(54)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_9_valueChanged(int value) { scene->getTraffic_lights().at(60)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_10_valueChanged(int value) { scene->getTraffic_lights().at(66)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_11_valueChanged(int value) { scene->getTraffic_lights().at(72)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_12_valueChanged(int value) { scene->getTraffic_lights().at(78)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_13_valueChanged(int value) { scene->getTraffic_lights().at(84)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_14_valueChanged(int value) { scene->getTraffic_lights().at(90)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_15_valueChanged(int value) { scene->getTraffic_lights().at(96)->setTraffic_value(value/100.0); }
void Dialog::on_p1_horizontalSlider_16_valueChanged(int value) { scene->getTraffic_lights().at(102)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_1_valueChanged(int value) { scene->getTraffic_lights().at(13)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_2_valueChanged(int value) { scene->getTraffic_lights().at(19)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_3_valueChanged(int value) { scene->getTraffic_lights().at(25)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_4_valueChanged(int value) { scene->getTraffic_lights().at(31)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_5_valueChanged(int value) { scene->getTraffic_lights().at(37)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_6_valueChanged(int value) { scene->getTraffic_lights().at(43)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_7_valueChanged(int value) { scene->getTraffic_lights().at(49)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_8_valueChanged(int value) { scene->getTraffic_lights().at(55)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_9_valueChanged(int value) { scene->getTraffic_lights().at(61)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_10_valueChanged(int value) { scene->getTraffic_lights().at(67)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_11_valueChanged(int value) { scene->getTraffic_lights().at(73)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_12_valueChanged(int value) { scene->getTraffic_lights().at(79)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_13_valueChanged(int value) { scene->getTraffic_lights().at(85)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_14_valueChanged(int value) { scene->getTraffic_lights().at(91)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_15_valueChanged(int value) { scene->getTraffic_lights().at(97)->setTraffic_value(value/100.0); }
void Dialog::on_p2_horizontalSlider_16_valueChanged(int value) { scene->getTraffic_lights().at(103)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_1_valueChanged(int value) { scene->getTraffic_lights().at(14)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_2_valueChanged(int value) { scene->getTraffic_lights().at(20)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_3_valueChanged(int value) { scene->getTraffic_lights().at(26)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_4_valueChanged(int value) { scene->getTraffic_lights().at(32)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_5_valueChanged(int value) { scene->getTraffic_lights().at(38)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_6_valueChanged(int value) { scene->getTraffic_lights().at(44)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_7_valueChanged(int value) { scene->getTraffic_lights().at(50)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_8_valueChanged(int value) { scene->getTraffic_lights().at(56)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_9_valueChanged(int value) { scene->getTraffic_lights().at(62)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_10_valueChanged(int value) { scene->getTraffic_lights().at(68)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_11_valueChanged(int value) { scene->getTraffic_lights().at(74)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_12_valueChanged(int value) { scene->getTraffic_lights().at(80)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_13_valueChanged(int value) { scene->getTraffic_lights().at(86)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_14_valueChanged(int value) { scene->getTraffic_lights().at(92)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_15_valueChanged(int value) { scene->getTraffic_lights().at(98)->setTraffic_value(value/100.0); }
void Dialog::on_p3_horizontalSlider_16_valueChanged(int value) { scene->getTraffic_lights().at(104)->setTraffic_value(value/100.0); }

/** TEMP: Cropped for platforms 1, 2 and 3
//void Dialog::on_concourse_horizontalSlider_1_valueChanged(int value)
//{ scene->getTraffic_lights().at(0)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_2_valueChanged(int value)
//{ scene->getTraffic_lights().at(4)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_3_valueChanged(int value)
//{ scene->getTraffic_lights().at(8)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_4_valueChanged(int value)
//{ scene->getTraffic_lights().at(2)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_5_valueChanged(int value)
//{ scene->getTraffic_lights().at(6)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_6_valueChanged(int value)
//{ scene->getTraffic_lights().at(10)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_7_valueChanged(int value)
//{ scene->getTraffic_lights().at(1)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_8_valueChanged(int value)
//{ scene->getTraffic_lights().at(5)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_9_valueChanged(int value)
//{ scene->getTraffic_lights().at(9)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_10_valueChanged(int value)
//{ scene->getTraffic_lights().at(3)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_11_valueChanged(int value)
//{ scene->getTraffic_lights().at(7)->setTraffic_value(value/100.0); }
//void Dialog::on_concourse_horizontalSlider_12_valueChanged(int value)
//{ scene->getTraffic_lights().at(11)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_1_valueChanged(int value)
//{ scene->getTraffic_lights().at(15)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_2_valueChanged(int value)
//{ scene->getTraffic_lights().at(21)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_3_valueChanged(int value)
//{ scene->getTraffic_lights().at(27)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_4_valueChanged(int value)
//{ scene->getTraffic_lights().at(33)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_5_valueChanged(int value)
//{ scene->getTraffic_lights().at(39)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_6_valueChanged(int value)
//{ scene->getTraffic_lights().at(45)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_7_valueChanged(int value)
//{ scene->getTraffic_lights().at(51)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_8_valueChanged(int value)
//{ scene->getTraffic_lights().at(57)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_9_valueChanged(int value)
//{ scene->getTraffic_lights().at(63)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_10_valueChanged(int value)
//{ scene->getTraffic_lights().at(69)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_11_valueChanged(int value)
//{ scene->getTraffic_lights().at(75)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_12_valueChanged(int value)
//{ scene->getTraffic_lights().at(81)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_13_valueChanged(int value)
//{ scene->getTraffic_lights().at(87)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_14_valueChanged(int value)
//{ scene->getTraffic_lights().at(93)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_15_valueChanged(int value)
//{ scene->getTraffic_lights().at(99)->setTraffic_value(value/100.0); }
//void Dialog::on_p4_horizontalSlider_16_valueChanged(int value)
//{ scene->getTraffic_lights().at(105)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_1_valueChanged(int value)
//{ scene->getTraffic_lights().at(16)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_2_valueChanged(int value)
//{ scene->getTraffic_lights().at(22)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_3_valueChanged(int value)
//{ scene->getTraffic_lights().at(28)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_4_valueChanged(int value)
//{ scene->getTraffic_lights().at(34)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_5_valueChanged(int value)
//{ scene->getTraffic_lights().at(40)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_6_valueChanged(int value)
//{ scene->getTraffic_lights().at(46)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_7_valueChanged(int value)
//{ scene->getTraffic_lights().at(52)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_8_valueChanged(int value)
//{ scene->getTraffic_lights().at(58)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_9_valueChanged(int value)
//{ scene->getTraffic_lights().at(64)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_10_valueChanged(int value)
//{ scene->getTraffic_lights().at(70)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_11_valueChanged(int value)
//{ scene->getTraffic_lights().at(76)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_12_valueChanged(int value)
//{ scene->getTraffic_lights().at(82)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_13_valueChanged(int value)
//{ scene->getTraffic_lights().at(88)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_14_valueChanged(int value)
//{ scene->getTraffic_lights().at(94)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_15_valueChanged(int value)
//{ scene->getTraffic_lights().at(100)->setTraffic_value(value/100.0); }
//void Dialog::on_p5_horizontalSlider_16_valueChanged(int value)
//{ scene->getTraffic_lights().at(106)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_1_valueChanged(int value)
//{ scene->getTraffic_lights().at(17)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_2_valueChanged(int value)
//{ scene->getTraffic_lights().at(23)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_3_valueChanged(int value)
//{ scene->getTraffic_lights().at(29)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_4_valueChanged(int value)
//{ scene->getTraffic_lights().at(35)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_5_valueChanged(int value)
//{ scene->getTraffic_lights().at(41)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_6_valueChanged(int value)
//{ scene->getTraffic_lights().at(47)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_7_valueChanged(int value)
//{ scene->getTraffic_lights().at(53)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_8_valueChanged(int value)
//{ scene->getTraffic_lights().at(59)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_9_valueChanged(int value)
//{ scene->getTraffic_lights().at(65)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_10_valueChanged(int value)
//{ scene->getTraffic_lights().at(71)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_11_valueChanged(int value)
//{ scene->getTraffic_lights().at(77)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_12_valueChanged(int value)
//{ scene->getTraffic_lights().at(83)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_13_valueChanged(int value)
//{ scene->getTraffic_lights().at(89)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_14_valueChanged(int value)
//{ scene->getTraffic_lights().at(95)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_15_valueChanged(int value)
//{ scene->getTraffic_lights().at(101)->setTraffic_value(value/100.0); }
//void Dialog::on_p6_horizontalSlider_16_valueChanged(int value)
//{ scene->getTraffic_lights().at(107)->setTraffic_value(value/100.0); }
*/

void Dialog::on_debugDisplayAlternativePathsCheckBox_toggled(bool checked)
{
    scene->globals.DEBUG_DISPLAY_ALTERNATIVE_PATHS = checked;
}

void Dialog::on_debugDisplayGridSquaresCheckBox_toggled(bool checked)
{
    scene->globals.DEBUG_DISPLAY_GRIDSQUARES = checked;
    scene->update();
}

void Dialog::on_debugDisplayNodesCheckBox_toggled(bool checked)
{
    scene->globals.DEBUG_DISPLAY_NODES = checked;
    if(!checked) {
        scene->removeDebugItems();
    } else if(checked) {
        scene->addDebugItems();
    }
}

void Dialog::on_mobile_radioButton_clicked()
{
    scene->globals.FIXED = false;
    scene->globals.MOBILE = true;
    scene->setTrafficLightDependants();
}

void Dialog::on_fixed_radioButton_clicked()
{
    scene->globals.MOBILE = false;
    scene->globals.FIXED = true;
    scene->setTrafficLightDependants();
}


