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

#ifndef AFX_POOL_H
#define AFX_POOL_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxMmu.h"

AFX_DEFINE_STRUCT(afxPoolPage)
{
    afxNat          usedCnt;
    afxNat32        usage;
    afxByte*        data;
};

AFX_DEFINE_STRUCT(afxPool)
{
    _AFX_DBG_FCC;
    afxNat          unitSiz;
    afxNat          unitsPerPage;
    afxNat          totalUsedCnt;
    afxNat          pageCnt;
    afxPoolPage*    pages;
    afxMmu      mem;
};

AFX void        AfxTakePool(afxPool* pool, afxNat unitSiz, afxNat unitsPerPage);
AFX void        AfxReleasePool(afxPool* pool);

AFX afxByte*    AfxAllocatePoolUnit(afxPool* pool, afxSize* idx);
AFX afxError    AfxAllocatePoolUnits(afxPool* pool, afxNat cnt, afxByte* units[]);

AFX void        AfxDeallocatePoolUnit(afxPool* pool, afxByte* unit);
AFX void        AfxDeallocatePoolUnits(afxPool* pool, afxNat cnt, afxByte* units[]);

AFX afxBool     AfxGetPoolItem(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnit(afxPool const* pool, afxSize idx, void **ptr);
AFX afxBool     AfxGetPoolUnits(afxPool const* pool, afxNat cnt, afxSize const idx[], void *ptr[]);
AFX afxBool     AfxGetLinearPoolUnits(afxPool const* pool, afxNat first, afxNat cnt, void *ptr[]);

AFX afxBool     AfxFindPoolUnitIndex(afxPool* pool, afxByte* unit, afxNat* idx, afxNat* localIdx);
AFX afxError    AfxOccupyPoolUnit(afxPool* pool, afxSize idx, void *val);

AFX afxNat      AfxEnumeratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, void *items[]);
AFX afxNat      AfxEnumeratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, void *items[]);

AFX afxNat      AfxCuratePoolItems(afxPool const* pool, afxNat first, afxNat cnt, afxBool(*f)(void* item, void* udd), void *udd);
AFX afxNat      AfxCuratePoolUnits(afxPool const* pool, afxNat first, afxNat cnt, afxBool freeOnly, afxBool (*f)(void* item, void* udd), void *udd);

#endif//AFX_POOL_H