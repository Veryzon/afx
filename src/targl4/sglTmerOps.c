/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#define _SGL_DBG_IGNORE_DEPTH_RANGE
#define _SGL_DBG_IGNORE_PRIM_RESTART
#define _SGL_DBG_IGNORE_DEPTH_CLAMP
#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

_SGL void _DpuSwitchFrontFace(sglDpu* dpu, _sglCmdBool const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.cwFrontFacing = cmd->value;
}

_SGL void _DpuSetCullMode(sglDpu* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.cullMode = cmd->value;
}

_SGL void _DpuSetPrimitiveTopology(sglDpu* dpu, _sglCmdNat const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextXformState.primTop = cmd->value;
}

_SGL void _DpuSetViewports(sglDpu* dpu, _sglCmdViewport const *cmd)
{
    afxError err = AFX_ERR_NONE;
    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VIEWPORTS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxViewport const* pvp = i ? &cmd->vp_[i] : &cmd->vp0;
        dpu->nextXformState.vps[first + i] = *pvp;
        dpu->nextViewportUpdMask |= AFX_BIT_OFFSET(first + i);
    }

    dpu->nextViewportUpdCnt = AfxMax(dpu->nextViewportUpdCnt, cnt);

    if (cmd->reset)
        dpu->nextXformState.vpCnt = cnt;
}

_SGL void _DpuBindVertexSources(sglDpu* dpu, _sglCmdVertexSources const *cmd)
{
    /*
        The values taken from elements i of pBuffers and pOffsets replace the current state for the vertex input binding firstBinding + i, for i in[0, bindingCount).
        The vertex input binding is updated to start at the offset indicated by pOffsets[i] from the start of the buffer pBuffers[i].
        If pSizes is not NULL then pSizes[i] specifies the bound size of the vertex buffer starting from the corresponding elements of pBuffers[i] plus pOffsets[i].
        All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
        If the nullDescriptor feature is enabled, elements of pBuffers can be VK_NULL_HANDLE, and can be used by the vertex shader.
        If a vertex input attribute is bound to a vertex input binding that is VK_NULL_HANDLE, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with(0, 0, 1).

        This command also dynamically sets the byte strides between consecutive elements within buffer pBuffers[i] to the corresponding pStrides[i] value when drawing using shader objects, or when the graphics pipeline is created with VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE set in VkPipelineDynamicStateCreateInfo::pDynamicStates.Otherwise, strides are specified by the VkVertexInputBindingDescription::stride values used to create the currently active pipeline.

        If drawing using shader objects or if the bound pipeline state object was also created with the VK_DYNAMIC_STATE_VERTEX_INPUT_EXT dynamic state enabled then vkCmdSetVertexInputEXT can be used instead of vkCmdBindVertexBuffers2 to set the stride.
    */

    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat first = cmd->first;
    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, first, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxBuffer buf;
        afxNat32 offset;
        afxNat32 range;
        afxNat32 stride;

        if (!i)
        {
            buf = cmd->source0.buf;
            offset = cmd->source0.offset;
            range = cmd->source0.range;
            stride = cmd->source0.stride;
        }
        else
        {
            buf = cmd->source_[i].buf;
            offset = cmd->source_[i].offset;
            range = cmd->source_[i].range;
            stride = cmd->source_[i].stride;
        }
        AfxAssert(!buf || stride);

        afxNat bindingIdx = first + i;
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, bindingIdx, 1);

        dpu->nextVinBindings.sources[bindingIdx].buf = buf;
        dpu->nextVinBindings.sources[bindingIdx].offset = offset;
        dpu->nextVinBindings.sources[bindingIdx].range = range;
        dpu->nextVinBindings.sources[bindingIdx].stride = stride;
        dpu->nextVinBindingsMask |= AFX_BIT_OFFSET(bindingIdx);
    }
    dpu->nextVinBindingsCnt = AfxMax(dpu->nextVinBindingsCnt, cnt);
}

