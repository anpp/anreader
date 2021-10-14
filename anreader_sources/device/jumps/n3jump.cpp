#include "n3jump.h"

static QString N3JumpFieldNames[] =
                              {
                                QObject::tr("DZ"),
                                QObject::tr("Exit altitude"),
                                QObject::tr("Deploy altitude"),
                                QObject::tr("Freefall time"),
                                QObject::tr("Canopy time"),
                                QObject::tr("Speed 12K"),
                                QObject::tr("Speed 9K"),
                                QObject::tr("Speed 6K"),
                                QObject::tr("Speed 3K"),
                                QObject::tr("Deleted"),
                                QObject::tr("Note")
                              };


//----------------------------------------------------------------------------------------------------------------------
N3Jump::N3Jump()
{

}

//----------------------------------------------------------------------------------------------------------------------
std::unique_ptr<t_jump_attribute> N3Jump::getPairs() const
{
    std::unique_ptr<t_jump_attribute> jump_attr = CustomJump::getPairs();

    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::DZ), m_dz));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::ExitAlt), m_exit_alt));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::DeplAlt), m_depl_alt));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::FreefallTime), m_freefall_time));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::CanopyTime), m_canopy_time));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed12K), m_speed12K));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed9K), m_speed9K));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed6K), m_speed6K));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed3K), m_speed3K));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Deleted), m_is_deleted));
    (*jump_attr).push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Note), m_note));

    return jump_attr;
}

void N3Jump::setPairs(const t_jump_attribute &pairs)
{
    CustomJump::setPairs(pairs);


    if(pairs.size() >= N3JumpNames::Note)
    {        

        for(int i = N3JumpNames::DZ; i <= N3JumpNames::Note; ++i)
        {
            auto found_atr = std::find_if(pairs.begin(), pairs.end(), [i] (const auto &atr)
             {
                 return atr.first == N3JumpFieldNames[i - CustomJumpNames::JumpDate - 1];
             });

            if(found_atr != pairs.end())
            {
                if(N3JumpNames::DZ == i && found_atr->second.canConvert(QMetaType::QString))
                    m_dz = found_atr->second.toString();

                if(N3JumpNames::ExitAlt == i && found_atr->second.canConvert(QMetaType::Int))
                    m_exit_alt = found_atr->second.toUInt();

                if(N3JumpNames::DeplAlt == i && found_atr->second.canConvert(QMetaType::Int))
                    m_depl_alt = found_atr->second.toUInt();

                if(N3JumpNames::FreefallTime == i && found_atr->second.canConvert(QMetaType::Int))
                    m_freefall_time = found_atr->second.toUInt();

                if(N3JumpNames::CanopyTime == i && found_atr->second.canConvert(QMetaType::Int))
                    m_canopy_time = found_atr->second.toUInt();

                if(N3JumpNames::Speed12K == i && found_atr->second.canConvert(QMetaType::Int))
                    m_speed12K = found_atr->second.toUInt();

                if(N3JumpNames::Speed9K == i && found_atr->second.canConvert(QMetaType::Int))
                    m_speed9K = found_atr->second.toUInt();

                if(N3JumpNames::Speed6K == i && found_atr->second.canConvert(QMetaType::Int))
                    m_speed6K = found_atr->second.toUInt();

                if(N3JumpNames::Speed3K == i && found_atr->second.canConvert(QMetaType::Int))
                    m_speed3K = found_atr->second.toUInt();

                if(N3JumpNames::Deleted == i && found_atr->second.canConvert(QMetaType::Bool))
                    m_is_deleted = found_atr->second.toBool();

                if(N3JumpNames::Note == i && found_atr->second.canConvert(QMetaType::QString))
                    m_note = found_atr->second.toString();
            }
        }
    }
}

const QString &N3Jump::field_name(const int n_field) const
{
    if(n_field < N3JumpNames::DZ)
        return CustomJump::field_name(n_field);

    int n_field_abs  = n_field - CustomJumpNames::JumpDate - 1; //parent offset
    n_field_abs = (n_field_abs > 0 ? n_field_abs: 0);

    return N3JumpFieldNames[n_field >= N3JumpNames::ExitAlt && n_field <= N3JumpNames::Note ? n_field_abs : 0];
}
