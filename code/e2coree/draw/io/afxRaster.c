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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_RASTER_C
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/core/afxFile.h"

 // 1d               =   1 x w11 >> lod
 // 1d array         = img x w11 >> lod
 // 2d               =   1 x wh1 >> lod
 // 2d array         = img x wh1
 // cube             =   1 x wh1 >> lod
 // cube array       = img x wh1 >> lod
 // 3d               =   1 x whd >> lod

_AFX afxNat AfxCountRasterSamples(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->sampleCnt;
}

_AFX afxPixelFormat AfxGetRasterFormat(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->fmt;
}

_AFXINL afxResult AfxTestRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->flags & bitmask;
}

_AFXINL void AfxUnflagRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    ras->flags &= ~bitmask;
}

_AFXINL void AfxFlagRaster(afxRaster ras, afxRasterFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    ras->flags |= bitmask;
}

_AFX afxNat AfxCountRasterLods(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->lodCnt;
}

_AFX afxNat AfxCountRasterLayers(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    return ras->layerCnt;
}

_AFX afxBool AfxGetRasterSwizzling(afxRaster ras, afxColorSwizzling const** csw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    if (csw)
        *csw = ras->swizzling;

    return !!ras->swizzling;
}

_AFX void AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssert(whd);

    whd[0] = ras->whd[0];
    whd[1] = ras->whd[1];
    whd[2] = ras->whd[2];

    while (lodIdx)
    {
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;
        whd[2] = whd[2] >> 1;
        --lodIdx;
    }
}

_AFX afxNat AfxMeasureRasterRow(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat bpr = (ras->whd[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE;
    AfxAssert(bpr); // bytes per row
    return bpr;
}

_AFX afxNat AfxMeasureRasterLayer(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat bpl = AfxMeasureRasterRow(ras) * ras->whd[1] * ras->whd[2];
    AfxAssert(bpl); // bytes per layer
    return bpl;
}

_AFX afxNat AfxLocateRasterRegion(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, layerIdx, 1);
    //AfxAssertRangef(ras->whd[0], offset[0], 1); // is a evil trick to use it to measure ras
    //AfxAssertRangef(ras->whd[1], offset[1], 1);
    //AfxAssertRangef(ras->whd[2], offset[2], 1);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat rowSiz = AfxMeasureRasterRow(ras);

    afxWhd offset2 = { offset[0], offset[1], offset[2] };
    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    afxNat memOff = 0;

    while (lodIdx)
    {
        offset2[0] = offset2[0] >> 1;
        offset2[1] = offset2[1] >> 1;
        offset2[2] = offset2[2] >> 1;
        rowSiz = rowSiz >> 1;
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;
        whd[2] = whd[2] >> 1;
        
        memOff += ras->layerCnt * whd[2] * whd[1] * rowSiz;
        --lodIdx;
    }

    memOff += layerIdx * whd[2] * whd[1] * rowSiz;
    memOff += offset2[2] * offset2[1] * ((offset2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);
    //memOff = AFX_ALIGN(memOff, AFX_SIMD_ALIGN);

    return memOff;
}

_AFX afxNat AfxMeasureRasterRegion(afxRaster ras, afxRasterRegion const *srcRgn) // can't be multiple because regions couldn't be continous.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(srcRgn);
    AfxAssertRange(ras->lodCnt, srcRgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, srcRgn->baseLayer, srcRgn->layerCnt);
    AfxAssertRange(ras->whd[0], srcRgn->offset[0], srcRgn->whd[0]);
    AfxAssertRange(ras->whd[1], srcRgn->offset[1], srcRgn->whd[1]);
    AfxAssertRange(ras->whd[2], srcRgn->offset[2], srcRgn->whd[2]);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(ras->fmt, &pfd);
    afxNat rowSiz = AfxMeasureRasterRow(ras);

    afxWhd offset2 = { srcRgn->offset[0], srcRgn->offset[1], srcRgn->offset[2] };
    afxWhd extent2 = { srcRgn->whd[0], srcRgn->whd[1], srcRgn->whd[2] };
    afxWhd whd = { ras->whd[0], ras->whd[1], ras->whd[2] };
    afxNat size = 0;

    afxNat lodIdx = srcRgn->lodIdx;
    
    while (lodIdx)
    {
        offset2[0] = offset2[0] >> 1;
        offset2[1] = offset2[1] >> 1;
        offset2[2] = offset2[2] >> 1;
        extent2[0] = extent2[0] >> 1;
        extent2[1] = extent2[1] >> 1;
        extent2[2] = extent2[2] >> 1;
        rowSiz = rowSiz >> 1;
        whd[0] = whd[0] >> 1;
        whd[1] = whd[1] >> 1;
        whd[2] = whd[2] >> 1;

        //memOff += (ras->layerCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        --lodIdx;
    }
    
    extent2[0] = AfxMax(extent2[0], 1);
    extent2[1] = AfxMax(extent2[1], 1);
    extent2[2] = AfxMax(extent2[2], 1);

    for (afxNat i = srcRgn->layerCnt; i--> 1;)
        size += (whd[2] * whd[1] * rowSiz);

    size += extent2[2] * extent2[1] * ((extent2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);

    size -= offset2[2] * offset2[1] * ((offset2[0] * AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE)) / AFX_BYTE_SIZE);

    size = AFX_ALIGN(size, AFX_SIMD_ALIGN);

    return size;
}

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxInputRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &in, afxFcc_IOS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    
    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
        AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
        AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
        AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

        afxNat rgnSiz = 0;
        afxNat dstRowSiz = 0;
        void *dstData;
        
        if (!(dstData = AfxOpenRasterRegion(ras, &rgn, AFX_TEX_OPEN_W, &rgnSiz, &dstRowSiz))) AfxThrowError();
        else
        {
            afxNat dstRowCnt = rgn.whd[1];
            afxNat srcRowSiz = op->bufRowSiz;
            afxNat srcRowCnt = op->bufRowCnt;

            AfxAssert(dstRowSiz >= srcRowSiz);
            AfxAssert(dstRowCnt >= srcRowCnt);

            afxNat rowSiz = srcRowSiz ? srcRowSiz : dstRowSiz;
            afxNat rowCnt = srcRowCnt ? srcRowCnt : dstRowCnt;
            afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            AfxAssert(rgnSiz >= dataSiz);

            if (AfxReadStreamAt(in, op->bufOffset, dstData, dataSiz, 0))
                AfxThrowError();

            AfxCloseRasterRegion(ras, &rgn);
        }
    }
    return err;
}

