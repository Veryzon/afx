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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_TRANSFORM_OPS_H
#define AFX_TRANSFORM_OPS_H

#include "qwadro/inc/math/afxViewport.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/draw/afxRaster.h"
#include "qwadro/inc/draw/afxBuffer.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/io/afxTypography.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"

AFX_DEFINE_STRUCT(avxVertexInputSource)
{
    afxBuffer           buf;
    afxNat32            offset; /// the start of buffer.
    afxNat32            range; /// the size in bytes of vertex data bound from buffer.
    afxNat32            stride; /// the byte stride between consecutive elements within the buffer.
    afxBool8            instance; /// addressing is a function of the instance index, else case, it is of the vertex index.
    afxNat32            instDivisor; /// the number of successive instances that will use the same value of the vertex attribute when instanced rendering is enabled.
};

AFX_DEFINE_STRUCT(avxVertexInputPoint) /// vertex attribute input stream
{
    afxNat              locationIdx; /// is the shader input location number for this attribute.
    afxNat              srcSlotIdx; /// is the binding number which this attribute takes its data from.
    afxVertexFormat     fmt; /// is the size and type of the vertex attribute data.
    afxNat32            offset; /// is a byte offset of this attribute relative to the start of an element in the vertex input binding.
    //akxVertexUsage      usage; /// special flags used to opportunistic optimization
};

  //////////////////////////////////////////////////////////////////////////////
 //// COMMANDS                                                             ////
//////////////////////////////////////////////////////////////////////////////

/// Set the viewport dynamically for a command buffer.
/// This command sets the viewport transformation parameters state for subsequent drawing commands when the graphics pipeline is created without viewport set.

/// The viewport parameters taken from element #i of @vp replace the current state for the viewport index @baseIdx + #i, for #i in[0, @cnt).

AVX afxCmdId            AvxCmdAdjustViewports
(
    avxCmdb             cmdb,
    afxNat              baseIdx, /// is the index of the first viewport whose parameters are updated by the command.
    afxNat              cnt, /// is the number of viewports whose parameters are updated by the command.
    afxViewport const   viewports[] /// is a pointer to an array of afxViewport structures specifying viewport parameters.
);

/// Bind vertex buffers to a command buffer and dynamically set strides.
/// The values taken from elements #i of @buf and @offset replace the current state for the vertex input binding @baseIdx + #i, for #i in [0, @cnt].
/// The vertex input binding is updated to start at the offset indicated by @offset[#i] from the start of the buffer @buf[#i].
/// If @range is not NIL then @range[#i] specifies the bound size of the vertex buffer starting from the corresponding elements of @buf[#i] plus @offset[#i].
/// If @range[#i] is WHOLE_SIZE then the bound size is from @buf[#i] plus @offset[#i] to the end of the buffer @buf[#i].
/// All vertex input attributes that use each of these bindings will use these updated addresses in their address calculations for subsequent drawing commands.
/// If the null descriptor feature is enabled, elements of @buf can be NIL, and can be used by the vertex shader.
/// If a vertex input attribute is bound to a vertex input binding that is NIL, the values taken from memory are considered to be zero, and missing G, B, or A components are filled with (0,0,1).

/// This command also dynamically sets the byte strides between consecutive elements within buffer @buf[#i] to the corresponding @stride[#i] value when drawing using shader objects, or when the graphics pipeline is created without vertex input binding stride set.

AVX afxCmdId            AvxCmdBindVertexSources
(
    avxCmdb             cmdb,
    afxNat              baseSlotIdx, /// is the index of the first vertex input binding whose state is updated by the command.
    afxNat              slotCnt, /// is the number of vertex input bindings whose state is updated by the command.
    afxBuffer           buffers[], /// is an array of buffer handles.
    afxNat32 const      offsets[], /// the start of buffer.
    afxNat32 const      ranges[], /// the size in bytes of vertex data bound from buffer.
    afxNat32 const      strides[] /// the byte stride between consecutive elements within the buffer.
    //afxNat32 const    divisor[] /// the number of successive instances that will use the same value of the vertex attribute when instanced rendering is used.
);

/// Bind an index buffer to a command buffer.

AVX afxCmdId            AvxCmdBindIndexSource
(
    avxCmdb             cmdb,
    afxBuffer           buf, /// is the buffer being bound.
    afxNat32            offset, /// is the starting offset in bytes within buffer used in index buffer address calculations.
    afxNat32            range, /// is the size in bytes of index data bound from buffer.
    afxNat32            idxSiz /// is a value specifying the size of the indices.
);

/// Set primitive topology state dynamically for a command buffer.
/// This command sets the primitive topology for subsequent drawing commands when drawing using shader objects, or when the graphics pipeline is created without primitive topology set.

AVX afxCmdId            AvxCmdSetPrimitiveTopology
(
    avxCmdb             cmdb,
    avxTopology         topology /// specifies the primitive topology to use for drawing.
);

/// Set front face orientation dynamically for a command buffer.

AVX afxCmdId            AvxCmdSwitchFrontFace
(
    avxCmdb             cmdb,
    afxBool             cw /// is a value specifying the front-facing triangle orientation to be used for culling.
);

/// Set cull mode dynamically for a command buffer.

AVX afxCmdId            AvxCmdSetCullMode
(
    avxCmdb             cmdb,
    avxCullMode         mode /// specifies the cull mode property to use for drawing.
);

AVX afxCmdId            AvxCmdBindVertexInput(avxCmdb cmdb, avxVertexInput vin);

#endif//AFX_TRANSFORM_OPS_H