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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <hidusage.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_HID_C
#define _AFX_GAMEPAD_C
#define _AFX_CONTROLLER_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/io/afxGamepad.h"
#include "qwadro/io/afxMouse.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxString.h"


_AFX afxError _AfxCtrlDtor(afxController ctrl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ctrl, afxFcc_CTRL);
    return err;
}

_AFX afxResult _AfxCtrlCtor(afxController ctrl, afxCookie const *cookie)
{
    AfxEntry("kbd=%p", ctrl);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

    afxNat const *ports = cookie->udd[0];
    afxNat port = (afxNat)ports[cookie->no];

    ctrl->hid.port = port;
    ctrl->hid.flags = NIL;


    return err;
}

_AFX afxClassConfig const _AfxCtrlClsConfig =
{
    .fcc = afxFcc_CTRL,
    .name = "Controller",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxController)),
    .mmu = NIL,
    .ctor = (void*)_AfxCtrlCtor,
    .dtor = (void*)_AfxCtrlDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireControllers(afxNat cnt, afxNat const port[], afxController controllers[])
{
    AfxEntry("cnt=%u,port=%p,controllers=%p", cnt, port, controllers);
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetControllerClass();
    AfxAssertClass(cls, afxFcc_CTRL);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxController tmp;
        
        if ((tmp = AfxGetController(port ? port[i] : 0)))
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                controllers[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&controllers[i], (void const*[]) { port ? (void*)&port[i] : 0 }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)controllers);
            break;
        }
    }
    return err;
}

_AFX afxNat AfxInvokeControllers(afxNat first, afxNat cnt, afxBool(*f)(afxController, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetControllerClass();
    AfxAssertClass(cls, afxFcc_CTRL);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateControllers(afxNat first, afxNat cnt, afxController controllers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(controllers);
    afxClass* cls = AfxGetControllerClass();
    AfxAssertClass(cls, afxFcc_CTRL);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)controllers);
}

_AFX afxNat AfxCountControllers(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetControllerClass();
    AfxAssertClass(cls, afxFcc_CTRL);
    return AfxCountInstances(cls);
}

_AFX afxController AfxGetController(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxNat i = 0;
    afxController ctrl;
    while (AfxEnumerateControllers(i, 1, &ctrl))
    {
        AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

        if (ctrl->hid.port == port)
            return ctrl;

        ++i;
    }
    return NIL;
}