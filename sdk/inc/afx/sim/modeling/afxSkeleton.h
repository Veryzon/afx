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

#ifndef AFX_SKELETON_H
#define AFX_SKELETON_H

#include "../afxPose.h"
#include "../afxWorldPose.h"
#include "afx/core/io/afxUri.h"

/// O objeto afxSkeleton � uma cole��o hier�rquica de articula��es que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na anima��o do mesmo. 

/// Cada articula��o no arranjo do afxSkeleton aloja o transforme para aquela articula��o em dois meios diferentes. 
/// Primeiro, afxTransform "local" � o transforme decomposto da articula��o relativa a seu parente imediato (dado pelo �ndice do parente).
/// Se esta n�o houver parente (isto �, o �ndex do parente � igual a AFX_INVALID_INDEX), ent�o esta � relativa a origem do afxSkeleton.

/// Segundo, "iw" � uma afxM4d que � o transforme invertido no world-space para a articula��o na postura padr�o do afxSkeleton (isto �, a postura na qual o afxSkeleton foi originalmente modelado).

/// O afxTransform "local" � usado primariamente na composi��o de anima��es, e a afxM4d "iw" � usada primariamente para deforma��o de malha, raz�o pela qual esta informa��o � alojada em dois diferentes formatos.

/// Cada articula��o tamb�m opcionalmente cont�m dados definidos pelo usu�rio "UDD".
/// Este campo aponta para algum dado, estranho � l�gica do Qwadro, associado com a articula��o, se houver qualquer.

AFX_DEFINE_HANDLE(afxSkeleton);

AFX_DEFINE_STRUCT(afxSkeletonBone)
{
    afxString*          name; // 32
    afxNat              parentIdx;
    afxTransform        local;
    afxM4d              iw;
    afxReal             lodError;
    void*               extendedData;
};

AFX_OBJECT(afxSkeleton)
{
    afxInstance           obj;
    afxUri*             uri; // 128
    afxNat              boneCnt;
    afxSkeletonBone     *bones; // afxNode
    afxNat              lodType;
};

AFX void*               AfxSkeletonGetSimulation(afxSkeleton skl);

AFX afxUri const*       AfxSkeletonGetUri(afxSkeleton skl);
AFX afxSkeleton         AfxSkeletonClone(afxSkeleton skl);

AFX afxNat              AfxSkeletonGetBoneCount(afxSkeleton skl);
AFX afxSkeletonBone*    AfxSkeletonGetBone(afxSkeleton skl, afxNat boneIdx);
AFX afxBool             AfxSkeletonFindBone(afxSkeleton skl, afxString const *name, afxNat *boneIdx);

AFX void                AfxSkeletonTransform(afxSkeleton skl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear);

AFX void                AfxSkeletonComputeRestPosture(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose *lp);
AFX void                AfxSkeletonComputeRestPose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxM4d const offset, afxWorldPose *wp);

AFX void                AfxSkeletonComputePose(afxSkeleton skl, afxNat firstArt, afxNat artCnt, afxPose const *lp, afxM4d const offset, afxWorldPose *wp);

#endif//AFX_SKELETON_H