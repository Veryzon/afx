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

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "awxPose.h"
#include "awxWorldPose.h"
#include "qwadro/io/afxUri.h"
#include "awxSimDefs.h"
#include "qwadro/math/afxMatrix.h"

/// O objeto awxSkeleton � uma cole��o hier�rquica de articula��es que descrevem a estrutura articular interna de um awxModel, e auxilia no manejo e na anima��o do mesmo. 

/// Cada articula��o no arranjo do awxSkeleton aloja o transforme para aquela articula��o em dois meios diferentes. 
/// Primeiro, afxTransform "local" � o transforme decomposto da articula��o relativa a seu parente imediato (dado pelo �ndice do parente).
/// Se esta n�o houver parente (isto �, o �ndex do parente � igual a AFX_INVALID_INDEX), ent�o esta � relativa a origem do awxSkeleton.

/// Segundo, "iw" � uma afxM4d que � o transforme invertido no world-space para a articula��o na postura padr�o do awxSkeleton (isto �, a postura na qual o awxSkeleton foi originalmente modelado).

/// O afxTransform "local" � usado primariamente na composi��o de anima��es, e a afxM4d "iw" � usada primariamente para deforma��o de malha, raz�o pela qual esta informa��o � alojada em dois diferentes formatos.

/// Cada articula��o tamb�m opcionalmente cont�m dados definidos pelo usu�rio "UDD".
/// Este campo aponta para algum dado, estranho � l�gica do Qwadro, associado com a articula��o, se houver qualquer.

/// Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
/// Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
/// The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.

AFX_DEFINE_STRUCT(awxSkeletonBone)
{
    afxBufferedString   id; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

#ifdef _AFX_SKELETON_C
AFX_OBJECT(awxSkeleton)
{
    afxNat              boneCnt;
    awxSkeletonBone*    bones; // awxNode
    afxNat              lodType;
    afxBufferedString   id; // 128
};
#endif//_AFX_SKELETON_C

AFX_DEFINE_STRUCT(awxSkeletonBuilder)
{
    _AFX_DBG_FCC;
    afxNat              boneCnt;
    awxSkeletonBone*    bones;
    afxNat              lodType;
    afxFixedString32         id;
    void                (*IndexRemapping)(void* data, afxNat boneIdx, afxNat outIdx);
};

AFX afxString const*    AfxGetSkeletonId(awxSkeleton skl);

AFX afxNat              AfxCountBones(awxSkeleton skl);
AFX afxNat              AfxCountBonesForLod(awxSkeleton skl, afxReal allowedErr);
AFX awxSkeletonBone*    AfxGetBone(awxSkeleton skl, afxNat boneIdx);
AFX afxBool             AfxFindBone(awxSkeleton skl, afxString const* id, afxNat *boneIdx);

AFX void                AfxComputeSkeletonRestLocalPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, awxPose *lp);
AFX void                AfxComputeSkeletonRestWorldPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, afxReal const offset[4][4], awxWorldPose *wp);

AFX void                AfxComputeSkeletonPose(awxSkeleton skl, afxNat firstArt, afxNat artCnt, awxPose const *lp, afxReal const offset[4][4], awxWorldPose *wp);

AFX void                AfxLocalPoseFromWorldPoseNoScale(awxSkeleton skl, awxPose* lp, awxWorldPose const* wp, afxReal const offset[4][4], afxNat firstBone, afxNat boneCnt);
AFX void                AfxGetWorldMatrixFromLocalPose(awxSkeleton skl, afxNat artIdx, awxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse);
AFX void                AfxGetAttachmentOffset(awxSkeleton skl, afxNat artIdx, awxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse);

AFX void AfxBuildIndexedCompositeBuffer(awxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][4][4]);
AFX void AfxBuildIndexedCompositeBufferTransposed(awxSkeleton skl, awxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxBuildSkeletons(afxSimulation sim, afxNat cnt, awxSkeletonBuilder const sklb[], awxSkeleton skl[]);

AFX void                AfxTransformSkeletons(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxNat cnt, awxSkeleton skl[]);

////////////////////////////////////////////////////////////////////////////////
// BUILDING OPERATIONS                                                        //
////////////////////////////////////////////////////////////////////////////////

AFXINL afxError         AfxBeginSkeletonBuilding(awxSkeletonBuilder* sklb, afxNat boneCnt, afxString const* id, afxNat lodType);
AFXINL void             AfxEndSkeletonBuilding(awxSkeletonBuilder* sklb);

AFXINL void             AfxResetBoneWithIw(awxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxReal const iw[4][4]);
AFXINL void             AfxResetBone(awxSkeletonBuilder const* sklb, afxNat boneIdx, afxString const* id, afxNat parentIdx, afxTransform const* lt, afxTransform const* wt);

#endif//AFX_SKELETON_H
