#include "customjump.h"

const static QString CustomJumpFieldNames[] =
                              {
                                QObject::tr("Number"),
                                QObject::tr("Date"),
                                QObject::tr("DZ"),
                                QObject::tr("Airplane"),
                                QObject::tr("Canopy"),
                                QObject::tr("Unknown")
                              };


//----------------------------------------------------------------------------------------------------------------------
CustomJump::CustomJump()
{

}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<t_jump_attribute> CustomJump::getPairs() const
{
    std::unique_ptr<t_jump_attribute> jump_attr = std::make_unique<t_jump_attribute>();

    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpNumber), m_jump_number));
    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpDate), m_jump_date));
    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::DZ), m_dz));
    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::AP), m_ap));
    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::Canopy), m_canopy));

    return jump_attr;
}

void CustomJump::setPairs(const t_jump_attribute &pairs)
{
    for(int i = 0; i <= CustomJumpNames::Canopy; ++i)
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

            if(CustomJumpNames::AP == i && found_atr->second.canConvert(QMetaType::QString))
                m_ap = found_atr->second.toString();

            if(CustomJumpNames::Canopy == i && found_atr->second.canConvert(QMetaType::QString))
                m_canopy = found_atr->second.toString();

        }
    }
}

const QString &CustomJump::field_name(const int n_field) const
{
    return CustomJumpFieldNames[n_field >= CustomJumpNames::JumpNumber && n_field <= CustomJumpNames::Canopy ? n_field : CustomJumpNames::CtUnk];
}

