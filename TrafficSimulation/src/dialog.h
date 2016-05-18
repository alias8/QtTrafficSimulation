#ifndef DIALOG_H
#define DIALOG_H

#include <QtCore>
#include <QDialog>
#include <QtGui>
#include <QListWidget>
#include <QGraphicsScene>
#include <QTimer>
#include "simulationcontroller.h"
#include "trafficlight.h"
#include "train.h"

namespace Ui
{
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void timer_spawner();
    ~Dialog();

private:
    Ui::Dialog *ui;
    SimulationController *scene;
    QTimer *timer;
    bool random_spawner;


private slots:

  void on_zoomHorizontalSlider_valueChanged(int value);
  void on_s1_button_clicked();
  void on_s2_button_clicked();
  void on_s3_button_clicked();
  void on_s4_button_clicked();
  void on_s5_button_clicked();
  void on_s6_button_clicked();
  void on_s7_button_clicked();
  void on_s8_button_clicked();
  void on_despawn_button_clicked();
  void on_spawnAll_button_clicked();
  void on_random_spawn_button_clicked();
  void on_random_spawn_spinBox_valueChanged(double arg1);
  void on_maxPeopleSlider_valueChanged(int value);

//  void on_concourse_horizontalSlider_1_valueChanged(int value);
//  void on_concourse_horizontalSlider_2_valueChanged(int value);
//  void on_concourse_horizontalSlider_3_valueChanged(int value);
//  void on_concourse_horizontalSlider_4_valueChanged(int value);
//  void on_concourse_horizontalSlider_5_valueChanged(int value);
//  void on_concourse_horizontalSlider_6_valueChanged(int value);
//  void on_concourse_horizontalSlider_7_valueChanged(int value);
//  void on_concourse_horizontalSlider_8_valueChanged(int value);
//  void on_concourse_horizontalSlider_9_valueChanged(int value);
//  void on_concourse_horizontalSlider_10_valueChanged(int value);
//  void on_concourse_horizontalSlider_11_valueChanged(int value);
//  void on_concourse_horizontalSlider_12_valueChanged(int value);
  void on_p1_horizontalSlider_1_valueChanged(int value);
  void on_p1_horizontalSlider_2_valueChanged(int value);
  void on_p1_horizontalSlider_3_valueChanged(int value);
  void on_p1_horizontalSlider_4_valueChanged(int value);
  void on_p1_horizontalSlider_5_valueChanged(int value);
  void on_p1_horizontalSlider_6_valueChanged(int value);
  void on_p1_horizontalSlider_7_valueChanged(int value);
  void on_p1_horizontalSlider_8_valueChanged(int value);
  void on_p1_horizontalSlider_9_valueChanged(int value);
  void on_p1_horizontalSlider_10_valueChanged(int value);
  void on_p1_horizontalSlider_11_valueChanged(int value);
  void on_p1_horizontalSlider_12_valueChanged(int value);
  void on_p1_horizontalSlider_13_valueChanged(int value);
  void on_p1_horizontalSlider_14_valueChanged(int value);
  void on_p1_horizontalSlider_15_valueChanged(int value);
  void on_p1_horizontalSlider_16_valueChanged(int value);
  void on_p2_horizontalSlider_1_valueChanged(int value);
  void on_p2_horizontalSlider_2_valueChanged(int value);
  void on_p2_horizontalSlider_3_valueChanged(int value);
  void on_p2_horizontalSlider_4_valueChanged(int value);
  void on_p2_horizontalSlider_5_valueChanged(int value);
  void on_p2_horizontalSlider_6_valueChanged(int value);
  void on_p2_horizontalSlider_7_valueChanged(int value);
  void on_p2_horizontalSlider_8_valueChanged(int value);
  void on_p2_horizontalSlider_9_valueChanged(int value);
  void on_p2_horizontalSlider_10_valueChanged(int value);
  void on_p2_horizontalSlider_11_valueChanged(int value);
  void on_p2_horizontalSlider_12_valueChanged(int value);
  void on_p2_horizontalSlider_13_valueChanged(int value);
  void on_p2_horizontalSlider_14_valueChanged(int value);
  void on_p2_horizontalSlider_15_valueChanged(int value);
  void on_p2_horizontalSlider_16_valueChanged(int value);
  void on_p3_horizontalSlider_1_valueChanged(int value);
  void on_p3_horizontalSlider_2_valueChanged(int value);
  void on_p3_horizontalSlider_3_valueChanged(int value);
  void on_p3_horizontalSlider_4_valueChanged(int value);
  void on_p3_horizontalSlider_5_valueChanged(int value);
  void on_p3_horizontalSlider_6_valueChanged(int value);
  void on_p3_horizontalSlider_7_valueChanged(int value);
  void on_p3_horizontalSlider_8_valueChanged(int value);
  void on_p3_horizontalSlider_9_valueChanged(int value);
  void on_p3_horizontalSlider_10_valueChanged(int value);
  void on_p3_horizontalSlider_11_valueChanged(int value);
  void on_p3_horizontalSlider_12_valueChanged(int value);
  void on_p3_horizontalSlider_13_valueChanged(int value);
  void on_p3_horizontalSlider_14_valueChanged(int value);
  void on_p3_horizontalSlider_15_valueChanged(int value);
  void on_p3_horizontalSlider_16_valueChanged(int value);

