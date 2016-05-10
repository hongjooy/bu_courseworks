#include "qt.h"
#include <QCoreApplication>
#include <QWidget>
#include <QtGui>
#include <QTime>
#include <QtCore>
#include <QSlider>
#include <QString>
#include <QPoint>
#include <QLCDNumber>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool alarm_flag = false;
int alarm_hr;
int alarm_min;
int current_hr;
int current_min;
int send_num;
int send_speed;
char line[10];
char lux[10];
bool light = false; //off
QString temp;
QString temp_s;
bool smart = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    tabs = new QTabWidget(this);
    tabs->setFixedSize(475,270);
    control = new QWidget(this);
   // alarm = new QWidget(this);
   // setting = new QWidget(this);
    clock = new QWidget(this);

    
    tabs->addTab(control, tr("Control"));
    tabs->addTab(clock, tr("Clock"));
    //tabs->addTab(alarm, tr("Alarm"));
   /// tabs->addTab(setting, tr("Settings"));

    /*------------- Control Tab ----------------*/
    controlLayout = new QVBoxLayout(control);
    controlLayout->setAlignment(Qt::AlignCenter);
    curtain_slider = new QSlider(Qt::Horizontal);

    curtain_slider->setRange(0,99);
    curtain_value = new QLabel("0",control);
    quit_button = new QPushButton("Light", control);
    curtain_value->setGeometry(QRect(QPoint(450, 90),QSize(50, 50)));
    p1 = new QPalette();
    p2 = new QPalette();
   // p3 = new QPalette();
    p1->setColor(QPalette::Button, Qt::blue);
    p2->setColor(QPalette::Button, Qt::red);
   // p3->setColor(QPalette::Text, Qt::white);
    

    curtain_speed = new QSpinBox(control);
    curtain_speed->setRange(1,5);
    curtain_speed->setGeometry(QRect(QPoint(100, 40),QSize(50, 50)));
    speed_button = new QPushButton("Set Speed",control);
    speed_button->setGeometry(QRect(QPoint(200, 40),QSize(100, 50)));
    speed_value = new QLabel("",control);
    send_button = new QPushButton("Go",control);
    smart_button = new QPushButton("SmartMode",control);
    
    
    // Make them pretty
    quit_button->setGeometry(QRect(QPoint(50, 150),QSize(100, 50)));
    send_button->setGeometry(QRect(QPoint(200, 150),QSize(100, 50)));
    smart_button->setGeometry(QRect(QPoint(350,150),QSize(100, 50)));
    controlLayout->addWidget(curtain_slider);

    smart_button->setPalette(*p1);
    //smart_button->setPalette(*p3);
    quit_button->setPalette(*p1);
    send_button->setPalette(*p1);
    speed_button->setPalette(*p1);
    /*-------------------- Clock Tab ----------------------*/
    clock_alarm = new QTimer(clock);
    clock_alarm->start(1000); //every minute

    clock_display = new QLCDNumber(clock);
    clock_display->setGeometry(70,20,200,200);
    clock_current = new QTimer(clock_display);
    clock_display->setFixedSize(300,100);
    clock_display->setNumDigits(8);
    clock_display->display(QTime::currentTime().toString(QString("hh:mm:ss")));
    clock_current->start(1000);

    set_alarm_hr = new QSpinBox(clock);
    set_alarm_hr->setGeometry(QRect(QPoint(60, 150),QSize(70, 40)));
    set_alarm_hr->setRange(0,23);
    set_alarm_min = new QSpinBox(clock);
    set_alarm_min->setGeometry(QRect(QPoint(150, 150),QSize(70, 40)));
    set_alarm_min->setRange(0,59);
    alarm_set_button = new QPushButton("Alarm",clock);
    alarm_set_button->setGeometry(QRect(QPoint(290, 150),QSize(70, 50)));
    

    /* Alarm Going Off Testing */
    test = new QLabel("alarm on hold",clock);
    is_set = new QLabel("                        off",clock);

    /*--------------- Signals & Slots ----------------*/
    //connect(quit_button, SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(quit_button, SIGNAL(clicked(bool)),this,SLOT(lightOnOff()));
    connect(smart_button, SIGNAL(clicked(bool)),this,SLOT(smartMode()));
    connect(clock_current,SIGNAL(timeout()),this,SLOT(clockTick()));
    connect(clock_alarm,SIGNAL(timeout()),this,SLOT(alarmBam()));
    connect(alarm_set_button,SIGNAL(clicked(bool)),this,SLOT(alarmToggle()));
    connect(send_button,SIGNAL(clicked(bool)),this,SLOT(sendVal()));
    connect(speed_button,SIGNAL(clicked(bool)),this,SLOT(setSpeed()));
    QObject::connect(curtain_slider,SIGNAL(valueChanged(int)),curtain_value,SLOT(setNum(int)));
    QObject::connect(curtain_speed,SIGNAL(valueChanged(int)),speed_value,SLOT(setNum(int)));
    //connect(curtain_slider,SIGNAL(valueChanged(int)),this,SLOT(sendVal()));
	
}

