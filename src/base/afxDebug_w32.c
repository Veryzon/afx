/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include "../../dep/stb_sprintf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <float.h>
#include <Windows.h>
#include <Shlwapi.h>
//#include <Mmsystem.h>
#pragma comment (lib, "Shlwapi")

#if 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi")
#endif

#include "../impl/afxExecImplKit.h"

extern afxString const qwadroSignature;

AFX_DEFINE_STRUCT(afxDebugger)
{
    afxMutex        mtx;
    afxCondition cond;
    afxBool         isLocked;

    afxUnit          unitLockerIdx;

    afxBool         running;
    void*           output;
    void*           dump;
    HWND            conWnd;
    HANDLE          conOutHnd;
}
debugger =
{
    .mtx = { 0 },
    .cond = { 0 },
    .isLocked = FALSE,

    .unitLockerIdx = -1,

    .running = FALSE,
    .output = NIL,
    .dump = NIL,
    .conWnd = NIL,
    .conOutHnd = INVALID_HANDLE_VALUE
};

AFX_DEFINE_STRUCT(_afxDbgChan)
{
    afxUnit32 fgCol;
    afxUnit32 bgCol;
    afxUnit16 beepFreq;
    afxUnit16 beepDur;
    afxBool8 echo;
    afxBool8 dump;
};

static _afxDbgChan dbgChannels[32] =
{
    {
        .fgCol = 0x000000FF, // grey : comment
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFFFFFF, // white : highlight
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0x00ff0000, // green : regular
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF00FF00, // blue : assistence
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0x00FFFF00, // turquish
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF00FF00, // purple
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFF0000, // yellow
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFF00FF, // yellow : advertence (aka warning)
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF000000, // red dark : assertion
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF0000FF, // red : critical error
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
};

_AFX void AfxResetDebugOutputChannel(afxUnit ch, afxBool echo, afxBool dump)
{
    dbgChannels[ch].echo = echo;
    dbgChannels[ch].dump = dump;
}

_AFX void AfxBeep(afxUnit freq, afxUnit dur)
{
    // Ah, the good old motherboard speaker beep.
#if 0
    // Using ASCII Bell.
    printf("\a"); // ASCII Bell character
    fflush(stdout); // Make sure it's sent immediately
#else
#ifndef _WIN32
    // A true hardware-level beep from the PC speaker.
    // Direct access via '/dev/console' (requires Root).
    int con_fd = open("/dev/console", O_WRONLY);
    if (con_fd == -1)
    {
        perror("open");
        return 1;
    }
    int duration = 1000; // in milliseconds
    int freq = 1000;     // frequency in Hz
    int tone = (1193180 / freq);
    ioctl(con_fd, KIOCSOUND, tone);
    usleep(duration * 1000);
    ioctl(con_fd, KIOCSOUND, 0); // Stop sound
    close(con_fd);
#else
    Beep(440, 300); // frequency in Hz, duration in ms
#endif
#endif
    return;
}

 _AFXINL afxChar const* _AfxDbgTrimFilename(afxChar const* path)
{
    afxChar const* start = (afxChar const*)path, *p = (afxChar const*)path;
    while (*(p)++);
    while (--(p) != start && *(p) != (afxChar)'/' && *(p) != (afxChar)'\\' && *(p) != (afxChar)':');
    return((*(p) == (afxChar)'/' || *(p) == (afxChar)'\\' || *(p) == (afxChar)':') ? (afxChar const*)++p : NIL);
}

_AFXINL afxResult _AfxReportFn(afxUnit ch, afxChar const* msg, afxUnit len)
{
    WORD tattr = 0;
    afxUnit color = 0;

    switch (ch % 10)
    {
    case 0: color = 0x000000FF; break; // grey : comment
    case 1: color = 0xFFFFFFFF; break; // white : highlight
    case 2: color = 0x00ff0000; break; // green : regular
    case 3: color = 0xFF00FF00; break; // blue : assistence
    case 4: color = 0x00FFFF00; break; // turquish
    case 5: color = 0xFF00FF00; break; // purple

    case 6: color = 0xFFFF0000; break; // yellow
    case 7: color = 0xFFFF00FF; break; // yellow : advertence (aka warning)
    case 8: color = 0xFF000000; break; // red dark : assertion
    case 9: color = 0xFF0000FF; break; // red : critical error
    default:break;
    }

    if (color & 0x000000FF)
        tattr |= FOREGROUND_INTENSITY;

    if (color & 0x0000FF00)
        tattr |= FOREGROUND_BLUE;

    if (color & 0x00FF0000)
        tattr |= FOREGROUND_GREEN;

    if (color & 0xFF000000)
        tattr |= FOREGROUND_RED;

    AfxPrint(color, msg);

    if (debugger.dump)
    {
        //fputs(msg, debugger.dump);
        fwrite(msg, 1, len, debugger.dump);
        fflush(debugger.dump);
    }
    return 1;
}

_AFXINL afxResult AfxReportf(afxUnit ch, afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len = 0;

        if ((ch % 10) >= 7)
        {
            // \a = BELL control code
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli?%s\n", _AfxDbgTrimFilename((char const * const)hint[0]), hint[1], (char const * const)hint[2]);
#else
            len = stbsp_sprintf(msg2, "%s:%i?%s\n", _AfxDbgTrimFilename((char const * const)hint[0]), (int)hint[1], (char const * const)hint[2]);
#endif
            //AfxBeep(1, 1);
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxReportFn(ch, msg2, len + len2);
        _AfxDbgUnlock();
    }
    return 0;
}

_AFXINL afxResult AfxReport(afxUnit ch, afxChar const* msg, afxUnit len)
{
    if (_AfxDbgLock())
    {
        _AfxReportFn(ch, msg, len);
        _AfxDbgUnlock();
    }
    return 0;
}

#if 0
void AfxLogCall_(afxHere const hint, afxChar const* args, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg, "%s(", (char const *const)hint[2]);
#else
        len = stbsp_sprintf(msg, "%s(", (char const *const)hint[2]);
#endif

        va_list args2;
        va_start(args2, args);
        int len2 = stbsp_vsprintf(&msg[len], args, args2);
        va_end(args2);
        msg[len + len2] = ')';
        msg[len + len2 + 1] = '\n';
        msg[len + len2 + 2] = '\0';

        _AfxLogFn(0x000000FF, msg);
        _AfxDbgUnlock();
    }
}

