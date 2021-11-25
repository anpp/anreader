#include "customjump.h"

const static QString CustomJumpFieldNames[] =
                              {
                                QObject::tr("Number"),
                                QObject::tr("Date"),
                                QObject::tr("DZ"),
                                QObject::tr("Aircraft"),
                                QObject::tr("Canopy"),
                                QObject::tr("Deleted"),
                                QObject::tr("Note"),
                                QObject::tr("Unknown")
                              };


//----------------------------------------------------------------------------------------------------------------------
CustomJump::CustomJump()
{

}


//----------------------------------------------------------------------------------------------------------------------
void CustomJump::setPairs(const t_jump_attribute &pairs)
{
    for(int i = 0; i < CustomJumpNames::CtUnk; ++i)
    {
        auto found_atr = std::find_if(pairs.begin(), pairs.end(), [i] (const auto &atr)
        {
            return atr.first == CustomJumpFieldNames[i];
        });

        if(found_atr != pairs.end())
        {
            if(CustomJumpNames::JumpNumber == i && found_atr->second.canConvert(QMetaType::Int))
                m_jump_number = found_atr->second.toUInt();

            if(CustomJumpNames::JumpDate == i && found_atr->second.canConvert(QMetaType::QDateTime))
                m_jump_date = found_atr->second.toDateTime();

            if(CustomJumpNames::DZ == i && found_atr->second.canConvert(QMetaType::QString))
                m_dz = found_atr->second.toString();

            if(CustomJumpNames::AC == i && found_atr->second.canConvert(QMetaType::QString))
                m_ac = found_atr->second.toString();

            if(CustomJumpNames::Canopy == i && found_atr->second.canConvert(QMetaType::QString))
                m_canopy = found_atr->second.toString();

            if(CustomJumpNames::Deleted == i && found_atr->second.canConvert(QMetaType::Bool))
                m_is_deleted = found_atr->second.toBool();

            if(CustomJumpNames::Note == i && found_atr->second.canConvert(QMetaType::QString))
                m_note = found_atr->second.toString();


        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
const QString &CustomJump::field_name(const int n_field)
{
    return CustomJumpFieldNames[n_field >= CustomJumpNames::JumpNumber && n_field < CustomJumpNames::CtUnk ? n_field : CustomJumpNames::CtUnk];
}


//----------------------------------------------------------------------------------------------------------------------
int CustomJump::index(const QString &field_name)
{
    int index = 0;
    for(const auto& value: CustomJumpFieldNames)
    {
        if(value == field_name)
            break;
        index++;
    }
    return index;
}