void MainWindow::clockTick()
{

    clock_display->display(QTime::currentTime().toString(QString("hh:mm:ss")));

	
}

void MainWindow::alarmToggle()
{

    alarm_hr = set_alarm_hr->value();
    alarm_min = set_alarm_min->value();

    if(alarm_flag){
        is_set->setText("                     off");
        alarm_set_button->setPalette(*p1);
        alarm_flag = false;
    }
    else{
        is_set->setText("                     on");
        alarm_set_button->setPalette(*p2);
        alarm_flag = true;
    }
}
void MainWindow::alarmBam()
{
    current_hr = QTime::currentTime().hour();
    current_min = QTime::currentTime().minute();


    if((current_hr==alarm_hr)&&(current_min==alarm_min)&&(alarm_flag)){
        test->setText("going off");
	FILE *fp;
    	fp = fopen("/dev/mygpio", "w");
	//printf("alarm pops\n");
  	fputs("v99",fp);
    	fclose(fp);
	curtain_slider->setValue(99);
	alarm_flag = false;

    }
    else{
        test->setText("alarm on hold");
    }

}

void MainWindow::sendVal()
{
    temp = curtain_value->text();
    send_num = temp.toInt();
 //   printf ("%d\n",send_num);
    sprintf(line,"v%d", send_num);
    //printf("%s\n",line);
    //printf("test\n");
    FILE *fp;
    fp = fopen("/dev/mygpio", "w");
    fputs(line,fp);
    fclose(fp);
    
}
void MainWindow::setSpeed()
{
    temp_s = speed_value->text();
    send_speed = temp_s.toInt();
 //   printf ("%d\n",send_num);
    sprintf(line,"a%d", send_speed);
   // printf("%s\n",line);
    //printf("test\n");
    FILE *fp;
    fp = fopen("/dev/mygpio", "w");
    fputs(line,fp);
    fclose(fp);
    
}



void MainWindow::lightOnOff()
{
    	if (light){ //if light is on
	
    		FILE *fp;
    		fp = fopen("/dev/mygpio", "w");
    		fputs("f0",fp);
		quit_button->setPalette(*p1);
		//printf ("sending f0\n");
    		fclose(fp);
		light = false;
   	 }
	else{ //if light is off
		FILE *fp;
    		fp = fopen("/dev/mygpio", "w");
    		fputs("f1",fp);
		quit_button->setPalette(*p2);
		//printf("sending f1\n");
		light = true;
    		fclose(fp);
	}
 
}



void MainWindow::smartMode()
{
    	if (smart){ //if light is on
	
    		FILE *fp;
    		fp = fopen("/dev/mygpio", "w");
    		fputs("s0",fp);
		//printf ("sending s0\n");
                smart_button->setPalette(*p1);
    		fclose(fp);
		smart = false;
   	 }
	else{ //if light is off
		FILE *fp;
    		fp = fopen("/dev/mygpio", "w");
                smart_button->setPalette(*p2);
    		fputs("s1",fp);
		//printf("sending s1\n");
		smart = true;
    		fclose(fp);
	}
 
}


