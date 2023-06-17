#ifndef N3JUMP_H
#define N3JUMP_H

#include "customjump.h"


enum N3JumpNames: int {
                        ExitAlt = CustomJumpNames::CtUnk,
                        DeplAlt,
                        FreefallTime,
                        CanopyTime,
                        Speed12K,
                        Speed9K,
                        Speed6K,
                        Speed3K,
                        Unk
                       };


class N3Jump : public CustomJump
{
public:
    explicit N3Jump();
    explicit N3Jump(uint jump_number, QDateTime jump_date, QString dz, QString ap, uint exit_alt, uint depl_alt, uint freefall_time, uint canopy_time, uint speed12K, uint speed9K, uint speed6K, uint speed3K, bool is_deleted) :
        CustomJump(jump_number, jump_date, dz, ap, is_deleted),
        m_exit_alt(exit_alt),
        m_depl_alt(depl_alt),
        m_freefall_time(freefall_time),
        m_canopy_time(canopy_time),
        m_speed12K(speed12K),
        m_speed9K(speed9K),
        m_speed6K(speed6K),
        m_speed3K(speed3K) { }

    ~N3Jump() override {}

    uint getExitAlt() const override {return m_exit_alt;}
    uint getDeplAlt() const override {return m_depl_alt;}
    uint getFreefallTime() const override {return m_freefall_time;}
    uint getCanopyTime() const override {return m_canopy_time;}
    inline uint getSpeed12K() const {return m_speed12K;}
    inline uint getSpeed9K() const {return m_speed9K;}
    inline uint getSpeed6K() const {return m_speed6K;}
    inline uint getSpeed3K() const {return m_speed3K;}

    void setExitAlt(uint value) override {m_exit_alt = value;}
    void setDeplAlt(uint value) override {m_depl_alt = value;}
    void setFreefallTime(uint value) override {m_freefall_time = value;}
    void setCanopyTime(uint value) override {m_canopy_time = value;}
    inline void setSpeed12K(uint value) {m_speed12K = value;}
    inline void setSpeed9K(uint value) {m_speed9K = value;}
    inline void setSpeed6K(uint value) {m_speed6K = value;}
    inline void setSpeed3K(uint value) {m_speed3K = value;}

    std::unique_ptr<t_jump_attribute> getPairs() const override;
    virtual void setPairs(const t_jump_attribute& pairs) override;
    static const QString& field_name(const int n_field);
    static size_t index(const QString& field_name);

private:   
    uint m_exit_alt;
    uint m_depl_alt;
    uint m_freefall_time;
    uint m_canopy_time;
    uint m_speed12K;
    uint m_speed9K;
    uint m_speed6K;
    uint m_speed3K;

};

#endif // N3JUMP_H
