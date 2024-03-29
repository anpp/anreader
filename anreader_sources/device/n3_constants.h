#ifndef CONSTANTS_H
#define CONSTANTS_H


enum XTEA: unsigned {key_schedule_constant = 0x9E3779B9, NumRounds = 16};

enum N3Constants: unsigned int {DelayInMs = 10000, TimeoutInMs = 20000, DelayBetweenCommands = 50, BlockSize = 32, BytesToPort = 8, msByteSendingDelay = 50, Type0RecordSizeInRaw = 97,
                                JumpRecordSize = 22, SummarySize = 0x001E, JumpDetailsSize = 0xF938, DeviceSettingsSize = 0x00010, N3NamesSize = 0x0142, AlarmsSettingsSize = 0x0054,
                                KeepAliveDelay = 1500, WriteRateDataSize = 24};

enum N3Commands: unsigned char {None = 0, InitCommand = 1, AckSuccess = 0x31, AckRepeat = 0x34, AckReady = 0x35,
                                ReadMemory = 0xA0, EndComm = 0xAF, KeepAlive = 0xA4, ReadDateTime = 0xA2, SetDateTime = 0xB2,
                                WriteMemory = 0xB0};

enum N3Addresses: unsigned int {Summary = 0x000A, DeviceSettings = 0x002A, SpeedGroups = 0x003A, DropZones = 0x0054, Airplanes = 0x0196, AlarmsNames = 0x2D8,
                           AlarmToneDirectory = 0x041A, AlarmToneData = 0x042C, AlarmsSettings = 0x04CC, JumpDetails = 0x0520};


enum class N3Types: int {Unknown = 0, Neptune = 1, Wave = 2, Tracker = 3, Data_Logger = 4, N3 = 5, N3A = 6, Atlas = 7, Atlas2 = 12};
enum class N3CommTypes: int {Unknown = 0, CT3 = 3, CT4 = 4, CT5 = 5};

#endif // CONSTANTS_H
