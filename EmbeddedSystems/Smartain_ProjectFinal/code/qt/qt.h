#ifndef QT_H
#define QT_H
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QAbstractSlider>
#include <QTimer>
#include <QString>
#include <QTime>
#include <QLCDNumber>
#include <QSpinBox>
#include <QPalette>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
private slots:
    void clockTick();
    void alarmToggle();
    void alarmBam();
    void sendVal();
    void lightOnOff();
    void smartMode();
    void setSpeed();
private:
    /* Tabs */
    QTabWidget *tabs;
    QWidget *control;
    //QWidget *alarm;
   // QWidget *setting;
    QWidget *clock;

    /* Control Tab Privates */
    QVBoxLayout *controlLayout;
    QPushButton *m_button;
    QPushButton *quit_button;
    QPushButton *send_button;
    QPushButton *smart_button;
    QSlider *curtain_slider;
    QLabel *curtain_value;
    QSpinBox *curtain_speed;
    QPushButton *speed_button;
    QLabel *speed_value;

    /* Clock Tab Privates */
    QTimer *clock_current;
    QTimer *clock_alarm;
    QLCDNumber *clock_display;
    QSpinBox *set_alarm_hr;
    QSpinBox *set_alarm_min;
    QLabel *test;
    QLabel *is_set; //checks if the alarm is set
    QPushButton *alarm_set_button;
    QString *value;
    
    /* Palettes */
    QPalette *p1; //blue
    QPalette *p2; //red
    //QPalette *p3; //white 
	


    /* Alarm Tab Privates */
    QVBoxLayout *alarmLayout;


    /* Setting Tab Privates */
    QVBoxLayout *settingLayout;


};


#endif // QT_H

