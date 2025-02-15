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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/inc/math/bound/afxFrustum.h"
#include "qwadro/inc/math/afxMatrix.h"
#include "qwadro/inc/math/bound/afxPlane.h"

_AFX afxV4d const AFX_V4D_LBN = { -1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RBN = { 1, -1, -1, 1 };
_AFX afxV4d const AFX_V4D_RTN = { 1, 1, -1, 1 };
_AFX afxV4d const AFX_V4D_LTN = { -1, 1, -1, 1 };

_AFX afxV4d const AFX_V4D_LBF = { -1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RBF = { 1, -1, 1, 1 };
_AFX afxV4d const AFX_V4D_RTF = { 1, 1, 1, 1 };
_AFX afxV4d const AFX_V4D_LTF = { -1, 1, 1, 1 };

_AFXINL void AfxFrustumCopy(afxFrustum* f, afxFrustum const* in)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(in);
    afxUnit i;

    AfxV4dCopy(f->origin, in->origin);

    for (i = 0; i < afxCubeFace_TOTAL; i++)
        AfxPlaneCopy(&f->planes[i], &in->planes[i]);

    for (i = 0; i < afxCubeCorner_TOTAL; i++)
        AfxV4dCopy(f->corners[i], in->corners[i]);
}

_AFXINL afxBool AfxFrustumDoesCullSphere(afxFrustum const* f, afxSphere const* sph)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(sph);

    for (afxUnit i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        if (!AfxPlaneTestSpheres(&f->planes[i], 1, sph))
            return FALSE;
    }
    return TRUE;
}

_AFXINL afxResult AfxAabbTestPlanes(afxBox const* aabb, afxUnit cnt, afxPlane const planes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(aabb, planes);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        afxPlane const* p = &planes[i];

        if (p->normal[0] > 0.0f)
        {
            minD = p->normal[0] * aabb->inf[0];
            maxD = p->normal[0] * aabb->sup[0];
        }
        else
        {
            minD = p->normal[0] * aabb->sup[0];
            maxD = p->normal[0] * aabb->inf[0];
        }

        if (p->normal[1] > 0.0f)
        {
            minD += p->normal[1] * aabb->inf[1];
            maxD += p->normal[1] * aabb->sup[1];
        }
        else
        {
            minD += p->normal[1] * aabb->sup[1];;
            maxD += p->normal[1] * aabb->inf[1];
        }

        if (planes[i].normal[2] > 0.0f)
        {
            minD += p->normal[2] * aabb->inf[2];
            maxD += p->normal[2] * aabb->sup[2];
        }
        else
        {
            minD += p->normal[2] * aabb->sup[2];
            maxD += p->normal[2] * aabb->inf[2];
        }

        // What side of the plane

        if (minD >= -p->offset)
            return +1;

        if (maxD <= -p->offset)
            return -1;
    }
    //Intersection
    return 0;
}

_AFXINL afxBool AfxFrustumTestAabbs(afxFrustum const* f, afxUnit cnt, afxBox const aabb[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(aabb);

    for (afxUnit i = 0; i < cnt; ++i)
    {
        if (AfxAabbTestPlanes(&aabb[i], afxCubeFace_TOTAL, f->planes) == -1)
            return FALSE;
    }
    return TRUE;
#if 0
    afxV4d v;
    afxBool volInside = TRUE;

    // check box outside/inside of frustum

    for (afxUnit i = 0; i < afxCubeFace_TOTAL; i++)
    {
        v[0] = f->planes[i][0];
        v[1] = f->planes[i][1];
        v[2] = f->planes[i][2];
        v[3] = f->planes[i][3];

        afxUnit out = 0;
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_INF][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_INF][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_INF][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        out += ((AfxV4dDot(v, AFX_V4D(aabb[AFX_AABB_SUP][0], aabb[AFX_AABB_SUP][1], aabb[AFX_AABB_SUP][2], 1.0f)) < 0.0) ? 1 : 0);
        
        if (out == 8)
        {
            volInside = FALSE;
            break;
        }
    }

    if (volInside)
    {
        // check frustum outside/inside box
        
        for (afxUnit j = 0; j < 3; j++)
        {
            afxUnit out;

            for (afxUnit i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] > aabb[AFX_AABB_SUP][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }

            for (afxUnit i = 0; i < afxCubeCorner_TOTAL; i++)
                out += ((f->corners[i][j] < aabb[AFX_AABB_INF][j]) ? 1 : 0);

            if (out == 8)
            {
                volInside = FALSE;
                break;
            }
        }
    }
    return !volInside;
#endif
}

_AFXINL afxBool AfxFrustumDoesCullFrustum(afxFrustum const* f, afxFrustum const* other)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(other);

    afxBool allOut = TRUE;

    for (afxUnit i = 0; i < afxCubeFace_TOTAL; ++i)
    {
        for (afxUnit j = 0; j < afxCubeCorner_TOTAL; ++j)
        {
            if (AfxPlaneFindV3d(&f->planes[i], other->corners[j]) < 0)
            {
                allOut = FALSE;
                break;
            }
        }

        if (!allOut)
            break;
    }
    return allOut;
}

