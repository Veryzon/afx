/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_CONTEXT_C
#define _AFX_TEXTURE_C
#include "../_classified/afxDrawClassified.h"
#include "afx/draw/io/afxTarga.h"

////////////////////////////////////////////////////////////////////////////////
// TEXTURE BLUEPRINT                                                          //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxTextureBlueprintEnd(afxTextureBlueprint *blueprint, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_TEXB);

    if (cnt)
    {
        AfxAssert(tex);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxBuildTextures(blueprint->dctx, 1, &tex[i], blueprint))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxReleaseObjects(1, (void*[]) { tex[j] });
                    tex[j] = NIL;
                }
                break;
            }
        }
    }

    for (afxNat i = 0; i < blueprint->imgCnt; i++)
    {
        if (blueprint->images[i].type == AFX_FCC_URI)
        {
            //if (blueprint->images[i].uri)
                AfxUriDeallocate(&blueprint->images[i].uri);
        }
    }

    return err;
}

_AFX void AfxAcquireTextureBlueprint(afxTextureBlueprint *blueprint, afxWhd const extent, afxPixelFormat fmt, afxFlags usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAdvertise("texb->fcc == AFX_FCC_TEXB");
    blueprint->fcc = AFX_FCC_TEXB;

    blueprint->dctx = NIL;

    AfxAssert(fmt);
    blueprint->fmt = fmt;

    AfxAssert(extent);
    blueprint->extent[0] = extent[0];
    blueprint->extent[1] = extent[1];
    blueprint->extent[2] = extent[2];

    AFX_ZERO(blueprint->images);
    blueprint->imgCnt = 0;
    blueprint->lodCnt = 1;

    blueprint->usage = usage;
}

AFX afxError AfxTextureBlueprintAddImage(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, void const *start, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_TEXB);
    AfxAssert(blueprint->imgCnt < (sizeof(blueprint->images) / sizeof(blueprint->images[0])));
    //AfxAssert(start);
    //AfxAssert(range);
    blueprint->images[blueprint->imgCnt].stream.fmt = fmt;
    
    if (extent)
    {
        blueprint->images[blueprint->imgCnt].stream.extent[0] = extent[0];
        blueprint->images[blueprint->imgCnt].stream.extent[1] = extent[1];
        blueprint->images[blueprint->imgCnt].stream.extent[2] = extent[2];
    }
    else
    {
        blueprint->images[blueprint->imgCnt].stream.extent[0] = 0;
        blueprint->images[blueprint->imgCnt].stream.extent[1] = 0;
        blueprint->images[blueprint->imgCnt].stream.extent[2] = 0;
    }

    blueprint->images[blueprint->imgCnt].data.start = start;
    blueprint->images[blueprint->imgCnt].data.range = range;
    blueprint->images[blueprint->imgCnt].type = NIL;
    ++blueprint->imgCnt;
    return err;
}

AFX afxError AfxTextureBlueprintAddImageFromStream(afxTextureBlueprint *blueprint, afxPixelFormat fmt, afxWhd const extent, afxStream *ios, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_TEXB);
    AfxAssert(blueprint->imgCnt < (sizeof(blueprint->images) / sizeof(blueprint->images[0])));
    AfxAssert(ios);
    AfxAssert(offset);
    AfxAssert(range);
    blueprint->images[blueprint->imgCnt].stream.fmt = fmt;

    if (extent)
    {
        blueprint->images[blueprint->imgCnt].stream.extent[0] = extent[0];
        blueprint->images[blueprint->imgCnt].stream.extent[1] = extent[1];
        blueprint->images[blueprint->imgCnt].stream.extent[2] = extent[2];
    }
    else
    {
        blueprint->images[blueprint->imgCnt].stream.extent[0] = 0;
        blueprint->images[blueprint->imgCnt].stream.extent[1] = 0;
        blueprint->images[blueprint->imgCnt].stream.extent[2] = 0;
    }

    blueprint->images[blueprint->imgCnt].stream.ios = ios;
    blueprint->images[blueprint->imgCnt].stream.offset = offset;
    blueprint->images[blueprint->imgCnt].stream.range = range;
    blueprint->images[blueprint->imgCnt].type = AFX_FCC_IOS;
    ++blueprint->imgCnt;
    return err;
}

