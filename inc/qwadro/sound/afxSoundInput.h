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

#ifndef ASX_SOUND_INPUT_H
#define ASX_SOUND_INPUT_H

#include "qwadro/sound/afxSoundDefs.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sound/afxSoundBridge.h"

typedef enum asxEventId
{
    asxEventId_FENCE,
    asxEventId_EXECUTE,
    asxEventId_UPLOAD,
    asxEventId_DOWNLOAD,
    asxEventId_PREFETCH,
} asxEventId;

AFX_DEFINE_STRUCT(asxEvent)
{
    asxEventId  id;
    afxBool         posted, accepted;
    void*           udd[0];
};

typedef afxBool(*afxSoundInputProc)(afxSoundInput sin, asxEvent const* ev);

AFX_DEFINE_STRUCT(afxSoundInputConfig)
{
    afxSoundInputProc   proc;
    afxNat              dragTxuBase;
    afxNat              dragTxuCnt;

    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxNat              minVtxBufCap;
    afxNat              maxVtxBufCap;
    afxNat              minIdxBufCap;
    afxNat              maxIdxBufCap;
    void*               udd;
};

ASX afxSoundDevice  AfxGetSoundInputDevice(afxSoundInput sin);

ASX void*           AfxGetSoundInputUdd(afxSoundInput sin);

// Connection

ASX afxError        AfxDisconnectSoundInput(afxSoundInput sin);
ASX afxBool         AfxReconnectSoundInput(afxSoundInput sin, afxSoundContext sctx);
ASX afxBool         AfxGetSoundInputContext(afxSoundInput sin, afxSoundContext* context);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireSoundInput(afxNat sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input);

ASX afxError        AfxOpenSoundInput(afxNat sdevId, afxUri const* endpoint, afxSoundInputConfig const* cfg, afxSoundInput* input);

#endif//ASX_SOUND_INPUT_H
