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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "qwadro/core/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/afxPipeline.h"
#include "qwadro/draw/afxXsh.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/core/afxUri.h"
#include "qwadro/core/afxSystem.h"
// OpenGL/Vulkan Continuous Integration

#define _AFX_DBG_IGNORE_PRIM_RESTART
#define _AFX_DBG_IGNORE_DEPTH_CLAMP

////////////////////////////////////////////////////////////////////////////////
// DRAW PIPELINE                                                              //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL afxResult _AfxRegisterOpenGlResourcesToQwadroDrawPipeline(afxPipeline pip)
{
    glVmt* gl = &(((afxDrawSystem)AfxGetObjectProvider(&pip->obj))->vmt);
#if 0
    afxShaderStage                      stages;
    afxShaderResourceType              type;
    afxPipelineSubresType           baseType;
    afxNat32                            set,
        binding,
        loc,
        inputAttachIdx,
        vecSiz,
        columns,
        arrSiz,
        off,
        siz;
    afxEmbeddedString(name, 16);
    afxPipelineSubresource const    *subresources;
#endif
    _DrawPipelineResource res, *resptr;
    GLuint gpuHandle = pip->gpuHandle;

    afxInt cnt = 0;
    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORM_BLOCKS, &cnt); _SglThrowErrorOccuried();

    for (afxInt i = 0; i < cnt; i++)
    {
        AFX_ZERO(&res);

        GLint binding, dataSiz, nameLen, subresCnt, subresIndices[1], refByVsh, refByFsh, refByGsh;
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_BINDING, &binding);  _SglThrowErrorOccuried();// The current block binding, as set either within the shader or from gl->UniformBlockBinding.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSiz);  _SglThrowErrorOccuried();// The buffer object storage size needed for this block.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);  _SglThrowErrorOccuried();// The length of this block's name.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &subresCnt);  _SglThrowErrorOccuried();// The number of active uniforms within this block.
        //glGetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &subresIndices); // params​ will be filled in with the uniform indices of all uniforms that are stored in this block.It will receive GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS number of uniforms.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, &refByVsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, &refByFsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.
        gl->GetActiveUniformBlockiv(gpuHandle, i, GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER, &refByGsh);  _SglThrowErrorOccuried();// GL_FALSE if the uniform block is not referenced by an active * shader, where * is the particular shader stage in question. It canv be VERTEX_SHADER, FRAGMENT_SHADER, or GEOMETRY_SHADER.


        res.binding = binding;
        res.resType = AFX_SHD_RES_TYPE_CONSTANT_BUFFER;
        res.set = 0;
        res.location = i;
        res.arraySize = 0;
        res.vecSize = 0;
        res.inputAttachmentIndex = 0;
        res.size = dataSiz;
        res.stages = 0;
        gl->GetActiveUniformBlockName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
        res.name[15] = '\0';
        //res.loc = gl->GetUniformLocation(gpuHandle, res.nameBuf); _SglThrowErrorOccuried();

        afxResult j = AfxPipeline.RegisterResource(pip, &res);
        AfxPipeline.GetResource(pip, j, &res);
        AfxAssert(i == j);
        AfxEcho("%d %d %d %d", res.loc, res.binding, i, j);
    }

    gl->GetProgramiv(gpuHandle, GL_ACTIVE_UNIFORMS, &cnt);

    for (GLuint i = 0; i < (GLuint)cnt; i++)
    {
        AFX_ZERO(&res);
        AfxDeployString(&res.name, res.nameBuf);

        afxBool valid = FALSE;

        GLint type, blockIdx;
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_TYPE, &type); _SglThrowErrorOccuried();
        gl->GetActiveUniformsiv(gpuHandle, 1, &i, GL_UNIFORM_BLOCK_INDEX, &blockIdx); _SglThrowErrorOccuried();

        if (blockIdx == -1)
        {
            switch (type)
            {
            case GL_SAMPLER_1D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_2D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_3D:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            case GL_SAMPLER_CUBE:
                gl->GetActiveUniformName(gpuHandle, i, sizeof(res.name), NIL, res.name); _SglThrowErrorOccuried();
                res.resType = AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER;
                valid = TRUE;
                break;
            default:
                AfxError("%X is a unhandled type", type);
                valid = FALSE;
                break;
            }

            if (valid)
            {
                res.location = gl->GetUniformLocation(gpuHandle, res.name); _SglThrowErrorOccuried();
                //res.loc = i;
                afxSupplyDescriptor entry[1];
                afxResult j = AfxPipeline.FindEntries(pip, 1, &res.name, entry);
                //AfxPipeline.GetResource(pip, j, &res);

                if (j == 1)
                {
                    AfxAssert(i == j);
                    gl->BindUniform(gpuHandle, );
                    AfxEcho("%d %d %d %d", res.location, res.binding, i, j);
                }
            }
        }
    }
    return 0;
}
#endif

