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

/// Um afxModule � uma API do QWADRO para Applicable Service Interface (.asi) e Installable Client Driver (.icd), 
/// estes que s�o os dois formatos execut�veis e vincul�veis que expandem o ecossistema de execu��o QWADRO.

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUri.h"

typedef enum afxModuleType
{
    afxModuleType_DLL, // has DllMain
    afxModuleType_ASI, // has 
    afxModuleType_ICD, // has DriverEntry
    afxModuleType_SYS, // has
} afxModuleType;

AFX void                AfxGetModulePath(afxModule mdle, afxUri* uri);

AFX void*               AfxGetSymbolAddress(afxModule mdle, afxString const* name);
AFX afxNat              AfxGetSymbolAddresses(afxModule mdle, afxNat cnt, afxString const names[], void* addresses[]);
AFX afxNat              AfxGetSymbolAddresses2(afxModule mdle, afxBool demangle, afxNat cnt, afxString const names[], void* addresses[]);


AFX void*               AfxFindModuleSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[]);

AFX afxNat              AfxFindSymbolAddresses(afxModule mdle, afxNat cnt, afxString const names[], void* addresses[]);

AFX void                AfxGetModuleVendor(afxModule mdle, afxString const* name);
AFX void                AfxGetModuleVersion(afxModule mdle, afxNat *major, afxNat *minor, afxNat *patch);

AFX void*               AfxGetModuleIdd(afxModule mdle);

////////////////////////////////////////////////////////////////////////////////

AFX afxBool             AfxFindModule(afxUri const *uri, afxModule* mdle);

AFX afxError            AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* mdle);

#endif//AFX_MODULE_H