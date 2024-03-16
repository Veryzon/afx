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

#ifndef AFX_POSE_H
#define AFX_POSE_H

#include "qwadro/math/afxTransform.h"


typedef enum quaternion_mode
{
    BlendQuaternionDirectly = 0x0,
    BlendQuaternionInverted = 0x1,
    BlendQuaternionNeighborhooded = 0x2,
    BlendQuaternionAccumNeighborhooded = 0x3,
} quaternion_mode;


AFX_DEFINE_STRUCT(awxArticulation)
{
    afxReal             weight;
    afxNat              cnt;
    afxTransform        xform;
    afxNat              traversalId;
};

AFX_DEFINE_STRUCT(awxPose)
{
    afxNat              artCnt;
    awxArticulation*    arts;
    afxReal             fillThreshold;
    afxNat              traversalId;
};

/// O objeto afxPostura � um buffer usado para manter o estado de um afxSkeleton de um afxModel como expressado em sua "pose local". 
/// Neste contexto, "pose local" significa que cada articula��o no afxSkeleton � representada por um afxTransform que � relativo a sua articula��o-parente imediata. 
/// Voc� pode criar uma afxPostura assim: 

AKX afxError        AfxAcquirePoses(void *sim, afxNat cnt, afxNat const artCnt[], awxPose *lp[]);

/// e quando voc� estiver satisfeito de us�-lo, voc� deve liber�-lo assim: 

AKX void            AfxReleasePoses(afxNat cnt, awxPose *lp[]);

/// Voc� pode encontrar o n�mero de articula��es representadas pelo awxPose assim: 

AKX afxNat          AfxGetPoseCapacity(awxPose const *lp);

/// Em qualquer tempo, voc� pode inspecionar ou modificar o estado alojado de uma articula��o na awxPose. Voc� acessa o estado da articula��o como um afxTransform assim: 

AKX afxTransform*   AfxGetPoseTransform(awxPose const *pose, afxNat artIdx);

/// Note que voc� recebe um ponteiro para o afxTransform alojado para a articula��o, assim sendo, modific�-lo modificar� a vers�o permanente alojada na awxPose. 
/// Portanto, n�o h� uma fun��o "Set" para AfxGetPoseTransform() devido ao fato de que voc� pode ler de ou escrever para o ponteiro que voc� recebe.

AKX void            AfxCopyPose(awxPose *pose, awxPose const *from);

AKX void            AfxApplyRootMotionVectorsToPose(awxPose *pose, afxReal const translation[3], afxReal const rotation[3]);

AKX void            AfxAccumulateLocalTransform(awxPose *LocalPose, int LocalPoseBoneIndex, int SkeletonBoneIndex, float Weight, const afxSkeleton ReferenceSkeleton, quaternion_mode Mode, const afxTransform *Transform);

#endif//AFX_POSE_H
