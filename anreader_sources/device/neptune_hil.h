#ifndef NEPTUNE_HIL_H
#define NEPTUNE_HIL_H


#include "neptune.h"

class Neptune_HiL : public Neptune
{
    Q_OBJECT
    enum Neptune_HiL_defs: uint {jumps_rate = 500};
public:
    explicit Neptune_HiL(QString portName, QObject *parent): Neptune(portName, parent) {}
    ~Neptune_HiL() override {}

    void read_details_jumps(unsigned int jump_index, unsigned int num_jumps) override;
    void read_summary_jumps() override;
    void read_settings() override;
    void read_dropzones() override;
    void read_airplanes() override;
    void read_alarms() override;
    void read_datetime() override;
    void end_communication() override;
    uint n_iterations_by_jumps(const uint n_jumps) override;
    void write_settings() override;
    void write_dropzones() override;
    void write_airplanes() override;
    void write_summary_jumps() override;
    void write_alarms_settings() override;
    void write_alarms_names() override;

private:
    void write_to_memory(unsigned int address, unsigned int length, QByteArray &wbytes);


signals:
};

#endif // NEPTUNE_HIL_H
