#ifndef N3WIDGET_H
#define N3WIDGET_H

#include <QTextEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFrame>
#include <QDateTimeEdit>
#include <QAction>

#include "dwidget.h"
#include "device/neptune_hil.h"


class N3Widget;
class QTimer;
class QAction;

class DeviceFrame : public QFrame
{
    enum n3widget_defs: int {element_height = 34, n_rows = 8, spacing = 4, line_height = 8, button_width = 70};
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

    QFrame line_horz1;
    QFrame line_horz2;

    QDateTimeEdit dte_clock;
    QPushButton pb_set_clock;

    QSpinBox sb_number;
    QPushButton pb_read_jumps;

    std::unique_ptr<QAction> m_clock_action;

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
    virtual void setupConnects() override;
    void addDeviceFrame();
    void deleteDeviceFrame();
    void makeFrame();
    void read_last_jumps(unsigned int n_jumps);    

    DeviceFrame *device_frame = nullptr;
    std::unique_ptr<QTimer> m_clock_timer;


public slots:
    virtual void stateChanged() override;
    void read_summary_settings();
    void readed_summary_settings();
    void read_jumps();
    void clockUpdate();
    void set_datetime();
};

#endif // N3WIDGET_H