void AfxReportComment_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = 0;// stbsp_sprintf(msg2, "\n ");
    }
        else
        {
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
            len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFFFFFF, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportMessage_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = 0;// stbsp_sprintf(msg2, "");
        }
        else
        {
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
            len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0x00FF0000, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportHint_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF00FF00, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportWarn_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFF0000, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportError_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const * const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const * const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF000000, msg2);
        _AfxDbgUnlock();
    }
}
#endif

_AFXINL afxResult _AfxDbgUnlock(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        //if (!(rslt = AfxLockMutex(&(debugger.mtx)))) printf("couldn't unlock mutex");
        //else
        {
            debugger.isLocked = FALSE;
            rslt = AfxUnlockMutex(&(debugger.mtx));
            AfxSignalCondition(&debugger.cond);
            afxSystem sys;

            if (AfxGetSystem(&sys))
            {
                afxUnit unitUnlockerIdx = AfxGetTid();
                //afxError err;
                //AFX_ASSERT(unitUnlockerIdx == debugger.unitLockerIdx);
            }
        }
    }
    return rslt;
}

_AFXINL afxResult _AfxDbgLock(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        if (!(rslt = !AfxLockMutex(&(debugger.mtx)))) printf("couldn't lock mutex");
        else
        {
            afxUnit oldUnitIdx = debugger.unitLockerIdx;
            afxSystem sys;
            
            if (AfxGetSystem(&sys))
            {
                debugger.unitLockerIdx = AfxGetTid();

                if (oldUnitIdx != debugger.unitLockerIdx)
                {
                    afxChar msg2[512];
                    stbsp_sprintf(msg2, "Entering the Thread Execution Environment %2u\n", debugger.unitLockerIdx);
                    //AfxReportf(6, NIL, msg2);
                }
            }

            debugger.isLocked = (afxBool)TRUE;
            //rslt = AfxUnlockMutex(&(debugger.mtx));
        }
    }
    return rslt;
}

_AFXINL afxBool _AfxDbgIsAttached(void)
{
    return debugger.running;
}

_AFXINL afxResult _AfxDbgDetach(void)
{
    if (_AfxDbgLock())
    {
        AfxReleaseConsole();

        if (debugger.dump)
        {
            //fclose(dbgCfg.dump);
            debugger.dump = NIL;
        }

        debugger.running = 0;
        AfxDismantleCondition(&(debugger.cond));
        AfxDismantleMutex(&(debugger.mtx));
    }
    return 0;
}

void sigHandler(int s)
{
    switch (s)
    {
    case SIGTERM:
    {
        _AfxReportFn(0, "\aEXCEPTION: termination request, sent to the program.", 0);
        break;
    }
    case SIGSEGV:
    {
        _AfxReportFn(0, "\aEXCEPTION: Invalid memory access (segmentation fault).", 0);
        break;
    }
    case SIGINT:
    {
        _AfxReportFn(0, "\aEXCEPTION: External interrupt, usually initiated by the user.", 0);
        break;
    }
    case SIGILL:
    {
        _AfxReportFn(0, "\aEXCEPTION: Invalid program image, such as invalid instruction.", 0);
        break;
    }
    case SIGABRT:
    {
        _AfxReportFn(0, "\aEXCEPTION: Abnormal termination condition, as is e.g. initiated by abort().", 0);
        break;
    }
    case SIGFPE:
    {
        _AfxReportFn(0, "\aEXCEPTION: Erroneous arithmetic operation such as divide by zero.", 0);
        break;
    }
    default: break;
    }
}

