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


#include "qwadro/io/afxResource.h"
#include "qwadro/core/afxSystem.h"
#include <sys/stat.h>

_AFXINL afxError AfxUnloadResource(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    
    if (res->unload)
        if (res->unload(res))
            AfxThrowError();

    return err;
}

_AFXINL afxError AfxLoadResource(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);

    if (res->load)
        if (res->load(res))
            AfxThrowError();

    return err;
}

_AFXINL afxError AfxRequestResource(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    res->lastReqTime = AfxGetTimer();

    if (res->state == AFX_RES_STATE_INITIAL)
        res->state = AFX_RES_STATE_PENDING;

    return err;
}

_AFXINL afxBool AfxResourceIsReady(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    return (res->state == AFX_RES_STATE_READY);
}

_AFXINL afxNat AfxExcerptResourceName(afxResource res, afxUri *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    AfxAssert(name);
    return AfxGetUriName(name, res->uri);
}

_AFXINL afxUri const* AfxGetResourceUri(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    return res->uri;
}

_AFX afxBool _AfxResEventHandler(afxHandle *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxResource res = (void*)obj;
    AfxAssertObject(res, afxFcc_RES);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxResEventFilter(afxHandle *obj, afxHandle *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxResource res = (void*)obj;
    AfxAssertObject(res, afxFcc_RES);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxResDtor(afxResource res)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("res=%p", res);
    AfxAssertObject(res, afxFcc_RES);

    if (res->monitor.chain)
        AfxPopLinkage(&res->monitor);

    return err;
}

_AFX afxError _AfxResCtor(void *cache, afxNat idx, afxResource res, afxResourceParadigm const *paradigms)
{
    AfxEntry("res=%p", res);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(res, afxFcc_RES);
    afxResourceParadigm const *paradigm = &paradigms[idx];
    
    if (paradigm->spec)
    {
        res->uri = AfxCloneUri(paradigm->spec->uri);
        res->load = paradigm->spec->load;
        res->unload = paradigm->spec->unload;
    }
    else
    {
        res->uri = NIL;
        res->load = NIL;
        res->unload = NIL;
    }

        
    res->state = AFX_RES_STATE_INITIAL;
    res->lastUpdTime = 0;
    res->lastReqTime = 0;

    AfxPushLinkage(&res->monitor, NIL);
    
    return err;
}

_AFXINL afxResource AfxAcquireResource(afxResourceSpecification const *spec)
{
    afxError err = AFX_ERR_NONE;
    afxResource res;

    afxResourceParadigm paradigm = 
    {
        spec
    };

    if (AfxClassAcquireObjects(AfxGetResourceClass(), NIL, 1, &paradigm, (afxHandle**)&res, AfxHint()))
        AfxThrowError();
    else
    {
        AfxEcho("Resource %p[%.4s]^%i acquired.", res, (afxChar const*)&(res->superset), res->obj.refCnt);
    }
    return res;
}

AFX afxClassConfig const _AfxResClassSpec;

afxClassConfig const _AfxResClassSpec =
{
    afxFcc_RES,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxResource)),
    NIL,
    (void*)_AfxResCtor,
    (void*)_AfxResDtor,
    .event = _AfxResEventHandler,
    .eventFilter = _AfxResEventFilter,
    "afxResource",
    NIL
};