#pragma once

//Target Win7 or later
#define _WIN32_WINNT 0x0601

//Include to do with targeting?
#include <sdkddkver.h>

#ifndef FULL_WINTARD

#define WIN32_LEANANDMEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR Wont compile with this one? :C
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOOPENFILE
#define NOSCROLL
#define NOMETAFILE
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC	
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif // !FULL_WINTARD

#define NOMINMAX
#define STRICT

#include <Windows.h>