AFX afxError AfxTextureBlueprintAddImagesFromResource(afxTextureBlueprint *blueprint, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_TEXB);
    AfxAssert(uri);
    AfxAssert(cnt <= blueprint->imgCnt - (sizeof(blueprint->images) / sizeof(blueprint->images[0])));

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(blueprint->imgCnt < (sizeof(blueprint->images) / sizeof(blueprint->images[0])));
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxCloneUri(&blueprint->images[blueprint->imgCnt].uri, &uri[i]);
        blueprint->images[blueprint->imgCnt].type = AFX_FCC_URI;
        ++blueprint->imgCnt;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TEXTURE                                                                    //
////////////////////////////////////////////////////////////////////////////////

_AFXINL afxResult AfxTestTexture(afxTexture tex, afxTextureFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->flags & bitmask;
}

_AFXINL void AfxUnflagTexture(afxTexture tex, afxTextureFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    tex->flags &= ~bitmask;
}

_AFXINL void AfxFlagTexture(afxTexture tex, afxTextureFlags bitmask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    tex->flags |= bitmask;
}

_AFX afxNat AfxGetTextureLodCount(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->lodCnt;
}

_AFX afxNat AfxGetTextureImageCount(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->imgCnt;
}

_AFX afxColorSwizzling const* AfxTextureGetColorSwizzling(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->swizzling;
}

_AFX afxNat AfxGetTextureSampleCount(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->sampleCnt;
}

_AFX afxPixelFormat AfxGetTextureFormat(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    return tex->fmt;
}

_AFX void AfxGetTextureExtent(afxTexture tex, afxNat lodIdx, afxWhd extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(tex->lodCnt > lodIdx);
    AfxAssert(extent);

    extent[0] = tex->whd[0];
    extent[1] = tex->whd[1];
    extent[2] = tex->whd[2];

    while (lodIdx)
    {
        extent[0] = extent[0] >> lodIdx;
        extent[1] = extent[1] >> lodIdx;
        extent[2] = extent[2] >> lodIdx;
        --lodIdx;
    }
}

_AFX afxNat AfxLocateTextureRegion(afxTexture tex, afxNat lodIdx, afxNat imgIdx, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssertRange(tex->lodCnt, lodIdx, 1);
    AfxAssertRange(tex->imgCnt, imgIdx, 1);
    //AfxAssertRangef(tex->whd[0], offset[0], 1); // is a evil trick to use it to measure tex
    //AfxAssertRangef(tex->whd[1], offset[1], 1);
    //AfxAssertRangef(tex->whd[2], offset[2], 1);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(tex->fmt, &pfd);
    afxNat pixelSiz = AFX_ALIGN(pfd.bpp, 32) / AFX_BYTE_SIZE;

    afxWhd offset2 = { offset[0], offset[1], offset[2] };
    afxWhd whd = { tex->whd[0], tex->whd[1], tex->whd[2] };
    afxNat memOff = 0;

    while (lodIdx)
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;
        
        memOff += (tex->imgCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        --lodIdx;
    };

    memOff += (imgIdx * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
    memOff += (offset2[2] * (offset2[1] * (offset2[0] * pixelSiz)));
    memOff = AFX_ALIGN(memOff, AFX_SIMD_ALIGN);

    return memOff;
}

_AFX afxNat AfxMeasureTextureRegion(afxTexture tex, afxTextureRegion const *rgn) // can't be multiple because regions couldn't be continous.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);

    afxPixelLayout pfd;
    AfxDescribePixelFormat(tex->fmt, &pfd);
    afxNat pixelSiz = AFX_ALIGN(pfd.bpp, 32) / AFX_BYTE_SIZE;

    afxWhd offset2 = { rgn->offset[0], rgn->offset[1], rgn->offset[2] };
    afxWhd extent2 = { rgn->extent[0], rgn->extent[1], rgn->extent[2] };
    afxWhd whd = { tex->whd[0], tex->whd[1], tex->whd[2] };
    afxNat size = 0;

    afxNat lodIdx = 0;
    do
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        extent2[0] = extent2[0] >> lodIdx;
        extent2[1] = extent2[1] >> lodIdx;
        extent2[2] = extent2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;

        //memOff += (tex->imgCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        ++lodIdx;
    } while (lodIdx < rgn->lodIdx);
    
    extent2[0] = AfxMaxi(extent2[0], 1);
    extent2[1] = AfxMaxi(extent2[1], 1);
    extent2[2] = AfxMaxi(extent2[2], 1);

    for (afxNat i = 1; i < rgn->imgCnt; i++)
        size += ((whd[2] * (whd[1] * (whd[0] * pixelSiz))));

    size += (extent2[2] * (extent2[1] * (extent2[0] * pixelSiz)));

    size -= (offset2[2] * (offset2[1] * (offset2[0] * pixelSiz)));

    size = AFX_ALIGN(size, AFX_SIMD_ALIGN);

    return size;
}

