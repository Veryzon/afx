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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#include "../dev/AvxImplKit.h"

_AVX afxNat _AvxDoutIsSuspended(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockShared(&dout->suspendSlock);
    afxNat suspendCnt = dout->suspendCnt;
    AfxExitSlockShared(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AvxDoutSuspendFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = ++dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxNat _AvxDoutResumeFunction(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxEnterSlockExclusive(&dout->suspendSlock);
    afxNat suspendCnt = --dout->suspendCnt;
    AfxExitSlockExclusive(&dout->suspendSlock);
    return suspendCnt;
}

_AVX afxError _AvxDoutFreeAllBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (dout->canvases)
    {
        AfxAssertObjects(dout->bufCnt, dout->canvases, afxFcc_CANV);
        AfxReleaseObjects(dout->bufCnt, dout->canvases);
    }
    return err;
}

_AVX afxError _AvxPresentDrawOutput(afxDrawOutput dout, afxNat bufIdx, afxNat queCnt, afxDrawQueue queues[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    AfxAssert(dout->presentingBufIdx == (afxAtom32)AFX_INVALID_INDEX);
    dout->presentingBufIdx = bufIdx;

    if (dout->presentCb && dout->presentCb(dout, bufIdx, queCnt, queues))
        AfxThrowError();

    afxClock currClock;
    AfxGetClock(&currClock);
    ++dout->outNo;

    if ((1.0 <= AfxGetSecondsElapsed(&dout->outCntResetClock, &currClock)))
    {
        dout->outCntResetClock = currClock;
        dout->outRate = dout->outNo; // 681 no showing (presenting from overlay thread (acquirer)), 818 frozen (present from draw thread (worker))
        dout->outNo = 0;

        afxReal64 ct = AfxGetSecondsElapsed(&dout->startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&dout->lastClock, &currClock);
        dout->lastClock = currClock;

        if (AfxTestObjectFcc(dout->endpointNotifyObj, afxFcc_WND))
        {
            //AfxFormatWindowCaption(dout->endpointNotifyObj, "%0f, %u --- Qwadro Video Graphics Infrastructure --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build", dt, dout->m.outRate);
        }

        if (dout->endpointNotifyFn)
        {
            dout->endpointNotifyFn(dout->endpointNotifyObj, bufIdx);
        }
    }

    dout->presentingBufIdx = (afxAtom32)AFX_INVALID_INDEX;
    AfxPushInterlockedQueue(&dout->freeBuffers, (afxNat[]) { bufIdx });
    //AfxDecAtom32(&dout->m.submCnt);

    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawDevice AfxGetDrawOutputDevice(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetProvider(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxError AfxGetDrawOutputIdd(afxDrawOutput dout, afxNat code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    dout->iddCb(dout, code, dst);
    return err;
}

_AVX void** AfxGetDrawOutputUdd(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->udd;
}

_AVX void AfxGetDrawOutputFrequency(afxDrawOutput dout, afxNat* rate)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(rate);
    *rate = dout->outRate;
}

_AVX void AfxGetDrawOutputAspectRatio(afxDrawOutput dout, afxReal64* wpOverHp, afxReal64* wrOverHr, afxReal64* wwOverHw)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert3(wpOverHp, wrOverHr, wwOverHw);
    *wpOverHp = dout->wpOverHp;
    *wrOverHr = dout->wrOverHr;
    *wwOverHw = dout->wwOverHw;
}

_AVX void AfxGetDrawOutputResolution(afxDrawOutput dout, afxWhd res, afxReal* refreshRate)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(res);
    res[0] = dout->res[0];
    res[1] = dout->res[1];
    res[2] = dout->res[2];
    AfxAssert(refreshRate);
    *refreshRate = dout->refreshRate;
}

_AVX void AfxGetDrawOutputExtent(afxDrawOutput dout, afxWhd whd)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    whd[0] = dout->whd[0];
    whd[1] = dout->whd[1];
    whd[2] = dout->whd[2];
}

_AVX void AfxGetDrawOutputExtentAsNdc(afxDrawOutput dout, afxV3d whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert(whd);
    afxWhd whd2;
    AfxGetDrawOutputExtent(dout, whd2);
    AfxV3dSet(whd, AfxNdcf(whd2[0], dout->res[0]), AfxNdcf(whd2[1], dout->res[1]), (afxReal)1);
}

_AVXINL afxError _AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    
    AfxWhdCopy(dout->whd, whd);
    dout->wwOverHw = (afxReal64)dout->whd[0] / (afxReal64)dout->whd[1];

    //afxEvent ev;
    //AfxEventDeploy(&ev, AFX_EVENT_DOUT_EXTENT, &dout->obj, dout->whd);
    //AfxNotifyObject(&dout->obj, &ev);

    AfxRevalidateDrawOutputBuffers(dout);
    return err;
}