_AFX afxError AfxOutputRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxStream out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &out, afxFcc_IOS);
    AfxAssert(opCnt);
    AfxAssert(ops);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
        AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
        AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
        AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

        afxNat rgnSiz = 0;
        afxNat srcRowSiz = 0;
        void *srcData;

        if (!(srcData = AfxOpenRasterRegion(ras, &rgn, AFX_TEX_OPEN_R, &rgnSiz, &srcRowSiz))) AfxThrowError();
        else
        {
            afxNat srcRowCnt = rgn.whd[1];
            afxNat dstRowSiz = op->bufRowSiz;
            afxNat dstRowCnt = op->bufRowCnt;

            AfxAssert(srcRowSiz >= dstRowSiz);
            AfxAssert(srcRowCnt >= dstRowCnt);

            afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
            afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
            afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            AfxAssert(rgnSiz >= dataSiz);

            if (AfxWriteStreamAt(out, op->bufOffset, srcData, dataSiz, 0))
                AfxThrowError();

            AfxCloseRasterRegion(ras, &rgn);
        }
    }
    return err;
}

_AFX afxError AfxFetchRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(uri);

    for (afxNat i = 0; i < opCnt; i++)
    {
        AfxAssertType(&uri[i], afxFcc_URI);
        afxFile file;

        if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { AFX_FILE_FLAG_R })) AfxThrowError();
        else
        {
            afxStream stream = AfxGetFileStream(file);

            if (AfxInputRasterRegions(ras, 1, &ops[i], stream))
                AfxThrowError();

            AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    return err;
}