////////////////////////////////////////////////////////////////////////////////
// SERIALIZATION AND STORAGE                                                  //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxInputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssertType(in, AFX_FCC_IOS);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat siz = 0;
        void *ptr;

        if ((ptr = AfxOpenTextureRegion(tex, &rgn[i], AFX_TEX_OPEN_W, &siz))) AfxThrowError();
        else
        {
            if (AfxReadStream(in, ptr, siz, 0))
                AfxThrowError();

            AfxCloseTextureRegion(tex, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxInputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(rgn);
    AfxAssertType(in, AFX_FCC_IOS);

    if (AfxInputTextureRegions(tex, 1, rgn, in))
        AfxThrowError();

    return err;
}

_AFX afxError AfxInputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssertType(in, AFX_FCC_IOS);
    AfxAssertRange(tex->lodCnt, lodIdx, 1);
    AfxAssertRange(tex->imgCnt, baseImg, imgCnt);

    afxTextureRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseImg = baseImg;
    rgn.imgCnt = imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    if (AfxInputTextureRegion(tex, &rgn, in))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(uri);

    if (AfxFetchTextureRegionsFromTarga(tex, cnt, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    if (AfxFetchTextureRegions(tex, 1, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFetchTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxTextureRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseImg = baseImg;
    rgn.imgCnt = imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    if (AfxFetchTextureRegion(tex, &rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxOutputTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat siz = 0;
        void *ptr;

        if ((ptr = AfxOpenTextureRegion(tex, &rgn[i], AFX_TEX_OPEN_R, &siz))) AfxThrowError();
        else
        {
            if (AfxWriteStream(out, ptr, siz, 0))
                AfxThrowError();

            AfxCloseTextureRegion(tex, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxOutputTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(rgn);
    AfxAssertType(out, AFX_FCC_IOS);

    if (AfxOutputTextureRegions(tex, 1, rgn, out))
        AfxThrowError();

    return err;
}

_AFX afxError AfxOutputTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxStream *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssertType(out, AFX_FCC_IOS);
    AfxAssertRange(tex->lodCnt, lodIdx, 1);
    AfxAssertRange(tex->imgCnt, baseImg, imgCnt);

    afxTextureRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseImg = baseImg;
    rgn.imgCnt = imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    if (AfxOutputTextureRegion(tex, &rgn, out))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(uri);

    if (AfxPrintTextureRegionsToTarga(tex, cnt, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);

    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    if (AfxPrintTextureRegions(tex, 1, rgn, uri))
        AfxThrowError();

    return err;
}

_AFX afxError AfxPrintTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxTextureRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseImg = baseImg;
    rgn.imgCnt = imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    if (AfxPrintTextureRegion(tex, &rgn, uri))
        AfxThrowError();

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// TRANSFERENCE AND COPY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxUpdateTextureRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], void const *src[], afxNat const siz[], afxPixelFormat const fmt[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(cnt);
    AfxAssert(rgn);
    AfxAssert(src);
    AfxAssert(fmt);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(tex->lodCnt > rgn[i].lodIdx);
        AfxAssertRange(tex->imgCnt, rgn[i].baseImg, rgn[i].imgCnt);
        AfxAssertRange(tex->whd[0], rgn[i].offset[0], rgn[i].extent[0]);
        AfxAssertRange(tex->whd[1], rgn[i].offset[1], rgn[i].extent[1]);
        AfxAssertRange(tex->whd[2], rgn[i].offset[2], rgn[i].extent[2]);

        afxNat size;
        void *data = AfxOpenTextureRegion(tex, &rgn[i], AFX_TEX_OPEN_W, &size);

        if (!data) AfxThrowError();
        else
        {
            AfxAssert(fmt[i] == tex->fmt);
            AfxAssert(size >= siz[i]);
            AfxCopy(data, src[i], AfxMini((siz[i]), size));

            AfxCloseTextureRegion(tex, &rgn[i]);
        }
    }
    return err;
}

_AFX afxError AfxUpdateTextureRegion(afxTexture tex, afxTextureRegion const *rgn, void const *src, afxNat siz, afxPixelFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);
    AfxAssert(src);
    AfxAssert(fmt);
    
    if (AfxUpdateTextureRegions(tex, 1, rgn, &src, &siz, &fmt))
        AfxThrowError();

    return err;
}

_AFX afxError AfxUpdateTexture(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, void const *src, afxNat siz, afxPixelFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(tex->lodCnt > lodIdx);
    AfxAssertRange(tex->imgCnt, baseImg, imgCnt);
    AfxAssert(src);
    AfxAssert(fmt);

    afxTextureRegion rgn;
    rgn.lodIdx = lodIdx;
    rgn.baseImg = baseImg;
    rgn.imgCnt = imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    if (AfxUpdateTextureRegion(tex, &rgn, &src, siz, fmt))
        AfxThrowError();

    return err;
}





///////////////////////////////////////////////////


_AFX void AfxCloseTextureRegion(afxTexture tex, afxTextureRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);

    if (tex->vmt->unmap(tex, rgn))
        AfxThrowError();
};

_AFX void* AfxOpenTextureRegion(afxTexture tex, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *size)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);
    
    void *ptr = NIL;

    if (tex->vmt->map(tex, rgn, flags, size, &ptr))
        AfxThrowError();

    return ptr;
}

_AFX afxError AfxBufferizeTexture(afxTexture tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    afxDrawContext dctx = AfxGetObjectProvider(tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);

    afxTextureRegion rgn;
    rgn.baseImg = 0;
    rgn.imgCnt = tex->imgCnt;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    rgn.extent[0] = tex->whd[0];
    rgn.extent[1] = tex->whd[1];
    rgn.extent[2] = tex->whd[2];

    afxNat siz = 0;

    for (afxNat i = 0; i < tex->lodCnt; i++)
    {
        rgn.lodIdx = i;
        siz += AfxMeasureTextureRegion(tex, &rgn);
    }

    void *maps = NIL;

    if (siz && !(maps = AfxReallocate(mem, tex->maps, AFX_ALIGN(siz, AFX_SIMD_ALIGN), AFX_SIMD_ALIGN, AfxSpawnHint()))) AfxThrowError();
    else
        tex->maps = maps;

    return err;
}

#if 0
_AFX afxError AfxReadjustTexture(afxTexture tex, afxWhd const extent)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(extent[0] != tex->whd[0] || extent[1] != tex->whd[1] || extent[2] != tex->whd[2]);

    afxWhd whd = { tex->whd[0], tex->whd[1], tex->whd[2] };
    tex->whd[0] = extent[0];
    tex->whd[1] = extent[1];
    tex->whd[2] = extent[2];

    if (AfxBufferizeTexture(tex))
    {
        AfxThrowError();
        tex->whd[0] = whd[0]; // try to restore it
        tex->whd[1] = whd[1];
        tex->whd[2] = whd[2];
        AfxBufferizeTexture(tex);
    }
    else
    {
        // Agora, for�emos a atualiza��o

        afxNat rgnSize;
        afxTextureRegion rgn;
        rgn.baseImg = 0;
        rgn.imgCnt = tex->imgCnt;
        rgn.offset[0] = 0;
        rgn.offset[1] = 0;
        rgn.offset[2] = 0;
        rgn.extent[0] = tex->whd[0];
        rgn.extent[1] = tex->whd[1];
        rgn.extent[2] = tex->whd[2];

        for (afxNat i = 0; i < tex->lodCnt; i++)
        {
            rgn.lodIdx = i;
            AfxOpenTextureRegion(tex, &rgn, AFX_TEX_OPEN_X, &rgnSize);
            AfxCloseTextureRegion(tex, &rgn);
        }

        afxEvent ev;
        AfxEventDeploy(&ev, AFX_EVENT_TEX_RESIZ, &tex->obj, tex->whd);
        AfxObjectEmitEvent(&tex->obj, &ev);
    }
    return err;
}
#endif

#if 0
_AFX void const* AfxTextureGetData(afxTexture tex, afxNat lod, afxNat layer, afxWhd const offset)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    static afxWhd const whdZeroed = { 0, 0, 0 };
    afxNat const *xyz = offset ? offset : whdZeroed;

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    AfxAssert(tex->lodCnt > lod);
    AfxAssert(tex->imgCnt > layer);
    AfxAssert(tex->whd[0] > xyz[0]);
    AfxAssert(tex->whd[1] > xyz[1]);
    AfxAssert(tex->whd[2] > xyz[2]);

    afxSize off = AfxLocateTextureRegion(tex, lod, layer, offset);
    afxByte *bytemap = &(tex->bytemap[off]);
    return bytemap;
}
#endif

