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

#ifndef AFX_STACK_H
#define AFX_STACK_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxMmu.h"

// A Stack is a linear data structure in which elements can be inserted and deleted only from one side of the list, called the top. 
// A stack follows the LIFO (Last In First Out) principle, i.e., the element inserted at the last is the first element to come out. 
// The insertion of an element into the stack is called push operation, and the deletion of an element from the stack is called pop operation. 
// In stack, we always keep track of the last element present in the list with a pointer called top.

AFX_DEFINE_STRUCT(afxStack)
{
    _AFX_DBG_FCC;
    afxNat              unitSiz;
    afxNat              unitsPerBlock;
    afxNat              totalUsedUnitCnt;
    afxAllocationBlock  *lastBlock;
    afxNat              maxUnits;
    afxNat              activeBlocks;
    afxNat              maxActiveBlocks;
    afxAllocationBlock  **blockDir;

    afxMmu           ctx;
};

AFX void        AfxAllocateStack(afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock);
AFX void        AfxAllocatePagedStack(afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock, afxNat maxUnits);
AFX void        AfxDeallocateStack(afxStack *stck);

AFX void        AfxEmptyStack(afxStack *stck);

AFX void*       AfxPushStackUnit(afxStack *stck, afxNat *idx);
AFX char        AfxPushStackUnits(afxStack *stck, afxNat cnt, afxNat *firstIdx, void const *initialVal);
AFX void        AfxPopStackUnits(afxStack *stck, afxNat cnt);
AFX void*       AfxGetStackUnit(afxStack *stck, afxNat idx);

AFX void        AfxDumpStackElement(afxStack *stck, afxNat idx, void *dst);
AFX void        AfxDumpStackElements(afxStack *stck, afxNat first, afxNat cnt, void *dst);

AFX void        AfxUpdateStackElement(afxStack *stck, afxNat idx, void const* src);
AFX void        AfxUpdateStackElements(afxStack *stck, afxNat first, afxNat cnt, void const* src);

AFX void        AfxSerializeStack(afxStack *stck, void *dst);

#endif//AFX_STACK_H