_AFX afxError AfxFetchRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxRasterIoOp op;
    op.rgn.lodIdx = lodIdx;
    AfxAssertRange(ras->lodCnt, op.rgn.lodIdx, 1);
    op.rgn.baseLayer = baseLayer;
    op.rgn.layerCnt = layerCnt;
    AfxAssertRange(ras->lodCnt, op.rgn.lodIdx, 1);
    AfxZeroWhd(op.rgn.offset);
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowSiz = 0;
    op.bufRowCnt = 0;

    if (AfxFetchRasterRegions(ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(uri);

    for (afxNat i = 0; i < opCnt; i++)
    {
        AfxAssertType(&uri[i], afxFcc_URI);
        afxFile file;

        if (AfxOpenFiles(1, &file, &uri[i], (afxFileFlags[]) { AFX_FILE_FLAG_W })) AfxThrowError();
        else
        {
            afxStream stream = AfxGetFileStream(file);

            if (AfxOutputRasterRegions(ras, 1, &ops[i], stream))
                AfxThrowError();

            AfxReleaseObjects(1, (void*[]) { file });
        }
    }
    return err;
}

_AFX afxError AfxPrintRaster(afxRaster ras, afxNat lodIdx, afxNat baseLayer, afxNat layerCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertRange(ras->lodCnt, lodIdx, 1);
    AfxAssertRange(ras->layerCnt, baseLayer, layerCnt);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxRasterIoOp op;
    op.rgn.lodIdx = lodIdx;
    AfxAssertRange(ras->lodCnt, op.rgn.lodIdx, 1);
    op.rgn.baseLayer = baseLayer;
    op.rgn.layerCnt = layerCnt;
    AfxAssertRange(ras->lodCnt, op.rgn.lodIdx, 1);
    AfxZeroWhd(op.rgn.offset);
    AfxGetRasterExtent(ras, 0, op.rgn.whd);
    op.bufOffset = 0;
    op.bufRowSiz = 0;
    op.bufRowCnt = 0;

    if (AfxPrintRasterRegions(ras, 1, &op, uri))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxUpdateRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], void const *src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(src);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
        AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
        AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
        AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

        afxNat rgnSiz = 0;
        afxNat dstRowSiz = 0;
        void *dstData;

        if (!(dstData = AfxOpenRasterRegion(ras, &rgn, AFX_TEX_OPEN_RW, &rgnSiz, &dstRowSiz))) AfxThrowError();
        else
        {
            afxNat dstRowCnt = rgn.whd[1];
            afxNat srcRowSiz = op->bufRowSiz;
            afxNat srcRowCnt = op->bufRowCnt;

            AfxAssert(dstRowSiz >= srcRowSiz);
            AfxAssert(dstRowCnt >= srcRowCnt);

            afxNat rowSiz = srcRowSiz ? srcRowSiz : dstRowSiz;
            afxNat rowCnt = srcRowCnt ? srcRowCnt : dstRowCnt;
            afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            AfxAssert(rgnSiz >= dataSiz);

            afxByte const* src2 = src;
            AfxCopy(rowCnt * rgn.whd[2], rowSiz, &src2[op->bufOffset], dstData);

            AfxCloseRasterRegion(ras, &rgn);
        }
    }
    return err;
}

_AFX afxError AfxDumpRasterRegions(afxRaster ras, afxNat opCnt, afxRasterIoOp const ops[], void *dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssert(opCnt);
    AfxAssert(ops);
    AfxAssert(dst);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(ras->lodCnt, rgn.lodIdx, 1);
        AfxAssertRange(ras->layerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(ras->whd[0], rgn.offset[0], rgn.whd[0]);
        AfxAssertRange(ras->whd[1], rgn.offset[1], rgn.whd[1]);
        AfxAssertRange(ras->whd[2], rgn.offset[2], rgn.whd[2]);

        afxNat rgnSiz = 0;
        afxNat srcRowSiz = 0;
        void *srcData;

        if (!(srcData = AfxOpenRasterRegion(ras, &rgn, AFX_TEX_OPEN_R, &rgnSiz, &srcRowSiz))) AfxThrowError();
        else
        {
            afxNat srcRowCnt = rgn.whd[1];
            afxNat dstRowSiz = op->bufRowSiz;
            afxNat dstRowCnt = op->bufRowCnt;

            AfxAssert(srcRowSiz >= dstRowSiz);
            AfxAssert(srcRowCnt >= dstRowCnt);

            afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
            afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
            afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            AfxAssert(rgnSiz >= dataSiz);

            afxByte* dst2 = dst;
            AfxCopy(rowCnt * rgn.whd[2], rowSiz, srcData, &dst2[op->bufOffset]);

            AfxCloseRasterRegion(ras, &rgn);
        }
    }
    return err;
}

