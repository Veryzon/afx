/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_PIPELINE_C
#define _AVX_RASTERIZER_C
#include "../impl/avxImplementation.h"
AVX afxError _AvxParseXmlPipelineBlueprint(afxXml const* xml, afxUnit elemIdx, afxUnit specId, avxPipelineBlueprint* pipb, avxShaderStage shaderStages[], afxUri shaderUris[], afxString shaderFns[]);

 // OpenGL/Vulkan Continuous Integration

_AVX afxDrawSystem AfxGetPipelineContext(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    afxDrawSystem dsys = AfxGetProvider(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxBool AfxGetPipelineLigature(avxPipeline pip, avxLigature* ligature)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxLigature liga = pip->liga;

    if (!liga)
    {
        // rebuild the pipeline ligature
        {
            afxUnit listedShaderCnt = 0;
            avxShader listedShaders[16];

            for (afxUnit j = 0; j < pip->stageCnt; j++)
                listedShaders[j] = pip->stages[j].shd;

            listedShaderCnt = pip->stageCnt;

            avxLigatureConfig ligc = { 0 };
            ligc.shaderCnt = listedShaderCnt;
            ligc.shaders = listedShaders;

            afxDrawSystem dsys = AfxGetPipelineContext(pip);
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
            if (AfxDeclareLigatures(dsys, 1, &ligc, &liga)) AfxThrowError();
            else
            {
                AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
                pip->liga = liga;
            }
        }
    }
    AFX_TRY_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    *ligature = liga;
    return !!liga;
}

_AVX afxBool AfxGetPipelineVertexInput(avxPipeline pip, avxVertexDecl* input)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    avxVertexDecl vin = pip->vin;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    *input = vin;
    return !!vin;
}

_AVX void AfxDescribePipeline(avxPipeline pip, avxPipelineInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    info->stageCnt = pip->stageCnt;
    info->primTopology = pip->primTop;
    info->primRestartEnabled = pip->primRestartEnabled;
    info->depthClampEnabled = pip->depthClampEnabled;
    info->cullMode = pip->cullMode;
    info->frontFacingInverted = pip->frontFacingInverted;
    info->liga = pip->liga;
    info->vin = pip->vin;
    
    // rasterization

    info->dsFlags = pip->dsFlags;
    info->msFlags = pip->msFlags;
    info->rasFlags = pip->rasFlags;
    info->pixelFlags = pip->pixelFlags;

    info->fillMode = pip->fillMode;
    info->lineWidth = pip->lineWidth;

    info->depthBiasEnabled = pip->depthBiasEnabled;
    info->depthBiasSlopeScale = pip->depthBiasSlopeScale;
    info->depthBiasConstFactor = pip->depthBiasConstFactor;
    info->depthBiasClamp = pip->depthBiasClamp;

    info->msEnabled = pip->msEnabled;

    if ((info->sampleLvl = pip->sampleLvl))
        AfxCopy2(info->sampleMasks, pip->sampleMasks, sizeof(pip->sampleMasks[0]), pip->sampleLvl);

    info->alphaToOneEnabled = pip->alphaToOneEnabled;
    info->alphaToCoverageEnabled = pip->alphaToCoverageEnabled;
    info->sampleShadingEnabled = pip->sampleShadingEnabled;
    info->minSampleShadingValue = pip->minSampleShadingValue;

    if ((info->stencilTestEnabled = pip->stencilTestEnabled))
    {
        info->stencilFront = pip->stencilFront;
        info->stencilBack = pip->stencilBack;
    }

    info->depthTestEnabled = pip->depthTestEnabled;
    info->depthCompareOp = pip->depthCompareOp;
    info->depthWriteDisabled = pip->depthWriteDisabled;
    info->dsFmt = pip->dsFmt;

    if ((info->depthBoundsTestEnabled = pip->depthBoundsTestEnabled))
        AfxV2dCopy(info->depthBounds, pip->depthBounds);

    if ((info->colorOutCnt = pip->outCnt))
        AfxCopy2(info->colorOuts, pip->outs, sizeof(pip->outs[0]), pip->outCnt);

    AfxColorCopy(info->blendConstants, pip->blendConstants);
    info->pixelLogicOpEnabled = pip->logicOpEnabled;
    info->pixelLogicOp = pip->logicOp;
}

_AVX afxUnit AfxGetColorOutputChannels(avxPipeline pip, afxUnit first, afxUnit cnt, avxColorOutput channels[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->outCnt, first, cnt);
    AFX_ASSERT(channels);
    afxUnit i = 0;

    for (; i < cnt; i++)
        channels[i] = pip->outs[first + i];

    return i;
}

