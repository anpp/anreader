#include "customjump.h"

static QString CustomJumpFieldNames[] = {QObject::tr("Number"),
                               QObject::tr("Date"),
                               QObject::tr("Exit altitude"),
                               QObject::tr("Deploy altitude"),
                               QObject::tr("Freefall time"),
                               QObject::tr("Canopy time"),
                               QObject::tr("Note"),
                               QObject::tr("Deleted"),
                              };


//----------------------------------------------------------------------------------------------------------------------
CustomJump::CustomJump()
{

}

//----------------------------------------------------------------------------------------------------------------------
t_jump_attribute CustomJump::getPairs() const
{
    t_jump_attribute jump_attr;

    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpNumber), m_jump_number));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpDate), m_jump_date));

    return jump_attr;
}

const QString &CustomJump::field_name(uint n_field) const
{
    return CustomJumpFieldNames[n_field <= CustomJumpNames::Deleted ? n_field : 0];
}