#if 0
_SGL void _DpuResetVertexStreams(sglDpu* dpu, _sglCmdVertexStreams const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, 0, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        //afxNat streamIdx = i;

        afxNat srcIdx = cmd->srcIdx[i];
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIB_BINDINGS, srcIdx, 1);
        afxNat stride = cmd->stride[i];
        afxBool instance = cmd->instance[i];

        dpu->nextVertexInput.streams[srcIdx].srcIdx = srcIdx;
        dpu->nextVertexInput.streams[srcIdx].stride = stride;
        dpu->nextVertexInput.streams[srcIdx].instance = instance;
        dpu->nextVtxInStreamUpdMask |= AFX_BIT_OFFSET(srcIdx);
    }
    dpu->nextVtxInStreamUpdCnt = AfxMax(dpu->nextVtxInStreamUpdCnt, cnt);
}

_SGL void _DpuResetVertexAttributes(sglDpu* dpu, _sglCmdVertexAttributes const *cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    afxNat cnt = cmd->cnt;
    AfxAssertRange(SGL_MAX_VERTEX_ATTRIBS, 0, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat location = cmd->location[i];
        afxVertexFormat fmt = cmd->fmt[i];
        afxNat srcIdx = cmd->srcIdx[i];
        afxNat32 offset = cmd->offset[i];

        afxNat attrIdx = location;
        AfxAssertRange(SGL_MAX_VERTEX_ATTRIBS, attrIdx, 1);
        dpu->nextVertexInput.attrs[attrIdx].location = location;
        dpu->nextVertexInput.attrs[attrIdx].fmt = fmt;
        dpu->nextVertexInput.attrs[attrIdx].srcIdx = srcIdx;
        dpu->nextVertexInput.attrs[attrIdx].offset = offset;
        dpu->nextVtxInAttribUpdMask |= AFX_BIT_OFFSET(attrIdx);
    }
    dpu->nextVtxInAttribUpdCnt = AfxMax(dpu->nextVtxInAttribUpdCnt, cnt);
}
#endif

_SGL void _DpuBindIndexSource(sglDpu* dpu, _sglCmdBufferRange const *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextVinBindings.idxSrcBuf = cmd->buf;
    dpu->nextVinBindings.idxSrcOff = cmd->offset;
    dpu->nextVinBindings.idxSrcRange = cmd->range;
    dpu->nextVinBindings.idxSrcSiz = cmd->stride;
    dpu->nextVinBindings.iboUpdReq = TRUE;
}

_SGL void _DpuBindVertexInput(sglDpu* dpu, _sglCmdVertexInput *cmd)
{
    afxError err = AFX_ERR_NONE;
    dpu->nextVin = cmd->vin;
    afxVertexInput vin = cmd->vin;

    if (vin)
    {

    }
}

