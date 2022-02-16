#ifndef MEMORYLIB
#define MEMORYLIB

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <psapi.h>
    #include "TlHelp32.h"
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MemoryLib
{
    private:

    #if defined(_WIN32) || defined(_WIN64)
        static inline STARTUPINFOA _sInfo;
        static inline PROCESS_INFORMATION _pInfo;
    #endif

    public:

    static inline uint64_t ExecAddress;
    static inline uint64_t BaseAddress;
    static inline bool BigEndian = false;

    #if defined(_WIN32) || defined(_WIN64)
        static inline DWORD PIdentifier = 0;
        static inline HANDLE PHandle = NULL;
        static inline char PName[MAX_PATH];

        static HMODULE FindBaseAddr(HANDLE InputHandle, string InputName)
        {
            HMODULE hMods[1024];
            DWORD cbNeeded;
            unsigned int i;

            if (EnumProcessModules(InputHandle, hMods, sizeof(hMods), &cbNeeded))
            {
                for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
                {
                    TCHAR szModName[MAX_PATH];
                    if (GetModuleFileNameEx(InputHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                    {
                        wstring wstrModName = szModName;
                        wstring wstrModContain = wstring(InputName.begin(), InputName.end());

                        if (wstrModName.find(wstrModContain) != string::npos)
                            return hMods[i];
                    }
                }
            }

            return nullptr;
        }

        static DWORD FindProcessId(const std::wstring& processName)
        {
            PROCESSENTRY32 processInfo;
            processInfo.dwSize = sizeof(processInfo);

            HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (processesSnapshot == INVALID_HANDLE_VALUE)
                return 0;

            Process32First(processesSnapshot, &processInfo);
            if (!processName.compare(processInfo.szExeFile))
            {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }

            while (Process32Next(processesSnapshot, &processInfo))
            {
                if (!processName.compare(processInfo.szExeFile))
                {
                    CloseHandle(processesSnapshot);
                    return processInfo.th32ProcessID;
                }
            }

            CloseHandle(processesSnapshot);
            return 0;
        }
        static int ExecuteProcess(string InputName, uint64_t InputAddress, bool InputEndian)
        {
            ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
            ZeroMemory(&_pInfo, sizeof(_pInfo));

            if (CreateProcessA(InputName.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &_sInfo, &_pInfo) == 0)
                return -1;

            BaseAddress = InputAddress;
            BigEndian = InputEndian;

            return 0;
        };
        static bool LatchProcess(string InputName, uint64_t InputAddress, bool InputEndian)
        {
            ZeroMemory(&_sInfo, sizeof(_sInfo)); _sInfo.cb = sizeof(_sInfo);
            ZeroMemory(&_pInfo, sizeof(_pInfo));

            PIdentifier = FindProcessId(wstring(InputName.begin(), InputName.end()));
            PHandle = OpenProcess(PROCESS_ALL_ACCESS, false, PIdentifier);

            GetProcessImageFileNameA(MemoryLib::PHandle, PName, MAX_PATH);
            BaseAddress = InputAddress;

            ExecAddress = (uint64_t)FindBaseAddr(PHandle, PName);
            BigEndian = InputEndian;

            if (PHandle == NULL)
                return false;

            return true;
        };
        static void ExternProcess(DWORD InputID, HANDLE InputH, uint64_t InputAddress)
        {
            PIdentifier = InputID;
            PHandle = InputH;

            GetProcessImageFileNameA(MemoryLib::PHandle, PName, MAX_PATH);

            BaseAddress = InputAddress;
            ExecAddress = (uint64_t)FindBaseAddr(PHandle, PName);
        };
    #endif

    static void SetBaseAddr(uint64_t InputAddress)
    {
        BaseAddress = InputAddress;
    }

    // Reader Functions

    static uint8_t ReadByte(uint64_t _addr, bool _absolute = false) { return ReadBytes(_addr, 1, _absolute)[0]; }
    static vector<uint8_t> ReadBytes(uint64_t _addr, int _len, bool _absolute = false)
    {
        vector<uint8_t> _buffer;
        _buffer.resize(_len);

        ReadProcessMemory(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), _buffer.data(), _len, 0);
        return _buffer;
    }
    static uint16_t ReadShort(uint64_t _addr, bool _absolute = false)
    {
        auto _buffer = ReadBytes(_addr, 2, _absolute);

        if (BigEndian)
            return (_buffer[0] << 8) | _buffer[1];
        else
            return (_buffer[1] << 8) | _buffer[0];
    }
    static uint32_t ReadInt(uint64_t _addr, bool _absolute = false)
    {
        auto _buffer = ReadBytes(_addr, 4, _absolute);

        if (BigEndian)
            return (_buffer[0] << 24) | (_buffer[1] << 16) | (_buffer[2] << 8) | (_buffer[3]);
        else
            return (_buffer[3] << 24) | (_buffer[2] << 16) | (_buffer[1] << 8) | (_buffer[0]);
    }
    static uint64_t ReadLong(uint64_t _addr, bool _absolute = false)
    {
        auto _buffer = ReadBytes(_addr, 8, _absolute);

        if (BigEndian)
            return ((uint64_t)_buffer[0] << 56) | ((uint64_t)_buffer[1] << 48) | ((uint64_t)_buffer[2] << 40) | ((uint64_t)_buffer[3] << 32) | ((uint64_t)_buffer[4] << 24) | ((uint64_t)_buffer[5] << 16) | ((uint64_t)_buffer[6] << 8) | ((uint64_t)_buffer[7]);

        else
            return ((uint64_t)_buffer[7] << 56) | ((uint64_t)_buffer[6] << 48) | ((uint64_t)_buffer[5] << 40) | ((uint64_t)_buffer[4] << 32) | ((uint64_t)_buffer[3] << 24) | ((uint64_t)_buffer[2] << 16) | ((uint64_t)_buffer[1] << 8) | ((uint64_t)_buffer[0]);
    }
    static float ReadFloat(uint64_t _addr, bool _absolute = false)
    {
        auto _value = ReadInt(_addr, _absolute);
        auto _return = *reinterpret_cast<float*>(&_value);
        return _return;
    }
    static bool ReadBool(uint64_t _addr, bool _absolute = false)
    {
        auto _value = ReadByte(_addr, _absolute);
        return _value == 0 ? false : true;
    }
    static string ReadString(uint64_t _addr, int _len, bool _absolute = false)
    {
        auto _value = ReadBytes(_addr, _len, _absolute);
        string _output(_value.begin(), _value.end());
        return _output;
    }
    
    // Writer Functions

    static void WriteByte(uint64_t _addr, uint8_t _val, bool _absolute = false)
    { 
        if (WriteProcessMemory(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), &_val, 1, 0) == 0)
        {
            DWORD _protectOld = 0;
            VirtualProtectEx(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), 256, PAGE_READWRITE, &_protectOld);
            WriteProcessMemory(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), &_val, 1, 0);
        }
    }
    static void WriteBytes(uint64_t _addr, vector<uint8_t> _val, bool _absolute = false)
    {
        if (WriteProcessMemory(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), _val.data(), _val.size(), 0) == 0)
        {
            DWORD _protectOld = 0;
            VirtualProtectEx(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), 256, PAGE_READWRITE, &_protectOld);
            WriteProcessMemory(PHandle, _absolute ? (void*)(_addr) : (void*)(_addr + BaseAddress), _val.data(), _val.size(), 0);
        }
    }
    static void WriteShort(uint64_t _addr, uint16_t _val, bool _absolute = false)
    {
        vector<uint8_t> _write(2);

        for (uint64_t i = 0; i < 2; i++)
        {
            if (BigEndian)
                _write[1 - i] = (_val >> (i * 8)) & 0xFF;

            else
                _write[i] = (_val >> (i * 8)) & 0xFF;
        }

        WriteBytes(_addr, _write, _absolute);
        _write.clear();
    }
    static void WriteInt(uint64_t _addr, uint32_t _val, bool _absolute = false)
    {
        vector<uint8_t> _write(4);

        for (uint64_t i = 0; i < 4; i++)
        {
            if (BigEndian)
                _write[3 - i] = (_val >> (i * 8)) & 0xFF;

            else
                _write[i] = (_val >> (i * 8)) & 0xFF;
        }

        WriteBytes(_addr, _write, _absolute);
        _write.clear();
    }
    static void WriteLong(uint64_t _addr, uint64_t _val, bool _absolute = false)
    {
        vector<uint8_t> _write(8);

        for (uint64_t i = 0; i < 8; i++)
        {
            if (BigEndian)
                _write[1 - i] = (_val >> (i * 8)) & 0xFF;

            else
                _write[i] = (_val >> (i * 8)) & 0xFF;
        }

        WriteBytes(_addr, _write, _absolute);
        _write.clear();
    }
    static void WriteFloat(uint64_t _addr, float _val, bool _absolute = false)
    {
        auto _value = *reinterpret_cast<uint32_t*>(&_val);
        WriteInt(_addr, _value, _absolute);
    }
    static void WriteBool(uint64_t _addr, bool _val, bool _absolute = false) { _val == true ? WriteByte(_addr, 1, _absolute) : WriteByte(_addr, 0, _absolute); }
    static void WriteString(uint64_t _addr, string _val, bool _absolute = false)
    {
        vector<uint8_t> _value(_val.begin(), _val.end());
        WriteBytes(_addr, _value, _absolute);
    }

    // Misc. Functions

    static void WriteExec(uint64_t _addr, vector<uint8_t> _val)
    {
        WriteProcessMemory(PHandle, (void*)(_addr + ExecAddress), _val.data(), _val.size(), 0);
    }

};
#endif
