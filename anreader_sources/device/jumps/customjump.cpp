#include "customjump.h"

static QString CustomJumpFieldNames[] =
                              {
                               QObject::tr("Number"),
                               QObject::tr("Date")
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

void CustomJump::setPairs(const t_jump_attribute &pairs)
{
    if(pairs.size() >= CustomJumpNames::JumpDate)
    {
        if((pairs.at(CustomJumpNames::JumpNumber).second).canConvert(QMetaType::Int))
           m_jump_number = (pairs.at(CustomJumpNames::JumpNumber).second).toUInt();

        if((pairs.at(CustomJumpNames::JumpDate).second).canConvert(QMetaType::QDateTime))
           m_jump_date = (pairs.at(CustomJumpNames::JumpDate).second).toDateTime();
    }
}

const QString &CustomJump::field_name(uint n_field) const
{
    return CustomJumpFieldNames[n_field <= CustomJumpNames::JumpDate ? n_field : 0];
}

