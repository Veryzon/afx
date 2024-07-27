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

/// O objeto afxSkeleton � uma cole��o hier�rquica de articula��es que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na anima��o do mesmo. 

/// Cada articula��o no arranjo do afxSkeleton aloja o transforme para aquela articula��o em dois meios diferentes. 
/// Primeiro, afxTransform "local" � o transforme decomposto da articula��o relativa a seu parente imediato (dado pelo �ndice do parente).
/// Se esta n�o houver parente (isto �, o �ndex do parente � igual a AFX_INVALID_INDEX), ent�o esta � relativa a origem do afxSkeleton.

/// Segundo, "iw" � uma afxM4d que � o transforme invertido no world-space para a articula��o na postura padr�o do afxSkeleton (isto �, a postura na qual o afxSkeleton foi originalmente modelado).

/// O afxTransform "local" � usado primariamente na composi��o de anima��es, e a afxM4d "iw" � usada primariamente para deforma��o de malha, raz�o pela qual esta informa��o � alojada em dois diferentes formatos.

/// Cada articula��o tamb�m opcionalmente cont�m dados definidos pelo usu�rio "UDD".
/// Este campo aponta para algum dado, estranho � l�gica do Qwadro, associado com a articula��o, se houver qualquer.

/// Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
/// Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
/// The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.

#ifndef AKX_SKELETON_H
#define AKX_SKELETON_H

#include "qwadro/space/akxPose.h"
#include "qwadro/space/akxPoseBuffer.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxSkeletonBlueprint)
{
    afxString32         id;
    afxNat              jointCnt;
    afxString const*    joints;
    afxNat const*       parentIdx;
    afxTransform const* local;
    afxReal const*      lodErr;
    afxM4d const*       iw;
    afxNat              lodType;
    afxBool             scalable;
    afxBool             deformable;
};

AKX afxBool             AfxGetSkeletonUrn(afxSkeleton skl, afxString* id);

AKX afxNat              AfxCountSkeletonJoints(afxSkeleton skl, afxReal allowedErr);

AKX afxNat              AfxFindSkeletonJoint(afxSkeleton skl, afxString const* id);
AKX afxNat              AfxFindSkeletonJoints(afxSkeleton skl, afxNat cnt, afxString const ids[], afxNat indices[]);

AKX afxString*          AfxGetSkeletonJointTag(afxSkeleton skl, afxNat jntIdx);

AKX afxM4d*             AfxGetSkeletonIwMatrices(afxSkeleton skl, afxNat baseJntIdx);

AKX void                AfxQuerySkeletonErrorTolerance(afxSkeleton skl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

AKX void                AfxLocalPoseFromPoseBuffer(afxSkeleton skl, akxPose lp, akxPoseBuffer const wp, afxM4d const offset, afxNat firstBone, afxNat boneCnt);
AKX void                AfxLocalPoseFromPoseBufferNoScale(afxSkeleton skl, akxPose lp, akxPoseBuffer const wp, afxM4d const offset, afxNat firstBone, afxNat boneCnt);
AKX void                AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat jntIdx, akxPose const lp, afxM4d const offset, afxM4d m, afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse);
AKX void                AfxGetSkeletonAttachmentOffset(afxSkeleton skl, afxNat jntIdx, akxPose const lp, afxM4d const offset, afxM4d m, afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse);

AKX void                AfxBuildIndexedCompositeBuffer(afxSkeleton skl, akxPoseBuffer const wp, afxNat const* indices, afxNat idxCnt, afxM4d buffer[]);
AKX void                AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, akxPoseBuffer const wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4]);

AKX void                AfxComputeRestLocalPose(afxSkeleton skl, afxNat baseJnt, afxNat jntCnt, akxPose lp);
AKX void                AfxComputeRestPoseBuffer(afxSkeleton skl, afxNat baseJnt, afxNat jntCnt, afxM4d const offset, akxPoseBuffer wp);

AKX void                AfxComputePoseBuffer(afxSkeleton skl, afxNat baseJnt, afxNat jntCnt, akxPose const lp, afxM4d const offset, akxPoseBuffer wp);
AKX void                AfxComputePoseBufferLod(afxSkeleton skl, afxNat baseJnt, afxNat jntCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, akxPose const lp, afxM4d const offset, akxPoseBuffer wp);
AKX void                AfxComputePoseBufferNoCompositeLod(afxSkeleton skl, afxNat baseJnt, afxNat jntCnt, afxNat firstValidLocalArt, afxNat validLocalArtCnt, akxPose const lp, afxM4d const offset, akxPoseBuffer wp);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleSkeletons(afxSimulation sim, afxNat cnt, akxSkeletonBlueprint const sklb[], afxSkeleton skeletons[]);

AKX void                AfxTransformSkeletons(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxNat cnt, afxSkeleton skeletons[]);

#endif//AKX_SKELETON_H
