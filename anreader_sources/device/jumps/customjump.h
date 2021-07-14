#ifndef CUSTOMJUMP_H
#define CUSTOMJUMP_H

#include <QtCore>
#include <QDateTime>
#include <math.h>
#include <memory>


class CustomJump;

enum CustomJumpNames: uint {JumpNumber = 0,
                        JumpDate = 1,
                        ExitAlt = 2,
                        DeplAlt = 3,
                        FreefallTime = 4,
                        CanopyTime = 5,
                        Note = 6,
                        Deleted = 7
                       };


typedef QVector<std::pair<QString, QVariant>> t_jump_attribute;
typedef std::vector<std::shared_ptr<CustomJump>> t_jumps;

class CustomJump
{
public:
    explicit CustomJump();
    explicit CustomJump(uint jump_number, QDateTime jump_date) :
        m_jump_number(jump_number),
        m_jump_date(jump_date){}

    virtual ~CustomJump(){}

    uint getJumpNumber() const {return m_jump_number;}
    QDateTime getJumpDate() const {return m_jump_date;}
    virtual uint getExitAlt() const = 0;
    virtual uint getDeplAlt() const = 0;
    virtual uint getFreefallTime() const = 0;
    virtual uint getCanopyTime() const = 0;
    virtual bool isDeleted() const = 0;
    virtual QString getNote() const = 0;

    void setJumpNumber(uint value) {m_jump_number = value;}
    void setJumpDate(QDateTime value) {m_jump_date = value;}
    virtual void setExitAlt(uint value) = 0;
    virtual void setDeplAlt(uint value) = 0;
    virtual void setFreefallTime(uint value) = 0;
    virtual void setCanopyTime(uint value) = 0;
    virtual void setDeleted(bool value) = 0;
    virtual void setNote(const QString& value) = 0;

    virtual t_jump_attribute getPairs() const;


protected:
    virtual const QString& field_name(uint n_field) const;

    uint m_jump_number;
    QDateTime m_jump_date;
};

#endif // CUSTOMJUMP_H
