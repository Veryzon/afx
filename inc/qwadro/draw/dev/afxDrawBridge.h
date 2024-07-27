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

/// No QWADRO, uma fila � dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
/// Isto porque, no QWADRO, voc� n�o executa as opera��es de desenho. Ao inv�s, voc� submete elas a serem processadas atrav�s de um afxDrawInput.
/// Uma afxDrawBridge livre, por sua vez, e que pode realizar determinada opera��o submetida, ent�o puxar� a submiss�o.

/// Uma afxDrawBridge � um objeto que mapeia afxDrawQueue para uma porta de execu��o de cargas de trabalho.
/// A afxDrawQueue � uma unidade de gest�o de transporte, onde se armazena a carga de trabalho at� sua consuma��o.

#ifndef AVX_DRAW_BRIDGE_H
#define AVX_DRAW_BRIDGE_H

#include "qwadro/draw/pipe/avxPipeline.h"
#include "qwadro/draw/afxRaster.h"
#include "qwadro/draw/afxBuffer.h"

typedef enum afxDrawBridgeFlag
{
    afxDrawBridgeFlag_DRAW      = AFX_BIT(0), // supports draw ops
    afxDrawBridgeFlag_DRAW_AUX  = AFX_BIT(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE   = AFX_BIT(2), // supports compute ops
    afxDrawBridgeFlag_TRANSFER  = AFX_BIT(3), // supports transfer ops
    afxDrawBridgeFlag_VHS       = AFX_BIT(4), // supports VHS enc/dec
} afxDrawBridgeFlags;

AFX_DEFINE_STRUCT(afxDrawBridgeConfig)
{
    afxDrawBridgeFlags  flags;
    afxNat              portIdx;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(afxDrawCommand)
{
    afxNat              cmdNo;
    afxLinkage          imm;
    afxLinkage          drawCall;
    afxLinkage          stateCh;
    afxLinkage          pipBind;
    afxLinkage          vbufBind;
    afxLinkage          ibufBind;
    afxLinkage          bufBind;
};

typedef enum
{
    avxSubmission_WORK = 1,
    avxSubmission_PRESENT,
    avxSubmission_TRANSFER,
    avxSubmission_MAPPING
} avxSubmissionType;

AFX_DEFINE_UNION(avxSubmissionData)
{
    struct
    {
        afxFlags            flags;
        afxNat              waitCnt;
        afxSemaphore        waitSems[4];
        afxNat64            waitValues[4];
        avxPipelineStage    waitStageMasks[4];
        afxNat32            waitReserveds[4];
        afxNat              cmdbCnt;
        avxCmdb             cmdbs[4];
        afxNat              signalCnt;
        afxSemaphore        signalSems[4];
        afxNat64            signalValues[4];
        avxPipelineStage    signalStageMasks[4];
        afxNat32            signalReserveds[4];
        afxFence            fence;
    }                       work;
    struct
    {
        afxNat          waitCnt;
        afxSemaphore    waits[4];
        afxNat          bufCnt;
        afxDrawOutput   outputs[4];
        afxNat          buffers[4];
        afxResult       results[4];
    }                   present;
    struct
    {
        union
        {
            struct
            {
                afxRaster       ras;
                afxRasterIo     op;
                union
                {
                    void*       dst;
                    void const* src;
                    afxBuffer   buf;
                    afxStream   iob;
                };
            }                   img;
            struct
            {
                afxBuffer       buf;
                afxSize         offset;
                afxNat          range;
                union
                {
                    void*       dst;
                    void const* src;
                    afxStream   iob;
                };
                afxSize         dataOff;
            }                   data;
        };
        afxFcc              srcFcc;
        afxFcc              dstFcc;
        afxCodec            codec;
        afxNat              decompressedSiz;
        afxNat              waitCnt;
        afxSemaphore        waits[4];
        afxNat              signalCnt;
        afxSemaphore        signals[4];
        afxFence            fence;
    } transfer;
    struct
    {
        afxBuffer           buf;
        afxSize             off;
        afxNat              ran;
        afxFlags            flags;
    } map;
};

AFX_DEFINE_STRUCT(avxSubmission)
{
    afxNat              baseQueIdx; // AFX_INVALID_INDEX to any.
    afxNat              queCnt;
    afxNat              submType;
    afxError            (*f)(void*, void*, afxNat, void*);
    void*               udd;
    afxNat              dataSiz;
    avxSubmissionData*  data;
};

AFX_DEFINE_STRUCT(afxExecutionRequest)
{
    afxSemaphore        wait;
    avxCmdb        cmdb;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxPresentationRequest)
{
    afxSemaphore        wait;
    afxDrawOutput       dout;
    afxNat              bufIdx;
};

AFX_DEFINE_STRUCT(afxTransferRequest)
{
    union
    {
        struct
        {
            afxRaster       ras;
            afxRasterIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxBuffer   buf;
                afxStream   iob;
            };
        }                   img;
        struct
        {
            afxBuffer       buf;
            afxBufferIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
        }                   data;
    };

    afxFcc              srcFcc;
    afxFcc              dstFcc;

    afxCodec            codec;
    afxNat              decompressedSiz;

    afxSemaphore        wait;
    afxSemaphore        signal;
};

AVX afxDrawDevice       AfxGetDrawBridgeDevice(afxDrawBridge ddge);
AVX afxDrawContext      AfxGetDrawBridgeContext(afxDrawBridge ddge);

AVX afxNat              AfxGetDrawBridgePort(afxDrawBridge ddge);

AVX afxNat              AfxEnqueueExecutionRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxExecutionRequest const req[]);
AVX afxNat              AfxEnqueueTransferRequest(afxDrawBridge ddge, afxFence fenc, afxNat cnt, afxTransferRequest const req[]);
AVX afxNat              AfxEnqueuePresentRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[]);
AVX afxNat              AfxEnqueueStampRequest(afxDrawBridge ddge, afxNat cnt, afxPresentationRequest const req[], afxV2d const origin, afxString const* caption);

AVX afxNat              AvxRequestDrawWork(afxDrawBridge ddge, afxNat cnt, avxSubmission const subm[]);

#endif//AVX_DRAW_BRIDGE_H
