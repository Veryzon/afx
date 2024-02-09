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

// This section is part of SIGMA GL/2.

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, h� um conceito de uma fila de submiss�o de trabalho para a GPU. Por�m, diferentemente do Vulkan, no QWADRO esta "fila" � separada em dois.
// A primeira parte cont�m a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AFX_DRAW_INPUT_H
#define AFX_DRAW_INPUT_H

#include "qwadro/io/afxUri.h"
#include "qwadro/draw/pipe/afxPipeline.h"
// provided classes.
#include "qwadro/draw/afxCamera.h"

typedef enum afxDrawEventId
{
    afxDrawEventId_FENCE,
    afxDrawEventId_EXECUTE,
    afxDrawEventId_PRESENT,
    afxDrawEventId_UPLOAD,
    afxDrawEventId_DOWNLOAD,
} afxDrawEventId;

AFX_DEFINE_STRUCT(afxDrawEvent)
{
    _AFX_DBG_FCC;
    afxDrawEventId  id;
    afxBool         posted, accepted;
    afxDrawThread   dthr;
    afxDrawDevice   ddev;
    afxNat          portIdx;
    afxDrawContext  dctx;
    union
    {
        void*       receiver;
        void*       obj;
    };
    void*           udd[4];
};

typedef afxBool(*afxDrawInputProc)(afxDrawInput din, afxDrawEvent const* ev);

AFX_DEFINE_STRUCT(afxDrawInputConfig)
{
    afxUri const*       endpoint;
    afxDrawInputProc    proc;
    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxNat              minVtxBufCap;
    afxNat              maxVtxBufCap;
    afxNat              minIdxBufCap;
    afxNat              maxIdxBufCap;
    void*               udd;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_INPUT_C
#ifndef _AFX_DRAW_INPUT_IMPL
AFX_OBJECT(afxDrawInput)
#else
struct afxBaseDrawInput
#endif
{
    afxLinkage          endpoint;
    afxDrawContext      dctx; // bound context
    afxMmu              mmu;

    afxChain            classes;
    afxClass            cameras;
    afxClass            ibuffers;
    afxClass            vbuffers;

    afxClipSpace        cachedClipCfg;

    afxFixedUri128      txdUris[8];
    afxFile             txdHandles[8];

    afxError            (*submitCb)(afxDrawInput, afxNat, afxDrawScript[], afxSemaphore[], afxPipelineStage const[], afxSemaphore[]);
    afxError            (*presentCb)(afxDrawInput, afxNat, afxDrawOutput[], afxNat[], afxSemaphore[]);

    afxArray            scripts;
    afxNat              minScriptReserve;

    afxNat              minVtxBufSiz;
    afxNat              maxVtxBufSiz;
    afxNat              minIdxBufSiz;
    afxNat              maxIdxBufSiz; // 13500000

    afxDrawInputProc    procCb;
    void*               udd;
};
#endif
#endif

AFX afxDrawDevice       AfxGetDrawInputDevice(afxDrawInput din);

// Connection
AFX afxBool             AfxDrawInputIsOnline(afxDrawInput din);
AFX afxDrawContext      AfxGetDrawInputContext(afxDrawInput din);
AFX afxError            AfxDisconnectDrawInput(afxDrawInput din);
AFX afxBool             AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx);

AFX afxClass*           AfxGetCameraClass(afxDrawInput din);
AFX afxClass*           AfxGetVertexBufferClass(afxDrawInput din);
AFX afxClass*           AfxGetIndexBufferClass(afxDrawInput din);

AFX afxNat              AfxEnumerateCameras(afxDrawInput din, afxNat first, afxNat cnt, afxCamera cam[]);

AFX afxError            AfxExecuteDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[], afxSemaphore wait[], afxPipelineStage const waitStage[], afxSemaphore signal[]);

AFX_DEFINE_STRUCT(afxPresentationQueueing)
{
    afxNat          cnt;
    afxDrawOutput   outputs[4];
    afxNat          outBufIdx[4];
    afxSemaphore    wait;
};

AFX afxError            AfxPresentDrawBuffers(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outBufIdx[], afxSemaphore wait[]);

AFX void*               AfxGetDrawInputUdd(afxDrawInput din);

////////////////////////////////////////////////////////////////////////////////

AFX void            AfxDescribeClipSpace(afxDrawInput din, afxClipSpace* clip);

AFX void            AfxComputeLookToMatrices(afxDrawInput din, afxReal const eye[3], afxReal const dir[3], afxReal v[4][4], afxReal iv[4][4]);
AFX void            AfxComputeLookAtMatrices(afxDrawInput din, afxReal const eye[3], afxReal const target[3], afxReal v[4][4], afxReal iv[4][4]);

AFX void            AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputeOrthographicMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxAabb const aabb, afxReal p[4][4], afxReal ip[4][4]);

AFX void            AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxReal p[4][4], afxReal ip[4][4]);
AFX void            AfxComputePerspectiveMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4]);

AFX afxError        AfxUplinkTextureDictionaries(afxDrawInput din, afxNat baseSlotIdx, afxNat slotCnt, afxUri const uris[]);

#endif//AFX_DRAW_INPUT_H