_SGL void SglFlushXformStateChanges(sglDpu* dpu)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    dpu->activeXformState.primTop = dpu->nextXformState.primTop;

    afxCullMode cullMode = dpu->nextXformState.cullMode;

    if (dpu->activeXformState.cullMode != cullMode)
    {
        /*
            GL_CULL_FACE
            If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
        */

        if (cullMode)
        {
            AfxAssert(!dpu->activeXformState.cullMode);
            gl->Enable(GL_CULL_FACE); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.cullMode);
            gl->Disable(GL_CULL_FACE); _SglThrowErrorOccuried();
        }

        if (cullMode)
        {
            /*
                glCullFace � specify whether front- or back-facing facets can be culled
                void glCullFace(GLenum mode);

                mode Specifies whether front- or back-facing facets are candidates for culling. Symbolic constants GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK are accepted. The initial value is GL_BACK.

                glCullFace specifies whether front- or back-facing facets are culled (as specified by mode) when facet culling is enabled.
                Facet culling is initially disabled. To enable and disable facet culling, call the glEnable and glDisable commands with the argument GL_CULL_FACE.
                Facets include triangles, quadrilaterals, polygons, and rectangles.

                glFrontFace specifies which of the clockwise and counterclockwise facets are front-facing and back-facing. See glFrontFace.

                If mode is GL_FRONT_AND_BACK, no facets are drawn, but other primitives such as points and lines are drawn.
            */
            gl->CullFace(AfxToGlCullMode(cullMode)); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.cullMode = cullMode;
    }

    if (dpu->activeXformState.cullMode)
    {
        afxBool cwFrontFacing = dpu->nextXformState.cwFrontFacing;

        if (dpu->activeXformState.cwFrontFacing != cwFrontFacing)
        {
            /*
                glFrontFace � define front- and back-facing polygons
                void glFrontFace(GLenum mode);

                mode Specifies the orientation of front-facing polygons. GL_CW and GL_CCW are accepted. The initial value is GL_CCW.

                In a scene composed entirely of opaque closed surfaces, back-facing polygons are never visible.
                Eliminating these invisible polygons has the obvious benefit of speeding up the rendering of the image.
                To enable and disable elimination of back-facing polygons, call glEnable and glDisable with argument GL_CULL_FACE.

                The projection of a polygon to window coordinates is said to have clockwise winding if an imaginary object following the path from its first vertex, its second vertex, and so on, to its last vertex, and finally back to its first vertex, moves in a clockwise direction about the interior of the polygon.
                The polygon's winding is said to be counterclockwise if the imaginary object following the same path moves in a counterclockwise direction about the interior of the polygon.
                glFrontFace specifies whether polygons with clockwise winding in window coordinates, or counterclockwise winding in window coordinates, are taken to be front-facing.
                Passing GL_CCW to mode selects counterclockwise polygons as front-facing; GL_CW selects clockwise polygons as front-facing.
                By default, counterclockwise polygons are taken to be front-facing.
            */
            gl->FrontFace(cwFrontFacing ? GL_CW : GL_CCW); _SglThrowErrorOccuried();
            dpu->activeXformState.cwFrontFacing = cwFrontFacing;
        }
    }

    if (dpu->nextViewportUpdMask)
    {
        afxNat cnt = dpu->nextViewportUpdCnt;
        AfxAssert(cnt);

        if (cnt)
        {
            afxMask updMask = dpu->nextViewportUpdMask;

            for (afxNat i = 0; i < SGL_MAX_VIEWPORTS; i++)
            {
                if (AfxTestBitEnabled(updMask, i))
                {
                    if (gl->ViewportArrayv)
                    {
                        GLfloat v[SGL_MAX_VIEWPORTS][4];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);

                        v[0][0] = dpu->nextXformState.vps[i].offset[0],
                            v[0][1] = dpu->nextXformState.vps[i].offset[1],
                            v[0][2] = dpu->nextXformState.vps[i].extent[0],
                            v[0][3] = dpu->nextXformState.vps[i].extent[1];

                        gl->ViewportArrayv(i, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLint x = (GLint)(dpu->nextXformState.vps[0].offset[0]);
                        GLint y = (GLint)(dpu->nextXformState.vps[0].offset[1]);
                        GLsizei w = (GLsizei)(dpu->nextXformState.vps[0].extent[0]);
                        GLsizei h = (GLsizei)(dpu->nextXformState.vps[0].extent[1]);
                        AfxAssert(i == 0);
                        gl->Viewport(x, y, w, h); _SglThrowErrorOccuried();
                    }
#ifndef _SGL_DBG_IGNORE_DEPTH_RANGE
                    if (gl->DepthRangeArrayv)
                    {
                        GLdouble v[SGL_MAX_VIEWPORTS][2];
                        AfxAssert(SGL_MAX_VIEWPORTS >= cnt);

                        v[0][0] = dpu->nextXformState.vps[i].depth[0],
                            v[0][1] = dpu->nextXformState.vps[i].depth[1];

                        gl->DepthRangeArrayv(0, 1, &v[0][0]); _SglThrowErrorOccuried();
                    }
                    else
                    {
                        GLdouble n = dpu->nextXformState.vps[0].depth[0];
                        GLdouble f = dpu->nextXformState.vps[0].depth[1];

                        AfxAssert(i == 0);
                        gl->DepthRange(n, f); _SglThrowErrorOccuried();
                    }
#endif
                    dpu->activeXformState.vps[i] = dpu->nextXformState.vps[i];
                }
            }
        }
        dpu->nextViewportUpdMask = NIL;
    }


