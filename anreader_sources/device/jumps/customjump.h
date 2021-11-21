#ifndef CUSTOMJUMP_H
#define CUSTOMJUMP_H

#include <QtCore>
#include <QDateTime>
#include <math.h>
#include <memory>


class CustomJump;

enum CustomJumpNames: int {
                            JumpNumber = 0,
                            JumpDate,
                            DZ,
                            AC,
                            Canopy,
                            CtUnk
                           };


typedef std::vector<std::pair<QString, QVariant>> t_jump_attribute;

typedef std::shared_ptr<CustomJump> ptr_jump;
typedef std::vector<ptr_jump> t_jumps;

class CustomJump
{
public:
    explicit CustomJump();
    explicit CustomJump(uint jump_number, QDateTime jump_date, QString dz, QString ac) :
        m_jump_number(jump_number),
        m_jump_date(jump_date),
        m_dz(dz),
        m_ac(ac),
        m_canopy("") {}

    virtual ~CustomJump(){}

    uint getJumpNumber() const {return m_jump_number;}
    QDateTime getJumpDate() const {return m_jump_date;}
    virtual uint getExitAlt() const = 0;
    virtual uint getDeplAlt() const = 0;
    virtual uint getFreefallTime() const = 0;
    virtual uint getCanopyTime() const = 0;
    virtual bool isDeleted() const = 0;
    virtual QString getNote() const = 0;
    virtual QString getDZ() const = 0;
    virtual QString getAC() const = 0;
    virtual QString getCanopy() const = 0;


    void setJumpNumber(uint value) {m_jump_number = value;}
    void setJumpDate(QDateTime value) {m_jump_date = value;}
    virtual void setExitAlt(uint value) = 0;
    virtual void setDeplAlt(uint value) = 0;
    virtual void setFreefallTime(uint value) = 0;
    virtual void setCanopyTime(uint value) = 0;
    virtual void setDeleted(bool value) = 0;
    virtual void setNote(const QString& value) = 0;
    virtual void setDZ(const QString& value) = 0;
    virtual void setAC(const QString& value) = 0;
    virtual void setCanopy(const QString& value) = 0;


    virtual std::unique_ptr<t_jump_attribute> getPairs() const;
    virtual void setPairs(const t_jump_attribute& pairs);
    virtual const QString& field_name(const int n_field) const;

protected:    

    uint m_jump_number;
    QDateTime m_jump_date;
    QString m_dz;
    QString m_ac;
    QString m_canopy;
};

#endif // CUSTOMJUMP_H
