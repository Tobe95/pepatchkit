#include "ini_reader.h"
#include <direct.h>
#include <stdlib.h>

static char ini_path[MAX_PATH];

UINT get_private_profile_int(LPCTSTR lpKeyName, INT nDefault)
{
	return GetPrivateProfileInt(INI_NAME, lpKeyName, nDefault, ini_path);
}

UINT get_private_profile_bool(LPCTSTR lpKeyName, INT nDefault)
{
	return get_private_profile_int(lpKeyName, nDefault);
}

DWORD get_private_profile_string(LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	return GetPrivateProfileString(INI_NAME, lpKeyName, lpDefault, lpReturnedString, nSize, ini_path);
}

FLOAT get_private_profile_float(LPCTSTR lpKeyName, LPCTSTR lpDefault)
{
	CHAR lpReturnedString[MAX_PATH];

	get_private_profile_string(lpKeyName, lpDefault, lpReturnedString, sizeof(lpReturnedString));

	return (FLOAT)atof(lpReturnedString);
}

void init_private_profile()
{
    GetModuleFileNameA(NULL, ini_path, MAX_PATH);

    char* lastSlash = strrchr(ini_path, '\\');
    if (lastSlash)
        *(lastSlash + 1) = '\0';

    strcat_s(ini_path, INI_NAME);
    strcat_s(ini_path, ".ini");
}