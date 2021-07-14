#ifndef N3WIDGET_H
#define N3WIDGET_H

#include <QTextEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>

#include "dwidget.h"
#include "device/neptune_hil.h"


class N3Widget;


class DeviceFrame : public QFrame
{
    enum n3widget_defs: int {element_height = 34, n_rows = 7, spacing = 4};
public:
    explicit DeviceFrame(QWidget *parent = nullptr);
    ~DeviceFrame() {}

private:
    QTextEdit te_sn;
    QTextEdit te_total_jumps;
    QTextEdit te_top_jump;
    QTextEdit te_next_jump;

    QTextEdit te_total_freefall_time;
    QTextEdit te_total_canopy_time;

    QSpinBox sb_number;
    QPushButton pb_read_jumps;

protected:

    friend class N3Widget;

signals:

public slots:

};


//==================================================================================================================


class N3Widget : public DWidget
{
public:
    explicit N3Widget(dtype device_type, QWidget *parent = nullptr);
    explicit N3Widget(QWidget *parent = nullptr);
    ~N3Widget() override;

    virtual void open() override;
    virtual void close() override;
    virtual void create() override;

private:
    void init();
    virtual void setupConnects() override;
    void addDeviceFrame();
    void deleteDeviceFrame();
    void makeFrame();
    void read_last_jumps(unsigned int n_jumps);    

    DeviceFrame *device_frame = nullptr;

public slots:
    virtual void stateChanged() override;
    void read_summary_settings();
    void readed_summary_settings();
    void read_jumps();
};

#endif // N3WIDGET_H
