/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_SCRIPT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_INDEX_BUFFER_C
#define _AFX_VERTEX_BUFFER_C
#include "afxDrawClassified.h"


_AFX afxString const afxDrawCmdStrings[] =
{
    AFX_STRING_LITERAL("PushPipeline"),
    AFX_STRING_LITERAL("PopPipeline"),

    AFX_STRING_LITERAL("EnableRasterization"),

    AFX_STRING_LITERAL("SwitchFrontFace"),
    AFX_STRING_LITERAL("SetCullMode"),

    AFX_STRING_LITERAL("EnableDepthBias"),
    AFX_STRING_LITERAL("SetDepthBias"),

    AFX_STRING_LITERAL("SetLineWidth"),
    
    AFX_STRING_LITERAL("EnableDepthTest"),
    AFX_STRING_LITERAL("EnableDepthWrite"),

    AFX_STRING_LITERAL("EnableStencilTest"),
    AFX_STRING_LITERAL("SetStencilCompareMask"),
    AFX_STRING_LITERAL("SetStencilWriteMask"),
    AFX_STRING_LITERAL("SetStencilReference"),

    AFX_STRING_LITERAL("EnableDepthBoundsTest"),
    AFX_STRING_LITERAL("SetDepthBounds"),

    AFX_STRING_LITERAL("SetBlendConstants"),

    AFX_STRING_LITERAL("ResetViewports"),
    AFX_STRING_LITERAL("UpdateViewports"),
    AFX_STRING_LITERAL("ResetScissors"),
    AFX_STRING_LITERAL("UpdateScissors"),

    AFX_STRING_LITERAL("BindIndexSource"),
    AFX_STRING_LITERAL("BindVertexSources"),

    AFX_STRING_LITERAL("Draw"),
    AFX_STRING_LITERAL("DrawIndirect"),
    AFX_STRING_LITERAL("DrawIndirectCount"),
    AFX_STRING_LITERAL("DrawIndexed"),
    AFX_STRING_LITERAL("DrawIndexedIndirect"),
    AFX_STRING_LITERAL("DrawIndexedIndirectCount"),
};

_AFX void AfxCmdBindPipeline(afxDrawScript dscr, afxNat level, afxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    dscr->stdCmds->BindPipeline(dscr, level, pip);
}

_AFX void AfxCmdBindBuffers(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, first, cnt);
    dscr->stdCmds->BindBuffers(dscr, set, first, cnt, buf, offset, range);
}

_AFX void AfxCmdBindTextures(afxDrawScript dscr, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, first, cnt);
    dscr->stdCmds->BindTextures(dscr, set, first, cnt, smp, tex);
}

// Draw rendering state

_AFX void AfxCmdSwitchFrontFace(afxDrawScript dscr, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SwitchFrontFace(dscr, cw);
}

_AFX void AfxCmdSetCullMode(afxDrawScript dscr, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetCullMode(dscr, mode);
}

_AFX void AfxCmdEnableDepthBias(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthBias(dscr, enable);
}

_AFX void AfxCmdSetDepthBias(afxDrawScript dscr, afxReal constFactor, afxReal clamp, afxReal slopeFactor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawContext dctx = AfxGetObjectProvider(dscr);
    dscr->stdCmds->SetDepthBias(dscr, constFactor, clamp, slopeFactor);
}

_AFX void AfxCmdSetLineWidth(afxDrawScript dscr, afxReal lineWidth)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetLineWidth(dscr, lineWidth);
}

_AFX void AfxCmdDisableRasterization(afxDrawScript dscr, afxBool disable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->DisableRasterization(dscr, disable);
}

_AFX void AfxCmdEnableDepthTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthTest(dscr, enable);
}

_AFX void AfxCmdSetDepthCompareOp(afxDrawScript dscr, afxCompareOp op)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetDepthCompareOp(dscr, op);
}

_AFX void AfxCmdEnableDepthWrite(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthWrite(dscr, enable);
}

_AFX void AfxCmdEnableStencilTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableStencilTest(dscr, enable);
}

_AFX void AfxCmdSetStencilCompareMask(afxDrawScript dscr, afxMask faceMask, afxNat32 compareMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilCompareMask(dscr, faceMask, compareMask);
}

_AFX void AfxCmdSetStencilWriteMask(afxDrawScript dscr, afxMask faceMask, afxNat32 writeMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilWriteMask(dscr, faceMask, writeMask);
}

_AFX void AfxCmdSetStencilReference(afxDrawScript dscr, afxMask faceMask, afxNat32 reference)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->SetStencilReference(dscr, faceMask, reference);
}

_AFX void AfxCmdEnableDepthBoundsTest(afxDrawScript dscr, afxBool enable)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EnableDepthBoundsTest(dscr, enable);
}

_AFX void AfxCmdSetDepthBounds(afxDrawScript dscr, afxReal const bounds[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(bounds);
    dscr->stdCmds->SetDepthBounds(dscr, bounds);
}

_AFX void AfxCmdSetBlendConstants(afxDrawScript dscr, afxReal const blendConstants[4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(blendConstants);
    dscr->stdCmds->SetBlendConstants(dscr, blendConstants);
}

// Viewport and scissor

_AFX void AfxCmdResetViewports(afxDrawScript dscr, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || vp);
    dscr->stdCmds->ResetViewports(dscr, cnt, vp);
}

_AFX void AfxCmdUpdateViewports(afxDrawScript dscr, afxNat first, afxNat cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, first, cnt);
    AfxAssert(!cnt || vp);
    dscr->stdCmds->UpdateViewports(dscr, first, cnt, vp);
}

_AFX void AfxCmdResetScissors(afxDrawScript dscr, afxNat cnt, afxRect const rc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(!cnt || rc);
    dscr->stdCmds->ResetScissors(dscr, cnt, rc);
}

_AFX void AfxCmdUpdateScissors(afxDrawScript dscr, afxNat first, afxNat cnt, afxRect const rect[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, first, cnt);
    AfxAssert(!cnt || rect);
    dscr->stdCmds->UpdateScissors(dscr, first, cnt, rect);
}

// Vertex input source

_AFX void AfxCmdBindVertexSources(afxDrawScript dscr, afxNat first, afxNat cnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, first, cnt);
    dscr->stdCmds->BindVertexSources(dscr, first, cnt, buf, offset, range);
}

_AFX void AfxCmdResetVertexStreams(afxDrawScript dscr, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    dscr->stdCmds->ResetVertexStreams(dscr, cnt, srcIdx, stride, instance);
}

_AFX void AfxCmdResetVertexAttributes(afxDrawScript dscr, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const streamIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(8, 0, cnt);
    dscr->stdCmds->ResetVertexAttributes(dscr, cnt, location, fmt, streamIdx, offset);
}

_AFX void AfxCmdBindIndexSource(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferSize(buf), offset, 1);
    AfxAssert(idxSiz);
    dscr->stdCmds->BindIndexSource(dscr, buf, offset, idxSiz);
}

_AFX void AfxCmdSetPrimitiveTopology(afxDrawScript dscr, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(topology < afxPrimTopology_TOTAL);
    dscr->stdCmds->SetPrimitiveTopology(dscr, topology);
}

// Canvas

_AFX void AfxCmdBeginCanvas(afxDrawScript dscr, afxCanvasConfig const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(state);
    dscr->stdCmds->BeginCanvas(dscr, state);
}

_AFX void AfxCmdEndCanvas(afxDrawScript dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->EndCanvas(dscr);
}

_AFX void AfxCmdNextPass(afxDrawScript dscr, afxBool useAuxScripts)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    dscr->stdCmds->NextPass(dscr, useAuxScripts);
}

_AFX void AfxCmdExecuteCommands(afxDrawScript dscr, afxNat cnt, afxDrawScript aux[])
{
    afxError err = AFX_ERR_NONE;
    dscr->stdCmds->ExecuteCommands(dscr, cnt, aux);
}

