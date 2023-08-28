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

#ifndef AFX_DRAW_QUEUE_H
#define AFX_DRAW_QUEUE_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/mem/afxArena.h"

// No QWADRO, uma fila � dividida em duas partes, afxDrawInput na vanguarda e afxDrawQueue na retaguarda.
// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawQueue foi criada consequentemente e associado ao mesmo.
// Isto porque, no QWADRO, voc� executa as opera��es de desenho. Ao inv�s, voc� submete elas a serem processadas atrav�s de um afxDrawInput.
// Uma afxDrawQueue livre, por sua vez, e que pode realizar determinada opera��o submetida, ent�o puxar� a submiss�o.

AFX_DEFINE_STRUCT(_afxDqueSubm)
{
    afxLinkage              chain;
    afxNat                  submNo; // this submission number ordinal (B2F)
    afxNat                  reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime                 pushTime; // submission (into input) time
    afxTime                 pullTime; // fecth (by queue) time
    afxTime                 complTime; // completation time    
    struct
    {
        afxNat              scriptCnt;
        afxDrawScript       scripts[4];
        void*               data[4];
    };
    struct
    {
        afxNat              outputCnt;
        afxDrawOutput       outputs[4];
        afxNat              outBufIdx[4];
    };
    afxError                (*exec)(afxDrawQueue,_afxDqueSubm*);
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

AFX_DEFINE_STRUCT(afxDrawSubmissionSpecification)
{
    afxNat              scriptCnt;
    afxDrawScript*      scripts;
    afxNat              outputCnt;
    afxDrawOutput*      outputs;
    afxNat*             outBufIdx;
    void*               data;
};

typedef enum afxDrawQueueFlags
{
    AFX_DQUE_DRAW           = AFX_FLAG(0), // supports draw ops
    //AFX_DQUE_DRAW_AUX       = AFX_FLAG(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    AFX_DQUE_COMPUTE        = AFX_FLAG(2), // supports compute ops
    AFX_DQUE_TRANSFER       = AFX_FLAG(3), // supports transfer ops
    AFX_DQUE_VHS            = AFX_FLAG(4), // supports VHS enc/dec
} afxDrawQueueFlags;

AFX_DEFINE_STRUCT(afxDrawQueueSpecification)
{
    afxNat                  portIdx;
    afxDrawQueueFlags       caps;
};

AFX_DECLARE_STRUCT(_afxDqueVmt);

#ifdef _AFX_DRAW_QUEUE_C
AFX_OBJECT(afxDrawQueue)
#else
struct afxBaseDrawQueue
#endif
{
    _afxDqueVmt const*      vmt;
    afxNat                  portIdx;

    afxSlock                pendingChainSlock;
    afxChain                pendingChain;

    afxSlock                reqLock;
    afxBool                 locked;
    afxBool                 immediate; // 0 = deferred, 1 = immediate
    afxSlock                arenaSlock;
    afxArena                cmdArena; // used by submission commands, not script commands.
};

AFX afxDrawDevice           AfxGetDrawQueueDriver(afxDrawQueue dque);

AFX afxError                AfxWaitForDrawQueueIdle(afxDrawQueue dque); // Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

#endif//AFX_DRAW_QUEUE_H