#include "mainwindow.h"
#include <QBoxLayout>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    serial = new QSerialPort(this);

    DrawGUI();

    initConnection();
}


MainWindow::~MainWindow()
{
}


void MainWindow::DrawGUI()
{
    lineEdit_distance = new QLineEdit("0");
    lineEdit_distance->setFixedWidth(25);
    label_cm = new QLabel("cm");
    label_cm->setFixedWidth(20);

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(lineEdit_distance);
    hLayout1->addWidget(label_cm);



    distanceBar = new QProgressBar;
    distanceBar->setRange(0, 19);
    //distanceBar->setMaximumWidth(150);
    distanceBar->setTextVisible(false);
    label_serialName = new QLabel("Serial port names:");
    comboBox_serialName = new QComboBox();

    FillComboBoxWithSerialPortNames();

    QVBoxLayout *vLayout1 = new QVBoxLayout;
    vLayout1->addWidget(distanceBar);
    vLayout1->addWidget(label_serialName);
    vLayout1->addWidget(comboBox_serialName);



    button_start = new QPushButton("Start");
    button_start->setEnabled(true);
    button_stop = new QPushButton("Stop");
    button_stop->setEnabled(false);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(button_start);
    hLayout2->addWidget(button_stop);



    button_refresh = new QPushButton("Refresh");
    button_refresh->setEnabled(true);
    button_exit = new QPushButton("Exit");
    button_exit->setEnabled(true);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(button_refresh);
    hLayout3->addWidget(button_exit);



    QVBoxLayout *vGeneralLayout = new QVBoxLayout;
    vGeneralLayout->addLayout(hLayout1);
    vGeneralLayout->addLayout(vLayout1);
    vGeneralLayout->addLayout(hLayout2);
    vGeneralLayout->addLayout(hLayout3);
    setLayout(vGeneralLayout);
}


void MainWindow::FillComboBoxWithSerialPortNames()
{
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
        comboBox_serialName->addItem(info.portName());

    comboBox_serialName->setCurrentIndex(0);
}


void MainWindow::initConnection()
{
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::Slot_readFromSerialPort);
    //connect(serial, SIGNAL(readyRead()), SLOT(Slot_readFromSerialPort()));
    connect(button_start, SIGNAL(clicked(bool)), this, SLOT(Slot_distanceProgramSTART()));
    connect(button_stop, SIGNAL(clicked(bool)), this, SLOT(Slot_distanceProgramSTOP()));
    connect(button_refresh, SIGNAL(clicked(bool)), this, SLOT(Slot_refresh()));
    connect(button_exit, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(lineEdit_distance, SIGNAL(textChanged(QString)), this, SLOT(Slot_distanceBarChange()));
    connect(comboBox_serialName, SIGNAL(currentIndexChanged(int)), this, SLOT(Slot_distanceProgramSTOP()));
}


void MainWindow::Slot_readFromSerialPort()
{
    QByteArray data = serial->readAll();
    //QString bufferData;
    //bufferData += data.constData();
    //if(!(QString(data) == "-"))
    //data.ass
    //label_distance->setText(bufferData);
    lineEdit_distance->setText(QString(data));

}


void MainWindow::Slot_distanceProgramSTART()
{
    button_start->setEnabled(false);
    button_stop->setEnabled(true);

    initializeSerialPort();

    CheckIsOpenSerialPort();
}


void MainWindow::initializeSerialPort()
{
    serial->setPortName(comboBox_serialName->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}


void MainWindow::CheckIsOpenSerialPort()
{
    if(!(serial->open(QIODevice::ReadWrite)))
    {
        QMessageBox::warning(0, "Error", "Serial port not open!!!");
        button_start->setEnabled(true);
        button_stop->setEnabled(false);
    }
}


void MainWindow::Slot_distanceProgramSTOP()
{
    if (serial->isOpen())
        serial->close();

    button_start->setEnabled(true);
    button_stop->setEnabled(false);
    lineEdit_distance->setText("0");
}


void MainWindow::Slot_refresh()
{
    Slot_distanceProgramSTOP();

    comboBox_serialName->clear();

    FillComboBoxWithSerialPortNames();

}


void MainWindow::Slot_distanceBarChange()
{
    distanceBar->setValue(lineEdit_distance->text().toInt(0, 10));

    if(distanceBar->value() < 7)
        distanceBar->setStyleSheet("QProgressBar::chunk{background-color: green}");
    else if(distanceBar->value() >= 7 && distanceBar->value() < 14)
        distanceBar->setStyleSheet("QProgressBar::chunk{background-color: yellow}");
    else
        distanceBar->setStyleSheet("QProgressBar::chunk{background-color: red}");
}
