#include "neptune_hil.h"

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_details_jumps(unsigned int jump_index, unsigned int num_jumps)
{
    if(summary().totalJumps() == 0)
        return;

    if(num_jumps + jump_index > summary().totalJumps())
        num_jumps = summary().totalJumps() - jump_index;

    if(num_jumps > Neptune_HiL_defs::jumps_rate)
    {
        uint num_cicles = num_jumps / Neptune_HiL_defs::jumps_rate;
        uint rest = num_jumps - (Neptune_HiL_defs::jumps_rate * num_cicles);
        for(uint i = 0; i < num_cicles; ++i)
        {
            executeCommand(N3Commands::ReadMemory, N3Addresses::JumpDetails + N3Constants::JumpRecordSize * jump_index, N3Constants::JumpRecordSize * Neptune_HiL_defs::jumps_rate);
            jump_index += Neptune_HiL_defs::jumps_rate;            
        }        
        if(rest > 0)
            executeCommand(N3Commands::ReadMemory, N3Addresses::JumpDetails + N3Constants::JumpRecordSize * jump_index, N3Constants::JumpRecordSize * rest);
    }
    else
        executeCommand(N3Commands::ReadMemory, N3Addresses::JumpDetails + N3Constants::JumpRecordSize * jump_index, N3Constants::JumpRecordSize * num_jumps);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_summary_jumps()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::Summary, N3Constants::SummarySize);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_settings()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::DeviceSettings, N3Constants::DeviceSettingsSize);
}

void Neptune_HiL::read_dropzones()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::DropZones, N3Constants::DZNamesSize);
}

void Neptune_HiL::read_airplanes()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::Airplanes, N3Constants::APNamesSize);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_datetime()
{
    executeCommand(N3Commands::ReadDateTime);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::end_communication()
{
    executeCommand(N3Commands::EndComm);
}

//----------------------------------------------------------------------------------------------------------------------
uint Neptune_HiL::n_iterations_by_jumps(const uint n_jumps)
{
   uint memory_length = (n_jumps * N3Constants::JumpRecordSize + sizeof(uint32_t));
   return ((memory_length + sizeof(uint32_t)) / N3Constants::BlockSize) + (((memory_length + sizeof(uint32_t)) % N3Constants::BlockSize) == 0 ? 0 : 1);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::write_settings()
{
    executeCommand(N3Commands::WriteMemory, N3Addresses::DeviceSettings, N3Constants::DeviceSettingsSize, &rawDataSettings);
}


