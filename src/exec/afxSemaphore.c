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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SEMAPHORE_C
#include "../dev/afxExecImplKit.h"

_AFX void* AfxGetSemaphoreContext(afxSemaphore sem)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);
    void* ctx = AfxGetProvider(sem);
    //AfxAssertObjects(1, &ctx, afxFcc_CTX);
    return ctx;
}

_AFX afxError _AfxSemStdDtor(afxSemaphore sem)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);

    sem->value = NIL;

    return err;
}

_AFX afxError _AfxSemStdCtor(afxSemaphore sem, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);

    sem->value = NIL;

    return err;
}

_AFX afxClassConfig const _AfxSemStdImplementation =
{
    .fcc = afxFcc_SEM,
    .name = "Semaphore",
    .desc = "Device Synchronization Semaphore",
    .fixedSiz = sizeof(AFX_OBJECT(afxSemaphore)),
    .mmu = NIL,
    .ctor = (void*)_AfxSemStdCtor,
    .dtor = (void*)_AfxSemStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireSemaphores(void* ctx, afxUnit cnt, afxSemaphore semaphores[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(semaphores);

    afxClass* cls = AfxGetSemaphoreClass(ctx);
    AfxAssertClass(cls, afxFcc_SEM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)semaphores, (void const*[]) { ctx }))
        AfxThrowError();

    return err;
}
