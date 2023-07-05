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

#ifndef AFX_MODEL_BLUEPRINT_H
#define AFX_MODEL_BLUEPRINT_H

#include "afxModel.h"

/// Um blueprint foi idealizado para ser um meta-objeto contendo a informa��o necess�ria para construir um modelo male�vel.

typedef enum afxModelBlueprintFlag
{
    AFX_MDLP_FLAG_DONT_SHARE_VBUF = AFX_FLAG(0),
    AFX_MDLP_FLAG_DONT_SHARE_IBUF = AFX_FLAG(1)
} afxModelBlueprintFlag;

AFX_DEFINE_STRUCT(afxModelBlueprint)
{
    afxFcc          fcc;
    void*           sim;
    afxUri128       uri;
    afxSkeleton     skl;
    afxTransform    init;
    afxArray        meshes;
};

// Inicializa uma blueprint, e reserva recursos.
AFX void                    AfxModelBlueprintDeploy(afxModelBlueprint *mdlb, void* sim, afxUri const *uri, afxSkeleton skl, afxTransform *initialPlacement, afxNat estMeshCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX void                    AfxModelBlueprintDiscard(afxModelBlueprint *mdlb);

AFX void                    AfxModelBlueprintErase(afxModelBlueprint *mdlb);

// Obt�m o blueprint do afxSkeleton.
AFX void                    AfxModelBlueprintSetSkeleton(afxModelBlueprint *mdlb, afxSkeleton skl);

AFX afxError                AfxModelBlueprintAddMeshes(afxModelBlueprint *mdlb, afxNat cnt, afxMesh msh[]);

#endif//AFX_MODEL_BLUEPRINT_H