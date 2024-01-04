#include "neptune_hil.h"

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_details_jumps(unsigned int jump_index, unsigned int num_jumps)
{
    if(summary().totalJumps() == 0)
        return;

    if(num_jumps + jump_index > summary().totalJumps())
        num_jumps = summary().totalJumps() - jump_index;

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

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_dropzones()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::DropZones, N3Constants::N3NamesSize);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_airplanes()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::Airplanes, N3Constants::N3NamesSize);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_alarms()
{
    executeCommand(N3Commands::ReadMemory, N3Addresses::AlarmNames, N3Constants::N3NamesSize);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::read_datetime()
{
    executeCommand(N3Commands::ReadDateTime);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::end_communication()
{
    executeCommand(N3Commands::EndComm, 0, 0, nullptr, 0);
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
    write_to_memory(N3Addresses::DeviceSettings, N3Constants::DeviceSettingsSize, rawDataSettings);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::write_dropzones()
{
    write_to_memory(N3Addresses::DropZones, N3Constants::N3NamesSize, dropzones().data());
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::write_airplanes()
{
    write_to_memory(N3Addresses::Airplanes, N3Constants::N3NamesSize, airplanes().data());
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::write_summary_jumps()
{
    write_to_memory(N3Addresses::Summary, N3Constants::SummarySize, rawDataSummary);
}

//----------------------------------------------------------------------------------------------------------------------
void Neptune_HiL::write_to_memory(unsigned int address, unsigned int length, QByteArray &wbytes)
{
    if(0 == length || 0 == address)
        return;

    uint rest = length % N3Constants::WriteRateDataSize;
    uint num_cicles = (length / N3Constants::WriteRateDataSize);

    m_NumBlocks += num_cicles + (rest > 0 ? 1 : 0);
    emit setProgress(m_NumBlocks);

    for(uint i = 0; i < num_cicles; ++i)
    {
        executeCommand(N3Commands::WriteMemory, address, N3Constants::WriteRateDataSize, wbytes.data() + (i * N3Constants::WriteRateDataSize));
        address += N3Constants::WriteRateDataSize;
    }
    if(rest > 0)
        executeCommand(N3Commands::WriteMemory, address, rest, wbytes.data() + ((num_cicles) * N3Constants::WriteRateDataSize));
}


