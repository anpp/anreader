#include "n3jump.h"

static QString N3JumpFieldNames[] =
                              {
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
t_jump_attribute N3Jump::getPairs() const
{
    t_jump_attribute jump_attr = CustomJump::getPairs();

    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::ExitAlt), m_exit_alt));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::DeplAlt), m_depl_alt));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::FreefallTime), m_freefall_time));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::CanopyTime), m_canopy_time));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed12K), m_speed12K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed9K), m_speed9K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed6K), m_speed6K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed3K), m_speed3K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Deleted), m_is_deleted));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Note), m_note));

    return jump_attr;
}

void N3Jump::setPairs(const t_jump_attribute &pairs)
{
    CustomJump::setPairs(pairs);

    const int po = CustomJumpNames::JumpDate; //parent_offset

    if(pairs.size() >= N3JumpNames::Note + po)
    {
        if((pairs.at(N3JumpNames::ExitAlt + po).second).canConvert(QMetaType::Int))
           m_exit_alt = (pairs.at(N3JumpNames::ExitAlt + po).second).toUInt();

        if((pairs.at(N3JumpNames::DeplAlt + po).second).canConvert(QMetaType::Int))
           m_depl_alt = (pairs.at(N3JumpNames::DeplAlt + po).second).toUInt();

        if((pairs.at(N3JumpNames::FreefallTime + po).second).canConvert(QMetaType::Int))
           m_freefall_time = (pairs.at(N3JumpNames::FreefallTime + po).second).toUInt();

        if((pairs.at(N3JumpNames::CanopyTime + po).second).canConvert(QMetaType::Int))
           m_canopy_time = (pairs.at(N3JumpNames::CanopyTime + po).second).toUInt();

        if((pairs.at(N3JumpNames::Speed12K + po).second).canConvert(QMetaType::Int))
           m_speed12K = (pairs.at(N3JumpNames::Speed12K + po).second).toUInt();

        if((pairs.at(N3JumpNames::Speed9K + po).second).canConvert(QMetaType::Int))
           m_speed9K = (pairs.at(N3JumpNames::Speed9K + po).second).toUInt();

        if((pairs.at(N3JumpNames::Speed6K + po).second).canConvert(QMetaType::Int))
           m_speed6K = (pairs.at(N3JumpNames::Speed6K + po).second).toUInt();

        if((pairs.at(N3JumpNames::Speed3K + po).second).canConvert(QMetaType::Int))
           m_speed3K = (pairs.at(N3JumpNames::Speed3K + po).second).toUInt();

        if((pairs.at(N3JumpNames::Deleted + po).second).canConvert(QMetaType::Bool))
           m_is_deleted = (pairs.at(N3JumpNames::Deleted + po).second).toBool();

        if((pairs.at(N3JumpNames::Note + po).second).canConvert(QMetaType::QString))
           m_note = (pairs.at(N3JumpNames::Note + po).second).toString();
    }

}

const QString &N3Jump::field_name(uint n_field) const
{
    return N3JumpFieldNames[n_field <= N3JumpNames::Note ? n_field : 0];
}
