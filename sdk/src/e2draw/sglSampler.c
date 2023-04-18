#include "sgl.h"

#include "afx/draw/pipelining/afxSampler.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxSamplerSpecification const *spec;
} _afxSmpCtorArgs;

////////////////////////////////////////////////////////////////////////////////
// SAMPLER                                                                    //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _AfxStdSmpUpdate(afxSampler smp, afxDrawEngine deng)
{
    //AfxEntry("smp=%p", smp);
    afxError err = NIL;
    AfxAssertObject(smp, AFX_FCC_SMP);

    sglVmt const* gl = &deng->wglVmt;

    if (!smp->gpuHandle)
    {
        if (!(smp->gpuHandle))
        {
            gl->GenSamplers(1, &(smp->gpuHandle)); _SglThrowErrorOccuried();
      
            AfxAssert(gl->IsSampler(smp->gpuHandle));

            GLenum magF = SglToGlTexelFilterMode(smp->magFilter);
            GLenum minF = SglToGlTexelFilterModeMipmapped(smp->minFilter, smp->mipmapFilter);
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_MAG_FILTER, magF); _SglThrowErrorOccuried();
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_MIN_FILTER, minF); _SglThrowErrorOccuried();

            GLint wrapS = SglToGlTexWrapMode(smp->uvw[0]);
            GLint wrapT = SglToGlTexWrapMode(smp->uvw[1]);
            GLint wrapR = SglToGlTexWrapMode(smp->uvw[2]);
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_WRAP_S, wrapS); _SglThrowErrorOccuried();
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_WRAP_T, wrapT); _SglThrowErrorOccuried();
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_WRAP_R, wrapR); _SglThrowErrorOccuried();

            if (smp->anisotropyEnabled)
            {
                gl->SamplerParameterf(smp->gpuHandle, GL_TEXTURE_MAX_ANISOTROPY, smp->anisotropyMaxDegree); _SglThrowErrorOccuried();
            }
            else
            {
                //gl->SamplerParameterf(smp->gpuHandle, GL_TEXTURE_MAX_ANISOTROPY, 0); _SglThrowErrorOccuried();
            }

            gl->SamplerParameterf(smp->gpuHandle, GL_TEXTURE_LOD_BIAS, smp->mipLodBias); _SglThrowErrorOccuried();
            gl->SamplerParameterf(smp->gpuHandle, GL_TEXTURE_MIN_LOD, smp->minLod); _SglThrowErrorOccuried();
            gl->SamplerParameterf(smp->gpuHandle, GL_TEXTURE_MAX_LOD, smp->maxLod); _SglThrowErrorOccuried();
            
            if (smp->compareEnabled)
            {
                // what about GL_TEXTURE_COMPARE_MODE?
            }
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_COMPARE_MODE, GL_NONE); _SglThrowErrorOccuried();
            GLint cop = SglToGlCompareOp(smp->compareOp);
            gl->SamplerParameteri(smp->gpuHandle, GL_TEXTURE_COMPARE_FUNC, cop); _SglThrowErrorOccuried();
            gl->SamplerParameterfv(smp->gpuHandle, GL_TEXTURE_BORDER_COLOR, (void*)smp->borderColor);

            smp->lastUpdTime = smp->lastUpdTime;
            AfxEcho("Sampler %p reinstanced.", smp);
        }
    }

    return err;
}

_SGL void _AfxSamplerDescribe(afxSampler smp, afxSamplerSpecification *spec)
{
    afxError err = NIL;
    AfxAssertObject(smp, AFX_FCC_SMP);
    spec->magFilter = smp->magFilter;
    spec->minFilter = smp->minFilter;
    spec->mipmapFilter = smp->mipmapFilter;
    spec->uvw[0] = smp->uvw[0];
    spec->uvw[1] = smp->uvw[1];
    spec->uvw[2] = smp->uvw[2];
    spec->anisotropyEnabled = smp->anisotropyEnabled;
    spec->anisotropyMaxDegree = smp->anisotropyMaxDegree;
    spec->compareEnabled = smp->compareEnabled;
    spec->compareOp = smp->compareOp;
    AfxColorSet(spec->borderColor, smp->borderColor[0], smp->borderColor[1], smp->borderColor[2], smp->borderColor[3]);
    spec->unnormalizedCoords = smp->unnormalizedCoords;
}

_SGL afxSampler _AfxDrawContextAcquireSampler(afxDrawContext dctx, afxSamplerSpecification const *spec)
{
    AfxEntry("dctx=%p,spec=%p", dctx, spec);
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxSampler smp = NIL;

    _afxSmpCtorArgs args =
    {
        dctx,
        spec
    };

    if (!(smp = AfxObjectAcquire(AfxDrawContextGetSamplerClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return smp;
}

_SGL afxError _AfxSmpDtor(afxSampler smp)
{
    afxError err = NIL;
    AfxEntry("smp=%p", smp);
    AfxAssertObject(smp, AFX_FCC_SMP);

    afxDrawContext dctx = AfxObjectGetProvider(&smp->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    sglVmt const* gl = &(dctx->queues[0]->wglVmt);

    if (smp->gpuHandle)
    {
        afxDrawContext dctx = AfxSamplerGetContext(smp);
        _SglEnqueueGlResourceDeletion(dctx, 0, 4, smp->gpuHandle);
        smp->gpuHandle = 0;
    }

    return err;
}

_SGL afxSmpImpl const _AfxStdSmpImpl;

afxSmpImpl const _AfxStdSmpImpl =
{
    _AfxSamplerDescribe
};

_SGL afxError _AfxSmpCtor(afxSampler smp, _afxSmpCtorArgs *args)
{
    afxError err = NIL;
    AfxEntry("smp=%p", smp);
    AfxAssertObject(smp, AFX_FCC_SMP);
    afxSamplerSpecification const *spec = args->spec;
    AfxAssert(spec);

    smp->magFilter = spec->magFilter;
    smp->minFilter = spec->minFilter;
    smp->mipmapFilter = spec->mipmapFilter;
    smp->uvw[0] = spec->uvw[0];
    smp->uvw[1] = spec->uvw[1];
    smp->uvw[2] = spec->uvw[2];
    smp->anisotropyEnabled = spec->anisotropyEnabled;
    smp->anisotropyMaxDegree = spec->anisotropyMaxDegree;
    smp->compareEnabled = spec->compareEnabled;
    smp->compareOp = spec->compareOp;
    AfxColorSet(smp->borderColor, spec->borderColor[0], spec->borderColor[1], spec->borderColor[2], spec->borderColor[3]);
    smp->unnormalizedCoords = spec->unnormalizedCoords;
    
    smp->lastUpdTime = AfxGetTimer();

    smp->gpuHandle = 0;

    return err;
}

_SGL afxClassSpecification const _AfxSmpClassSpec;

afxClassSpecification const _AfxSmpClassSpec =
{
    AFX_FCC_SMP,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxSampler)),
    NIL,
    (void*)_AfxSmpCtor,
    (void*)_AfxSmpDtor,
    "Sampler",
    &_AfxStdSmpImpl
};