#ifndef N3JUMP_H
#define N3JUMP_H

#include "customjump.h"


enum N3JumpNames: uint {Speed12K = 0,
                        Speed9K = 1,
                        Speed6K = 2,
                        Speed3K = 3
                       };


class N3Jump : public CustomJump
{
public:
    explicit N3Jump();
    explicit N3Jump(uint jump_number, QDateTime jump_date, uint exit_alt, uint depl_alt, uint freefall_time, uint canopy_time, uint speed12K, uint speed9K, uint speed6K, uint speed3K, bool is_deleted) :
        CustomJump(jump_number, jump_date),
        m_exit_alt(exit_alt),
        m_depl_alt(depl_alt),
        m_freefall_time(freefall_time),
        m_canopy_time(canopy_time),
        m_speed12K(speed12K),
        m_speed9K(speed9K),
        m_speed6K(speed6K),
        m_speed3K(speed3K),
        m_note(""),
        m_is_deleted(is_deleted) { }

    ~N3Jump() override { }

    uint getExitAlt() const override {return m_exit_alt;}
    uint getDeplAlt() const override {return m_depl_alt;}
    uint getFreefallTime() const override {return m_freefall_time;}
    uint getCanopyTime() const override {return m_canopy_time;}
    uint getSpeed12K() const {return m_speed12K;}
    uint getSpeed9K() const {return m_speed9K;}
    uint getSpeed6K() const {return m_speed6K;}
    uint getSpeed3K() const {return m_speed3K;}
    QString getNote() const override {return m_note;}
    bool isDeleted() const override {return m_is_deleted;}

    void setExitAlt(uint value) override {m_exit_alt = value;}
    void setDeplAlt(uint value) override {m_depl_alt = value;}
    void setFreefallTime(uint value) override {m_freefall_time = value;}
    void setCanopyTime(uint value) override {m_canopy_time = value;}
    void setSpeed12K(uint value) {m_speed12K = value;}
    void setSpeed9K(uint value) {m_speed9K = value;}
    void setSpeed6K(uint value) {m_speed6K = value;}
    void setSpeed3K(uint value) {m_speed3K = value;}
    void setNote(const QString& value) override {m_note = value;}
    void setDeleted(bool value) override {m_is_deleted = value;}

    t_jump_attribute getPairs() const override;

private:
    virtual const QString& field_name(uint n_field) const override;

    uint m_exit_alt;
    uint m_depl_alt;
    uint m_freefall_time;
    uint m_canopy_time;
    uint m_speed12K;
    uint m_speed9K;
    uint m_speed6K;
    uint m_speed3K;
    QString m_note;
    bool m_is_deleted;

};

#endif // N3JUMP_H
