#include "n3jump.h"

static QString N3JumpFieldNames[] =
                              {QObject::tr("Speed 12K"),
                               QObject::tr("Speed 9K"),
                               QObject::tr("Speed 6K"),
                               QObject::tr("Speed 3K")
                              };


//----------------------------------------------------------------------------------------------------------------------
N3Jump::N3Jump()
{

}

//----------------------------------------------------------------------------------------------------------------------
t_jump_attribute N3Jump::getPairs() const
{
    t_jump_attribute jump_attr = CustomJump::getPairs();

    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::ExitAlt), m_exit_alt));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::DeplAlt), m_depl_alt));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::FreefallTime), m_freefall_time));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::CanopyTime), m_canopy_time));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed12K), m_speed12K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed9K), m_speed9K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed6K), m_speed6K));
    jump_attr.push_back(std::make_pair(N3Jump::field_name(N3JumpNames::Speed3K), m_speed3K));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::Deleted), m_is_deleted));
    jump_attr.push_back(std::make_pair(CustomJump::field_name(CustomJumpNames::Note), m_note));

    return jump_attr;
}

const QString &N3Jump::field_name(uint n_field) const
{
    return N3JumpFieldNames[n_field <= N3JumpNames::Speed3K ? n_field : 0];
}
