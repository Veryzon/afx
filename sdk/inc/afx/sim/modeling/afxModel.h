/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "afxSkeleton.h"

/// O objeto afxModel descreve uma cole��o de afxMesh'es que s�o todas ligadas ao mesmo afxSkeleton.
/// Isso � essencialmente qualquer grupo conectado de malhas que s�o animadas em conjuntura.
/// Um ator seria um modelo, assim seria um ve�culo, ou mesmo uma edifica��o inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo � feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

AFX_DEFINE_HANDLE(afxModel);

AFX_DEFINE_STRUCT(afxMeshSlot)
{
    afxMesh             msh;
};

AFX_OBJECT(afxModel)
{
    afxObject           obj;
    afxUri*             uri; // 128
    afxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              meshSlotCnt;
    afxMeshSlot         *meshSlots;
};

AFX afxUri const*       AfxModelGetUri(afxModel mdl);
AFX afxSkeleton         AfxModelGetSkeleton(afxModel mdl);

AFX void                AfxModelGetInitialPlacement(afxModel mdl, afxM4d m);
AFX void                AfxModelTransform(afxModel mdl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags);

AFX afxMesh             AfxModelGetBoundMesh(afxModel mdl, afxNat mshIdx);
AFX afxNat              AfxModelGetMeshBindingCount(afxModel mdl);

#endif//AFX_MODEL_H