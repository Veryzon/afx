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

#ifndef AFX_CANVAS_BLUEPRINT_H
#define AFX_CANVAS_BLUEPRINT_H

#include "afx/draw/pipelining/afxCanvas.h"

AFX_DEFINE_STRUCT(afxCanvasBlueprint)
{
    afxDrawContext      dctx;
    afxWhd              extent;
    afxNat              rasterCnt;
    struct
    {
        afxSurface      surf; // one for each rasCnt. pass NIL to it be created using rasFmt and rasUsage. If exists must has been created with AFX_TEX_FLAG_SURFACE_RASTER flag.
        afxPixelFormat  fmt; // one for each rasCnt
        afxTextureFlags usage; // one for each rasCnt
    }                   rasters[8];
    struct
    {
        afxSurface      surf; // actually unsupported
        afxPixelFormat  fmt;
        afxTextureFlags usage;
    }                   ds[2];
};

//afxDrawContext dctx, afxNat cnt,

AFXINL void         AfxBeginCanvasBlueprint(afxCanvasBlueprint *blueprint, afxDrawContext dctx, afxWhd const extent);

AFXINL void         AfxCanvasBlueprintSetExtent(afxCanvasBlueprint *blueprint, afxWhd const extent);
AFXINL void         AfxCanvasBlueprintSetDepth(afxCanvasBlueprint *blueprint, afxPixelFormat depth, afxPixelFormat stencil);

AFXINL void         AfxCanvasBlueprintAddRaster(afxCanvasBlueprint *blueprint, afxPixelFormat fmt, afxTextureFlags usage);
AFXINL void         AfxCanvasBlueprintAddExistingRaster(afxCanvasBlueprint *blueprint, afxSurface existing);

AFX afxError        AfxBuildCanvases(afxCanvasBlueprint const *blueprint, afxNat cnt, afxCanvas canv[]);

#endif//AFX_CANVAS_BLUEPRINT_H