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
std::unique_ptr<t_jump_attribute> CustomJump::getPairs() const
{
    std::unique_ptr<t_jump_attribute> jump_attr = std::make_unique<t_jump_attribute>();

    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpNumber), m_jump_number));
    (*jump_attr).push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::JumpDate), m_jump_date));

    return jump_attr;
}

void CustomJump::setPairs(const t_jump_attribute &pairs)
{
    if(pairs.size() >= CustomJumpNames::JumpDate)
    {
        for(int i = 0; i <= CustomJumpNames::JumpDate; ++i)
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
            }
        }
    }
}

const QString &CustomJump::field_name(const int n_field) const
{
    return CustomJumpFieldNames[n_field >= CustomJumpNames::JumpNumber && n_field <= CustomJumpNames::JumpDate ? n_field : 0];
}

