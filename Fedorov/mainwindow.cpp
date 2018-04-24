#include "mainwindow.h"
#include <QBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    serial = new QSerialPort(this);

    label_distance = new QLabel("0");
    label_cm = new QLabel("cm");

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(label_distance);
    hLayout1->addWidget(label_cm);

    label_serialName = new QLabel("Serial port names:");
    comboBox_serialName = new QComboBox();
    FillComboBoxWithSerialPortNames();

    QVBoxLayout *vLayout1 = new QVBoxLayout;
    vLayout1->addWidget(label_serialName);
    vLayout1->addWidget(comboBox_serialName);

    button_start = new QPushButton("Start");
    button_start->setEnabled(true);
    button_stop = new QPushButton("Stop");
    button_stop->setEnabled(false);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(button_start);
    hLayout2->addWidget(button_stop);

    QVBoxLayout *vGeneralLayout = new QVBoxLayout;
    vGeneralLayout->addLayout(hLayout1);
    vGeneralLayout->addLayout(vLayout1);
    vGeneralLayout->addLayout(hLayout2);

    setLayout(vGeneralLayout);


    connect(serial, &QSerialPort::readyRead, this, &MainWindow::Slot_readFromSerialPort);
    //connect(serial, SIGNAL(readyRead()), SLOT(Slot_readFromSerialPort()));
    connect(button_start, SIGNAL(clicked(bool)), this, SLOT(Slot_distanceProgramSTART()));
    connect(button_stop, SIGNAL(clicked(bool)), this, SLOT(Slot_distanceProgramSTOP()));
}


MainWindow::~MainWindow()
{

}


void MainWindow::FillComboBoxWithSerialPortNames()
{
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
        comboBox_serialName->addItem(info.portName());

    comboBox_serialName->setCurrentIndex(0);
}


void MainWindow::Slot_readFromSerialPort()
{
    QByteArray data = serial->readAll();

    //if(!(QString(data) == "-"))
        label_distance->setText(QString(data));


}


void MainWindow::Slot_distanceProgramSTART()
{
    button_start->setEnabled(false);
    button_stop->setEnabled(true);

    initializeSerialPort();
}


void MainWindow::Slot_distanceProgramSTOP()
{
    if (serial->isOpen())
        serial->close();

    button_start->setEnabled(true);
    button_stop->setEnabled(false);
    label_distance->setText("0");
}


void MainWindow::initializeSerialPort()
{
    serial->setPortName(comboBox_serialName->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!(serial->open(QIODevice::ReadWrite)))
    {
        QMessageBox::warning(0, "Error", "Serial port not open!!!");
    }
}