_AVX afxUnit AfxGetMultisamplingMasks(avxPipeline pip, afxUnit first, afxUnit cnt, afxMask sampleMask[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
    AFX_ASSERT_RANGE(pip->sampleLvl, first, cnt);
    AFX_ASSERT(sampleMask);
    afxUnit i = 0;

    for (afxUnit i = 0; i < cnt; i++)
        sampleMask[i] = pip->sampleMasks[first + i];

    return i;
}

_AVX afxBool AfxGetPipelineShader(avxPipeline pip, avxShaderStage stage, avxShader* shader)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    afxBool found = FALSE;

    for (afxUnit slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
    {
        avxShaderSlot* slot = &pip->stages[slotIdx];

        if (slot->stage != stage)
            continue;

        found = TRUE;
        AFX_ASSERT(shader);
        *shader = slot->shd;
    }
    return found;
}

_AVX afxUnit AfxGetPipelineShaders(avxPipeline pip, afxIndex first, afxUnit cnt, avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(pip->stageCnt, first, cnt);
    AFX_ASSERT(cnt);
    AFX_ASSERT(shaders);
    afxUnit hitCnt = 0;
    cnt = AfxMin(pip->stageCnt, cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        shaders[i] = pip->stages[i].shd;
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxError AfxRelinkPipelineFunction(avxPipeline pip, avxShaderStage stage, avxShader shd, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    afxBool found = FALSE;
    avxShaderSlot* slot;
    afxUnit slotIdx;

    for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
    {
        slot = &pip->stages[slotIdx];

        if (slot->stage == stage)
        {
            found = TRUE;
            break;
        }
    }

    if (!found)
    {
        for (slotIdx = 0; slotIdx < pip->stageCnt; slotIdx++)
        {
            slot = &pip->stages[slotIdx];
            
            if (slot->stage == NIL) // if is unused
            {
                found = TRUE;
                slot->stage = stage;
                break;
            }
        }
    }

    if (found && (slot->shd != shd))
    {
        slot = &pip->stages[slotIdx];

        if (shd)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
            AfxReacquireObjects(1, &shd);
        }

        if (slot->shd)
        {
            AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &slot->shd);
            AfxDisposeObjects(1, &slot->shd);
        }

        slot->shd = shd;
        AfxMakeString8(&slot->fn, fn);

        // rebuild the pipeline ligature
        {
            afxUnit listedShaderCnt = 0;
            avxShader listedShaders[16];

            for (afxUnit j = 0; j < pip->stageCnt; j++)
                listedShaders[j] = pip->stages[j].shd;

            listedShaderCnt = pip->stageCnt;

            avxLigatureConfig ligc = { 0 };
            ligc.shaderCnt = listedShaderCnt;
            ligc.shaders = listedShaders;

            avxLigature liga;
            afxDrawSystem dsys = AfxGetPipelineContext(pip);
            AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
            if (AfxDeclareLigatures(dsys, 1, &ligc, &liga)) AfxThrowError();
            else
            {
                if (pip->liga)
                    AfxDisposeObjects(1, &pip->liga);

                pip->liga = liga;
            }
        }
    }
    return err;
}

_AVX afxError AfxUplinkPipelineFunction(avxPipeline pip, avxShaderStage stage, afxUri const* uri, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);

    if (AfxIsUriBlank(uri))
    {
        AfxThrowError();
        return err;
    }

    avxShader shd;
    afxDrawSystem dsys = AfxGetPipelineContext(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (AfxAcquireShaders(dsys, 1, uri, NIL, &shd))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if (AfxRelinkPipelineFunction(pip, stage, shd, fn, specIds, specValues))
        AfxThrowError();

    AfxDisposeObjects(1, &shd);

    return err;
}

_AVX afxError AfxRecompilePipelineFunction(avxPipeline pip, avxShaderStage stage, afxString const* code, afxString const* fn, afxUnit const specIds[], void const* specValues[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    AFX_ASSERT_RANGE(avxShaderStage_TOTAL, stage, 1);
    AFX_ASSERT(code);

    if (AfxIsStringEmpty(code))
    {
        AfxThrowError();
        return err;
    }

    avxShader shd;
        
    afxDrawSystem dsys = AfxGetPipelineContext(pip);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    if (AfxAcquireShaders(dsys, 1, NIL, code, &shd))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    if (AfxRelinkPipelineFunction(pip, stage, shd, fn, specIds, specValues))
        AfxThrowError();

    AfxDisposeObjects(1, &shd);

    return err;
}

_AVX afxError _AvxPipStdDtorCb(avxPipeline pip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    for (afxUnit i = pip->stageCnt; i-- > 0;)
    {
        avxShaderSlot* slot = &pip->stages[i];
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &slot->shd);
        AfxDisposeObjects(1, &slot->shd);
    }

    if (pip->liga)
    {
        AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &pip->liga);
        AfxDisposeObjects(1, &pip->liga);
    }

    if (pip->vin)
    {
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &pip->vin);
        AfxDisposeObjects(1, &pip->vin);
    }

    afxObjectStash const stashes[] =
    {
        {
            .cnt = pip->stageCnt,
            .siz = sizeof(pip->stages[0]),
            .var = (void*)&pip->stages
        },
        {
            .cnt = pip->sampleLvl,
            .siz = sizeof(pip->sampleMasks[0]),
            .var = (void*)&pip->sampleMasks
        },
        {
            .cnt = pip->outCnt,
            .siz = sizeof(pip->outs[0]),
            .var = (void*)&pip->outs
        }
    };

    if (AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxPipStdCtorCb(avxPipeline pip, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);

    afxDrawSystem dsys = args[0];
    avxPipelineBlueprint const *pipb = ((avxPipelineBlueprint const*)args[1]) + invokeNo;
    avxPipelineBlueprint const *razb = ((avxPipelineBlueprint const*)args[2]) + invokeNo;
    
    // GRAPHICS STATE SETTING

    afxUnit sampleMaskCnt = AfxMin(razb->sampleLvl, AVX_MAX_SAMPLE_MASKS);
    afxUnit colorOutCnt = AfxMin(AfxMax(1, razb->colorOutCnt), AVX_MAX_COLOR_OUTPUTS);

    pip->liga = NIL;

    pip->transformationDisabled = !!pipb->transformationDisabled;
    pip->tesselationEnabled = !pip->transformationDisabled && !!pipb->tesselationEnabled;
    pip->rasterizationDisabled = !pip->transformationDisabled && !!pipb->rasterizationDisabled;

    pip->primTop = pipb->primTop;
    pip->patchControlPoints = pipb->patchCtrlPoints;
    pip->cullMode = pipb->cullMode;
    pip->vpCnt = AfxClamp(pipb->vpCnt, 1, 8);

    avxRasterizationFlags rasFlags = razb->rasFlags;
    avxMultisamplingFlags msFlags = razb->msFlags;
    avxDepthStencilFlags dsFlags = razb->dsFlags;
    avxColorOutputFlags pixelFlags = razb->pixelFlags;
    pip->rasFlags = NIL;
    pip->msFlags = NIL;
    pip->dsFlags = NIL;
    pip->pixelFlags = NIL;

    pip->fillMode = razb->fillMode;
    pip->lineWidth = razb->lineWidth;

    pip->depthBiasEnabled = !!razb->depthBiasEnabled;
    pip->depthBiasSlopeScale = razb->depthBiasSlopeScale;
    pip->depthBiasConstFactor = razb->depthBiasConstFactor;
    pip->depthBiasClamp = razb->depthBiasClamp;

    pip->msEnabled = !!razb->msEnabled;
    pip->minSampleShadingValue = razb->minSampleShadingValue;

    pip->depthTestEnabled = !!razb->depthTestEnabled;
    pip->depthCompareOp = razb->depthCompareOp;
    pip->depthWriteDisabled = !!razb->depthWriteDisabled;

    pip->stencilTestEnabled = !!razb->stencilTestEnabled;
    pip->stencilFront = razb->stencilFront;
    pip->stencilBack = razb->stencilBack;

    pip->depthBoundsTestEnabled = !!razb->depthBoundsTestEnabled;
    AfxV2dCopy(pip->depthBounds, razb->depthBounds);

    pip->dsFmt = razb->dsFmt; // ?

    // deveria ser s� o blend/write, j� que s� podemos determinar as sa�das quando assembleado com fragment shaders enquanto pipeline completo.

    AfxV4dCopy(pip->blendConstants, razb->blendConstants);

    pip->logicOpEnabled = !!razb->pixelLogicOpEnabled;
    pip->logicOp = razb->pixelLogicOp;

    afxUnit stageCnt = 0;// pipb->stageCnt;
    //AFX_ASSERT(stageCnt);

    if (!pip->transformationDisabled)
    {
        ++stageCnt;

        if (pip->tesselationEnabled)
            ++stageCnt, ++stageCnt; // twice

        if (pipb->primShaderSupported)
            ++stageCnt;

        if (!pip->rasterizationDisabled)
            ++stageCnt;
    }
    else stageCnt = 1; // compute shader

    afxObjectStash const stashes[] =
    {
        {
            .cnt = stageCnt,
            .siz = sizeof(pip->stages[0]),
            .var = (void*)&pip->stages
        },
        {
            .cnt = sampleMaskCnt,
            .siz = sizeof(pip->sampleMasks[0]),
            .var = (void*)&pip->sampleMasks
        },
        {
            .cnt = colorOutCnt,
            .siz = sizeof(pip->outs[0]),
            .var = (void*)&pip->outs
        }
    };

    if (AfxAllocateInstanceData(pip, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        pip->outCnt = colorOutCnt;
        for (afxUnit i = 0; i < colorOutCnt; i++)
            pip->outs[i] = i < razb->colorOutCnt ? razb->colorOuts[i] : (avxColorOutput) { 0 };

        pip->sampleLvl = sampleMaskCnt;
        for (afxUnit i = 0; i < sampleMaskCnt; i++)
            pip->sampleMasks[i] = i < razb->sampleLvl ? razb->sampleMasks[i] : (afxMask) { 0 };

        pip->stageCnt = stageCnt;
        for (afxUnit i = 0; i < pip->stageCnt; i++)
        {
            avxShaderSlot* slot = &pip->stages[i];
            AfxZero(slot, sizeof(*slot));
        }

        if (pip->transformationDisabled) pip->stages[0].stage = avxShaderStage_COMPUTE;
        else
        {
            afxUnit nextStageIdx = 0;
            pip->stages[nextStageIdx++].stage = avxShaderStage_VERTEX;

            if (pip->tesselationEnabled)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_TESS_CTRL;
                pip->stages[nextStageIdx++].stage = avxShaderStage_TESS_EVAL;
            }

            if (pipb->primShaderSupported)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_PRIMITIVE;
            }

            if (!pip->rasterizationDisabled)
            {
                pip->stages[nextStageIdx++].stage = avxShaderStage_FRAGMENT;
            }
        }

        if ((pip->vin = pipb->vin))
        {
            pip->vin = pipb->vin;
            AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &pip->vin);
            AfxReacquireObjects(1, &pip->vin);
        }

        if (err && AfxDeallocateInstanceData(pip, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }
    AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
    return err;
}