_AFX afxError AfxGenerateTextureLods(afxTexture tex, afxNat firstLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);
    (void)firstLod;
    AfxAssert(lodCnt);
    AfxThrowError();
    return err;
}

_AFX afxError AfxTextureSwapChannels(afxTexture tex, afxColorSwizzle a, afxColorSwizzle b, afxTextureRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &tex, AFX_FCC_TEX);

    AfxAssert(rgn);
    AfxAssert(tex->lodCnt > rgn->lodIdx);
    AfxAssertRange(tex->imgCnt, rgn->baseImg, rgn->imgCnt);
    AfxAssertRange(tex->whd[0], rgn->offset[0], rgn->extent[0]);
    AfxAssertRange(tex->whd[1], rgn->offset[1], rgn->extent[1]);
    AfxAssertRange(tex->whd[2], rgn->offset[2], rgn->extent[2]);

    afxPixelFormat fmt = AfxGetTextureFormat(tex);
    AfxAssert(fmt == AFX_PFD_BGR8 || fmt == AFX_PFD_BGRA8 || fmt == AFX_PFD_RGB8 || fmt == AFX_PFD_RGBA8);
    
    afxPixelLayout pfd;
    AfxDescribePixelFormat(fmt, &pfd);
    afxNat pixelSiz = pfd.bpp / AFX_BYTE_SIZE;

    AfxAssert(a < AFX_COLOR_SIZZLE_ZERO);

    afxNat rgnSize;
    afxByte *bytemap = AfxOpenTextureRegion(tex, rgn, AFX_TEX_OPEN_RW, &rgnSize);

    // swap the R and B values to get RGB since the bitmap color format is in BGR
    for (afxNat k = 0; k < rgn->extent[0] * rgn->extent[1] * pixelSiz; k += pixelSiz)
    {
        afxByte temp = (b != AFX_COLOR_SIZZLE_ZERO ? (b != AFX_COLOR_SIZZLE_ONE ? bytemap[k + b] : 1) : 0);
        bytemap[k + b] = bytemap[k + a];
        bytemap[k + a] = temp;
    }

    AfxCloseTextureRegion(tex, rgn);
    return err;
}

