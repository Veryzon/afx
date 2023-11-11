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

#include "afxPose.h"
#include "afxWorldPose.h"
#include "afx/core/afxUri.h"
#include "afxSimDefs.h"

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

AFX_DEFINE_HANDLE(afxSkeleton);

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxString           id; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

#ifdef _AFX_SKELETON_C
AFX_OBJECT(afxSkeleton)
{
    afxUri              id; // 128
    afxNat              boneCnt;
    afxSkeletonBone*    bones; // afxNode
    afxNat              lodType;
};
#endif

AFX_DEFINE_STRUCT(afxSkeletonBuilder)
{
    void                (*GetInfo)(void* data, afxNat* boneCnt, afxNat* lodType);
    afxNat              (*GetParent)(void* data, afxNat boneIdx);
    void                (*GetLocalTransform)(void* data, afxNat boneIdx, afxTransform* local);
    void                (*GetIw)(void* data, afxNat boneIdx, afxReal iw[4][4]);
    afxReal             (*GetLodError)(void* data, afxNat boneIdx);
    void                (*GetTag)(void* data, afxNat boneIdx, afxString* dst);
    void                (*GetExtendedData)(void* data, afxNat boneIdx, void** udd);
    void                (*IndexRemapping)(void* data, afxNat boneIdx, afxNat outIdx);
};

AFX afxUri const*       AfxGetSkeletonId(afxSkeleton skl);

AFX afxNat              AfxCountBones(afxSkeleton skl);
AFX afxNat              AfxCountBonesForLod(afxSkeleton skl, afxReal allowedErr);
AFX afxSkeletonBone*    AfxGetBone(afxSkeleton skl, afxNat boneIdx);
AFX afxBool             AfxFindBone(afxSkeleton skl, afxString const* id, afxNat *boneIdx);

AFX void                AfxComputeSkeletonRestLocalPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose *lp);
AFX void                AfxComputeSkeletonRestWorldPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxReal const offset[4][4], afxWorldPose *wp);

AFX void                AfxComputeSkeletonPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose const *lp, afxReal const offset[4][4], afxWorldPose *wp);

AFX void                AfxLocalPoseFromWorldPoseNoScale(afxSkeleton skl, afxPose* lp, afxWorldPose const* wp, afxReal const offset[4][4], afxNat firstBone, afxNat boneCnt);
AFX void                AfxGetWorldMatrixFromLocalPose(afxSkeleton skl, afxNat artIdx, afxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseBoneArray, afxNat const* sparseBoneArrayReverse);
AFX void                AfxGetAttachmentOffset(afxSkeleton skl, afxNat artIdx, afxPose const* lp, afxReal const offset[4][4], afxReal m[4][4], afxNat const* sparseArtArray, afxNat const* sparseArtArrayReverse);

AFX void AfxBuildIndexedCompositeBuffer(afxSkeleton skl, afxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][4][4]);
AFX void AfxBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxWorldPose const* wp, afxNat const* indices, afxNat idxCnt, afxReal buffer[][3][4]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxBuildSkeletons(afxSimulation sim, afxSkeletonBuilder const* sklb, afxNat cnt, void* data[], afxSkeleton skl[]);

AFX void                AfxTransformSkeletons(afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal const affine[3], afxReal affineTol, afxReal linearTol, afxNat cnt, afxSkeleton skl[]);

#endif//AFX_SKELETON_H