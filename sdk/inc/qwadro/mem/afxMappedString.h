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

#ifndef AFX_INDEXED_STRING_H
#define AFX_INDEXED_STRING_H

#include "qwadro/core/afxString.h"
#include "qwadro/mem/afxArray.h"

AFX_DEFINE_STRUCT(afxMappedString)
{
    afxString           str; // stores just a offset into string arena
    afxStringBase    arena;
};

AFX_OBJECT(afxStringBase)
{
    afxNat              cnt;
    afxString*          strings;
    afxArray            buf;
};

AFX afxError    AfxCatalogStrings(afxStringBase strc, afxNat cnt, afxString const sources[], afxNat strIdx[], afxNat strIdxStride);

AFX afxNat      AfxCatalogStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[]);

AFX afxError    AfxResolveStrings(afxStringBase strc, afxNat cnt, afxNat strIdx[], afxString dst[]);

AFX afxNat      AfxResolveStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[]);

AFX afxError    AfxAcquireStringCatalogs(afxNat cnt, afxStringBase catalogs[]);

#endif//AFX_INDEXED_STRING_H