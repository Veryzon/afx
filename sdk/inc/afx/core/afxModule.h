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

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "afx/core/afxObject.h"

/// Um afxModule � uma API do QWADRO para Applicable Service Interface (.asi) e Installable Client Driver (.icd), 
/// estes que s�o os dois formatos execut�veis e vincul�veis que expandem o sistema de execu��o QWADRO.

AFX_OBJECT(afxModule)
{
    afxObject           obj;
#ifdef _AFX_MODULE_C
    afxUri*             path; // 128
    void                *osHandle;
    afxBool             hasBeenLoaded;
#endif
};

AFX afxError            AfxAcquireModules(afxNat cnt, afxUri const uri[], afxModule mdle[]);
AFX void                AfxReleaseModules(afxNat cnt, afxModule mdle[]);

AFX afxUri const*       AfxGetModulePath(afxModule mdle);

AFX void*               AfxFindModuleSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[]);

AFX void                AfxGetModuleVendor(afxModule mdle, afxString const* name);
AFX void                AfxGetModuleVersion(afxModule mdle, afxNat *major, afxNat *minor, afxNat *patch);

#endif//AFX_MODULE_H