_AVX afxError AfxAdjustDrawOutput(afxDrawOutput dout, afxWhd const whd)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    afxWhd whd2;
    AfxAssert4(whd, whd[0], whd[1], whd[2]);
    whd2[0] = AfxMax(whd[0], 1);
    whd2[1] = AfxMax(whd[1], 1);
    whd2[2] = AfxMax(whd[2], 1);
    AfxAssertExtent(dout->res[0], whd[0]);
    AfxAssertExtent(dout->res[1], whd[1]);
    AfxAssertExtent(dout->res[2], whd[2]);

    afxBool changed = (whd2[0] != dout->whd[0] || whd2[1] != dout->whd[1] || whd2[2] != dout->whd[2]);

    if (changed)
    {
        ++dout->resizing; // temporarily lock it to avoid reentrance of platform hooks.

        if (dout->adjustCb)
        {
            if (dout->adjustCb(dout, whd2))
                AfxThrowError();
        }
        else if (_AfxAdjustDrawOutput(dout, whd2))
            AfxThrowError();

        if (!err)
        {
            afxV2d ndc;
            AfxGetDrawOutputExtent(dout, whd2);
            AfxNdcV2d(ndc, AfxSpawnV2d(whd2[0], whd2[1]), AfxSpawnV2d(dout->res[0], dout->res[1]));
            AfxLogEcho("Draw output %03u adjusted. %ux%u %ux%u %.3fx%.3f %f", AfxGetObjectId(dout), whd2[0], whd2[1], dout->res[0], dout->res[1], ndc[0], ndc[1], dout->wwOverHw);
        }

        --dout->resizing;
    }
    return err;
}

_AVX afxError AfxAdjustDrawOutputFromNdc(afxDrawOutput dout, afxV3d const whd)
// normalized (bethween 0 and 1 over the total available) porportions of exhibition area.
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssert4(whd, whd[0], whd[1], whd[2]);

    afxWhd const whd2 =
    {
        (afxNat)AfxUnndcf(AfxMax(1, whd[0]), dout->res[0]),
        (afxNat)AfxUnndcf(AfxMax(1, whd[1]), dout->res[1]),
        AfxMax(1, whd[2])
    };
    return AfxAdjustDrawOutput(dout, whd2);
}

_AVX afxBool AfxResetDrawOutputResolution(afxDrawOutput dout, afxWhd const res, afxReal refreshRate, afxReal64 physAspectRatio)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxBool rslt = TRUE;

    afxV3d whdNdc;
    AfxGetDrawOutputExtentAsNdc(dout, whdNdc);

    if (res)
    {
        AfxAssert3(res[0], res[1], res[2]);
        dout->res[0] = AfxMax(1, res[0]);
        dout->res[1] = AfxMax(1, res[1]);
        dout->res[2] = AfxMax(1, res[2]);
        dout->wrOverHr = (afxReal64)dout->res[0] / (afxReal64)dout->res[1];
    }

    if (refreshRate)
        dout->refreshRate = refreshRate;

    if (physAspectRatio)
        dout->wpOverHp = physAspectRatio;

    AfxLogEcho("afxDrawOutput %03u readapted. %ux%u @ %fHz <%fr, %fp>", AfxGetObjectId(dout), dout->res[0], dout->res[1], dout->refreshRate, dout->wrOverHr, dout->wpOverHp);

    // readjust draw output extent if it overflows the new resolution.

    afxWhd whd;
    afxBool readjust = FALSE;
    AfxGetDrawOutputExtent(dout, whd);

    if (whd[0] > dout->res[0])
        whd[0] = dout->res[0], readjust = TRUE;

    if (whd[1] > dout->res[1])
        whd[1] = dout->res[1], readjust = TRUE;

    if (whd[2] > dout->res[2])
        whd[2] = dout->res[2], readjust = TRUE;

    if (readjust)
        AfxAdjustDrawOutputFromNdc(dout, whdNdc);

    return rslt;
}

