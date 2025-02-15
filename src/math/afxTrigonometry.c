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

#include "qwadro/inc/math/afxTrigonometry.h"

_AFXINL void AfxComputeTriangleNormal(afxV3d const a, afxV3d const b, afxV3d const c, afxV3d n)
{
    afxV3d t, t2;
    AfxV3dSub(t, b, a);
    AfxV3dSub(t2, c, a);
    AfxV3dCross(n, t, t2);
    AfxV3dNormalize(n, n);
}

// BaryCentric

_AFXINL void AfxBarycentricV2d(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // Adapted from DirectXMath. Works?

    afxV2d p10;
    AfxV2dSub(p10, b, a);

    afxV2d fv;
    AfxFillV2d(fv, f);

    afxV2d p20;
    AfxV2dSub(p20, c, a);

    afxV2d gv;
    AfxFillV2d(gv, g);

    afxV2d tmp;
    AfxV2dMad(tmp, p10, fv, a);
    AfxV2dMad(v, p20, gv, tmp);
}

_AFXINL void AfxBarycentricV3d(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV3d p10;
    AfxV3dSub(p10, b, a);

    afxV3d fv;
    AfxFillV3d(fv, f);

    afxV3d p20;
    AfxV3dSub(p20, c, a);

    afxV3d gv;
    AfxFillV3d(gv, g);

    afxV3d tmp;
    AfxV3dMad(tmp, p10, fv, a);
    AfxV3dMad(v, p20, gv, tmp);
}

_AFXINL void AfxBarycentricV4d(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxReal f, afxReal g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath
    // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec4barycentric

    afxV4d p10;
    AfxV4dSub(p10, b, a);

    afxV4d fv;
    AfxFillV4d(fv, f);

    afxV4d p20;
    AfxV4dSub(p20, c, a);

    afxV4d gv;
    AfxFillV4d(gv, g);

    afxV4d tmp;
    AfxV4dMad(tmp, p10, fv, a);
    AfxV4dMad(v, p20, gv, tmp);
}

// BaryCentric2

_AFXINL void AfxBarycentricV2d2(afxV2d v, afxV2d const a, afxV2d const b, afxV2d const c, afxV2d const f, afxV2d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV2d p10;
    AfxV2dSub(p10, b, a);

    afxV2d p20;
    AfxV2dSub(p20, c, a);

    afxV2d tmp;
    AfxV2dMad(tmp, p10, f, a);
    AfxV2dMad(v, p20, g, tmp);
}

_AFXINL void AfxBarycentricV3d2(afxV3d v, afxV3d const a, afxV3d const b, afxV3d const c, afxV3d const f, afxV3d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV3d p10;
    AfxV3dSub(p10, b, a);

    afxV3d p20;
    AfxV3dSub(p20, c, a);

    afxV3d tmp;
    AfxV3dMad(tmp, p10, f, a);
    AfxV3dMad(v, p20, g, tmp);
}

_AFXINL void AfxBarycentricV4d2(afxV4d v, afxV4d const a, afxV4d const b, afxV4d const c, afxV4d const f, afxV4d const g)
{
    // Result = Position0 + f * (Position1 - Position0) + g * (Position2 - Position0)
    // From DirectXMath

    afxV4d p10;
    AfxV4dSub(p10, b, a);

    afxV4d p20;
    AfxV4dSub(p20, c, a);

    afxV4d tmp;
    AfxV4dMad(tmp, p10, f, a);
    AfxV4dMad(v, p20, g, tmp);
}