_AFXINL void AfxAabbAbsorbFrustum(afxBox* aabb, afxFrustum const* f)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(aabb);
    AfxAabbAbsorbAtv4d(aabb, afxCubeCorner_TOTAL, f->corners);
}

_AFXINL void AfxFrustumRecomputePlanes(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(pv);
    afxV3d x, y, z, w;
    AfxV3dSet(x, pv[0][0], pv[1][0], pv[2][0]);
    AfxV3dSet(y, pv[0][1], pv[1][1], pv[2][1]);
    AfxV3dSet(z, pv[0][2], pv[1][2], pv[2][2]);
    AfxV3dSet(w, pv[0][3], pv[1][3], pv[2][3]);

    afxV3d wMx, wPx, wMy, wPy, wMz, wPz;
    AfxV3dSub(wMx, w, x);
    AfxV3dAdd(wPx, w, x);
    AfxV3dSub(wMy, w, y);
    AfxV3dAdd(wPy, w, y);
    AfxV3dSub(wMz, w, z);
    AfxV3dAdd(wPz, w, z);

    afxReal ww = pv[3][3];

    AfxPlaneReset(&f->planes[afxCubeFace_R], wMx, ww - pv[3][0]); // Right
    AfxPlaneReset(&f->planes[afxCubeFace_L], wPx, ww + pv[3][0]); // Left

    AfxPlaneReset(&f->planes[afxCubeFace_B], wPy, ww + pv[3][1]); // Bottom
    AfxPlaneReset(&f->planes[afxCubeFace_T], wMy, ww - pv[3][1]); // Top 

    AfxPlaneReset(&f->planes[afxCubeFace_F], wMz, ww - pv[3][2]); // Near (LH Coords)
    AfxPlaneReset(&f->planes[afxCubeFace_B], wPz, ww + pv[3][2]); // Far
}

_AFXINL void AfxFrustumRecomputeCorners(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(pv);

    afxV4d v;
    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_LBN);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_LBN], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_RBN);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_RBN], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_RTN);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_RTN], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_LTN);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_LTN], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_LBF);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_LBF], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_RBF);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_RBF], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_RTF);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_RTF], v);

    AfxV4dPostMultiplyM4d(v, pv, AFX_V4D_LTF);
    v[0] /= v[3];
    v[1] /= v[3];
    v[2] /= v[3];
    AfxV4dCopy(f->corners[afxCubeCorner_LTF], v);
}

_AFXINL void AfxRecomputeFrustum3(afxFrustum* f, afxM4d const pv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(pv);
    AfxFrustumRecomputePlanes(f, pv);
    AfxFrustumRecomputeCorners(f, pv);
}

_AFXINL void AfxRecomputeFrustum(afxFrustum* f, afxM4d const v, afxM4d const p)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);
    AFX_ASSERT(v);
    AFX_ASSERT(p);

    afxM4d pv;
    AfxM4dMultiplyTransposed(pv, v, p);
    AfxV4dPostMultiplyM4d(f->origin, pv, AFX_V4D_ZERO);
    AfxRecomputeFrustum3(f, pv);
}