#ifndef _SGL_DBG_IGNORE_PRIM_RESTART

    afxBool primRestartEnabled = dpu->nextXformState.primTop;

    if (dpu->activeXformState.primRestartEnabled != primRestartEnabled)
    {
        /*
            GL_PRIMITIVE_RESTART
            Enables primitive restarting.
            If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index.
            See glPrimitiveRestartIndex.
        */

        if (primRestartEnabled)
        {
            AfxAssert(!dpu->activeXformState.primRestartEnabled);
            gl->Enable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();

            /*
                glPrimitiveRestartIndex � specify the primitive restart index
                void glPrimitiveRestartIndex(GLuint index);
                index Specifies the value to be interpreted as the primitive restart index.

                glPrimitiveRestartIndex specifies a vertex array element that is treated specially when primitive restarting is enabled. This is known as the primitive restart index.

                When one of the Draw* commands transfers a set of generic attribute array elements to the GL, if the index within the vertex arrays corresponding to that set is equal to the primitive restart index, then the GL does not process those elements as a vertex.
                Instead, it is as if the drawing command ended with the immediately preceding transfer, and another drawing command is immediately started with the same parameters, but only transferring the immediately following element through the end of the originally specified elements.

                When either glDrawElementsBaseVertex, glDrawElementsInstancedBaseVertex or glMultiDrawElementsBaseVertex is used, the primitive restart comparison occurs before the basevertex offset is added to the array index.
            */

            //gl->PrimitiveRestartIndex(); _SglThrowErrorOccuried();

            /*
                GL_PRIMITIVE_RESTART_FIXED_INDEX
                Enables primitive restarting with a fixed index.
                If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the fixed primitive index for the specified index type.
                The fixed index is equal to 2^n - 1
                where n is equal to 8 for GL_UNSIGNED_BYTE, 16 for GL_UNSIGNED_SHORT and 32 for GL_UNSIGNED_INT.
            */

            //gl->Enable(GL_PRIMITIVE_RESTART_FIXED_INDEX); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.primRestartEnabled);
            gl->Disable(GL_PRIMITIVE_RESTART); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.primRestartEnabled = primRestartEnabled;
    }
#endif

#ifndef _SGL_DBG_IGNORE_DEPTH_CLAMP

    afxBool depthClampEnabled = dpu->nextXformState.depthClampEnabled;

    if (dpu->activeXformState.depthClampEnabled != depthClampEnabled)
    {
        /*
            GL_DEPTH_CLAMP
            If enabled, the -wc =< zc =< wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange.
        */

        if (depthClampEnabled)
        {
            AfxAssert(!dpu->activeXformState.depthClampEnabled);
            gl->Enable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }
        else
        {
            AfxAssert(dpu->activeXformState.depthClampEnabled);
            gl->Disable(GL_DEPTH_CLAMP); _SglThrowErrorOccuried();
        }
        dpu->activeXformState.depthClampEnabled = depthClampEnabled;
    }
#endif
}

_SGL afxCmdId _SglEncodeCmdBindVertexInput(afxDrawStream diob, afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);
    
    _sglCmdVertexInput *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->vin = vin;
    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.BindVertexInput) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdBindVertexSources(afxDrawStream diob, afxNat baseSlot, afxNat slotCnt, afxBuffer buf[], afxNat32 const offset[], afxNat32 const range[], afxNat32 const stride[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    afxNat extraVisCnt = slotCnt;

    if (extraVisCnt)
        --extraVisCnt;

    _sglCmdVertexSources *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (extraVisCnt * sizeof(cmd->source_[0])));
    AfxAssert(cmd);
    cmd->first = baseSlot;
    cmd->cnt = slotCnt;

    cmd->source0.buf = buf ? buf[0] : NIL;
    cmd->source0.range = range ? range[0] : 0;
    cmd->source0.offset = offset ? offset[0] : 0;
    cmd->source0.stride = stride ? stride[0] : 0;

    for (afxNat i = 0; i < extraVisCnt; i++)
    {
        if ((cmd->source_[i].buf = buf ? buf[1 + i] : NIL))
        {
            AfxAssertObjects(1, &cmd->source_[i].buf, afxFcc_BUF);
        }

        cmd->source_[i].offset = offset ? offset[1 + i] : 0;
        cmd->source_[i].range = range ? range[1 + i] : 0;
        cmd->source_[i].stride = stride ? stride[1 + i] : 0;
        //cmd->spec[i].instance = FALSE;
        //cmd->spec[i].instDivisor = 1;

        if (cmd->source_[i].buf)
        {
            AfxAssertRange(AfxGetBufferCapacity(cmd->source_[i].buf), cmd->source_[i].offset, cmd->source_[i].range);
        }
    }

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.BindVertexSources) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

