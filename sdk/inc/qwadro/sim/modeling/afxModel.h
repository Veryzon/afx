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

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "qwadro/sim/afxSkeleton.h"

/// O objeto afxModel descreve uma cole��o de afxMesh'es que s�o todas ligadas ao mesmo afxSkeleton.
/// Isso � essencialmente qualquer grupo conectado de malhas que s�o animadas em conjuntura.
/// Um ator seria um modelo, assim seria um ve�culo, ou mesmo uma edifica��o inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo � feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

#ifdef _AFX_MODEL_C
AFX_DEFINE_STRUCT(afxMeshRig)
{
    afxMesh             msh;
    afxMaterial         txd;
    afxNat*             jointMap;
    afxNat*             jointMapOrig;
    afxSkeleton         sklOrig;
};

AFX_OBJECT(afxModel)
{
    afxSkeleton         skl;
    afxNat              rigCnt;
    afxMeshRig*         rigs;
    afxAabb             aabb;
    afxTransform        init;
    afxString           id; // 32
    afxStringCatalog    strc;
};
#endif

AFX_DEFINE_STRUCT(afxModelBlueprint)
/// Data needed for model assembly
{
    afxString32         id;
    afxSkeleton         skl;
    afxTransform        init;
    afxNat              mshCnt;
    afxMesh*            meshes;
    afxStringCatalog    strc;
};

AKX afxBool             AfxGetModelId(afxModel mdl, afxString* id);

AKX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);

AKX void                AfxComputeModelBaseOffset(afxModel mdl, afxM4d m);
AKX void                AfxUpdateModelBaseOffset(afxModel mdl, afxTransform const* xform);

AKX afxNat              AfxCountModelRigs(afxModel mdl);
AKX afxNat              AfxCountRiggedMeshes(afxModel mdl);
AKX afxNat              AfxEnumerateRiggedMeshes(afxModel mdl, afxNat baseSlot, afxNat slotCnt, afxMesh meshes[]);
AKX afxError            AfxRigMeshes(afxModel mdl, afxSkeleton origSkl, afxNat baseSlot, afxNat slotCnt, afxMesh const meshes[]);

AKX afxBool             AfxRiggedMeshIsTransplanted(afxModel mdl, afxNat slotIdx);
AKX afxSkeleton         AfxGetRiggedMeshOriginalSkeleton(afxModel mdl, afxNat slotIdx);
AKX afxNat const*       AfxGetRiggedMeshOriginalMapping(afxModel mdl, afxNat slotIdx);

AKX afxMaterial         AfxGetRiggedMeshTxd(afxModel mdl, afxNat slotIdx);
AKX void                AfxSetRiggedMeshTxd(afxModel mdl, afxNat slotIdx, afxMaterial mtl);
AKX afxMaterial         AfxFindRiggedMeshMaterial(afxModel mdl, afxNat slotIdx, afxString const* id);

AKX afxNat const*       AfxGetRiggedMeshMapping(afxModel mdl, afxNat slotIdx);

AKX void                AfxComputeRiggedMeshMatrices(afxModel mdl, afxNat slotIdx, awxPoseBuffer const* posb, afxNat baseJoint, afxNat jointCnt, afxM4d m[]);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleModel(afxSimulation sim, afxNat cnt, afxModelBlueprint const blueprints[], afxModel models[]);

AKX void                AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxNat cnt, afxModel models[]);

#endif//AFX_MODEL_H