_SGL afxError _SglLoadShaderBlueprint(afxShaderBlueprint* shdb, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    afxUri fext;
    AfxGetUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxGetUriPath(&fpath, uri);

        if (0 == AfxCompareStringLiteralCi(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxString query;
                AfxGetUriQueryString(uri, TRUE, &query);

                afxNat xmlElemIdx = 0;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Shader"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    AfxShaderBlueprintBegin(shdb, NIL, NIL, NIL, 0, 0, 0);

                    if (AfxParseXmlBackedShaderBlueprint(shdb, 0, &xml, xmlElemIdx)) AfxThrowError();
                    else
                    {

                    }
                    //AfxShaderBlueprintEnd(shdb, 0, NIL);
                }
                AfxReleaseXml(&xml);
            }
        }
    }
    return err;
}

_SGL afxError _SglDpuBindAndSyncPip(sglDpuIdd* dpu, afxBool syncOnly, afxPipeline pip)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (!pip)
    {
        gl->BindProgramPipeline(0); _SglThrowErrorOccuried();
        //gl->UseProgram(0); _SglThrowErrorOccuried();
        //gl->BindVertexArray(0); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &pip, afxFcc_PIP);
        GLuint glHandle = pip->glHandle;
        GLuint glHandleXformStages = pip->glHandleXformStages;
        sglUpdateFlags devUpdReq = (pip->updFlags & SGL_UPD_FLAG_DEVICE);

        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteProgramPipelines(1, &glHandle); _SglThrowErrorOccuried();

                if (glHandleXformStages)
                {
                    gl->DeleteProgram(glHandleXformStages); _SglThrowErrorOccuried();
                }
                glHandle = NIL;
                glHandleXformStages = NIL;
            }

            //afxArray code;
            afxChar errStr[1024];
            GLuint tmpShdGlHandles[4];
            //AfxAllocateArray(&code, 2048, sizeof(afxChar), NIL);

            for (afxNat stageIdx = 0; stageIdx < pip->base.stageCnt; stageIdx++)
            {
                //AfxEmptyArray(&code);
                //AfxLoadGlScript(&code, &pip->base.stages[stageIdx].shd.uri);
                afxShaderBlueprint shdb;
                _SglLoadShaderBlueprint(&shdb, &pip->base.stages[stageIdx].shd.uri);
                
                GLuint shader;

                if (!(shader = gl->CreateShader(AfxToGlShaderStage(pip->base.stages[stageIdx].stage))))
                {
                    _SglThrowErrorOccuried();
                }
                else
                {
                    GLint compiled = 0;
                    //gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)code.bytemap }, (GLint const[]) { code.cnt }); _SglThrowErrorOccuried();
                    gl->ShaderSource(shader, 1, (GLchar const*const[]) { (void*)shdb.codes.bytemap }, (GLint const[]) { shdb.codes.cnt }); _SglThrowErrorOccuried();
                    gl->CompileShader(shader); _SglThrowErrorOccuried();
                    gl->GetShaderiv(shader, GL_COMPILE_STATUS, &compiled); _SglThrowErrorOccuried();

                    if (compiled == GL_FALSE)
                    {
                        AfxThrowError();
                        gl->GetShaderInfoLog(shader, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                        AfxError(errStr);
                    }
                    else
                    {
                        tmpShdGlHandles[stageIdx] = shader;
                    }
                    //gl->DeleteShader(shader); _SglThrowErrorOccuried();
                }

                AfxShaderBlueprintEnd(&shdb, 0, NIL);

                if (err)
                {
                    for (afxNat i = stageIdx; i-- > 0;)
                    {
                        gl->DeleteShader(tmpShdGlHandles[i]); _SglThrowErrorOccuried();
                        tmpShdGlHandles[i] = NIL;
                    }
                    break;
                }
            }

            //AfxDeallocateArray(&code);

            if (!err)
            {
                if (!(glHandleXformStages = gl->CreateProgram()))
                {
                    _SglThrowErrorOccuried();
                }
                else
                {
                    afxBool linked;
                    gl->ProgramParameteri(glHandleXformStages, GL_PROGRAM_SEPARABLE, GL_TRUE); _SglThrowErrorOccuried();

                    for (afxNat stageIdx = 0; stageIdx < pip->base.stageCnt; stageIdx++)
                    {
                        gl->AttachShader(glHandleXformStages, tmpShdGlHandles[stageIdx]); _SglThrowErrorOccuried();
                    }

                    gl->LinkProgram(glHandleXformStages); _SglThrowErrorOccuried();
                    gl->GetProgramiv(glHandleXformStages, GL_LINK_STATUS, &linked); _SglThrowErrorOccuried();

                    for (afxNat stageIdx = pip->base.stageCnt; stageIdx-- > 0;)
                    {
                        gl->DetachShader(glHandleXformStages, tmpShdGlHandles[stageIdx]); _SglThrowErrorOccuried();
                    }

                    if (linked == GL_FALSE)
                    {
                        AfxThrowError();
                        gl->GetProgramInfoLog(glHandleXformStages, sizeof(errStr), NIL, (GLchar*)errStr); _SglThrowErrorOccuried();
                        AfxError(errStr);
                    }

                    if (!err)
                    {
                        AfxAssert(NIL == glHandle);
                        gl->GenProgramPipelines(1, &glHandle); _SglThrowErrorOccuried();
                        gl->BindProgramPipeline(glHandle); _SglThrowErrorOccuried();
                        GLbitfield xformStages = NIL;

                        for (afxNat stageIdx = 0; stageIdx < pip->base.stageCnt; stageIdx++)
                            xformStages |= AfxToGlShaderStageBit(pip->base.stages[stageIdx].stage);

                        gl->UseProgramStages(glHandle, xformStages, glHandleXformStages); _SglThrowErrorOccuried();
                        
                        for (afxNat i = 0; i < pip->base.wiringCnt; i++)
                            if (_SglDpuBindAndResolveLego(dpu, glHandleXformStages, pip->base.wiring[i].set, pip->base.wiring[i].legt, gl))
                                AfxThrowError();

                        if (pip->base.razr)
                        {
                            if (_SglDpuBindAndSyncRazr(dpu, pip->base.razr))
                                AfxThrowError();

                            gl->UseProgramStages(glHandle, GL_FRAGMENT_SHADER_BIT, pip->base.razr->glHandleFragShdProg); _SglThrowErrorOccuried();

                            for (afxNat i = 0; i < pip->base.wiringCnt; i++)
                                if (_SglDpuBindAndResolveLego(dpu, pip->base.razr->glHandleFragShdProg, pip->base.wiring[i].set, pip->base.wiring[i].legt, gl))
                                    AfxThrowError();
                        }

                        gl->ValidateProgramPipeline(glHandle);

                        if (syncOnly)
                        {
                            gl->BindProgramPipeline(0); _SglThrowErrorOccuried();
                        }

                        if (err)
                        {
                            gl->DeleteProgramPipelines(1, &glHandle); _SglThrowErrorOccuried();
                            glHandle = NIL;
                        }
                    }

                    if (err)
                    {
                        gl->DeleteProgram(glHandleXformStages); _SglThrowErrorOccuried();
                        glHandleXformStages = NIL;
                    }
                }

                for (afxNat stageIdx = pip->base.stageCnt; stageIdx-- > 0;)
                {
                    gl->DeleteShader(tmpShdGlHandles[stageIdx]); _SglThrowErrorOccuried();
                    tmpShdGlHandles[stageIdx] = NIL;
                }
            }

            pip->glHandle = glHandle;
            pip->glHandleXformStages = glHandleXformStages;

            if (!err)
            {
                AfxEcho("afxPipeline %p hardware-side data instanced.", pip);
                pip->updFlags &= ~(SGL_UPD_FLAG_DEVICE);

#if 0
                GLuint vao;
                gl->GenVertexArrays(1, &vao); _SglThrowErrorOccuried();
                gl->BindVertexArray(vao); _SglThrowErrorOccuried();

                afxShader vsh;

                if (AfxFindLinkedShader(pip, afxShaderStage_VERTEX, &vsh))
                {
                    afxNat attrCnt = AfxCountPipelineInputs(pip);

                    afxNat32 offset[SGL_MAX_VERTEX_ATTRIB_BINDINGS] = { 0 };

                    for (afxNat i = 0; i < attrCnt; i++)
                    {
                        afxPipelineInputLocation in;
                        AfxGetPipelineInputs(pip, i, 1, &in);

                        afxNat location = in.location;
                        afxNat srcIdx = in.stream;

                        GLint glsiz;
                        GLenum gltype;
                        GLuint glStride;
                        AfxToGlVertexFormat(in.fmt, &glsiz, &gltype, &glStride);

                        gl->BindAttribLocation(glHandle, pip->base.ins[i].location, AfxGetStringData(&(vsh->base.ioDecls[i].semantic), 0)); _SglThrowErrorOccuried();

                        AfxAssert(16 > location);  // max vertex attrib
                        gl->EnableVertexAttribArray(location); _SglThrowErrorOccuried();
                        AfxAssert(gl->BindVertexBuffer);
                        gl->VertexAttribFormat(location, glsiz, gltype, FALSE, offset[srcIdx]); _SglThrowErrorOccuried();
                        //afxNat srcIdx = streamIdx;// dpu->state.vertexInput.streams[streamIdx].srcIdx;
                        //AfxAssertRange(_SGL_MAX_VBO_PER_BIND, srcIdx, 1);
                        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, srcIdx, 1);
                        gl->VertexAttribBinding(location, srcIdx); _SglThrowErrorOccuried();

                        offset[srcIdx] += AfxVertexFormatGetSize(in.fmt);

                        // TODO mover VAO para Pipeline. A indireção de stream/source supostamente permite
                    }
                    pip->vertexInput.attrCnt = attrCnt;
                }
                pip->vertexInput.vao = vao;
#else

#endif

            }
        }
        else
        {
            if (!syncOnly)
            {
                AfxAssert(gl->IsProgramPipeline(glHandle));
                gl->BindProgramPipeline(glHandle); _SglThrowErrorOccuried();
            }
        }
    }
    return err;
}

