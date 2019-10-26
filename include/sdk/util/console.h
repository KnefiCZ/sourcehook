#ifndef SDK_CONSOLE_H
#define SDK_CONSOLE_H
#pragma once

#include "../common.h"

// You need to statically link tier0.lib!
/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#define PRINTF_FORMAT_STRING _Printf_format_string_

DLL_IMPORT void Msg(PRINTF_FORMAT_STRING const tchar* pMsg, ...);

#endif