_AFX void AfxCloseRasterRegion(afxRaster ras, afxRasterRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->whd[2]);

    if (ras->unmap(ras, rgn))
        AfxThrowError();
};

_AFX void* AfxOpenRasterRegion(afxRaster ras, afxRasterRegion const *rgn, afxRasterOpenFlags flags, afxNat *rgnSiz, afxNat *rowSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    AfxAssert(rgn);
    AfxAssertRange(ras->lodCnt, rgn->lodIdx, 1);
    AfxAssertRange(ras->layerCnt, rgn->baseLayer, rgn->layerCnt);
    AfxAssertRange(ras->whd[0], rgn->offset[0], rgn->whd[0]);
    AfxAssertRange(ras->whd[1], rgn->offset[1], rgn->whd[1]);
    AfxAssertRange(ras->whd[2], rgn->offset[2], rgn->whd[2]);
    
    void *ptr = NIL;

    if (ras->map(ras, rgn, flags, rgnSiz, rowSiz, &ptr))
        AfxThrowError();

    return ptr;
}

_AFX afxError AfxBufferizeRaster(afxRaster ras)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ras, afxFcc_RAS);

    afxDrawContext dctx = AfxGetObjectProvider(ras);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxRasterRegion srcRgn;
    srcRgn.baseLayer = 0;
    srcRgn.layerCnt = ras->layerCnt;
    srcRgn.offset[0] = 0;
    srcRgn.offset[1] = 0;
    srcRgn.offset[2] = 0;
    AfxGetRasterExtent(ras, 0, srcRgn.whd);

    afxNat siz = 0;

    for (afxNat i = 0; i < ras->lodCnt; i++)
    {
        srcRgn.lodIdx = i;
        siz += AfxMeasureRasterRegion(ras, &srcRgn);
    }

    void *maps = NIL;

    if (siz && !(maps = AfxReallocate(mmu, ras->maps, AFX_ALIGN(siz, AFX_SIMD_ALIGN), 1, AFX_SIMD_ALIGN, AfxHint()))) AfxThrowError();
    else
        ras->maps = maps;

    return err;
}

_AFX afxError AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster ras[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(info);
    AfxAssert(ras);

    afxClass* cls = AfxGetRasterClass(dctx);
    AfxAssertClass(cls, afxFcc_RAS);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ras, (void const*[]) { (void*)info }))
        AfxThrowError();

    AfxAssertObjects(cnt, ras, afxFcc_RAS);

    return err;
}

_AFX afxRaster AfxAssembleRaster(afxDrawContext dctx, afxRasterFlags flags, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    afxRaster ras;

    if (AfxAssembleRastersFromTarga(dctx, flags, cnt, uri, &ras))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AFX afxRaster AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterFlags flags, afxUri const uri[6])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    afxRaster ras;

    if (!(ras = AfxAssembleRaster(dctx, flags | AFX_TEX_FLAG_CUBEMAP, 6, uri)))
        AfxThrowError();

    AfxTryAssertObjects(1, &ras, afxFcc_RAS);

    return ras;
}

_AFX afxError AfxReadjustRasters(afxDrawInput din, afxWhd const whd, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertObjects(cnt, rasters, afxFcc_RAS);
    AfxAssert(whd);
    AfxAssert(whd[0]);
    AfxAssert(whd[1]);
    AfxAssert(whd[2]);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxRaster ras = rasters[i];
        AfxAssertObjects(1, &ras, afxFcc_RAS);


    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// QUEUED OPERATIONS                                                          //
////////////////////////////////////////////////////////////////////////////////