_SGL afxError _SglPipDtor(afxPipeline pip)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = AfxGetObjectProvider(pip);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssert(pip->base.stages);
    AfxDeallocate(mmu, pip->base.stages);

    if (pip->base.wiring)
    {
        for (afxNat i = 0; i < pip->base.wiringCnt; i++)
            AfxReleaseObjects(1, (void*[]) { pip->base.wiring[i].legt });

        AfxDeallocate(mmu, pip->base.wiring);
    }

    if (pip->base.razr)
    {
        AfxReleaseObjects(1, (void*[]) { pip->base.razr });
    }

    if (pip->base.vin)
    {
        AfxReleaseObjects(1, (void*[]) { pip->base.vin });
    }

    if (pip->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 8, pip->glHandle);
        pip->glHandle = 0;
    }

    if (pip->glHandleXformStages)
    {
        _SglDctxDeleteGlRes(dctx, 5, pip->glHandleXformStages);
        pip->glHandle = 0;
    }
    return err;
}

_SGL afxError _SglPipCtor(afxPipeline pip, afxCookie const* cookie)
{
    AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pip, afxFcc_PIP);

    afxDrawContext dctx = cookie->udd[0];
    afxPipelineConfig const *pipb = ((afxPipelineConfig const*)cookie->udd[1]) + cookie->no;
    //AfxAssertType(pipb, afxFcc_PIPB);

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    // GRAPHICS STATE SETTING

    afxPipelinePrimitiveFlags primFlags = pipb->primFlags;
    pip->base.primFlags = NIL;

    //if (primFlags)
    {
        if (primFlags & afxPipelinePrimitiveFlag_TOPOLOGY)
            pip->base.primTop = pipb->primTop;

        if (primFlags & afxPipelinePrimitiveFlag_CTRL_POINTS)
            pip->base.patchControlPoints = pipb->patchControlPoints;

        if (primFlags & afxPipelinePrimitiveFlag_CULL_MODE)
            pip->base.cullMode = pipb->cullMode;
    }

    if ((pip->base.razr = pipb->razr))
    {
        AfxReacquireObjects(1, (void*[]) { pip->base.razr });
    }

    // XFORM STATE SETTING

    pip->base.stages = NIL;
    afxNat stageCnt = pipb->shdCnt;
    AfxAssert(stageCnt);
    pip->base.stageCnt = 0;

    if (!(pip->base.stages = AfxAllocate(mmu, sizeof(pip->base.stages[0]), stageCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        afxShaderBlueprint shdb[5];

        for (afxNat i = 0; i < stageCnt; i++)
        {
            AfxMakeFixedUri128(&pip->base.stages[pip->base.stageCnt].shd, &pipb->shdUri[i]);
            AfxMakeFixedString8(&pip->base.stages[pip->base.stageCnt].fn, &pipb->shdFn[i]);
            pip->base.stages[pip->base.stageCnt].stage = pipb->shdStage[i];

            _SglLoadShaderBlueprint(&shdb[i], &pip->base.stages[pip->base.stageCnt].shd.uri);

            pip->base.stageCnt++;
        }

        if (!err)
        {
            AfxAssert(pip->base.stageCnt == stageCnt);

            _SglLoadShaderBlueprint(&shdb[4], &pip->base.razr->base.fragShd.uri);

            afxPipelineRigBlueprint legb[/*_SGL_MAX_LEGO_PER_BIND*/4];

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintBegin(&legb[i], 1);

                if (AfxLegoBlueprintAddShaderContributions(&legb[i], i, stageCnt, shdb))
                {
                    AfxThrowError();
                }

                if (err)
                {

                    for (afxNat j = 0; j < i; j++)
                    {
                        AfxLegoBlueprintEnd(&legb[i], NIL);
                    }
                    break;
                }
            }

            afxNat setCnt = 0;

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                setCnt += AfxCountArrayElements(&legb[i].bindings) ? 1 : 0;

            pip->base.wiringCnt = 0;

            if (setCnt && !(pip->base.wiring = AfxAllocate(mmu, sizeof(pip->base.wiring[0]), setCnt, 0, AfxHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
                {
                    if (AfxCountArrayElements(&legb[i].bindings))
                    {
                        if (AfxBuildBindSchemas(dctx, 1, &(pip->base.wiring[pip->base.wiringCnt].legt), &legb[i]))
                        {
                            AfxThrowError();
                        }
                        else
                        {
                            AfxAssertObjects(1, &(pip->base.wiring[pip->base.wiringCnt].legt), afxFcc_LEGO);
                            pip->base.wiring[pip->base.wiringCnt].set = i;
                            ++pip->base.wiringCnt;
                        }
                    }

                    if (i >= setCnt)
                        break;
                }
            }

            for (afxNat i = 0; i < /*_SGL_MAX_LEGO_PER_BIND*/4; i++)
            {
                AfxLegoBlueprintEnd(&legb[i], NIL);
            }

            if (!err)
            {
                pip->base.inCnt = 0;

                for (afxNat i = 0; i < stageCnt; i++)
                {
                    if (afxShaderStage_VERTEX == shdb->stage)
                    {
                        for (afxNat j = 0; j < shdb->inOuts.cnt; j++)
                        {
                            afxShaderBlueprintInOut shdio = *(afxShaderBlueprintInOut*)AfxGetArrayUnit(&shdb->inOuts, j);

                            pip->base.ins[pip->base.inCnt].location = shdio.location;
                            //pip->base.ins[pip->base.inCnt].binding = pip->base.ins[pip->base.inCnt].location;
                            pip->base.ins[pip->base.inCnt].fmt = shdio.fmt;
                            //pip->base.ins[pip->base.inCnt].offset = 0;
                            pip->base.ins[pip->base.inCnt].stream = shdio.stream;
                            pip->base.inCnt++;
                        }
                        break;
                    }
                }

                pip->base.vin = NIL;

                afxNat totalAttrCnt = pip->base.inCnt;
                afxNat32 offset[SGL_MAX_VERTEX_ATTRIB_BINDINGS] = { 0 };
                afxVertexInputAttr vinAttrs[16];
                afxNat vinAttrCnt = { 0 };
                afxVertexInputStream vinStreams[16] = {0};
                afxNat vinStreamMap[16] = { 0 };
                afxNat vinStreamCnt = 0;

                for (afxNat i = 0; i < totalAttrCnt; i++)
                {
                    afxNat srcIdx = pip->base.ins[i].stream;
                    afxNat vinStreamIdx = AFX_INVALID_INDEX;

                    for (afxNat j = 0; j < vinStreamCnt; j++)
                    {
                        if (vinStreamMap[j] == srcIdx)
                        {
                            vinStreamIdx = j;
                            break;
                        }
                    }

                    if (vinStreamIdx == AFX_INVALID_INDEX)
                    {
                        vinStreamMap[(vinStreamIdx = vinStreamCnt)] = srcIdx;
                        ++vinStreamCnt;
                    }

                    vinAttrs[vinAttrCnt].location = pip->base.ins[i].location;
                    vinAttrs[vinAttrCnt].streamIdx = vinStreamIdx;
                    vinAttrs[vinAttrCnt].offset = offset[vinStreamIdx];
                    afxNat attrSiz = AfxVertexFormatGetSize(pip->base.ins[i].fmt);
                    offset[vinStreamIdx] += attrSiz;
                    vinStreams[vinStreamIdx].stride += attrSiz;
                    vinAttrs[vinAttrCnt].fmt = pip->base.ins[i].fmt;
                    ++vinAttrCnt;

                    // TODO mover VAO para Pipeline. A indireção de stream/source supostamente permite
                }

                if (totalAttrCnt && !(pip->base.vin = AfxAcquireVertexInput(dctx, vinStreamCnt, vinStreams, vinAttrCnt, vinAttrs)))
                    AfxThrowError();

                if (!err)
                {
                    
                    pip->glHandle = 0;
                    pip->updFlags = SGL_UPD_FLAG_DEVICE_INST;
                }
            }

            for (afxNat i = stageCnt; i-- > 0;)
            {
                
                AfxShaderBlueprintEnd(&shdb[i], 0, NIL);
            }

            if (err && pip->base.wiring)
            {
                for (afxNat i = 0; i < pip->base.wiringCnt; i++)
                    AfxReleaseObjects(1, (void*[]) { pip->base.wiring[i].legt });

                AfxDeallocate(mmu, pip->base.wiring);
            }
        }

        if (err)
        {
            AfxAssert(pip->base.stages);
            AfxDeallocate(mmu, pip->base.stages);
        }
    }
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return err;
}

_SGL afxClassConfig _SglPipClsConfig =
{
    .fcc = afxFcc_PIP,
    .name = "Pipeline",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxPipeline)),
    .mmu = NIL,
    .ctor = (void*)_SglPipCtor,
    .dtor = (void*)_SglPipDtor
};