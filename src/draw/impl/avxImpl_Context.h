/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_IMPL___CONTEXT_H
#define AVX_IMPL___CONTEXT_H

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "qwadro/inc/mem/afxInterlockedQueue.h"
#include "qwadro/inc/mem/afxSlabAllocator.h"

typedef enum avxCmdbState
/// Each command buffer is always in one of the following states
{
    // When a command buffer is allocated, it is in the initial state.
    // Some commands are able to reset a command buffer (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    // Command buffers in the initial state canv only be moved to the recording state, or freed.
    avxCmdbState_INITIAL,

    // BeginCmdBuffer() changes the state of a command buffer from the initial state to the recording state.
    // Once a command buffer is in the recording state, AvxCmd* commands canv be used to record to the command buffer.
    avxCmdbState_RECORDING,

    // AfxCompileCmdBuffer() ends the recording of a command buffer, and moves it from the recording state to the executable state.
    // Executable command buffers canv be submitted, reset, or recorded to another command buffer.
    avxCmdbState_EXECUTABLE,

    // Queue submission of a command buffer changes the state of a command buffer from the executable state to the pending state.
    // Whilst in the pending state, applications must not attempt to modify the command buffer in any way - as the device may be processing the commands recorded to it.
    // Once execution of a command buffer completes, the command buffer either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    // A synchronization command should be used to detect when this occurs.
    avxCmdbState_PENDING,

    // Some operations, such as modifying or deleting a resource that was used in a command recorded to a command buffer, will transition the state of that command buffer into the invalid state.
    // Command buffers in the invalid state canv only be reset or freed.
    avxCmdbState_INVALID,

    avxCmdbState_INTERNAL_EXECUTING,
} avxCmdbState;

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDctxDdi);
#else
AFX_DEFINE_STRUCT(_avxDctxDdi)
{
    afxError(*end)(afxDrawContext);
    afxError(*reset)(afxDrawContext, afxBool freeMem, afxBool permanent);
};
#endif

#ifdef _AVX_DRAW_CONTEXT_C
#ifdef _AVX_CMD_BUFFER_IMPL
AFX_OBJECT(_avxDrawContext)
#else
AFX_OBJECT(afxDrawContext)
#endif
{
    _avxDctxDdi const*pimpl;
    afxArena        cmdArena; // owned by dsys data for specific port
    afxChain        commands;
    avxCmdbState    state;

    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.

    afxUnit         portId;
    afxUnit         poolIdx;
    afxBool         disposable; // if true, at execution end, it is moved to invalid state and considered in recycle chain.

    afxDrawLimits const* devLimits;
    afxDrawFeatures const* enabledFeatures;

    afxBool         inRenderPass;
    afxBool         inVideoCoding;

    struct
    {
        avxCanvas           canv;
        afxRect             area;
        avxPipeline         pip;
        avxVertexDecl       vtxd;
        avxBufferedStream   vbos[16];
        avxBuffer           ibo;
    };
};
#endif//_AVX_DRAW_CONTEXT_C

