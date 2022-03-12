#ifndef CUSTOMJUMP_H
#define CUSTOMJUMP_H

#include <QtCore>

class CustomJump;

enum CustomJumpNames: int {
                            JumpNumber = 0,
                            JumpDate,
                            DZ,
                            AC,
                            Canopy,
                            Deleted,
                            Note,
                            CtUnk
                           };


typedef std::vector<std::pair<QString, QVariant>> t_jump_attribute;

typedef std::shared_ptr<CustomJump> ptr_jump;
typedef std::vector<ptr_jump> t_jumps;

class CustomJump
{
public:
    explicit CustomJump();
    explicit CustomJump(uint jump_number, QDateTime jump_date, QString dz, QString ac, bool is_deleted) :
        m_jump_number(jump_number),
        m_jump_date(jump_date),
        m_dz(dz),
        m_ac(ac),
        m_canopy(""),
        m_note(""),
        m_is_deleted(is_deleted)
    {}

    virtual ~CustomJump(){}

    inline uint getJumpNumber() const {return m_jump_number;}
    inline QDateTime getJumpDate() const {return m_jump_date;}
    virtual uint getExitAlt() const = 0;
    virtual uint getDeplAlt() const = 0;
    virtual uint getFreefallTime() const = 0;
    virtual uint getCanopyTime() const = 0;
    virtual const QString& getDZ() const {return m_dz;}
    virtual const QString& getAC() const {return m_ac;}
    virtual const QString& getCanopy() const {return m_canopy;}
    virtual const QString& getType() const {return m_type;}
    virtual const QString& getNote() const {return m_note;}
    virtual bool isDeleted() const {return m_is_deleted;}


    inline void setJumpNumber(uint value) {m_jump_number = value;}
    inline void setJumpDate(QDateTime value) {m_jump_date = value;}
    virtual void setExitAlt(uint value) = 0;
    virtual void setDeplAlt(uint value) = 0;
    virtual void setFreefallTime(uint value) = 0;
    virtual void setCanopyTime(uint value) = 0;
    virtual void setDZ(const QString& value) {m_dz = value;}
    virtual void setAC(const QString& value) {m_ac = value;}
    virtual void setCanopy(const QString& value) {m_canopy = value; }
    virtual void setType(const QString& value) {m_type = value; }
    virtual void setNote(const QString& value) {m_note = value;}
    virtual void setDeleted(bool value) {m_is_deleted = value;}

    virtual std::unique_ptr<t_jump_attribute> getPairs() const = 0;
    virtual void setPairs(const t_jump_attribute& pairs);
    static const QString& field_name(const int n_field);
    static int index(const QString& field_name);

protected:    

    uint m_jump_number;
    QDateTime m_jump_date;
    QString m_dz;
    QString m_ac;
    QString m_canopy;
    QString m_type;
    QString m_note;
    bool m_is_deleted;
};

#endif // CUSTOMJUMP_H
