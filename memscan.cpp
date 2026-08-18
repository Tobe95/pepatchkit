#include "memscan.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdint.h>
#include <cwchar>
#include <vector>
#include <cstdlib>

struct PatternByte
{
    uint8_t value;
    bool wildcard;
};

std::vector<PatternByte> ParsePattern(const char* pattern)
{
    std::vector<PatternByte> result;

    for (size_t i = 0; pattern[i]; )
    {
        if (pattern[i] == ' ')
        {
            i++;
            continue;
        }

        if (pattern[i] == '?')
        {
            result.push_back({0, true});

            // skip '?' or '??'
            i++;
            if (pattern[i] == '?')
                i++;
        }
        else
        {
            char byteStr[3] = { pattern[i], pattern[i + 1], 0 };
            uint8_t byte = (uint8_t)strtoul(byteStr, nullptr, 16);

            result.push_back({byte, false});
            i += 2;
        }
    }

    return result;
}

unsigned long long FindPattern(const wchar_t ModuleTarget[], const char *patternString)
{
    auto parsed = ParsePattern(patternString);
    size_t pattern_size = parsed.size();
    
    HANDLE module_snapshot = 0;
    MODULEENTRY32W me32;
    
	me32.dwSize = sizeof(MODULEENTRY32W);

	module_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
	Module32FirstW(module_snapshot, &me32);

    do 
    {
        if (wcscmp(me32.szModule, ModuleTarget) == 0) 
        {
            auto* base = reinterpret_cast<uint8_t*>(me32.modBaseAddr);
            
            for (size_t i = 0; i <= me32.modBaseSize - pattern_size; i++) 
            {
                bool found = true;
                for (size_t j = 0; j < pattern_size; j++) 
                {
                    if (!parsed[j].wildcard && parsed[j].value != base[i + j]) 
                    {
                        found = false;
                        break;
                    }
                }
                
                if (found) 
                { 
                    CloseHandle(module_snapshot);
                    return i + (uintptr_t)me32.modBaseAddr;
                }
            }
			break;
        }
    }
    while (Module32NextW(module_snapshot, &me32));

    CloseHandle(module_snapshot);
    return 0;
}