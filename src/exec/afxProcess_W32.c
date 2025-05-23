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
#include <Windows.h>

#define _AFX_CORE_C
#include "../impl/afxExecImplKit.h"

void CreateIsolatedProcess(const char *application)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    HANDLE hJob;

    // Initialize security attributes
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = NULL;

    // Create a Job Object to manage process resources
    hJob = CreateJobObject(&sa, NULL);
    if (hJob == NULL) {
        AfxReport(0, "CreateJobObject failed: %d\n", GetLastError());
        return;
    }

    // Set job limits (e.g., limit the number of processes)
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY;
    jeli.JobMemoryLimit = 50 * 1024 * 1024; // Example: limit to 50 MB

    if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
    {
        AfxReport(0, "SetInformationJobObject failed: %d\n", GetLastError());
        return;
    }

    // Initialize the STARTUPINFO structure
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    // Initialize PROCESS_INFORMATION structure
    ZeroMemory(&pi, sizeof(pi));

    // Create the isolated process
    if (!CreateProcess(NULL, (LPSTR)application, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        AfxReport(0, "CreateProcess failed: %d\n", GetLastError());
        return;
    }

    // Assign the process to the job object
    if (!AssignProcessToJobObject(hJob, pi.hProcess)) {
        AfxReport(0, "AssignProcessToJobObject failed: %d\n", GetLastError());
        return;
    }

    // Resume the process
    ResumeThread(pi.hThread);

    // Wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hJob);
}