_AFX afxError AfxBuildTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxTextureBlueprint const *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    AfxEntry("cnt=%u,blueprint=%p,tex=%p", cnt, blueprint, tex);

    if (AfxAcquireObjects(&dctx->textures, cnt, (afxHandle*)tex, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxResult AfxFindTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    afxResult rslt = 0;

    afxNat j = 0;
    afxTexture tmp;
    while (AfxEnumerateTextures(dctx, j, 1, &tmp))
    {
        AfxAssertObjects(1, &tmp, AFX_FCC_TEX);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxUri const*texUri = NIL;

            if (texUri && AfxUriIsEquivalent(texUri, &uri[i]))
            {
                tex[i] = tmp;
                rslt++;
                break;
            }
        }

        if (rslt == (afxResult)cnt)
            break;

        ++j;
    }
    return rslt;
}

_AFX afxError AfxLoadTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_CTX);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxAssert(!AfxUriIsBlank(&uri[i]));

        AfxEcho("Uploading texture '%.*s'", AfxPushString(&uri[i].str));

        afxUri fext;
        AfxExcerptUriExtension(&fext, &uri[i], FALSE);

        if (AfxUriIsBlank(&fext)) AfxThrowError();
        else
        {
            afxUri fpath, query;
            AfxExcerptUriPath(&fpath, &uri[i]);
            AfxExcerptUriQuery(&query, &uri[i], TRUE);

            if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".tga", 4))
            {
                if (AfxLoadTexturesTarga(dctx, 1, &tex[i], &uri[i])) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &tex[i], AFX_FCC_TEX);
                }
            }
            else if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".xml", 4))
            {
                AfxError("Extension (%.*s) supported not implemented yet.", AfxPushString(AfxUriGetStringConst(&fext)));
                AfxThrowError();
            }
            else
            {
                AfxError("Extension (%.*s) not supported.", AfxPushString(AfxUriGetStringConst(&fext)));
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObjects(1, (void*[]) { tex[j] });
                tex[j] = NIL;
            }
            break;
        }
    }
    return err;
}

