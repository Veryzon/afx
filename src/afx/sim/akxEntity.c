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

#define _AFX_SIM_C
#define _AFX_ENTITY_C
#include "qwadro/sim/afxSimulation.h"

_AKX afxError _AfxEntDtor(akxEntity ent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ent, afxFcc_ENT);

    return err;
}

_AKX afxError _AfxEntCtor(akxEntity ent, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ent, afxFcc_ENT);

    afxBody bod = *(afxBody*)cookie->udd[1];
    (void)bod;

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireEntities(afxSimulation sim, afxNat cnt, akxEntity ent[], afxBody bod[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    for (afxNat i = 0; i < cnt; i++)
        if (AfxAcquireObjects(AfxGetEntityClass(sim), 1, (afxObject*)&ent[i], (void const*[]) { NIL, &bod[i]}))
            AfxThrowError();

    return err;
}

_AKX afxClassConfig _AfxEntMgrCfg =
{
    .fcc = afxFcc_ENT,
    .name = "Entity",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(akxEntity)),
    .mmu = NIL,
    .ctor = (void*)_AfxEntCtor,
    .dtor = (void*)_AfxEntDtor
};