  // TEMP: Cropped for platforms 1, 2 and 3
//  void on_p4_horizontalSlider_1_valueChanged(int value);
//  void on_p4_horizontalSlider_2_valueChanged(int value);
//  void on_p4_horizontalSlider_3_valueChanged(int value);
//  void on_p4_horizontalSlider_4_valueChanged(int value);
//  void on_p4_horizontalSlider_5_valueChanged(int value);
//  void on_p4_horizontalSlider_6_valueChanged(int value);
//  void on_p4_horizontalSlider_7_valueChanged(int value);
//  void on_p4_horizontalSlider_8_valueChanged(int value);
//  void on_p4_horizontalSlider_9_valueChanged(int value);
//  void on_p4_horizontalSlider_10_valueChanged(int value);
//  void on_p4_horizontalSlider_11_valueChanged(int value);
//  void on_p4_horizontalSlider_12_valueChanged(int value);
//  void on_p4_horizontalSlider_13_valueChanged(int value);
//  void on_p4_horizontalSlider_14_valueChanged(int value);
//  void on_p4_horizontalSlider_15_valueChanged(int value);
//  void on_p4_horizontalSlider_16_valueChanged(int value);
//  void on_p5_horizontalSlider_1_valueChanged(int value);
//  void on_p5_horizontalSlider_2_valueChanged(int value);
//  void on_p5_horizontalSlider_3_valueChanged(int value);
//  void on_p5_horizontalSlider_4_valueChanged(int value);
//  void on_p5_horizontalSlider_5_valueChanged(int value);
//  void on_p5_horizontalSlider_6_valueChanged(int value);
//  void on_p5_horizontalSlider_7_valueChanged(int value);
//  void on_p5_horizontalSlider_8_valueChanged(int value);
//  void on_p5_horizontalSlider_9_valueChanged(int value);
//  void on_p5_horizontalSlider_10_valueChanged(int value);
//  void on_p5_horizontalSlider_11_valueChanged(int value);
//  void on_p5_horizontalSlider_12_valueChanged(int value);
//  void on_p5_horizontalSlider_13_valueChanged(int value);
//  void on_p5_horizontalSlider_14_valueChanged(int value);
//  void on_p5_horizontalSlider_15_valueChanged(int value);
//  void on_p5_horizontalSlider_16_valueChanged(int value);
//  void on_p6_horizontalSlider_1_valueChanged(int value);
//  void on_p6_horizontalSlider_2_valueChanged(int value);
//  void on_p6_horizontalSlider_3_valueChanged(int value);
//  void on_p6_horizontalSlider_4_valueChanged(int value);
//  void on_p6_horizontalSlider_5_valueChanged(int value);
//  void on_p6_horizontalSlider_6_valueChanged(int value);
//  void on_p6_horizontalSlider_7_valueChanged(int value);
//  void on_p6_horizontalSlider_8_valueChanged(int value);
//  void on_p6_horizontalSlider_9_valueChanged(int value);
//  void on_p6_horizontalSlider_10_valueChanged(int value);
//  void on_p6_horizontalSlider_11_valueChanged(int value);
//  void on_p6_horizontalSlider_12_valueChanged(int value);
//  void on_p6_horizontalSlider_13_valueChanged(int value);
//  void on_p6_horizontalSlider_14_valueChanged(int value);
//  void on_p6_horizontalSlider_15_valueChanged(int value);
//  void on_p6_horizontalSlider_16_valueChanged(int value);
  void on_normalDemographicSlider_valueChanged(int value);
  void on_businessmanDemographicSlider_valueChanged(int value);
  void on_touristDemographicSlider_valueChanged(int value);
  void on_trains_p1_off_spinBox_valueChanged(int arg1);
  void on_trains_p2_off_spinBox_valueChanged(int arg1);
  void on_trains_p3_off_spinBox_valueChanged(int arg1);
  void on_trains_p1_on_spinBox_valueChanged(int arg1);
  void on_trains_p2_on_spinBox_valueChanged(int arg1);
  void on_trains_p3_on_spinBox_valueChanged(int arg1);
  void on_trains_p1_pushButton_clicked();
  void on_trains_p2_pushButton_clicked();
  void on_trains_p3_pushButton_clicked();
  void on_hide_sideBar_pushButton_toggled(bool checked);
  void on_manualRadioButton_clicked();
  void on_offRadioButton_clicked();
  void on_randomRadioButton_clicked();
  void on_densityRadioButton_clicked();
  void on_trains_p1_passengers_spinBox_valueChanged(int arg1);
  void on_trains_p2_passengers_spinBox_valueChanged(int arg1);
  void on_trains_p3_passengers_spinBox_valueChanged(int arg1);
  void on_speedHorizontalSlider_valueChanged(int value);
  void on_start_timer_pushButton_toggled(bool checked);
  void on_reset_timer_pushButton_clicked();
  void on_about_pushButton_clicked();
  void on_fastForward_pushButton_toggled(bool checked);
  void on_exp1_start_pushButton_clicked();
  void on_exp2_start_pushButton_clicked();
  void on_distribution_p1_left_radioButton_clicked();
  void on_distribution_p1_middle_radioButton_clicked();
  void on_distribution_p1_right_radioButton_clicked();
  void on_distribution_p1_ends_radioButton_clicked();
  void on_distribution_p1_double_radioButton_clicked();
  void on_distribution_p1_even_radioButton_clicked();
  void on_distribution_p2_left_radioButton_clicked();
  void on_distribution_p2_middle_radioButton_clicked();
  void on_distribution_p2_right_radioButton_clicked();
  void on_distribution_p2_ends_radioButton_clicked();
  void on_distribution_p2_double_radioButton_clicked();
  void on_distribution_p2_even_radioButton_clicked();
  void on_distribution_p3_left_radioButton_clicked();
  void on_distribution_p3_middle_radioButton_clicked();
  void on_distribution_p3_right_radioButton_clicked();
  void on_distribution_p3_ends_radioButton_clicked();
  void on_distribution_p3_double_radioButton_clicked();
  void on_distribution_p3_even_radioButton_clicked();
  void on_distributionRadioButton_clicked();
  void on_densityCheckSlider_valueChanged(int value);
  void on_debugDisplayAlternativePathsCheckBox_toggled(bool checked);
  void on_debugDisplayGridSquaresCheckBox_toggled(bool checked);
  void on_debugDisplayNodesCheckBox_toggled(bool checked);
  void on_mobile_radioButton_clicked();
  void on_fixed_radioButton_clicked();
  void on_exp3_start_pushButton_clicked();
};

#endif // DIALOG_H