_AFX afxError AfxAcquireTextures(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    for (afxNat i = 0; i < cnt; i++)
    {
        tex[i] = NIL;

        afxUri name;
        AfxAssertType(&uri[i], AFX_FCC_URI);
        AfxAssert(!(AfxUriIsBlank(&uri[i])));
        AfxExcerptUriName(&name, &uri[i]);
        AfxAssert(!(AfxUriIsBlank(&name)));

        if (1 == AfxFindTextures(dctx, 1, &tex[i], &name))
        {
            AfxAssertObjects(1, &tex[i], AFX_FCC_TEX);
            AfxReacquireObjects(1, (void*) { &tex[i] });
            //rslt++;
        }
        else if (AfxLoadTextures(dctx, 1, &tex[i], &uri[i]))  AfxThrowError();
        else
        {
            AfxAssertObjects(1, &tex[i], AFX_FCC_TEX);
        }

        for (afxNat j = 0; j < i; j++)
        {
            AfxReleaseObjects(1, (void*[]) { tex[j] });
            tex[j] = NIL;
        }
        break;
    }
    return err;
}

_AFX afxNat AfxEnumerateTextures(afxDrawContext dctx, afxNat first, afxNat cnt, afxTexture tex[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(tex);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return AfxEnumerateInstances(&dctx->textures, first, cnt, (afxHandle*)tex);
}