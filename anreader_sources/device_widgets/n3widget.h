#ifndef N3WIDGET_H
#define N3WIDGET_H

#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFrame>
#include <QLineEdit>
#include <QAction>
#include <QToolButton>

#include "dwidget.h"


class N3Widget;
class QTimer;
class QAction;
class QCheckBox;
class N3Names;

class DeviceFrame : public QFrame
{
    enum n3widget_defs: int {element_height = 34, n_rows = 8, spacing = 4, line_height = 8, button_height = 24};
    enum ExpiredType: int {None = -1, OldN3 = 0, N3 = 1, AtlasRev3 = 2, Atlas = 3, Atlas2Rev5 = 4};
public:        
    explicit DeviceFrame(QWidget *parent = nullptr, ExpiredType et = None);
    ~DeviceFrame() {}

    int calcCorrectKoeff() const;

private:
    QLineEdit te_sn;
    QLineEdit te_total_jumps;
    QLineEdit te_top_jump;
    QLineEdit te_next_jump;

    QLineEdit te_total_freefall_time;
    QLineEdit te_total_canopy_time;

    QFrame line_horz0;
    QFrame line_horz1;
    QFrame line_horz2;

    QToolButton tb_settings;
    QToolButton tb_dropzones;
    QToolButton tb_aircrafts;
    QToolButton tb_alarms;

    QLineEdit le_clock;
    QToolButton pb_edit_clock;

    QSpinBox sb_number;
    QToolButton pb_read_jumps;

    QAction* m_set_clock_action = nullptr;

    QCheckBox* m_cbCorrectDate = nullptr;
    ExpiredType m_et = ExpiredType::None;

protected:

    friend class N3Widget;

signals:

public slots:

};


//==================================================================================================================


class N3Widget : public DWidget
{
    Q_OBJECT
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
    void N3Names_dialog(N3Names& names);

    DeviceFrame *device_frame = nullptr;
    std::unique_ptr<QTimer> m_clock_timer;

signals:
    void device_disconnect();

public slots:
    virtual void stateChanged() override;
    void read_summary_settings();
    void readed_summary_settings();
    void read_jumps();
    void clockUpdate();
    void set_current_datetime();
    void choice_datetime();
    void N3Settings();
    void N3Dropzones();
    void N3Airplanes();
    void N3Alarms();
    void edit_alarms();
};

#endif // N3WIDGET_H
