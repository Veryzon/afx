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

#ifndef AFX_POSE_H
#define AFX_POSE_H

#include "qwadro/math/afxTransform.h"

AFX_DEFINE_STRUCT(awxPoseTransform)
{
    afxReal             weight;
    afxNat              cnt;
    afxTransform        xform;
    afxNat              traversalId;
};

AFX_DEFINE_STRUCT(awxPose)
{
    afxNat              cap;
    awxPoseTransform*   xforms;
    afxReal             fillThreshold;
    afxNat              traversalId;
};

/// O objeto afxPostura � um buffer usado para manter o estado de um awxSkeleton de um awxModel como expressado em sua "pose local". 
/// Neste contexto, "pose local" significa que cada articula��o no awxSkeleton � representada por um afxTransform que � relativo a sua articula��o-parente imediata. 
/// Voc� pode criar uma afxPostura assim: 

AFX afxError        AfxAcquirePoses(void *sim, afxNat cnt, afxNat const cap[], awxPose *lp[]);

/// e quando voc� estiver satisfeito de us�-lo, voc� deve liber�-lo assim: 

AFX void            AfxReleasePoses(afxNat cnt, awxPose *lp[]);

/// Voc� pode encontrar o n�mero de articula��es representadas pelo awxPose assim: 

AFX afxNat          AfxGetPoseCapacity(awxPose const *lp);

/// Em qualquer tempo, voc� pode inspecionar ou modificar o estado alojado de uma articula��o na awxPose. Voc� acessa o estado da articula��o como um afxTransform assim: 

AFX afxTransform*   AfxGetPoseTransform(awxPose const *pose, afxNat artIdx);

/// Note que voc� recebe um ponteiro para o afxTransform alojado para a articula��o, assim sendo, modific�-lo modificar� a vers�o permanente alojada na awxPose. 
/// Portanto, n�o h� uma fun��o "Set" para AfxGetPoseTransform() devido ao fato de que voc� pode ler de ou escrever para o ponteiro que voc� recebe.

AFX void            AfxCopyPose(awxPose *pose, awxPose const *from);

#endif//AFX_POSE_H