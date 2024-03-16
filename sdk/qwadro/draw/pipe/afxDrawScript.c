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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_C
//#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_SCRIPT_C
#include "qwadro/core/afxClass.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/core/afxSystem.h"

_AVX afxError AfxRecycleDrawScript(afxDrawScript dscr, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    (void)freeRes;

    if (dscr->state == afxDrawScriptState_PENDING) AfxThrowError();
    else
    {
        if (dscr->resetCb(dscr)) AfxThrowError();
        else
        {
            dscr->state = afxDrawScriptState_INITIAL;
        }
    }
    return err;
}

_AVX afxError AfxCompileDrawScript(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    if (dscr->state != afxDrawScriptState_RECORDING) AfxThrowError();
    else
    {
        if (dscr->endCb(dscr)) AfxThrowError();
        else
        {
            dscr->state = afxDrawScriptState_EXECUTABLE;
        }
    }
    return err;
}

_AVX afxError AfxRecordDrawScript(afxDrawScript dscr, afxDrawScriptUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    if (!(dscr->state == afxDrawScriptState_INITIAL || dscr->state == afxDrawScriptState_EXECUTABLE)) AfxThrowError();
    else
    {
        if (dscr->beginCb(dscr, usage)) AfxThrowError();
        else
        {
            dscr->state = afxDrawScriptState_RECORDING;
        }
    }
    return err;
}

AVX afxDrawScriptState AfxGetDrawScriptState(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->state;
}

AVX afxNat AfxGetDrawScriptPort(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->portIdx;
}

_AVX afxDrawInput AfxGetDrawScriptInput(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawInput din = dscr->din;
    //AfxAssertType(din, afxFcc_DIN);
    return din;
}

_AVX afxError AfxAcquireDrawScripts(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawScript dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext dctx;

    if (!(dctx = AfxGetDrawInputContext(din))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, afxFcc_DCTX);
        afxNat cnt2 = 0;

        for (afxNat i = 0; i < din->scripts.cnt; i++)
        {
            afxDrawScript dscr2 = *(afxDrawScript*)AfxGetArrayUnit(&din->scripts, i);
            AfxAssertObjects(1, &dscr2, afxFcc_DSCR);

            if (afxDrawScriptState_INVALID == AfxGetDrawScriptState(dscr2))
            {
                if (AfxRecycleDrawScript(dscr2, TRUE)) AfxThrowError();
                else
                {
                    dscr[cnt2] = dscr2;
                    ++cnt2;
                }
            }

            if (cnt2 >= cnt)
                break;
        }

        if (cnt2 < cnt)
        {
            afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);

            if (AfxAcquireObjects(&ddev->ports[portIdx].scripts, cnt - cnt2, (afxObject*)&dscr[cnt2], (void const*[]) { dctx, (void*)&portIdx, &din }))
                AfxThrowError();
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dscr[i] = NIL;

    return err;
}
