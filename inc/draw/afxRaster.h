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

  //////////////////////////////////////////////////////////////////////////////
 //// SIGMA GL/2                                                           ////
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_RASTER_H
#define AVX_RASTER_H

#include "qwadro/inc/draw/afxPixel.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/math/afxWhd.h"

typedef enum afxRasterFlag
{
#if !0
    afxRasterFlag_LINEAR_TILING = AFX_BIT(9), /// specifies linear tiling (texels are laid out in memory in row-major order, possibly with some padding on each row).
    //afxRasterFlag_OPTIMAL = AFX_BIT(10), /// specifies optimal tiling (texels are laid out in an implementation-dependent arrangement, for more efficient memory access).
    //afxRasterFlag         = afxRasterFlag_LINEAR | afxRasterFlag_OPTIMAL,
#endif
    afxRasterFlag_CUBEMAP   = AFX_BIT(1),

    // autogenerated by acquisition op
    afxRasterFlag_1D        = AFX_BIT(2),
    afxRasterFlag_2D        = AFX_BIT(3),
    afxRasterFlag_3D        = AFX_BIT(4),
    afxRasterFlag_LAYERED   = AFX_BIT(5), // has more than 1 layer.
    afxRasterFlag_SUBSAMPLED= AFX_BIT(6), // has more than 1 LOD.
    afxRasterFlag_RESAMPLED = AFX_BIT(7), // has more than 1 LOD.
    afxRasterFlag_REVALIDATE= AFX_BIT(31)
} afxRasterFlags;

typedef enum afxRasterUsage
{
    afxRasterUsage_SRC      = AFX_BIT(0), /// The texture can be used as the source of a copy operation.
    afxRasterUsage_DST      = AFX_BIT(1), /// The texture can be used as the destination of a copy or write operation.
    afxRasterUsage_TRANSFER = (afxRasterUsage_SRC | afxRasterUsage_DST),
    afxRasterUsage_SAMPLING = AFX_BIT(2), /// The texture can be bound for use as a sampled texture in a shader.
    afxRasterUsage_STORAGE  = AFX_BIT(3), /// The texture can be bound for use as a storage texture in a shader.
    afxRasterUsage_DRAW     = AFX_BIT(4), /// The texture can be used as a color or depth/stencil attachment in a render pass.
    afxRasterUsage_VIDEO    = (afxRasterUsage_SAMPLING | afxRasterUsage_DRAW),

    afxRasterFlag_USAGE     = afxRasterUsage_TRANSFER | afxRasterUsage_SAMPLING | afxRasterUsage_STORAGE | afxRasterUsage_DRAW,
} afxRasterUsage;

AFX_DEFINE_STRUCT(afxRasterRegion)
{
    afxNat              lodIdx;
    afxWhd              origin; /// is the initial x, y, z offsets in texels of the sub-region of the source or destination afxRaster data.
    afxWhd              whd; /// is the size in texels of the afxRaster to copy in width, height and depth.
};

AFX_DEFINE_STRUCT(afxRasterIo)
/// Especifica��o de opera��o de transfer�ncia arbitr�ria de afxRaster.
{
    afxRasterRegion     rgn;
    afxSize             offset; /// is the offset in bytes from the start of the buffer object where the afxRaster data is copied from or to.
    afxNat              rowStride; /// (aka bytes per row) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
    afxNat              rowCnt; /// (aka rows per layer) specify in texels a subregion of a larger two- or three-dimensional afxRaster in buffer memory, and control the addressing calculations. If either of these values is zero, that aspect of the buffer memory is considered to be tightly packed according to the imageExtent.
};

AFX_DEFINE_STRUCT(afxRasterCopy)
/// Especifica��o de opera��o de c�pia de afxRaster.
{
    afxNat              srcLodIdx;
    afxWhd              srcOrigin; /// select the initial x, y, and z offsets in texels of the sub-regions of the source and destination afxRaster data.
    afxRasterRegion     dst;
};