// Draw

_AFX void AfxCmdDraw(afxDrawScript dscr, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(vtxCnt);
    AfxAssert(instCnt);
    dscr->stdCmds->Draw(dscr, vtxCnt, instCnt, firstVtx, firstInst);
}

_AFX void AfxCmdDrawIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX void AfxCmdDrawIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(cntBuf);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AFX void AfxCmdDrawIndexed(afxDrawScript dscr, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(idxCnt);
    AfxAssert(instCnt);
    dscr->stdCmds->DrawIndexed(dscr, idxCnt, instCnt, firstIdx, vtxOff, firstInst);
}

_AFX void AfxCmdDrawIndexedIndirect(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssert(drawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndexedIndirect(dscr, buf, offset, drawCnt, stride);
}

_AFX void AfxCmdDrawIndexedIndirectCount(afxDrawScript dscr, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertObjects(1, &cntBuf, afxFcc_BUF);
    AfxAssert(maxDrawCnt);
    AfxAssert(stride);
    dscr->stdCmds->DrawIndexedIndirectCount(dscr, buf, offset, cntBuf, cntBufOff, maxDrawCnt, stride);
}

_AFX void AfxCmdDrawPrefab(afxDrawScript dscr, afxDrawPrefab prefab, afxNat instCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertRange(afxDrawPrefab_TOTAL, prefab, 1);
    AfxAssert(instCnt);
    dscr->stdCmds->DrawPrefab(dscr, prefab, instCnt);
}









#if 0
_AFX void AfxCmdBindManagedIndexSource(afxDrawScript dscr, afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxNat32 offset, idxSiz;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &offset, NIL, &idxSiz);
    AfxCmdBindIndexSource(dscr, ibuf->buf, offset, idxSiz);
}

_AFX void AfxCmdBindManagedVertexSources(afxDrawScript dscr, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[], afxBool inst, afxNat divisor)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
	AfxAssert(8 > first);
	AfxAssert(8 >= cnt);

    afxNat visCnt = 0;
    afxNat secIndices[16];
    afxVertexInputSource vis[16];
    afxNat vipCnt = 0;
    afxVertexInputPoint vip[16];

    for (afxNat i = 0; i < cnt; i++)
    {
        if (vbuf[i])
        {
            AfxAssertObjects(1, &vbuf[i], afxFcc_VBUF);
            afxNat arr = vtxArr ? vtxArr[i] : 0;

            afxNat secIdx;
            vip[i].locationIdx = arr;
            AfxDescribeVertexAttribute(vbuf[i], arr, &secIdx, &vip[i].fmt, &vip[i].offset, &vip[i].usage);
            ++vipCnt;

            afxBool found = FALSE;

            for (afxNat j = 0; j < visCnt; j++)
            {
                if (secIndices[j] == secIdx)
                {
                    found = TRUE;
                    vip[i].srcSlotIdx = j;
                    break;
                }
            }

            if (!found)
            {
                vip[i].srcSlotIdx = visCnt;
                secIndices[visCnt] = secIdx;

                afxNat32 secBase, secRange, secStride;
                AfxDescribeVertexStorage(vbuf[i], secIdx, &secBase, &secRange, &secStride);

                afxNat subOff = baseVtx ? (secStride * baseVtx[i]) : 0;

                vis[visCnt].buf = vbuf[i]->buf;
                vis[visCnt].offset = secBase + subOff;
                vis[visCnt].range = secRange - vis[visCnt].offset;
                vis[visCnt].stride = secStride;

                vis[visCnt].instance = !!inst;
                vis[visCnt].instDivisor = divisor;

                ++visCnt;
            }
        }
    }

    //AfxCmdSetVertexInputLayout(dscr, vipCnt, vip);
    //AfxCmdBindVertexSources(dscr, first, visCnt, vis);
    AfxThrowError();
}
#endif