_AVX afxError AfxPrintDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxNat portIdx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    avxCanvas canv;

    if (!AfxGetDrawOutputCanvas(dout, bufIdx, &canv)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);

        if (AfxPrintDrawBuffer(canv, 0, portIdx, NIL, uri))
            AfxThrowError();
    }
    return err;
}

// CONNECTION //////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetDrawOutputContext(afxDrawOutput dout, afxDrawContext* context)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawContext dctx = AfxGetLinker(&dout->dctx);

    if (dctx)
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        AfxAssert(context);
        *context = dctx;
    }
    return !!dctx;
}

_AVX afxError AfxDisconnectDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!AfxReconnectDrawOutput(dout, NIL))
        AfxThrowError();

    afxDrawContext dctx;
    AfxAssert(!AfxGetDrawOutputContext(dout, &dctx));
    return err;
}

_AVX afxBool AfxReconnectDrawOutput(afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    
    AfxLockMutex(&ddev->relinkedCndMtx);

    while (ddev->relinking)
        AfxWaitCondition(&ddev->relinkedCnd, &ddev->relinkedCndMtx);

    afxDrawContext from = NIL;
    AfxGetDrawOutputContext(dout, &from);

    if (dctx != from)
    {
        ddev->relinking = TRUE;
        dout->reconnecting = TRUE;

        if (from)
        {
            AfxAssertObjects(1, &from, afxFcc_DCTX);

            AfxWaitForDrawOutput(dout);

            afxDrawOutput found;
            AfxIterateLinkageB2F(AFX_OBJECT(afxDrawOutput), found, &from->outputs, dctx)
            {
                AfxAssertObjects(1, &found, afxFcc_DOUT);

                if (found == dout)
                {
                    do
                    {
                        AfxAssertObjects(1, &from, afxFcc_DCTX);

                        if (AfxWaitForDrawContext(from, AFX_TIME_INFINITE))
                            AfxThrowError();

                    } while (AfxLoadAtom32(&dout->submCnt));

                    AfxAssertObjects(1, &dout, afxFcc_DOUT);

                    if (ddev->relinkDoutCb && ddev->relinkDoutCb(ddev, NIL, 1, &dout)) // if disconnection was refused
                    {
                        AfxThrowError();
                    }
                    else
                    {
                        AfxPopLinkage(&dout->dctx);
                        AfxRevalidateDrawOutputBuffers(dout);
                        AfxAssertObjects(1, &from, afxFcc_DCTX);
                        AfxReleaseObjects(1, &from);
                    }
                    break;
                }
            }
            AfxAssert(found == dout);
        }

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
            {
                AfxThrowError();
                dctx = NIL; // so just disconnect
            }
            else
            {
                AfxReacquireObjects(1, &dctx);
                AfxPushLinkage(&dout->dctx, &dctx->outputs);

                AfxAssertObjects(1, &dout, afxFcc_DOUT);

                if (ddev->relinkDoutCb && ddev->relinkDoutCb(ddev, dctx, 1, &dout)) // if reconnection was refused
                {
                    AfxThrowError();
                    AfxPopLinkage(&dout->dctx);
                    AfxReleaseObjects(1, &dctx);
                }
                else
                {
                    AfxRevalidateDrawOutputBuffers(dout);
                }
            }
        }

        dout->reconnecting = FALSE;
        ddev->relinking = FALSE;
    }

    AfxUnlockMutex(&ddev->relinkedCndMtx);
    AfxSignalCondition(&ddev->relinkedCnd);
    return !err;
}

// BUFFERIZATION ///////////////////////////////////////////////////////////////

_AVX afxNat AfxCountDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    return dout->bufCnt;
}

