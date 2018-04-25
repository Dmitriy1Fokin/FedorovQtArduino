#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QPushButton *button_start;
    QPushButton *button_stop;
    QLabel *label_distance;
    QLabel *label_cm;
    QLabel *label_serialName;
    QComboBox *comboBox_serialName;
    QSerialPort *serial;

    void FillComboBoxWithSerialPortNames();
    void initializeSerialPort();

    QProgressBar *distanceBar; // may be delete


private slots:
    void Slot_readFromSerialPort();
    void Slot_distanceProgramSTART();
    void Slot_distanceProgramSTOP();
    void Slot_distanceBarChange();
};

#endif // MAINWINDOW_H