_AFXINL afxResult _AfxDbgAttach(afxChar const* file)
{
    (void)file;
    afxError err = AFX_ERR_NONE;
    
    AfxBeep(0, 0);

    if (debugger.running)
    {
        AfxThrowError();
    }
    else
    {
        AfxDeployMutex(&(debugger.mtx), AFX_MTX_RECURSIVE);
        AfxDeployCondition(&(debugger.cond));
        debugger.isLocked = TRUE;

        signal(SIGTERM, sigHandler);
        signal(SIGSEGV, sigHandler);
        signal(SIGINT, sigHandler);
        signal(SIGILL, sigHandler);
        signal(SIGABRT, sigHandler);
        signal(SIGFPE, sigHandler);

#ifdef AVX_FLOATING_POINT_VALIDATION_ENABLED
        unsigned int u;
        u = _controlfp(0, 0);
        u = u & ~(_EM_INVALID | /*_EM_DENORMAL |*/ _EM_ZERODIVIDE | _EM_OVERFLOW /*| _EM_UNDERFLOW  | _EM_INEXACT*/);
        _controlfp(u, _MCW_EM);
#endif

        afxChar path[1024];

        if (file)
        {
            if (file[0])
            {
                AfxSprintf(path, "%s", file);
                //AfxPopulateString(path, file);

                if (!(debugger.dump = fopen(path, "w+")))
                    AfxThrowError();
            }
        }
        else
        {
            afxChar sys[2048], exeName[2048];
            GetModuleFileNameA(NIL, exeName, sizeof(exeName));
            PathStripPathA(exeName);
            
            GetModuleFileNameA(GetModuleHandleA("e2coree.dll"), sys, sizeof(sys));
            PathRemoveFileSpecA(sys);
            
            time_t t;
            time(&t);
            struct tm * ti;
            ti = localtime(&t);
            
            AfxSprintf(path, "%s(%04i%02i%02i-%02i%02i%02i).log", exeName, 1900 + ti->tm_year, ti->tm_mon, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
            
            if (!(debugger.dump = fopen(path, "w+"))) AfxThrowError();
            else
            {
                //fclose(debugger.dump);

                //if (!(debugger.dump = fopen(path, "w+")))
                    //AfxThrowError();
            }
        }

        if (debugger.dump || (!file || !file[0]))
        {
            //setbuf(debugger.dump);
            debugger.running = 1;

#if 0
            if (!AfxAttachDebugTerminal()) AfxThrowError();
            else
            {
                short fontSizX = 16, fontSizY = 16;
                CONSOLE_FONT_INFOEX cfi = { .cbSize = sizeof(cfi), .nFont = 0, .dwFontSize = { fontSizX, fontSizY }, .FontFamily = FF_DONTCARE, .FontWeight = FW_NORMAL };
                wcscpy(cfi.FaceName, L"Consolas");
                SetCurrentConsoleFontEx(debugger.conOutHnd, FALSE, &cfi);
                COORD coord = GetLargestConsoleWindowSize(debugger.conOutHnd);
                
                MoveWindow(debugger.conWnd, -4, -4, 80 * fontSizX, coord.Y * fontSizY, TRUE);
                //ShowScrollBar(debugger.conWnd, SB_BOTH, FALSE);

                coord.X = 80;
                coord.Y = 5000;
                SetConsoleScreenBufferSize(debugger.conOutHnd, coord);

                SetConsoleTextAttribute(debugger.conOutHnd, (WORD)(~(MAXUINT16) | FOREGROUND_RED | FOREGROUND_GREEN));

                for (afxUnit i = 0; i < 1120; i++)
                    AfxReportf(0xFFFF0000, "%.*s", 1, &qwadroSignature.start[i * 1]);

                Sleep(1000);

                SetConsoleTextAttribute(debugger.conOutHnd, (WORD)(~(MAXUINT16) | FOREGROUND_INTENSITY));
                _AfxDbgUnlock();
                return TRUE;
            }
#else
            if (AfxReacquireConsole())
            {
                //_AfxDbgUnlock();
                return TRUE;
            }
#endif

            debugger.running = 0;

            if (debugger.dump)
            {
                fclose(debugger.dump);
                debugger.dump = NIL;
            }
        }

        AfxDismantleCondition(&(debugger.cond));
        AfxDismantleMutex(&(debugger.mtx));
    }
    return FALSE;
}

_AFXINL void AfxCatchError_(afxError err_, afxHere const hint)
{
    afxError err = NIL;
    AFX_ASSERT(err_);
    AFX_ASSERT(hint);

    if (err_)
    {
        AfxReportError("");
        afxInt a = 0;
    }
}

_AFX void AfxRaiseException(afxError errCode, afxHere const hint)
{
    afxError err = NIL;
    AFX_ASSERT(errCode);
    AFX_ASSERT(hint);

    if (errCode)
    {
        //AfxReportError("");
        afxInt a = 0;
    }
}