_AVX afxError AfxWaitForDrawOutput(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    AfxLockMutex(&dout->idleCndMtx);

    while (dout->submCnt)
        AfxWaitCondition(&dout->idleCnd, &dout->idleCndMtx);

    AfxUnlockMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError AfxLockDrawOutputBuffer(afxDrawOutput dout, afxTime timeout, afxNat *bufIdx)
// Pull an available draw output buffer (usually from the WSI).
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat bufIdx2 = AFX_INVALID_INDEX;

    if (AfxLoadAtom32(&dout->reconnecting)) AfxThrowError();
    else
    {
        if (dout->lockCb)
        {
            if (!(err = dout->lockCb(dout, timeout, &bufIdx2)))
            {
                AfxAssert(AFX_INVALID_INDEX != bufIdx2);
                AfxAssertRange(dout->bufCnt, bufIdx2, 1);
            }
        }
        else
        {
            afxBool success = FALSE;
            afxTime time, t2;
            AfxGetTime(&time);

            afxClock start, last;

            if (timeout)
            {
                AfxGetClock(&start);
                last = start;
            }

            while (1)
            {
                afxNat lockedBufIdx = AFX_INVALID_INDEX;

                if (AfxPopInterlockedQueue(&dout->freeBuffers, &lockedBufIdx))
                {
                    avxCanvas canv = dout->canvases[lockedBufIdx];

                    if (canv)
                    {
                        AfxAssertObjects(1, &canv, afxFcc_CANV);
                    }
                    bufIdx2 = lockedBufIdx;
                    success = TRUE;
                }

                if (success)
                    break;

                if (!success && (!timeout || timeout < AfxGetTime(&t2) - time))
                {
                    err = __LINE__;
                    bufIdx2 = AFX_INVALID_INDEX;
                    break;
                }
            }
        }
    }
    AfxAssert(bufIdx);
    *bufIdx = bufIdx2;
    return err;
}

_AVX afxError AfxDisposeDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    
    if (bufIdx < dout->bufCnt)
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxNat[]) { bufIdx });
    else
        AfxThrowError();

    return err;
}

_AVX afxNat AfxGetDrawOutputCanvases(afxDrawOutput dout, afxNat first, afxNat cnt, avxCanvas canvases[])
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat totalBufCnt = dout->bufCnt;
    AfxAssertRange(totalBufCnt, first, cnt);
    afxNat rslt = 0;

    if (totalBufCnt > first)
    {
        AfxAssert(canvases);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxNat bufIdx = first + i;
            AfxAssertRange(totalBufCnt, bufIdx, 1);
            avxCanvas canv;

            if ((canv = dout->canvases[bufIdx]))
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                ++rslt;
            }
            canvases[i] = canv;
        }
    }
    return rslt;
}

_AVX afxBool AfxGetDrawOutputCanvas(afxDrawOutput dout, afxNat bufIdx, avxCanvas* canvas)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxBool rslt;
    avxCanvas canv;
    while ((rslt = AfxGetDrawOutputCanvases(dout, bufIdx, 1, &canv)))
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);
        AfxAssert(canvas);
        *canvas = canv;
        break;
    }
    return rslt;
}

_AVX afxNat AfxGetDrawOutputBuffers(afxDrawOutput dout, afxNat first, afxNat cnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxNat totalBufCnt = dout->bufCnt;
    AfxAssertRange(totalBufCnt, first, cnt);
    afxNat rslt = 0;

    if (totalBufCnt > first)
    {
        AfxAssert(rasters);
    
        for (afxNat i = 0; i < cnt; i++)
        {
            afxNat bufIdx = first + i;
            AfxAssertRange(totalBufCnt, bufIdx, 1);
            afxRaster ras = NIL;
            avxCanvas canv;

            if (AfxGetDrawOutputCanvas(dout, bufIdx, &canv))
            {
                AfxAssertObjects(1, &canv, afxFcc_CANV);

                if (AfxGetDrawBuffers(canv, 0, 1, &ras))
                {
                    AfxAssertObjects(1, &ras, afxFcc_RAS);
                    ++rslt;
                }
            }
            rasters[i] = ras;
        }
    }
    return rslt;
}

_AVX afxBool AfxGetDrawOutputBuffer(afxDrawOutput dout, afxNat bufIdx, afxRaster* raster)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxAssertRange(dout->bufCnt, bufIdx, 1);
    afxBool rslt;
    afxRaster ras;
    while ((rslt = AfxGetDrawOutputBuffers(dout, bufIdx, 1, &ras)))
    {
        AfxAssertObjects(1, &ras, afxFcc_RAS);
        AfxAssert(raster);
        *raster = ras;
        break;
    }
    return rslt;
}