#if 0
_SGL afxCmdId _SglEncodeCmdResetVertexStreams(afxDrawStream diob, afxNat cnt, afxNat const srcIdx[], afxNat32 const stride[], afxBool const instance[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdVertexStreams *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->stride[i] = stride ? stride[i] : 0;
        cmd->instance[i] = instance ? instance[i] : FALSE;
    }

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.ResetVertexStreams) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetVertexAttributes(afxDrawStream diob, afxNat cnt, afxNat const location[], afxVertexFormat const fmt[], afxNat const srcIdx[], afxNat32 const offset[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdVertexAttributes *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->cnt = cnt;

    for (afxNat i = 0; i < cnt; i++)
    {
        cmd->location[i] = location ? location[i] : 0;
        cmd->fmt[i] = fmt ? fmt[i] : NIL;
        AfxAssert(cmd->fmt[i]);
        cmd->srcIdx[i] = srcIdx ? srcIdx[i] : 0;
        cmd->offset[i] = offset ? offset[i] : 0;
    }

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.ResetVertexAttributes) / sizeof(void*), sizeof(cmd), &cmd->cmd);
}
#endif

_SGL afxCmdId _SglEncodeCmdBindIndexSource(afxDrawStream diob, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdBufferRange *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);

    cmd->offset = offset;
    cmd->range = range;
    cmd->stride = idxSiz;

    if ((cmd->buf = buf))
    {
        AfxAssertObjects(1, &cmd->buf, afxFcc_BUF);
        AfxAssertRange(AfxGetBufferCapacity(cmd->buf), cmd->offset, cmd->range);
    }

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.BindIndexSource) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

// Fixed-function vertex post-processing

_SGL afxCmdId _SglEncodeCmdSetPrimitiveTopology(afxDrawStream diob, afxPrimTopology topology)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = topology;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.SetPrimitiveTopology) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSwitchFrontFace(afxDrawStream diob, afxBool cw)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdBool *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = cw;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.SwitchFrontFace) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdSetCullMode(afxDrawStream diob, afxCullMode mode)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdNat *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->value = mode;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.SetCullMode) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

// Viewports, synthesis areas and scissors.

_SGL afxCmdId _SglEncodeCmdReadjustViewports(afxDrawStream diob, afxNat32 first, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    afxNat extraVpCnt = cnt;

    if (extraVpCnt)
        --extraVpCnt;

    _sglCmdViewport *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (extraVpCnt * sizeof(cmd->vp_[0])));
    AfxAssert(cmd);
    cmd->first = first;
    cmd->cnt = cnt;
    cmd->reset = FALSE;
    cmd->vp0 = vp[0];

    for (afxNat i = 0; i < extraVpCnt; i++)
        cmd->vp_[i] = vp[1 + i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.ReadjustViewports) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdId _SglEncodeCmdResetViewports(afxDrawStream diob, afxNat32 cnt, afxViewport const vp[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    afxNat extraVpCnt = cnt;

    if (extraVpCnt)
        --extraVpCnt;

    _sglCmdViewport *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (extraVpCnt * sizeof(cmd->vp_[0])));
    AfxAssert(cmd);
    cmd->first = 0;
    cmd->cnt = cnt;
    cmd->reset = TRUE;
    cmd->vp0 = vp[0];

    for (afxNat i = 0; i < extraVpCnt; i++)
        cmd->vp_[i] = vp[1 + i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, Transformation.ResetViewports) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdTransformation const _SglEncodeCmdTransformationVmt =
{
    .BindVertexInput = _SglEncodeCmdBindVertexInput,

    .BindVertexSources = _SglEncodeCmdBindVertexSources,
    //.ResetVertexStreams = _SglEncodeCmdResetVertexStreams,
    //.ResetVertexAttributes = _SglEncodeCmdResetVertexAttributes,
    .BindIndexSource = _SglEncodeCmdBindIndexSource,
    .SetPrimitiveTopology = _SglEncodeCmdSetPrimitiveTopology,

    .ResetViewports = _SglEncodeCmdResetViewports,
    .ReadjustViewports = _SglEncodeCmdReadjustViewports,

    .SwitchFrontFace = _SglEncodeCmdSwitchFrontFace,
    .SetCullMode = _SglEncodeCmdSetCullMode
};