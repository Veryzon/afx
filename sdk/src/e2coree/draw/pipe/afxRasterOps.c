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

_AFX afxCmdId AfxCmdDownloadRaster(afxDrawScript dscr, afxRaster ras, afxStream dst, afxNat opCnt, afxRasterIoOp const ops[], afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &dst, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return dscr->stdCmds->ras.rw(dscr, ras, dst, opCnt, ops, TRUE, cdc);
}

_AFX afxCmdId AfxCmdDownloadRasterRegion(afxDrawScript dscr, afxRaster ras, afxRasterRegion const* rgn, afxStream dst, afxNat dstOffset, afxNat dstRowSiz, afxNat dstRowCnt, afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &dst, afxFcc_BUF);    
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = dstOffset;
    op.bufRowSiz = dstRowSiz;
    op.bufRowCnt = dstRowCnt;
    return AfxCmdDownloadRaster(dscr, ras, dst, 1, &op, cdc);
}

_AFX afxCmdId AfxCmdUploadRaster(afxDrawScript dscr, afxRaster ras, afxStream src, afxNat opCnt, afxRasterIoOp const ops[], afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_IOS);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return dscr->stdCmds->ras.rw(dscr, ras, src, opCnt, ops, FALSE, cdc);
}

_AFX afxCmdId AfxCmdUploadRasterRegion(afxDrawScript dscr, afxRaster ras, afxRasterRegion const* rgn, afxStream src, afxNat srcOffset, afxNat srcRowSiz, afxNat srcRowCnt, afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_IOS);
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = srcOffset;
    op.bufRowSiz = srcRowSiz;
    op.bufRowCnt = srcRowCnt;
    return AfxCmdUploadRaster(dscr, ras, src, 1, &op, cdc);
}

_AFX afxCmdId AfxCmdPackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return dscr->stdCmds->ras.pak(dscr, ras, buf, opCnt, ops, FALSE);
}

_AFX afxCmdId AfxCmdPackRasterRegion(afxDrawScript dscr, afxRaster ras, afxRasterRegion const* rgn, afxBuffer buf, afxNat bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;
    return AfxCmdPackRaster(dscr, ras, buf, 1, &op);
}

_AFX afxCmdId AfxCmdUnpackRaster(afxDrawScript dscr, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterIoOp const* op = &ops[i];
        AfxAssertRange(AfxCountRasterLods(ras), op->rgn.lodIdx, 1);
        AfxGetRasterExtent(ras, op->rgn.lodIdx, whd);
        AfxAssertRange(AfxCountRasterLayers(ras), op->rgn.baseLayer, op->rgn.layerCnt);
        AfxAssertWhd(whd, op->rgn.offset, op->rgn.whd);
        AfxAssert(op->bufRowSiz);
        AfxAssert(op->bufRowCnt);
    }
    return dscr->stdCmds->ras.pak(dscr, ras, buf, opCnt, ops, TRUE);
}

_AFX afxCmdId AfxCmdUnpackRasterRegion(afxDrawScript dscr, afxRaster ras, afxRasterRegion const* rgn, afxBuffer buf, afxNat bufOffset, afxNat bufRowSiz, afxNat bufRowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);    
    afxRasterIoOp op;
    AfxAssert(rgn);
    op.rgn = *rgn;
    op.bufOffset = bufOffset;
    op.bufRowSiz = bufRowSiz;
    op.bufRowCnt = bufRowCnt;
    return AfxCmdUnpackRaster(dscr, ras, buf, 1, &op);
}

_AFX afxCmdId AfxCmdCopyRaster(afxDrawScript dscr, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxWhd whd;
        afxRasterCopyOp const* op = &ops[i];

        AfxGetRasterExtent(src, 0, whd);
        AfxAssertWhd(whd, op->srcOffset, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(src), op->srcLodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(src), op->srcBaseLayer, op->srcLayerCnt);

        AfxGetRasterExtent(dst, 0, whd);
        AfxAssertWhd(whd, op->dst.offset, op->dst.whd);
        AfxAssertRange(AfxCountRasterLods(dst), op->dst.lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(dst), op->dst.baseLayer, op->dst.layerCnt);
    }
    return dscr->stdCmds->ras.cpy(dscr, src, dst, opCnt, ops);
}

_AFX afxCmdId AfxCmdCopyRasterRegion(afxDrawScript dscr, afxRaster src, afxNat srcLodIdx, afxNat srcBaseLayer, afxNat srcLayerCnt, afxWhd srcOffset, afxRaster dst, afxRasterRegion const* dstRgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);    
    afxRasterCopyOp op;
    op.srcLodIdx = srcLodIdx;
    op.srcBaseLayer = srcBaseLayer;
    op.srcLayerCnt = srcLayerCnt;
    AfxCopyWhd(op.srcOffset, srcOffset);
    AfxAssert(dstRgn);
    op.dst = *dstRgn;
    return AfxCmdCopyRaster(dscr, src, dst, 1, &op);
}

AFX afxCmdId AfxCmdSubsampleRaster(afxDrawScript dscr, afxRaster ras, afxNat baseLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(AfxCountRasterLods(ras), baseLod, lodCnt);
    return dscr->stdCmds->ras.mip(dscr, ras, baseLod, lodCnt);
}

_AFX afxCmdId AfxCmdTransformRaster(afxDrawScript dscr, afxRaster ras, afxReal const m[4][4], afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(m);
    AfxAssert(rgnCnt);
    AfxAssert(regions);

    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];

        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->offset, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);
    }
    return dscr->stdCmds->ras.xform(dscr, ras, m, rgnCnt, regions);
}

_AFX afxCmdId AfxCmdTransformRasterRegion(afxDrawScript dscr, afxRaster ras, afxReal const m[4][4], afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(m);
    AfxAssert(rgn);
    return AfxCmdTransformRaster(dscr, ras, m, 1, rgn);
}

_AFX afxCmdId AfxCmdSwizzleRaster(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const regions[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgnCnt);
    AfxAssert(regions);
    
    for (afxNat i = 0; i < rgnCnt; i++)
    {
        afxWhd whd;
        afxRasterRegion const* rgn = &regions[i];
        
        AfxGetRasterExtent(ras, 0, whd);
        AfxAssertWhd(whd, rgn->offset, rgn->whd);
        AfxAssertRange(AfxCountRasterLods(ras), rgn->lodIdx, 1);
        AfxAssertRange(AfxCountRasterLayers(ras), rgn->baseLayer, rgn->layerCnt);
    }
    return dscr->stdCmds->ras.swizzle(dscr, ras, a, b, rgnCnt, regions);
}

_AFX afxCmdId AfxCmdSwizzleRasterRegion(afxDrawScript dscr, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxRasterRegion const* rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(a || b);
    AfxAssert(rgn);
    return AfxCmdSwizzleRaster(dscr, ras, a, b, 1, rgn);
}