_AVX afxError AfxRevalidateDrawOutputBuffers(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);

    if (!err)
    {
        AfxTryAssertObjects(dout->bufCnt, dout->canvases, afxFcc_CANV);
        AfxReleaseObjects(dout->bufCnt, dout->canvases);
        AfxAssert(!dout->canvases[0]);
        afxDrawContext dctx;

        if (AfxGetDrawOutputContext(dout, &dctx))
        {
            AfxWaitForDrawOutput(dout);

            afxSurfaceConfig surCfgs[3] =
            {
                {
                    .fmt = dout->pixelFmt,
                    .sampleCnt = 1,
                    .rasUsage = dout->bufUsage,
                    .rasFlags = dout->bufFlags
                },
                {
                    .fmt = dout->pixelFmtDs[0],
                    .sampleCnt = 1,
                    .rasUsage = dout->bufUsageDs[0],
                    .rasFlags = dout->bufFlagsDs[0]
                },
                {
                    .fmt = dout->pixelFmtDs[1],
                    .sampleCnt = 1,
                    .rasUsage = dout->bufUsageDs[1],
                    .rasFlags = dout->bufFlagsDs[1]
                }
            };

            afxNat surCnt = 1;

            if (dout->pixelFmtDs[0])
                ++surCnt;

            if (dout->pixelFmtDs[1])
                ++surCnt;

            if (AfxCoacquireCanvas(dctx, dout->whd, surCnt, surCfgs, dout->bufCnt, dout->canvases)) AfxThrowError();
            else
            {
                AfxAssertObjects(dout->bufCnt, dout->canvases, afxFcc_CANV);
                
                for (afxNat i = 0; i < dout->bufCnt; i++)
                {
                    avxCanvas canv = dout->canvases[i];
                    AfxAssertObjects(1, &canv, afxFcc_CANV);
                    AfxRedoDrawBuffers(canv);
                }
            }
        }
    }
    return err;
}

_AVX afxError _AvxDoutDtorCb(afxDrawOutput dout)
{
    afxError err = AFX_ERR_NONE;
    /// dout must be a valid afxDrawOutput handle.
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    afxDrawDevice ddev = AfxGetDrawOutputDevice(dout);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxDisconnectDrawOutput(dout);
    //dout->disabled;

    AfxAssert(!dout->idd);

    _AvxDoutSuspendFunction(dout);

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->canvases[0]),
            .var = (void**)&dout->canvases
        }
    };

    AfxDeallocateInstanceData(dout, AFX_COUNTOF(stashs), stashs);

    AfxDismantleInterlockedQueue(&dout->freeBuffers);

    AfxDismantleSlock(&dout->suspendSlock);

    AfxCleanUpCondition(&dout->idleCnd);
    AfxCleanUpMutex(&dout->idleCndMtx);

    return err;
}

