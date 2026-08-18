#include "helpers.h"
#include <Windows.h>
#include <filesystem>

std::wstring GetCurrentProcessName()
{
    WCHAR fileName[MAX_PATH];
    GetModuleFileNameW(NULL, fileName, MAX_PATH);
    return std::filesystem::path(fileName).filename().wstring();
}

void asi_loader()
{
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);

    std::filesystem::path exePath(buffer);
    std::filesystem::path exeDir = exePath.parent_path();

    for (const auto& entry : std::filesystem::directory_iterator(exeDir))
    {
        const auto& path = entry.path();
        if (path.extension() == L".asi")
        {
            LoadLibraryW(path.c_str());
        }
    }
}