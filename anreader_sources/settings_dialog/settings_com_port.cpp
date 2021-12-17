#include "settings_com_port.h"
#include <QIntValidator>
#include <QSerialPort>
#include <QLineEdit>


//--------------------------------------------------------------------------------------------------------------------------------
SettingsCOMPort::SettingsCOMPort(QWidget *parent) : QWidget(parent),
                                                    m_intValidator(new QIntValidator(0, 4000000, this))
{
    setupUi(this);

    baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(baudRateBox,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingsCOMPort::checkCustomBaudRatePolicy);

    fillPortsParameters();
}


//--------------------------------------------------------------------------------------------------------------------------------
void SettingsCOMPort::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !baudRateBox->itemData(idx).isValid();
    baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        baudRateBox->clearEditText();
        QLineEdit *edit = baudRateBox->lineEdit();
        edit->setValidator(m_intValidator);
    }
}



//--------------------------------------------------------------------------------------------------------------------------------
void SettingsCOMPort::fillPortsParameters()
{
    baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudRateBox->addItem(tr("Custom"));

    dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsBox->setCurrentIndex(3);

    parityBox->addItem(tr("None"), QSerialPort::NoParity);
    parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}