_AVX afxError _AvxDoutCtorCb(afxDrawOutput dout, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(dout, sizeof(dout[0]));

    afxDrawDevice ddev = args[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawOutputConfig const* cfg = ((afxDrawOutputConfig const *)args[1]) + invokeNo;
    AfxAssert(cfg);
    
    AfxPushLinkage(&dout->dctx, NIL);

    // endpoint
    dout->resizing = FALSE;
    dout->resizable = TRUE;
    dout->refreshRate = 1;
    dout->res[0] = 1;
    dout->res[1] = 1;
    dout->res[2] = 1;
    dout->wrOverHr = dout->res[0] / dout->res[1];
    dout->wpOverHp = AfxFindPhysicalAspectRatio(dout->res[0], dout->res[1]);
    dout->presentAlpha = cfg->presentAlpha; // consider transparency for window composing.
    dout->presentTransform = cfg->presentTransform; // The default is to leave it as it is.
    dout->presentMode = cfg->presentMode; // The default avxPresentMode_LIFO is already zero.
    dout->doNotClip = !!cfg->doNotClip; // don't spend resources doing off-screen draw.
    dout->udd[0] = cfg->udd[0];
    dout->udd[1] = cfg->udd[1];
    dout->udd[2] = cfg->udd[2];
    dout->udd[3] = cfg->udd[3];
    dout->endpointNotifyObj = cfg->endpointNotifyObj;
    dout->endpointNotifyFn = cfg->endpointNotifyFn;
    dout->idd = NIL;

    // canvas
    dout->whd[0] = AfxMax(1, cfg->whd[0]);
    dout->whd[1] = AfxMax(1, cfg->whd[1]);
    dout->whd[2] = AfxMax(1, cfg->whd[2]);
    dout->wwOverHw = dout->whd[0] / dout->whd[1];
    dout->colorSpc = cfg->colorSpc ? cfg->colorSpc : avxColorSpace_SRGB; // sRGB is the default
    dout->pixelFmt = cfg->pixelFmt ? cfg->pixelFmt : avxFormat_BGRA8; // or avxFormat_RGBA8R ?
    dout->bufUsage = cfg->bufUsage | afxRasterUsage_DRAW;
    dout->bufFlags = cfg->bufFlags;
    dout->pixelFmtDs[0] = cfg->pixelFmtDs[0];
    dout->pixelFmtDs[1] = cfg->pixelFmtDs[1];
    dout->bufUsageDs[0] = cfg->bufUsageDs[0] | afxRasterUsage_DRAW;
    dout->bufUsageDs[1] = cfg->bufUsageDs[1] | afxRasterUsage_DRAW;
    dout->bufFlagsDs[0] = cfg->bufFlagsDs[0];
    dout->bufFlagsDs[1] = cfg->bufFlagsDs[1];
    
    // swapchain
    dout->bufCnt = AfxMax(1, cfg->minBufCnt); // 2 or 3; double or triple buffered for via-memory presentation.

    dout->submCnt = 0;
    dout->presentingBufIdx = AFX_INVALID_INDEX;
    dout->reconnecting = FALSE;
    dout->suspendCnt = 1;
    AfxDeploySlock(&dout->suspendSlock);

    AfxSetUpMutex(&dout->idleCndMtx, AFX_MTX_PLAIN);
    AfxSetUpCondition(&dout->idleCnd);

    dout->lockCb = NIL;
    dout->canvases = NIL;
    AfxDeployInterlockedQueue(&dout->freeBuffers, sizeof(afxNat), AFX_ALIGNED_SIZEOF(dout->bufCnt, 2));

    for (afxNat i = 0; i < dout->bufCnt; i++)
        AfxPushInterlockedQueue(&dout->freeBuffers, (afxNat[]) { i });

    afxObjectStash const stashs[] =
    {
        {
            .cnt = dout->bufCnt,
            .siz = sizeof(dout->canvases[0]),
            .var = (void**)&dout->canvases
        }
    };

    if (AfxAllocateInstanceData(dout, AFX_COUNTOF(stashs), stashs)) AfxThrowError();
    else
    {
        AfxZero(dout->canvases, sizeof(dout->canvases[0]) * dout->bufCnt);

        AfxAssert(dout->res[0]);
        AfxAssert(dout->res[1]);
        AfxAssert(dout->res[2]);
        AfxAssertExtent(dout->res[0], dout->whd[0]);
        AfxAssertExtent(dout->res[1], dout->whd[1]);
        AfxAssertExtent(dout->res[2], dout->whd[2]);

        AfxAssert(dout->bufCnt);
        AfxAssert(dout->bufUsage & afxRasterUsage_DRAW);
        AfxAssert(dout->refreshRate);
        AfxAssert(dout->wpOverHp);
        AfxAssert(dout->wrOverHr);
        AfxAssert(dout->wwOverHw);
        

        if (err)
            AfxDeallocateInstanceData(dout, AFX_COUNTOF(stashs), stashs);
    }
    return err;
}

_AVX afxClassConfig const _AvxDoutStdImplementation =
{
    .fcc = afxFcc_DOUT,
    .name = "DrawOutput",
    .desc = "Draw Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawOutput)),
    .ctor = (void*)_AvxDoutCtorCb,
    .dtor = (void*)_AvxDoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxConfigureDrawOutput(afxNat ddevId, afxDrawOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ddevId != AFX_INVALID_INDEX);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        AfxAssert(cfg);
        *cfg = (afxDrawOutputConfig const) { 0 };
        cfg->minBufCnt = 3;
        cfg->bufUsage = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
        cfg->bufUsageDs[0] = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
        cfg->bufUsageDs[1] = afxRasterUsage_DRAW | afxRasterUsage_SAMPLING;
        cfg->colorSpc = avxColorSpace_SRGB;
        cfg->pixelFmt = avxFormat_BGRA8;
        cfg->pixelFmtDs[0] = avxFormat_X8D24;
        cfg->pixelFmtDs[1] = NIL;
    }
    return err;
}

_AVX afxError AfxOpenDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* output)
// file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxCallDevice(&ddev->dev, 1, NIL)) AfxThrowError(); // let the device build its DPUs.
        else
        {
            afxClass* cls = (afxClass*)AvxGetDrawOutputClass(ddev);
            AfxAssertClass(cls, afxFcc_DOUT);
            afxDrawOutput dout;

            if (!cfg) AfxThrowError();
            else
            {
                if (AfxAcquireObjects(cls, 1, (afxObject*)&dout, (void const*[]) { ddev, cfg })) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &dout, afxFcc_DOUT);
                    AfxAssert(output);
                    *output = dout;
                }
            }
        }
    }
    return err;
}