AFX_DEFINE_STRUCT(afxRasterLayout)
/// Information about the layout of the raster subresource.
{
    afxSize             offset; /// the byte offset from the start of the raster or the plane where the raster subresource begins.
    afxNat              rowStride; /// the number of bytes between each row of texels in an raster.
    afxNat              depthStride; ///  the number of bytes between each layer or (3D) slice of an raster.
    afxNat              siz; /// the size in bytes of the raster subresource. siz includes any extra memory that is required based on rowStride.
};

AFX_DEFINE_STRUCT(afxRasterInfo)
{
    afxPixelFormat      fmt;
    afxWhd              whd;
    afxNat              lodCnt;
    afxNat              mipCnt;
    afxNat              sampleCnt;
    afxRasterUsage      usage;
    afxRasterFlags      flags;
    void*               udd;
};

// LOD is mip level or sample level, depending on raster

AVX afxDrawContext  AfxGetRasterContext(afxRaster ras);

AVX void*           AfxGetRasterUdd(afxRaster ras);

AVX afxNat          AfxGetRasterBpp(afxRaster ras);

AVX afxBool         AfxRasterIs1d(afxRaster ras);
AVX afxBool         AfxRasterIs3d(afxRaster ras);
AVX afxBool         AfxRasterIsCubemap(afxRaster ras);
AVX afxBool         AfxRasterIsLayered(afxRaster ras);

AVX afxNat          AfxCountRasterMipmaps(afxRaster ras);
AVX afxNat          AfxCountRasterSamples(afxRaster ras);

AVX afxRasterUsage  AfxGetRasterUsage(afxRaster ras);
AVX afxRasterFlags  AfxGetRasterFlags(afxRaster ras);
AVX afxPixelFormat  AfxGetRasterFormat(afxRaster ras);
AVX void            AfxGetRasterExtent(afxRaster ras, afxNat lodIdx, afxWhd whd);

AVX void            AfxGetRasterSwizzling(afxRaster ras, afxNat subIdx, avxColorSwizzling* csw);

AVX void            AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc);

AVX afxBool         AfxQueryRasterLayout(afxRaster ras, afxNat lodIdx, afxNat layerIdx, afxRasterLayout* layout);

AVX afxRasterUsage  AfxTestRasterUsage(afxRaster ras, afxRasterUsage mask);
AVX afxRasterFlags  AfxTestRasterFlags(afxRaster ras, afxRasterFlags mask);

// Update texture raster data from arbitrary raw allocation. A safe way of copying.
AVX afxError        AfxUpdateRaster(afxRaster ras, afxNat portId, afxNat opCnt, afxRasterIo const ops[], void const* src);
AVX afxError        AfxDumpRaster(afxRaster ras, afxNat portId, afxNat opCnt, afxRasterIo const ops[], void* dst);

// Stream in/out texture raster data from/to a stream.
AVX afxError        AfxUploadRaster(afxRaster ras, afxNat portId, afxNat opCnt, afxRasterIo const ops[], afxStream in);
AVX afxError        AfxDownloadRaster(afxRaster ras, afxNat portId, afxNat opCnt, afxRasterIo const ops[], afxStream out);

// Stream in/out texture raster data from/to a file.
AVX afxError        AfxReloadRaster(afxRaster ras, afxRasterIo const* op, afxNat lodCnt, afxUri const* uri); // reload
AVX afxError        AfxPrintRaster(afxRaster ras, afxNat portId, afxRasterIo const* op, afxNat lodCnt, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxAcquireRasters(afxDrawContext dctx, afxNat cnt, afxRasterInfo const info[], afxRaster rasters[]);

AVX afxError        AfxLoadRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxNat cnt, afxUri const uri[], afxRaster rasters[]);

AVX afxRaster       AfxAssembleRaster(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxNat cnt, afxUri const layers[]);
AVX afxError        AfxAssembleRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxNat cnt, afxUri const layers[], afxRaster* ras);
AVX afxRaster       AfxAssembleCubemapRasters(afxDrawContext dctx, afxRasterUsage usage, afxRasterFlags flags, afxUri const* dir, afxUri const faces[6]);

#endif//AVX_RASTER_H