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

#define _AFX_DRAW_C
#define _AFX_DRAW_SCRIPT_C
#include "qwadro/mem/afxArena.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/pipe/afxDrawScript.h"

_AFX afxCmdId AfxCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.EnableDepthBias(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawContext dctx = AfxGetObjectProvider(dscr);
    return dscr->stdCmds->Rasterization.SetDepthBias(dscr, constFactor, clamp, slopeFactor);
}

_AFX afxCmdId AfxCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.SetLineWidth(dscr, lineWidth);
}

_AFX afxCmdId AfxCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.DisableRasterization(dscr, disable);
}

_AFX afxCmdId AfxCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.EnableDepthTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.SetDepthCompareOp(dscr, op);
}

_AFX afxCmdId AfxCmdDisableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.DisableDepthWrite(dscr, enable);
}

_AFX afxCmdId AfxCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.EnableStencilTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.SetStencilCompareMask(dscr, faceMask, compareMask);
}

_AFX afxCmdId AfxCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.SetStencilWriteMask(dscr, faceMask, writeMask);
}

_AFX afxCmdId AfxCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.SetStencilReference(dscr, faceMask, reference);
}

_AFX afxCmdId AfxCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.EnableDepthBoundsTest(dscr, enable);
}

_AFX afxCmdId AfxCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(bounds);
    return dscr->stdCmds->Rasterization.SetDepthBounds(dscr, bounds);
}

_AFX afxCmdId AfxCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(blendConstants);
    return dscr->stdCmds->Rasterization.SetBlendConstants(dscr, blendConstants);
}

_AFX afxCmdId AfxCmdResetScissors(afxDrawScript dscr, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    return dscr->stdCmds->Rasterization.ResetScissors(dscr, cnt, rc);
}

_AFX afxCmdId AfxCmdReadjustScissors(afxDrawScript dscr, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return dscr->stdCmds->Rasterization.ReadjustScissors(dscr, baseIdx, cnt, rect);
}

_AFX afxCmdId AfxCmdResetAreas(afxDrawScript dscr, afxBool exclusive, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    return dscr->stdCmds->Rasterization.ResetAreas(dscr, exclusive, cnt, rc);
}

_AFX afxCmdId AfxCmdReadjustAreas(afxDrawScript dscr, afxBool exclusive, afxNat baseIdx, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, baseIdx, cnt);
    AfxAssert(!cnt || rect);
    return dscr->stdCmds->Rasterization.ReadjustAreas(dscr, exclusive, baseIdx, cnt, rect);
}

_AFX afxCmdId AfxCmdBeginSynthesis(afxDrawScript dscr, afxSynthesisConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(state);
    return dscr->stdCmds->Rasterization.BeginSynthesis(dscr, state);
}

_AFX afxCmdId AfxCmdFinishSynthesis(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.FinishSynthesis(dscr);
}

_AFX afxCmdId AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->stdCmds->Rasterization.NextPass(dscr, useAuxScripts);
}