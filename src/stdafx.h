#pragma once

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>

#include "..\duilib\Duilib\UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#	ifdef _UNICODE
#		pragma comment(lib,"..\\..\\duilib\\Lib\\DuiLib_ud.lib")
#	else
#		pragma comment(lib,"..\\..\\duilib\\Lib\\DuiLib_d.lib")
#	endif
#else
#	ifdef _UNICODE
#		pragma comment(lib,"..\\..\\duilib\\Lib\\DuiLib_u.lib")
#	else
#		pragma comment(lib,"..\\..\\duilib\\Lib\\DuiLib.lib")
#	endif
#endif