AFX_DEFINE_STRUCT(avxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(avxCmd)
{
    avxCmdHdr hdr;

    // BUFFER OPERATIONS

    struct
    {
        avxCmdHdr hdr;

        avxBuffer src;
        avxBuffer dst;
        afxUnit opCnt;
        avxBufferCopy AFX_SIMD ops[];
    } CopyBuffer;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxUnit value;
    } FillBuffer;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit offset;
        afxUnit range;
    } ClearBuffer;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxByte AFX_SIMD data[];
    } UpdateBuffer;

    struct
    {
        avxCmdHdr hdr;

        afxRaster ras;
        avxBuffer buf;
        afxUnit opCnt;
        afxRasterIo AFX_SIMD ops[];
    } PackRaster;
    struct
    {
        avxCmdHdr hdr;

        afxRaster ras;
        avxBuffer buf;
        afxUnit opCnt;
        afxRasterIo AFX_SIMD ops[];
    } UnpackRaster;
    struct
    {
        avxCmdHdr hdr;

        afxRaster src;
        afxRaster dst;
        afxUnit opCnt;
        afxRasterCopy AFX_SIMD ops[];
    } CopyRaster;
    struct
    {
        avxCmdHdr hdr;

        afxDrawContext dctx;
        afxRaster src;
        afxRaster dst;
        avxTexelFilter flt;
        afxUnit opCnt;
        afxRasterBlit AFX_SIMD ops[];
    } BlitRaster;
    struct
    {
        avxCmdHdr hdr;

        afxDrawContext dctx;
        afxRaster src;
        afxRaster dst;
        afxUnit opCnt;
        afxRasterCopy AFX_SIMD ops[];
    } ResolveRaster;
    struct
    {
        avxCmdHdr hdr;

        afxDrawContext dctx;
        afxRaster ras;
        avxClearValue value;
        afxUnit opCnt;
        afxRasterBlock AFX_SIMD ops[];
    } ClearRaster;
    struct
    {
        avxCmdHdr hdr;

        afxRaster ras;
        afxUnit baseLod;
        afxUnit lodCnt;
    } SubsampleRaster;

    // GENERAL OPERATIONS
    struct
    {
        avxCmdHdr hdr;

        afxUnit segment;
        avxPipeline pip;
        avxVertexDecl vin;
        afxFlags dynamics;
    } BindPipeline;
    struct
    {
        avxCmdHdr hdr;

        afxUnit set;
        afxUnit baseIdx;
        afxUnit cnt;
        avxBufferMap AFX_SIMD maps[];
    } BindBuffers;
    struct
    {
        avxCmdHdr hdr;

        afxUnit set;
        afxUnit baseIdx;
        afxUnit cnt;
        afxRaster AFX_SIMD rasters[];
    } BindRasters;
    struct
    {
        avxCmdHdr hdr;

        afxUnit set;
        afxUnit baseIdx;
        afxUnit cnt;
        avxSampler AFX_SIMD samplers[];
    } BindSamplers;
    struct
    {
        avxCmdHdr hdr;

        afxUnit cnt;
        afxDrawContext AFX_SIMD contexts[];
    } ExecuteCommands;
    struct
    {
        avxCmdHdr hdr;

        avxDrawIndirect data;
    } Draw;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 drawCnt;
        afxUnit32 stride;
    } DrawIndirect;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        avxBuffer cntBuf;
        afxUnit32 cntBufOff;
        afxUnit32 maxDrawCnt;
        afxUnit32 stride;
    } DrawIndirectCount;
    struct
    {
        avxCmdHdr hdr;

        avxDrawIndexedIndirect data;
    } DrawIndexed;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 drawCnt;
        afxUnit32 stride;
    } DrawIndexedIndirect;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        avxBuffer cntBuf;
        afxUnit32 cntBufOff;
        afxUnit32 maxDrawCnt;
        afxUnit32 stride;
    } DrawIndexedIndirectCount;
    struct
    {
        avxCmdHdr hdr;

        avxDispatchIndirect data;
    } Dispatch;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
    } DispatchIndirect;
    struct
    {
        avxCmdHdr hdr;

        afxUnit offset;
        afxUnit siz;
        afxByte AFX_SIMD data[];
    } PushConstants;
    struct
    {
        avxCmdHdr hdr;

        afxM4d  v;
        afxV2d  at;
        afxUnit len;
        afxByte AFX_SIMD data[];
    } StampDebug;

    // TRANSFORMATION OPERATIONS

    struct
    {
        avxCmdHdr hdr;

        avxVertexDecl vin;
    } DeclareVertex;
    struct
    {
        avxCmdHdr hdr;

        afxBool cw;
    } SwitchFrontFace;
    struct
    {
        avxCmdHdr hdr;

        avxCullMode mode;
    } SetCullMode;
    struct
    {
        avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        afxViewport AFX_SIMD viewports[];
    } AdjustViewports;
    struct
    {
        avxCmdHdr hdr;

        afxUnit baseSlotIdx;
        afxUnit cnt;
        avxBufferedStream AFX_SIMD src[];
    } BindVertexBuffers;
    struct
    {
        avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 range;
        afxUnit32 idxSiz;
    } BindIndexBuffer;
    struct
    {
        avxCmdHdr hdr;

        avxTopology topology;
    } SetPrimitiveTopology;

    // RASTERIZATION OPERATIOS

    struct
    {
        avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthBias;
    struct
    {
        avxCmdHdr hdr;

        afxReal constFactor;
        afxReal clamp;
        afxReal slopeFactor;
    } SetDepthBias;
    struct
    {
        avxCmdHdr hdr;

        afxReal lineWidth;
    } SetLineWidth;
    struct
    {
        avxCmdHdr hdr;

        afxBool disable;
    } DisableRasterization;
    struct
    {
        avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthTest;
    struct
    {
        avxCmdHdr hdr;

        avxCompareOp compareOp;
    } SetDepthCompareOp;
    struct
    {
        avxCmdHdr hdr;

        afxBool disable;
    } DisableDepthWrite;
    struct
    {
        avxCmdHdr hdr;

        afxBool enable;
    } EnableStencilTest;
    struct
    {
        avxCmdHdr hdr;

        afxMask faceMask;
        afxUnit32 compareMask;
    } SetStencilCompareMask;
    struct
    {
        avxCmdHdr hdr;

        afxMask faceMask;
        afxUnit32 writeMask;
    } SetStencilWriteMask;
    struct
    {
        avxCmdHdr hdr;

        afxMask faceMask;
        afxUnit32 reference;
    } SetStencilReference;
    struct
    {
        avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthBoundsTest;
    struct
    {
        avxCmdHdr hdr;

        afxV2d bounds;
    } SetDepthBounds;
    struct
    {
        avxCmdHdr hdr;

        afxV2d blendContants;
    } SetBlendConstants;
    struct
    {
        avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        afxRect rects[];
    } AdjustScissors;
    struct
    {
        avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        afxRect AFX_SIMD rects[];
    } AdjustCurtains;
    struct
    {
        avxCmdHdr hdr;

        avxCanvas       canv;
        afxRect         area;
        afxBool         hasD;
        avxDrawTarget   depth;
        afxBool         hasS;
        avxDrawTarget   stencil;
        afxUnit         targetCnt;
        afxUnit         baseLayer;
        afxUnit         layerCnt;
        avxDrawTarget AFX_SIMD targets[];
    } CommenceDrawScope;
    struct
    {
        avxCmdHdr hdr;

        afxInt nothing;
    } ConcludeDrawScope;
    struct
    {
        avxCmdHdr hdr;

        afxBool useAuxContexts;
    } NextPass;

    // QUERY OPERATIONS
    struct
    {
        avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit queryIdx;
        afxBool precise;
    } BeginQuery;
    struct
    {
        avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit queryIdx;
    } EndQuery;
    struct
    {
        avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit baseQuery;
        afxUnit queryCnt;
        avxBuffer buf;
        afxSize offset;
        afxSize stride;
        afxQueryResultFlags flags;
    } CopyQueryResults;
    struct
    {
        avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit baseQuery;
        afxUnit queryCnt;
    } ResetQueries;
    struct
    {
        avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit queryIdx;
        avxPipelineStage stage;
    } WriteTimestamp;

    struct
    {
        avxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } PushDebugScope;
    struct
    {
        avxCmdHdr hdr;

        afxInt nothing;
    } PopDebugScope;
    struct
    {
        avxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } InsertDebugLabel;
};

AFX_DEFINE_UNION(avxCmdList)
{
    struct
    {
        void* CopyBuffer;
        void* FillBuffer;
        void* ClearBuffer;
        void* UpdateBuffer;

        void* PackRaster;
        void* UnpackRaster;
        void* CopyRaster;
        void* BlitRaster;
        void* ResolveRaster;
        void* ClearRaster;
        void* SubsampleRaster;

        // GENERAL OPERATIONS
        void* BindPipeline;
        void* BindBuffers;
        void* BindRasters;
        void* BindSamplers;
        void* ExecuteCommands;
        void* Draw;
        void* DrawIndirect;
        void* DrawIndirectCount;
        void* DrawIndexed;
        void* DrawIndexedIndirect;
        void* DrawIndexedIndirectCount;
        void* Dispatch;
        void* DispatchIndirect;
        void* PushConstants;
        void* StampDebug;

        // TRANSFORMATION OPERATIONS
        void* DeclareVertex;
        void* SwitchFrontFace;
        void* SetCullMode;
        void* AdjustViewports;
        void* BindVertexBuffers;
        void* BindIndexBuffer;
        void* SetPrimitiveTopology;

        // RASTERIZATION OPERATIOS
        void* EnableDepthBias;
        void* SetDepthBias;
        void* SetLineWidth;
        void* DisableRasterization;
        void* EnableDepthTest;
        void* SetDepthCompareOp;
        void* DisableDepthWrite;
        void* EnableStencilTest;
        void* SetStencilCompareMask;
        void* SetStencilWriteMask;
        void* SetStencilReference;
        void* EnableDepthBoundsTest;
        void* SetDepthBounds;
        void* SetBlendConstants;
        void* AdjustScissors;
        void* AdjustCurtains;
        void* CommenceDrawScope;
        void* ConcludeDrawScope;
        void* NextPass;

        // QUERY OPERATIONS
        void* BeginQuery;
        void* EndQuery;
        void* CopyQueryResults;
        void* ResetQueries;
        void* WriteTimestamp;

        void* PushDebugScope;
        void* PopDebugScope;
        void* InsertDebugLabel;
    };
    void(*f[])(void*, avxCmd const*);
};

#define AVX_GET_STD_CMD_ID(cmdName_) (offsetof(avxCmdList, cmdName_) / sizeof(void*))

AVX avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AVX afxError _AvxDctxImplResetCb(afxDrawContext dctx, afxBool freeMem, afxBool permanent);
AVX afxError _AvxDctxImplEndCb(afxDrawContext dctx);


AVX avxCmdbState    _AvxGetCommandStatus(afxDrawContext dctx);

AVX afxClassConfig const _AVX_DCTX_CLASS_CONFIG;
AVX _avxDctxDdi const _AVX_DCTX_DDI;


#endif//AVX_IMPL___CONTEXT_H