_AVX afxClassConfig const _AVX_PIP_CLASS_CONFIG =
{
    .fcc = afxFcc_PIP,
    .name = "Pipeline",
    .desc = "Draw Pipeline State",
    .fixedSiz = sizeof(AFX_OBJECT(avxPipeline)),
    .ctor = (void*)_AvxPipStdCtorCb,
    .dtor = (void*)_AvxPipStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssembleComputePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const blueprints[], avxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = (afxClass*)_AvxGetPipelineClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)blueprints, (void*)blueprints }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxAssemblePipelines(afxDrawSystem dsys, afxUnit cnt, avxPipelineBlueprint const cfg[], avxPipeline pipelines[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(pipelines);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxGetPipelineClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_PIP);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)pipelines, (void const*[]) { dsys, (void*)cfg, (void*)cfg }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxLoadPipeline(afxDrawSystem dsys, avxVertexDecl vin, afxUri const* uri, avxPipeline* pipeline)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_TRY_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(uri && !AfxIsUriBlank(uri));

    AfxLogEcho("Uploading pipeline '%.*s'", AfxPushString(&uri->str));

    afxXml xml;
    afxString query;
    afxUri fpath, fext;
    AfxGetUriQueryString(uri, TRUE, &query);
    AfxClipUriExtension(&fext, uri, FALSE);
    AfxClipUriPath(&fpath, uri);

    if (AfxIsUriBlank(&fext)) AfxThrowError();
    else if (0 != AfxCompareString(AfxGetUriString(&fext), 0, ".xml", 4, TRUE))
    {
        AfxLogError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
        AfxThrowError();
    }
    else if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
    else
    {
        afxUnit xmlElemIdx;
        if (!AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Pipeline"), &AfxStaticString("id"), 1, &query, &xmlElemIdx)) AfxThrowError();
        {
            AFX_ASSERT(xmlElemIdx != AFX_INVALID_INDEX);

            avxPipelineBlueprint defConfig = { 0 };
            defConfig.depthCompareOp = avxCompareOp_LESS;
            defConfig.fillMode = avxFillMode_SOLID;
            avxPipelineBlueprint info = defConfig;
            afxUri shdUris[16];
            avxShaderStage shdStages[16];
            afxString shdFns[16];
            
            if (_AvxParseXmlPipelineBlueprint(&xml, xmlElemIdx, 0, &info, shdStages, shdUris, shdFns)) AfxThrowError();
            else
            {
                info.vin = vin;

                avxPipeline pip = NIL;

                if (AfxAssemblePipelines(dsys, 1, &info, &pip)) AfxThrowError();
                else
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAZR, 1, &pip);
                    AFX_ASSERT(pipeline);
                    *pipeline = pip;

                    for (afxUnit i = 0; i < info.stageCnt; i++)
                    {
                        AfxUplinkPipelineFunction(pip, shdStages[i], &shdUris[i], &shdFns[i], NIL, NIL);
                    }
                }
            }
        }
        AfxCleanUpXml(&xml);
    }
    return err;
}
