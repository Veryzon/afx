﻿/*
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

#define _AFX_CORE_C
#define _AFX_ICD_C
#define _AFX_DRAW_C
#define _AFX_THREAD_C
#define _AFX_DRAW_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DRIVER_C
#include "sgl.h"
#include "sglDdrv.h"
#include "qwadro/core/afxSystem.h"
 //#pragma comment(lib, "opengl32")

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <assert.h>

static const char*Ebenezer = "Ebenezer";

extern afxClassConfig _SglDscrClsConfig;
extern afxClassConfig _SglDqueClsConfig;

AFXINL afxDrawSystem _AfxGetDsysData(void);

static const char *glVmtNames[] =
{
    // 100
    "glCullFace", // 0
    "glFrontFace",
    "glHint",
    "glLineWidth",
    "glPointSize",
    "glPolygonMode",
    "glScissor",
    "glTexParameterf",
    "glTexParameterfv",
    "glTexParameteri",
    "glTexParameteriv",
    "glTexImage1D",
    "glTexImage2D",
    "glDrawBuffer",
    "glClear",
    "glClearColor",
    "glClearStencil",
    "glClearDepth",
    "glStencilMask",
    "glColorMask",
    "glDepthMask",
    "glDisable",
    "glEnable",
    "glFinish",
    "glFlush",
    "glBlendFunc",
    "glLogicOp",
    "glStencilFunc",
    "glStencilOp",
    "glDepthFunc",
    "glPixelStoref",
    "glPixelStorei",
    "glReadBuffer",
    "glReadPixels",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glGetTexImage",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetTexLevelParameterfv",
    "glGetTexLevelParameteriv",
    "glIsEnabled",
    "glDepthRange",
    "glViewport", // 47
    // 110
    "glDrawArrays", // 0
    "glDrawElements",
    "glGetPointerv",
    "glPolygonOffset",
    "glCopyTexImage1D",
    "glCopyTexImage2D",
    "glCopyTexSubImage1D",
    "glCopyTexSubImage2D",
    "glTexSubImage1D",
    "glTexSubImage2D",
    "glBindTexture",
    "glDeleteTextures",
    "glGenTextures",
    "glIsTexture", // 13
    // 120
    "glDrawRangeElements", // 0
    "glTexImage3D",
    "glTexSubImage3D",
    "glCopyTexSubImage3D", // 3
    // 130
    "glActiveTexture", // 0
    "glSampleCoverage",
    "glCompressedTexImage3D",
    "glCompressedTexImage2D",
    "glCompressedTexImage1D",
    "glCompressedTexSubImage3D",
    "glCompressedTexSubImage2D",
    "glCompressedTexSubImage1D",
    "glGetCompressedTexImage", // 8
    // 140
    "glBlendFuncSeparate", // 0
    "glMultiDrawArrays",
    "glMultiDrawElements",
    "glPointParameterf",
    "glPointParameterfv",
    "glPointParameteri",
    "glPointParameteriv",
    "glBlendColor",
    "glBlendEquation", // 8
    // 150
    "glGenQueries", // 0
    "glDeleteQueries",
    "glIsQuery",
    "glBeginQuery",
    "glEndQuery",
    "glGetQueryiv",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glBindBuffer",
    "glDeleteBuffers",
    "glGenBuffers",
    "glIsBuffer",
    "glBufferData",
    "glBufferSubData",
    "glGetBufferSubData",
    "glMapBuffer",
    "glUnmapBuffer",
    "glGetBufferParameteriv",
    "glGetBufferPointerv", // 18
    // 200
    "glBlendEquationSeparate", // 0
    "glDrawBuffers",
    "glStencilOpSeparate",
    "glStencilFuncSeparate",
    "glStencilMaskSeparate",
    "glAttachShader",
    "glBindAttribLocation",
    "glCompileShader",
    "glCreateProgram",
    "glCreateShader",
    "glDeleteProgram",
    "glDeleteShader",
    "glDetachShader",
    "glDisableVertexAttribArray",
    "glEnableVertexAttribArray",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetAttribLocation",
    "glGetProgramiv",
    "glGetProgramInfoLog",
    "glGetShaderiv",
    "glGetShaderInfoLog",
    "glGetShaderSource",
    "glGetUniformLocation",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribdv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glGetVertexAttribPointerv",
    "glIsProgram",
    "glIsShader",
    "glLinkProgram",
    "glShaderSource",
    "glUseProgram",
    "glUniform1f",
    "glUniform2f",
    "glUniform3f",
    "glUniform4f",
    "glUniform1i",
    "glUniform2i",
    "glUniform3i",
    "glUniform4i",
    "glUniform1fv",
    "glUniform2fv",
    "glUniform3fv",
    "glUniform4fv",
    "glUniform1iv",
    "glUniform2iv",
    "glUniform3iv",
    "glUniform4iv",
    "glUniformMatrix2fv",
    "glUniformMatrix3fv",
    "glUniformMatrix4fv",
    "glValidateProgram",
    "glVertexAttrib1d",
    "glVertexAttrib1dv",
    "glVertexAttrib1f",
    "glVertexAttrib1fv",
    "glVertexAttrib1s",
    "glVertexAttrib1sv",
    "glVertexAttrib2d",
    "glVertexAttrib2dv",
    "glVertexAttrib2f",
    "glVertexAttrib2fv",
    "glVertexAttrib2s",
    "glVertexAttrib2sv",
    "glVertexAttrib3d",
    "glVertexAttrib3dv",
    "glVertexAttrib3f",
    "glVertexAttrib3fv",
    "glVertexAttrib3s",
    "glVertexAttrib3sv",
    "glVertexAttrib4Nbv",
    "glVertexAttrib4Niv",
    "glVertexAttrib4Nsv",
    "glVertexAttrib4Nub",
    "glVertexAttrib4Nubv",
    "glVertexAttrib4Nuiv",
    "glVertexAttrib4Nusv",
    "glVertexAttrib4bv",
    "glVertexAttrib4d",
    "glVertexAttrib4dv",
    "glVertexAttrib4f",
    "glVertexAttrib4fv",
    "glVertexAttrib4iv",
    "glVertexAttrib4s",
    "glVertexAttrib4sv",
    "glVertexAttrib4ubv",
    "glVertexAttrib4uiv",
    "glVertexAttrib4usv",
    "glVertexAttribPointer", // 92
    // 210
    "glUniformMatrix2x3fv", // 0
    "glUniformMatrix3x2fv",
    "glUniformMatrix2x4fv",
    "glUniformMatrix4x2fv",
    "glUniformMatrix3x4fv",
    "glUniformMatrix4x3fv", // 5
    // 300
    "glColorMaski", // 0
    "glGetBooleani_v",
    "glGetIntegeri_v",
    "glEnablei",
    "glDisablei",
    "glIsEnabledi",
    "glBeginTransformFeedback",
    "glEndTransformFeedback",
    "glBindBufferRange",
    "glBindBufferBase",
    "glTransformFeedbackVaryings",
    "glGetTransformFeedbackVarying",
    "glClampColor",
    "glBeginConditionalRender",
    "glEndConditionalRender",
    "glVertexAttribIPointer",
    "glGetVertexAttribIiv",
    "glGetVertexAttribIuiv",
    "glVertexAttribI1i",
    "glVertexAttribI2i",
    "glVertexAttribI3i",
    "glVertexAttribI4i",
    "glVertexAttribI1ui",
    "glVertexAttribI2ui",
    "glVertexAttribI3ui",
    "glVertexAttribI4ui",
    "glVertexAttribI1iv",
    "glVertexAttribI2iv",
    "glVertexAttribI3iv",
    "glVertexAttribI4iv",
    "glVertexAttribI1uiv",
    "glVertexAttribI2uiv",
    "glVertexAttribI3uiv",
    "glVertexAttribI4uiv",
    "glVertexAttribI4bv",
    "glVertexAttribI4sv",
    "glVertexAttribI4ubv",
    "glVertexAttribI4usv",
    "glGetUniformuiv",
    "glBindFragDataLocation",
    "glGetFragDataLocation",
    "glUniform1ui",
    "glUniform2ui",
    "glUniform3ui",
    "glUniform4ui",
    "glUniform1uiv",
    "glUniform2uiv",
    "glUniform3uiv",
    "glUniform4uiv",
    "glTexParameterIiv",
    "glTexParameterIuiv",
    "glGetTexParameterIiv",
    "glGetTexParameterIuiv",
    "glClearBufferiv",
    "glClearBufferuiv",
    "glClearBufferfv",
    "glClearBufferfi",
    "glGetStringi",
    "glIsRenderbuffer",
    "glBindRenderbuffer",
    "glDeleteRenderbuffers",
    "glGenRenderbuffers",
    "glRenderbufferStorage",
    "glGetRenderbufferParameteriv",
    "glIsFramebuffer",
    "glBindFramebuffer",
    "glDeleteFramebuffers",
    "glGenFramebuffers",
    "glCheckFramebufferStatus",
    "glFramebufferTexture1D",
    "glFramebufferTexture2D",
    "glFramebufferTexture3D",
    "glFramebufferRenderbuffer",
    "glGetFramebufferAttachmentParameteriv",
    "glGenerateMipmap",
    "glBlitFramebuffer",
    "glRenderbufferStorageMultisample",
    "glFramebufferTextureLayer",
    "glMapBufferRange",
    "glFlushMappedBufferRange",
    "glBindVertexArray",
    "glDeleteVertexArrays",
    "glGenVertexArrays",
    "glIsVertexArray", // 83
    // 310
    "glDrawArraysInstanced", // 0
    "glDrawElementsInstanced",
    "glTexBuffer",
    "glPrimitiveRestartIndex",
    "glCopyBufferSubData",
    "glGetUniformIndices",
    "glGetActiveUniformsiv",
    "glGetActiveUniformName",
    "glGetUniformBlockIndex",
    "glGetActiveUniformBlockiv",
    "glGetActiveUniformBlockName",
    "glUniformBlockBinding", // 11
    // 320
    "glDrawElementsBaseVertex", // 0
    "glDrawRangeElementsBaseVertex",
    "glDrawElementsInstancedBaseVertex",
    "glMultiDrawElementsBaseVertex",
    "glProvokingVertex",
    "glFenceSync",
    "glIsSync",
    "glDeleteSync",
    "glClientWaitSync",
    "glWaitSync",
    "glGetInteger64v",
    "glGetSynciv",
    "glGetInteger64i_v",
    "glGetBufferParameteri64v",
    "glFramebufferTexture",
    "glTexImage2DMultisample",
    "glTexImage3DMultisample",
    "glGetMultisamplefv",
    "glSampleMaski", // 18
    // 330
    "glBindFragDataLocationIndexed", // 0
    "glGetFragDataIndex",
    "glGenSamplers",
    "glDeleteSamplers",
    "glIsSampler",
    "glBindSampler",
    "glSamplerParameteri",
    "glSamplerParameteriv",
    "glSamplerParameterf",
    "glSamplerParameterfv",
    "glSamplerParameterIiv",
    "glSamplerParameterIuiv",
    "glGetSamplerParameteriv",
    "glGetSamplerParameterIiv",
    "glGetSamplerParameterfv",
    "glGetSamplerParameterIuiv",
    "glQueryCounter",
    "glGetQueryObjecti64v",
    "glGetQueryObjectui64v",
    "glVertexAttribDivisor",
    "glVertexAttribP1ui",
    "glVertexAttribP1uiv",
    "glVertexAttribP2ui",
    "glVertexAttribP2uiv",
    "glVertexAttribP3ui",
    "glVertexAttribP3uiv",
    "glVertexAttribP4ui",
    "glVertexAttribP4uiv", // 27
    // 400
    "glMinSampleShading", // 0
    "glBlendEquationi",
    "glBlendEquationSeparatei",
    "glBlendFunci",
    "glBlendFuncSeparatei",
    "glDrawArraysIndirect",
    "glDrawElementsIndirect",
    "glUniform1d",
    "glUniform2d",
    "glUniform3d",
    "glUniform4d",
    "glUniform1dv",
    "glUniform2dv",
    "glUniform3dv",
    "glUniform4dv",
    "glUniformMatrix2dv",
    "glUniformMatrix3dv",
    "glUniformMatrix4dv",
    "glUniformMatrix2x3dv",
    "glUniformMatrix2x4dv",
    "glUniformMatrix3x2dv",
    "glUniformMatrix3x4dv",
    "glUniformMatrix4x2dv",
    "glUniformMatrix4x3dv",
    "glGetUniformdv",
    "glGetSubroutineUniformLocation",
    "glGetSubroutineIndex",
    "glGetActiveSubroutineUniformiv",
    "glGetActiveSubroutineUniformName",
    "glGetActiveSubroutineName",
    "glUniformSubroutinesuiv",
    "glGetUniformSubroutineuiv",
    "glGetProgramStageiv",
    "glPatchParameteri",
    "glPatchParameterfv",
    "glBindTransformFeedback",
    "glDeleteTransformFeedbacks",
    "glGenTransformFeedbacks",
    "glIsTransformFeedback",
    "glPauseTransformFeedback",
    "glResumeTransformFeedback",
    "glDrawTransformFeedback",
    "glDrawTransformFeedbackStream",
    "glBeginQueryIndexed",
    "glEndQueryIndexed",
    "glGetQueryIndexediv", // 45
    // 410
    "glReleaseShaderCompiler", // 0
    "glShaderBinary",
    "glGetShaderPrecisionFormat",
    "glDepthRangef",
    "glClearDepthf",
    "glGetProgramBinary",
    "glProgramBinary",
    "glProgramParameteri",
    "glUseProgramStages",
    "glActiveShaderProgram",
    "glCreateShaderProgramv",
    "glBindProgramPipeline",
    "glDeleteProgramPipelines",
    "glGenProgramPipelines",
    "glIsProgramPipeline",
    "glGetProgramPipelineiv",
    "glProgramUniform1i",
    "glProgramUniform1iv",
    "glProgramUniform1f",
    "glProgramUniform1fv",
    "glProgramUniform1d",
    "glProgramUniform1dv",
    "glProgramUniform1ui",
    "glProgramUniform1uiv",
    "glProgramUniform2i",
    "glProgramUniform2iv",
    "glProgramUniform2f",
    "glProgramUniform2fv",
    "glProgramUniform2d",
    "glProgramUniform2dv",
    "glProgramUniform2ui",
    "glProgramUniform2uiv",
    "glProgramUniform3i",
    "glProgramUniform3iv",
    "glProgramUniform3f",
    "glProgramUniform3fv",
    "glProgramUniform3d",
    "glProgramUniform3dv",
    "glProgramUniform3ui",
    "glProgramUniform3uiv",
    "glProgramUniform4i",
    "glProgramUniform4iv",
    "glProgramUniform4f",
    "glProgramUniform4fv",
    "glProgramUniform4d",
    "glProgramUniform4dv",
    "glProgramUniform4ui",
    "glProgramUniform4uiv",
    "glProgramUniformMatrix2fv",
    "glProgramUniformMatrix3fv",
    "glProgramUniformMatrix4fv",
    "glProgramUniformMatrix2dv",
    "glProgramUniformMatrix3dv",
    "glProgramUniformMatrix4dv",
    "glProgramUniformMatrix2x3fv",
    "glProgramUniformMatrix3x2fv",
    "glProgramUniformMatrix2x4fv",
    "glProgramUniformMatrix4x2fv",
    "glProgramUniformMatrix3x4fv",
    "glProgramUniformMatrix4x3fv",
    "glProgramUniformMatrix2x3dv",
    "glProgramUniformMatrix3x2dv",
    "glProgramUniformMatrix2x4dv",
    "glProgramUniformMatrix4x2dv",
    "glProgramUniformMatrix3x4dv",
    "glProgramUniformMatrix4x3dv",
    "glValidateProgramPipeline",
    "glGetProgramPipelineInfoLog",
    "glVertexAttribL1d",
    "glVertexAttribL2d",
    "glVertexAttribL3d",
    "glVertexAttribL4d",
    "glVertexAttribL1dv",
    "glVertexAttribL2dv",
    "glVertexAttribL3dv",
    "glVertexAttribL4dv",
    "glVertexAttribLPointer",
    "glGetVertexAttribLdv",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
    "glScissorArrayv",
    "glScissorIndexed",
    "glScissorIndexedv",
    "glDepthRangeArrayv",
    "glDepthRangeIndexed",
    "glGetFloati_v",
    "glGetDoublei_v", // 87
    // 420
    "glDrawArraysInstancedBaseInstance", // 0
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glGetInternalformativ",
    "glGetActiveAtomicCounterBufferiv",
    "glBindImageTexture",
    "glMemoryBarrier",
    "glTexStorage1D",
    "glTexStorage2D",
    "glTexStorage3D",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced", // 11
    // 430
    "glClearBufferData", // 0
    "glClearBufferSubData",
    "glDispatchCompute",
    "glDispatchComputeIndirect",
    "glCopyImageSubData",
    "glFramebufferParameteri",
    "glGetFramebufferParameteriv",
    "glGetInternalformati64v",
    "glInvalidateTexSubImage",
    "glInvalidateTexImage",
    "glInvalidateBufferSubData",
    "glInvalidateBufferData",
    "glInvalidateFramebuffer",
    "glInvalidateSubFramebuffer",
    "glMultiDrawArraysIndirect",
    "glMultiDrawElementsIndirect",
    "glGetProgramInterfaceiv",
    "glGetProgramResourceIndex",
    "glGetProgramResourceName",
    "glGetProgramResourceiv",
    "glGetProgramResourceLocation",
    "glGetProgramResourceLocationIndex",
    "glShaderStorageBlockBinding",
    "glTexBufferRange",
    "glTexStorage2DMultisample",
    "glTexStorage3DMultisample",
    "glTextureView",
    "glBindVertexBuffer",
    "glVertexAttribFormat",
    "glVertexAttribIFormat",
    "glVertexAttribLFormat",
    "glVertexAttribBinding",
    "glVertexBindingDivisor",
    "glDebugMessageControl",
    "glDebugMessageInsert",
    "glDebugMessageCallback",
    "glGetDebugMessageLog",
    "glPushDebugGroup",
    "glPopDebugGroup",
    "glObjectLabel",
    "glGetObjectLabel",
    "glObjectPtrLabel",
    "glGetObjectPtrLabel", // 42
    // 440
    "glBufferStorage", // 0
    "glClearTexImage",
    "glClearTexSubImage",
    "glBindBuffersBase",
    "glBindBuffersRange",
    "glBindTextures",
    "glBindSamplers",
    "glBindImageTextures",
    "glBindVertexBuffers", // 8
    // 450
    "glClipControl", // 0
    "glCreateTransformFeedbacks",
    "glTransformFeedbackBufferBase",
    "glTransformFeedbackBufferRange",
    "glGetTransformFeedbackiv",
    "glGetTransformFeedbacki_v",
    "glGetTransformFeedbacki64_v",
    "glCreateBuffers",
    "glNamedBufferStorage",
    "glNamedBufferData",
    "glNamedBufferSubData",
    "glCopyNamedBufferSubData",
    "glClearNamedBufferData",
    "glClearNamedBufferSubData",
    "glMapNamedBuffer",
    "glMapNamedBufferRange",
    "glUnmapNamedBuffer",
    "glFlushMappedNamedBufferRange",
    "glGetNamedBufferParameteriv",
    "glGetNamedBufferParameteri64v",
    "glGetNamedBufferPointerv",
    "glGetNamedBufferSubData",
    "glCreateFramebuffers",
    "glNamedFramebufferRenderbuffer",
    "glNamedFramebufferParameteri",
    "glNamedFramebufferTexture",
    "glNamedFramebufferTextureLayer",
    "glNamedFramebufferCanvas",
    "glNamedFramebufferCanvass",
    "glNamedFramebufferReadBuffer",
    "glInvalidateNamedFramebufferData",
    "glInvalidateNamedFramebufferSubData",
    "glClearNamedFramebufferiv",
    "glClearNamedFramebufferuiv",
    "glClearNamedFramebufferfv",
    "glClearNamedFramebufferfi",
    "glBlitNamedFramebuffer",
    "glCheckNamedFramebufferStatus",
    "glGetNamedFramebufferParameteriv",
    "glGetNamedFramebufferAttachmentParameteriv",
    "glCreateRenderbuffers",
    "glNamedRenderbufferStorage",
    "glNamedRenderbufferStorageMultisample",
    "glGetNamedRenderbufferParameteriv",
    "glCreateTextures",
    "glTextureBuffer",
    "glTextureBufferRange",
    "glTextureStorage1D",
    "glTextureStorage2D",
    "glTextureStorage3D",
    "glTextureStorage2DMultisample",
    "glTextureStorage3DMultisample",
    "glTextureSubImage1D",
    "glTextureSubImage2D",
    "glTextureSubImage3D",
    "glCompressedTextureSubImage1D",
    "glCompressedTextureSubImage2D",
    "glCompressedTextureSubImage3D",
    "glCopyTextureSubImage1D",
    "glCopyTextureSubImage2D",
    "glCopyTextureSubImage3D",
    "glTextureParameterf",
    "glTextureParameterfv",
    "glTextureParameteri",
    "glTextureParameterIiv",
    "glTextureParameterIuiv",
    "glTextureParameteriv",
    "glGenerateTextureMipmap",
    "glBindTextureUnit",
    "glGetTextureImage",
    "glGetCompressedTextureImage",
    "glGetTextureLevelParameterfv",
    "glGetTextureLevelParameteriv",
    "glGetTextureParameterfv",
    "glGetTextureParameterIiv",
    "glGetTextureParameterIuiv",
    "glGetTextureParameteriv",
    "glCreateVertexArrays",
    "glDisableVertexArrayAttrib",
    "glEnableVertexArrayAttrib",
    "glVertexArrayElementBuffer",
    "glVertexArrayVertexBuffer",
    "glVertexArrayVertexBuffers",
    "glVertexArrayAttribBinding",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribIFormat",
    "glVertexArrayAttribLFormat",
    "glVertexArrayBindingDivisor",
    "glGetVertexArrayiv",
    "glGetVertexArrayIndexediv",
    "glGetVertexArrayIndexed64iv",
    "glCreateSamplers",
    "glCreateProgramPipelines",
    "glCreateQueries",
    "glGetQueryBufferObjecti64v",
    "glGetQueryBufferObjectiv",
    "glGetQueryBufferObjectui64v",
    "glGetQueryBufferObjectuiv",
    "glMemoryBarrierByRegion",
    "glGetTextureSubImage",
    "glGetCompressedTextureSubImage",
    "glGetGraphicsResetStatus",
    "glGetnCompressedTexImage",
    "glGetnTexImage",
    "glGetnUniformdv",
    "glGetnUniformfv",
    "glGetnUniformiv",
    "glGetnUniformuiv",
    "glReadnPixels",
    "glTextureBarrier", // 109
    // 460
    "glSpecializeShader", // 0
    "glMultiDrawArraysIndirectCount",
    "glMultiDrawElementsIndirectCount",
    "glPolygonOffsetClamp" // 3
};

_SGL int glVmtBases[][10] =
{
    {   0,   0,   0,   0,   0,   0,   0,   0, 0, 0 }, // version zero doesnt exist in GL
    {   0,  48,  62,  66,  75,  84,   0,   0, 0, 0 },
    { 103, 196,   0,   0,   0,   0,   0,   0, 0, 0 },
    { 202, 286, 298, 317,   0,   0,   0,   0, 0, 0 },
    { 345, 391, 479, 491, 534, 543, 653, 657, 0, 0 },
};

_SGL int glVmtRanges[][10] =
{
    {  0,  0,  0,  0, 0,   0, 0, 0, 0, 0 }, // version zero doesnt exist in GL
    { 48, 14,  4,  9, 9,  19, 0, 0, 0, 0 },
    { 93,  6,  0,  0, 0,   0, 0, 0, 0, 0 },
    { 84, 12, 19, 28, 0,   0, 0, 0, 0, 0 },
    { 46, 88, 12, 43, 9, 110, 4, 0, 0, 0 },
};

static const char *wglNames[] =
{
#if 0
    "wglCreateContext",
    "wglDeleteContext",
    "wglGetCurrentContext",
    "wglGetCurrentDC",
    "wglGetProcAddress",
    "wglMakeCurrent",
    "wglChoosePixelFormat",
    "wglDescribePixelFormat",
    "wglSetPixelFormat",
    "wglGetPixelFormat",
    "wglSwapBuffers",
#endif
    // ARB/EXT
    "wglChoosePixelFormatARB",
    "wglMakeContextCurrentARB",
    "wglGetCurrentReadDCARB",
    "wglCreateContextAttribsARB",
    "wglGetExtensionsStringARB",
    "wglSwapIntervalEXT",
};

afxError SglLoadOpenGlVmt(HMODULE opengl32, afxNat base, afxNat cnt, char const *names[], void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;
    afxFixedString128 tmp;
    AfxMakeFixedString128(&tmp, 0);
    afxString name;
    afxString arbAndExt[2];
    AfxMakeString(&arbAndExt[0], "ARB", 3);
    AfxMakeString(&arbAndExt[1], "EXT", 3);
    
    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(opengl32);
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        AfxMakeString(&name, names[base + i], 0);
        void *sym = NIL;

        if (f)
        {
            if ((sym = f((void*)name.start)))
            {
                if (echo)
                {
                    AfxEcho("%.3u %.*s core found.", i, AfxPushString(&name));
                }
            }
            else
            {
                for (afxNat j = 0; j < 2; j++)
                {
                    AfxFormatString(&tmp.str, "%.*s%.*s", AfxPushString(&name), AfxPushString(&arbAndExt[j]));

                    if ((sym = f((void*)tmp.buf)))
                    {
                        if (echo)
                        {
                            AfxAdvertise("%.3u %.*s ext found.", i, AfxPushString(&tmp.str.str));
                        }
                        break;
                    }
                    else if ((sym = GetProcAddress(opengl32, (void*)tmp.buf)))
                    {
                        if (echo)
                        {
                            AfxAdvertise("%.3u %.*s fallback found.", i, AfxPushString(&tmp.str.str));
                        }
                        break;
                    }
                }
            }
        }

        if (!sym && (sym = GetProcAddress(opengl32, (void*)name.start)))
        {
            if (echo)
            {
                AfxEcho("%.3u %.*s fallback found.", i, AfxPushString(&name));
            }
        }

        vmt[base + i] = sym;
    }
    return err;
}

_SGL GLenum AfxToGlFrontFace(afxFrontFace ff)
{
    static GLenum const v[] =
    {
        GL_CCW,
        GL_CW
    };

    return v[ff];
};

_SGL GLenum AfxToGlCullMode(afxCullMode cm)
{
    afxError err;

    static GLenum const v[] =
    {
        NIL,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK
    };
    AfxAssert(v[afxCullMode_BACK] == GL_BACK);
    return v[cm];
};

_SGL GLenum AfxToGlFillMode(afxFillMode pm)
{
    static GLenum const v[] =
    {
        GL_FILL,
        GL_LINE,
        GL_POINT
    };

    return v[pm];
};

_SGL GLenum SglToGlCompareOp(afxCompareOp co)
{
    static GLenum const v[] =
    {
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_GREATER,
        GL_ALWAYS
    };

    return v[co];
};

_SGL GLuint AfxToGlShaderStage(afxShaderStage s)
{
    static GLuint const v[] =
    {
        NIL,
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
        GL_TESS_CONTROL_SHADER,
        GL_TESS_EVALUATION_SHADER,
        GL_COMPUTE_SHADER,
        NIL
    };

    return v[s];
};

_SGL GLuint AfxToGlShaderStageBit(afxShaderStage s)
{
    static GLuint const v[] =
    {
        NIL,
        GL_VERTEX_SHADER_BIT,
        GL_FRAGMENT_SHADER_BIT,
        GL_GEOMETRY_SHADER_BIT,
        GL_TESS_CONTROL_SHADER_BIT,
        GL_TESS_EVALUATION_SHADER_BIT,
        GL_COMPUTE_SHADER_BIT,
        NIL
    };

    return v[s];
};

_SGL GLenum AfxToGlTopology(afxPrimTopology pm)
{
    static GLuint const v[] =
    {
        GL_POINTS,

        GL_LINES,
        GL_LINE_STRIP,

        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,

        GL_LINES_ADJACENCY,
        GL_LINE_STRIP_ADJACENCY,

        GL_TRIANGLES_ADJACENCY,
        GL_TRIANGLE_STRIP_ADJACENCY,

        GL_PATCHES
    };

    return v[pm];
};

_SGL GLenum AfxToGlBufferUsage(afxBufferUsage usage)
{
    static GLenum const v[] =
    {
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        GL_PIXEL_PACK_BUFFER,
        GL_PIXEL_UNPACK_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        GL_ARRAY_BUFFER,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_DRAW_INDIRECT_BUFFER
    };
    //errado
    return v[usage];
};

_SGL GLenum AfxToGlStencilOp(afxStencilOp so)
{
    static GLenum const v[] =
    {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INCR,
        GL_DECR,
        GL_INVERT,
        GL_INCR_WRAP,
        GL_DECR_WRAP
    };

    return v[so];
}

_SGL GLenum AfxToGlBlendOp(afxBlendOp f)
{
    static GLenum const v[] =
    {
        GL_FUNC_ADD,
        GL_FUNC_SUBTRACT,
        GL_FUNC_REVERSE_SUBTRACT,
        GL_MIN,
        GL_MAX
    };

    return v[f];
}

_SGL GLenum AfxToGlLogicOp(afxLogicOp f)
{
    afxError err;
    static GLenum const v[] =
    {
        GL_NOOP,
        GL_CLEAR,
        GL_SET,
        GL_COPY,
        GL_COPY_INVERTED,
        GL_INVERT,
        GL_AND,
        GL_NAND,
        GL_OR,
        GL_NOR,
        GL_XOR,
        GL_EQUIV,
        GL_AND_REVERSE,
        GL_AND_INVERTED,
        GL_OR_REVERSE,
        GL_OR_INVERTED
    };
    AfxAssert(v[afxLogicOp_NOP] == GL_NOOP);
    return v[f];
}

_SGL GLenum AfxToGlBlendFactor(afxBlendFactor f)
{
    static GLenum const v[] =
    {
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_CONSTANT_COLOR,
        GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA,
        GL_ONE_MINUS_CONSTANT_ALPHA,
        GL_SRC_ALPHA_SATURATE,
        GL_SRC1_COLOR,
        GL_ONE_MINUS_SRC1_COLOR,
        GL_SRC1_ALPHA,
        GL_ONE_MINUS_SRC1_ALPHA
    };

    return v[f];
}

_SGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxNat32* stride)
{
    GLint const sizv[] =
    {
        0,
        1,
        2,
        3,
        4,
    };

    //static_assert(afxVertexFormat_V4D == 4, "");

    GLenum const typv[] =
    {
        GL_INVALID_ENUM,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
    };

    if (siz)
        *siz = sizv[fmt];

    if (typ)
        *typ = typv[fmt];

    if (stride)
        *stride = (sizv[fmt] * (typv[fmt] == GL_FLOAT ? sizeof(afxReal) : 0));

    //static_assert(afxVertexFormat_TOTAL == 5, "");

}

_SGL afxChar const sigmaSignature[] =
{
    "\n       ::::::::    :::::::::::    ::::::::    ::::     ::::       :::          "
    "\n      :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:        "
    "\n      +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+       "
    "\n      +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:      "
    "\n             +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+      "
    "\n      #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#      "
    "\n       ########    ###########    ########    ###       ###   ###     ###      "
    "\n                                                                               "
    "\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n              (c) 2017 SIGMA Technology Group --- Federacao SIGMA              "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
};

_SGL afxChar const targaSigmaSignature[];
afxChar const targaSigmaSignature[] =
{
    "\n           :::::::::::     :::     :::::::::   ::::::::      :::               "
    "\n               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             "
    "\n               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            "
    "\n               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           "
    "\n               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           "
    "\n               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           "
    "\n               ###     ###     ### ###    ###  ########  ###     ###           "
    "\n                                                                               "
    "\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n              (c) 2017 SIGMA Technology Group --- Federacao SIGMA              "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
};

_SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;
    
    if (!(rslt = dpu->MakeCurrent(hdc, hrc)))
        //if (!(rslt = wglMakeCurrent(hdc, hrc)))
            AfxThrowError();

    return rslt;
}

_SGL afxError SglSwapBuffers(HDC hdc, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;

    
    if (!(dpu->SwapBuffers(hdc)))
        if (!(SwapBuffers(hdc)))
            AfxThrowError();

    //SetWindowTextA(doutD->wglWnd, doutD->title.buf); // deadlocks mem threads on exit
    //UpdateWindow(doutD->wglWnd);
    //AfxYieldThreading();

    return err;
}

_SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;
    int fmt;

    if (!(fmt = dpu->ChoosePixelFormat(hdc, ppfd)))
        if (!(fmt = ChoosePixelFormat(hdc, ppfd)))
            AfxThrowError();

    return fmt;
}

_SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;

    if (!(rslt = dpu->SetPixelFormat(hdc, format, ppfd)))
        if (!(rslt = SetPixelFormat(hdc, format, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = dpu->DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
        if (!(rslt = DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglGetPixelFormat(HDC hdc, sglDpuIdd const *dpu)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = dpu->GetPixelFormat(hdc)))
        if (!(rslt = GetPixelFormat(hdc)))
            AfxThrowError();

    return rslt;
}

_SGL LRESULT WINAPI _SglWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (dout)
    {
        AfxAssertObjects(1, &dout, afxFcc_DOUT);

        AfxAssert(dout->wnd == hWnd);

        switch (message)
        {
        case WM_CLOSE:
        {

            // Sent as a signal that a window or an application should terminate.
            // wParam = This parameter is not used.
            // lParam = This parameter is not used.
            // If an application processes this message, it should return zero.

            //_SglDoutProcess(dout);

            //while (0 < AfxReleaseObject(&doutD->obj));
#if 0
            afxDrawContext dctx = AfxGetDrawOutputContext(dout);

            if (!dctx) AfxReleaseDrawOutput(dout);
            else
            {
                struct msgdecl { afxSize tid; afxSize hwnd; afxSize wParam; afxSize lParam; } *msgdecl = AfxAllocate(NIL, sizeof(struct msgdecl), 1, AfxHint());
                msgdecl->tid = GetThreadId(dctxD->thread);
                msgdecl->hwnd = hWnd;
                msgdecl->wParam = wParam;
                msgdecl->lParam = lParam;
                PostThreadMessageA(msgdecl->tid, message, NIL, msgdecl);
            }
#endif//0
            return 0; // handled. Also prevent process at end of this function
        }
        case WM_SIZE:
        {
            //if (doutD->resizable)
            {
                /*
                    WM_SIZE
                        Sent to a window after its size has changed.

                    wParam --- The type of resizing requested. This parameter canv be one of the following values.
                        SIZE_MAXHIDE = 4 --- Message is sent to mem pop-up windows when some other window is maximized.
                        SIZE_MAXIMIZED = 2 --- The window has been maximized.
                        SIZE_MAXSHOW = 3 --- Message is sent to mem pop-up windows when some other window has been restored to its former size.
                        SIZE_MINIMIZED = 1 --- The window has been minimized.
                        SIZE_RESTORED = 0 --- The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

                    lParam
                        The low-order word of lParam specifies the new width of the client area.
                        The high-order word of lParam specifies the new height of the client area.

                */

                afxWhd whdNew = { LOWORD(lParam), HIWORD(lParam), 1 }; // já é client area.

                if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
                {
                    AfxReadjustDrawOutput(dout, whdNew);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            afxWhd const resolution = { GetDeviceCaps(dout->dc, HORZRES), GetDeviceCaps(dout->dc, VERTRES), GetDeviceCaps(dout->dc, PLANES) };
            dout->base.refreshRate = GetDeviceCaps(dout->dc, VREFRESH);
            afxReal64 physAspRatio = AfxTryFindPhysicalAspectRatio(GetDeviceCaps(dout->dc, HORZSIZE), GetDeviceCaps(dout->dc, VERTSIZE));
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
            
            RECT rect;
            GetClientRect(dout->wnd, &(rect));
            afxWhd whdNew = { rect.right - rect.left, rect.bottom - rect.top, 1 };

            if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
            {
                AfxReadjustDrawOutput(dout, whdNew);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxSubV2d(dout->base.absCursorMove, dout->base.absCursorPos, curr);
            AfxCopyV2d(dout->base.absCursorPos, curr);

            afxV2d screen = { AfxScalar(dout->base.whd[0]), AfxScalar(dout->base.whd[1]) };

            AfxExtractV2dInNdc(dout->base.ndcCursorPos, dout->base.absCursorPos, screen);
            AfxExtractV2dInNdc(dout->base.ndcCursorMove, dout->base.absCursorMove, screen);

            //data2->breake = TRUE;
            break;
        }
#if 0
        case WM_DROPFILES: // Sent when the user drops a file on the window of an application that has registered itself as a recipient of dropped files.
        {
            // TODO build a kind of handler for draw input to deal with it.

            afxFileDrop fdrop;
            AFX_ZERO(&fdrop);

            POINT ppt;
            HDROP hDrop = (HDROP)wParam;
            DragQueryPoint(hDrop, &ppt);

            fdrop.x = ppt.x;
            fdrop.y = ppt.y;
            afxNat cnt = DragQueryFileA(hDrop, 0xFFFFFFFF, NIL, NIL);

            AfxAllocateArray(&fdrop.files, sizeof(afxChar const*), 0, AfxHint());

            afxChar* name = NIL;
            afxNat len = 0;
            afxNat i;

            afxDrawContext dctx;
            AfxGetDrawOutputContext(dout, &dctx);
            AfxAssertType(dctxD, afxFcc_DCTX);
            afxMmu mmu = AfxGetDrawContextMmu(dctx);
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            for (i = 0; i < cnt; i++)
            {
                len = DragQueryFileA(hDrop, i, NIL, 0);

                if (!(name = AfxAllocate(mmu, len + 1, 1, 0, AfxHint()))) AfxError("");
                else
                {
                    DragQueryFileA(hDrop, i, name, len + 1);
                    afxNat arrIdx;
                    void *arrel = AfxInsertArrayUnit(&fdrop.files, &arrIdx);
                    AfxCopy(1, sizeof(name), name, arrel);
                }
            }

            //afxEvent ev;
            //AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &doutD->obj, &fdrop);
            //AfxObjectEmitEvent(&doutD->obj, &ev);

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxEcho("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
            }

            for (i = 0; i < AfxCountArrayElements(&fdrop.files); i++)
            {
                AfxDeallocate(mmu, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
            }

            AfxDeallocateArray(&fdrop.files);

            DragFinish(hDrop);
            //data2->breake = TRUE;
            //return 0;
            break;
        }
#endif
        case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
        {
            dout->base.focused = FALSE;
            // TODO pause draw input to reduce resource usage?

            //BinkPause(Back_Bink, 1);
            //BinkPause(Alpha_Bink, 1);
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            dout->base.focused = TRUE;
            // TODO resume draw input of pause to reduce resource usage?

            //BinkPause(Back_Bink, 0);
            //BinkPause(Alpha_Bink, 0);
            break;
        }
#if !0
        case WM_PAINT:
        {
            ValidateRect(hWnd, NULL);
#if 0
            DefWindowProc(hWnd, message, wParam, lParam);
            afxDrawContext dctx = AfxGetDrawOutputContext(dout);

            if (dctx)
            {
                afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
                afxNat unitIdx;
                AfxGetThreadingUnit(&unitIdx);
                sglDpuIdd* dpu = &ddev->dpus[unitIdx];

                
                SglSwapBuffers(dpu->dc, dpu);
            }
#endif
            return 0;
        }
#endif
        case WM_ERASEBKGND:
        {
            ValidateRect(hWnd, NULL);
            // Flicker is usually caused by interference via WM_ERASEBKGND. If you haven't already, intercept WM_ERASEBKGND and do nothing in the regions where you are displaying OpenGL content.
            return 1;
        }
        default: break;
        }
        //_SglDoutProcess(dout);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
1  2  3  4
2  1  4  3
3  4  1  2
4  3  2  1
*/
#if 0
_SGL afxError _SglCreateCombinedDeviceContext(afxDrawDevice ddev, afxNat unitIdx)
{
    //AfxEntry("oglWndClss=%p,shareCtx=%p,phwnd=%p,phdc=%p,phrc=%p", ddev->oglWndClss, 0, phwnd, phdc, phrc);
    afxError err = AFX_ERR_NONE;
    HGLRC(WINAPI*_wglCreateContext)(HDC);
    BOOL(WINAPI*_wglDeleteContext)(HGLRC);
    HGLRC(WINAPI*_wglGetCurrentContext)(VOID);
    HDC(WINAPI*_wglGetCurrentDC)(VOID);
    PROC(WINAPI*_wglGetProcAddress)(LPCSTR);
    BOOL(WINAPI*_wglMakeCurrent)(HDC, HGLRC);
    int(WINAPI*_wglChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    int(WINAPI*_wglDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
    BOOL(WINAPI*_wglSetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    int(WINAPI*_wglGetPixelFormat)(HDC hdc);

    void* opengl32 = ddev->opengl32;
    wglVmt* wgl = (wglVmt*)&ddev->dpus[unitIdx].wgl;
    //SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, wgl->ptr, 0);

    _wglCreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
    _wglDeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
    _wglGetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
    _wglGetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
    _wglGetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    _wglMakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
    _wglChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    _wglSetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    _wglDescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
    _wglGetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");

    HDC bkpHdc = _wglGetCurrentDC();
    HGLRC bkpGlrc = _wglGetCurrentContext();

    afxNat32 wndStyles = WS_DISABLED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    HWND tmpHwnd = CreateWindowA(ddev->wndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, ddev->wndClss.hInstance, ddev);

    if (!(tmpHwnd))
        AfxThrowError();
    else
    {
        HDC tmpHdc = GetDC(tmpHwnd);
        //hdc = GetWindowDC(hwnd);

        if (!(tmpHdc)) AfxThrowError();
        else
        {
            PIXELFORMATDESCRIPTOR pfd = { 0 };
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SUPPORT_COMPOSITION | PFD_SWAP_EXCHANGE;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = GetDeviceCaps(tmpHdc, BITSPIXEL);
            //pfd.cDepthBits = 24;
            //pfd.cStencilBits = 8;

            int pxlfmt;

            if (!(pxlfmt = _wglChoosePixelFormat(tmpHdc, &(pfd))))
                if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &(pfd))))
                    AfxThrowError();

            if (!_wglDescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd))
                if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                    AfxThrowError();

            if (!_wglSetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                if (!SetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                    AfxThrowError();
            
            HGLRC tmpHrc = _wglCreateContext(tmpHdc);

            if (!tmpHrc) AfxThrowError();
            else
            {
                if (!(_wglMakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                else
                {
                    SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, wgl->ptr, 0);
                    AfxAssert(wgl->ChoosePixelFormatARB);

                    if (!(ddev->dpus[unitIdx].wnd = CreateWindowA(ddev->wndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, ddev->wndClss.hInstance, &ddev->dpus[unitIdx]))) AfxThrowError();
                    else
                    {
                        if (!(ddev->dpus[unitIdx].dc = GetDC(ddev->dpus[unitIdx].wnd))) AfxThrowError();
                        else
                        {
                            /*
                            WGL_SWAP_METHOD_ARB
                            If the pixel format supports a back buffer, then this indicates
                            how they are swapped. If this attribute is set to
                            WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back
                            buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping
                            copies the back buffer contents to the front buffer; if it is
                            set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are
                            copied to the front buffer but the back buffer contents are
                            undefined after the operation. If the pixel format does not
                            support a back buffer then this parameter is set to
                            WGL_SWAP_UNDEFINED_ARB. The <iLayerPlane> parameter is ignored
                            if this attribute is specified.
                            */
                            int pxlAttrPairs[][2] =
                            {
                                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                                //{ WGL_SUPPORT_GDI_ARB, GL_TRUE },
                                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                                { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                                { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB }, // WGL_TYPE_RGBA_FLOAT_ARB
                                { WGL_COLOR_BITS_ARB, GetDeviceCaps(ddev->dpus[unitIdx].dc, BITSPIXEL) },
                                //{ WGL_ALPHA_BITS_ARB, 8 },
                                //{ WGL_DEPTH_BITS_ARB, 0 },
                                //{ WGL_STENCIL_BITS_ARB, 0 },
                                //{ WGL_TRANSPARENT_ARB, GL_TRUE },
                                { WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
                                //{ WGL_SAMPLE_BUFFERS_ARB, GL_TRUE },  // works on Intel, didn't work on Mesa
                                //{ WGL_SAMPLES_ARB, 8 },  // works on Intel, didn't work on Mesa
                                //{ WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT }, // WGL_COLORSPACE_LINEAR_EXT // works on Mesa, didn't work on Intel
                                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // works on Mesa, didn't work on Intel
                                { NIL, NIL },
                            };

                            UINT formatCount;
                            ddev->dpus[unitIdx].dcPxlFmt = 0;

                            if (!wgl->ChoosePixelFormatARB(ddev->dpus[unitIdx].dc, &pxlAttrPairs[0][0], 0, 1, &(ddev->dpus[unitIdx].dcPxlFmt), &(formatCount))) AfxThrowError();
                            else
                            {
                                AfxAssert(ddev->dpus[unitIdx].dcPxlFmt);
                                AfxAssert(formatCount);

                                AFX_ZERO(&ddev->dpus[unitIdx].dcPfd);
                                SglDescribePixelFormat(ddev->dpus[unitIdx].dc, ddev->dpus[unitIdx].dcPxlFmt, sizeof(pfd), &(ddev->dpus[unitIdx].dcPfd), &wgl);

                                if (!(SglSetPixelFormat(ddev->dpus[unitIdx].dc, ddev->dpus[unitIdx].dcPxlFmt, &(ddev->dpus[unitIdx].dcPfd), &wgl))) AfxThrowError();
                                else
                                {
                                    int ctxAttrPairs[][2] =
                                    {
                                        { WGL_CONTEXT_MAJOR_VERSION_ARB, 3 },
                                        { WGL_CONTEXT_MINOR_VERSION_ARB, 3 },
                                        { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB }, // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                                        { WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
        #ifdef _AFX_DEBUG
                                        | WGL_CONTEXT_DEBUG_BIT_ARB
        #endif
                                        },
                                        { NIL, NIL }
                                    };

                                    if (!(ddev->dpus[unitIdx].glrc = wgl->CreateContextAttribsARB(ddev->dpus[unitIdx].dc, unitIdx ? ddev->dpus[unitIdx].glrc : NIL, (void*)ctxAttrPairs))) AfxThrowError();
                                    else
                                    {
                                        if (!(wgl->MakeCurrent(ddev->dpus[unitIdx].dc, ddev->dpus[unitIdx].glrc))) AfxThrowError();
                                        else
                                        {
                                            SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, wgl->ptr, 0 == unitIdx);
                                            SglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(glVmtNames) / sizeof(glVmtNames[0]), glVmtNames, &ddev->dpus[unitIdx].gl.ptr, 0 == unitIdx);

                                            wgl->MakeCurrent(tmpHdc, tmpHrc);
                                        }

                                        if (err && ddev->dpus[unitIdx].glrc)
                                            wgl->DeleteContext(ddev->dpus[unitIdx].glrc);
                                    }
                                }
                            }

                            if (err && ddev->dpus[unitIdx].dc)
                                ReleaseDC(ddev->dpus[unitIdx].wnd, ddev->dpus[unitIdx].dc);
                        }

                        if (err && ddev->dpus[unitIdx].wnd)
                            DestroyWindow(ddev->dpus[unitIdx].wnd);
                    }
                }

                if (tmpHrc)
                    _wglDeleteContext(tmpHrc);
            }

            if (tmpHdc)
                ReleaseDC(tmpHwnd, tmpHdc);
        }

        if (tmpHwnd)
            DestroyWindow(tmpHwnd);
    }
    _wglMakeCurrent(bkpHdc, bkpGlrc);
    return err;
}
#endif

_SGL afxError _SglDdevProcessResDel(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    sglDpuIdd *dpu = &ddev->dpus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    glVmt const *gl = &dpu->gl;

    //HDC dc = wgl->GetCurrentDC();
    HGLRC rc = dpu->GetCurrentContext();
    AfxAssert(rc == dpu->glrc);

    if (/*dc != idd->wglDc || */rc != dpu->glrc)
    {
        if (!dpu->MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            dpu->MakeCurrent(dpu->dc, dpu->glrc);
        }
    }

    _sglDeleteGlRes *delRes;

    if (AfxTryEnterSlockExclusive(&dpu->deletionLock))
    {
        if ((delRes = AfxPullNextQueueUnit(&dpu->deletionQueue)))
        {
            switch (delRes->type)
            {
            case 0: // buf
                AfxAssert(gl->IsBuffer(delRes->gpuHandle));
                gl->DeleteBuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsBuffer(delRes->gpuHandle));
                break;
            case 1: // tex
                AfxAssert(gl->IsTexture(delRes->gpuHandle));
                gl->DeleteTextures(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsTexture(delRes->gpuHandle));
                break;
            case 2: // surf
                AfxAssert(gl->IsRenderBuffer(delRes->gpuHandle));
                gl->DeleteRenderbuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsRenderBuffer(delRes->gpuHandle));
                break;
            case 3: // canv
                AfxAssert(gl->IsFramebuffer(delRes->gpuHandle));
                gl->DeleteFramebuffers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsFramebuffer(delRes->gpuHandle));
                break;
            case 4: // smp
                AfxAssert(gl->IsSampler(delRes->gpuHandle));
                gl->DeleteSamplers(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsSampler(delRes->gpuHandle));
                break;
            case 5: // pip
                AfxAssert(gl->IsProgram(delRes->gpuHandle));
                gl->DeleteProgram(delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsProgram(delRes->gpuHandle));
                break;
            case 6: // shd
                AfxAssert(gl->IsShader(delRes->gpuHandle));
                gl->DeleteShader(delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsShader(delRes->gpuHandle));
                break;
            case 7: // vao
                AfxAssert(gl->IsVertexArray(delRes->gpuHandle));
                gl->GenVertexArrays(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsVertexArray(delRes->gpuHandle));
                break;
            case 8: // pip
            {
                AfxAssert(gl->IsProgramPipeline(delRes->gpuHandle));
                gl->DeleteProgramPipelines(1, &delRes->gpuHandle); _SglThrowErrorOccuried();
                AfxAssert(!gl->IsProgramPipeline(delRes->gpuHandle));
                break;
            }
            default:
                AfxError("");
                break;
            }

            delRes->gpuHandle = 0;
            delRes->type = 0;

            AfxPopNextQueue(&dpu->deletionQueue);
        }
        AfxExitSlockExclusive(&dpu->deletionLock);
    }
    return err;
}

_SGL void _SglDctxDeleteGlRes(afxDrawContext dctx, afxNat type, GLuint gpuHandle)
{
    //AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    afxDrawDevice ddev = AfxGetObjectProvider(dctx);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);    
    
    afxNat32 unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    sglDpuIdd*dpu = &ddev->dpus[unitIdx];
    AfxEnterSlockExclusive(&dpu->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandle = gpuHandle;
    delRes.type = type;
    
    if (AfxPushQueueUnit(&dpu->deletionQueue, &delRes))
        AfxThrowError();

    AfxExitSlockExclusive(&dpu->deletionLock);
};

_SGL void APIENTRY _glDbgMsgCb(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    afxChar*sourceStr, *typeStr, *severityStr;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
    case GL_DEBUG_TYPE_MARKER: typeStr = "MARKER"; break;
    case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
    case GL_DEBUG_SEVERITY_LOW: severityStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_HIGH: severityStr = "HIGH"; break;
    }
    AfxError("%s %s %s %u %s", sourceStr, typeStr, severityStr, id, message);
}

_SGL void _SglDpuLoadBaseSymbols(afxDrawDevice ddev, afxNat unitIdx)
{
    sglDpuIdd *dpu = &ddev->dpus[unitIdx];
    HMODULE opengl32 = dpu->opengl32;
    dpu->CreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
    dpu->DeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
    dpu->GetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
    dpu->GetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
    dpu->GetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    dpu->MakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
    dpu->ChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    dpu->SetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    dpu->DescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
    dpu->GetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");
    dpu->SwapBuffers = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
    dpu->SwapIntervalEXT = (void*)dpu->GetProcAddress("wglSwapIntervalEXT");
    
    if (!dpu->CreateContext)
        dpu->CreateContext = (void*)GetProcAddress(opengl32, "DrvCreateContext");

    if (!dpu->DeleteContext)
        dpu->DeleteContext = (void*)GetProcAddress(opengl32, "DrvDeleteContext");

    if (!dpu->GetCurrentContext)
        dpu->GetCurrentContext = (void*)GetProcAddress(opengl32, "DrvGetCurrentContext");

    if (!dpu->GetCurrentDC)
        dpu->GetCurrentDC = (void*)GetProcAddress(opengl32, "DrvGetCurrentDC");

    if (!dpu->GetProcAddress)
        dpu->GetProcAddress = (void*)GetProcAddress(opengl32, "DrvGetProcAddress");

    if (!dpu->MakeCurrent)
        dpu->MakeCurrent = (void*)GetProcAddress(opengl32, "DrvMakeCurrent");

    if (!dpu->ChoosePixelFormat)
        dpu->ChoosePixelFormat = (void*)GetProcAddress(opengl32, "DrvChoosePixelFormat");
        
    if (!dpu->SetPixelFormat)
        dpu->SetPixelFormat = (void*)GetProcAddress(opengl32, "DrvSetPixelFormat");
            
    if (!dpu->DescribePixelFormat)
        dpu->DescribePixelFormat = (void*)GetProcAddress(opengl32, "DrvDescribePixelFormat");
                
    if (!dpu->GetPixelFormat)
        dpu->GetPixelFormat = (void*)GetProcAddress(opengl32, "DrvGetPixelFormat");

    if (!dpu->SwapBuffers)
        dpu->SwapBuffers = (void*)GetProcAddress(opengl32, "DrvSwapBuffers");
}

_SGL afxError _SglBuildDpu(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    //afxNat procUnitIdx = AfxGetThreadingUnit();
    sglDpuIdd *dpu = &ddev->dpus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    glVmt const* gl = &dpu->gl;

    afxMmu mmu = AfxGetDrawSystemMmu();

    dpu->numOfFedIndices = 0;
    dpu->numOfFedVertices = 0;
    dpu->numOfFedInstances = 0;

    dpu->submissionSuspended = FALSE;

    dpu->activeTmpFboIdx = 0;
    dpu->activeVaoIdx = 0;

    AfxZero(1, sizeof(dpu->vao), dpu->vao);
    AfxZero(1, sizeof(dpu->tmpFbo), dpu->tmpFbo);

    // RESET DEFAULT STATE TO OPENGL DEFAULTS

    //AfxZero(1, sizeof(dpu->state), &dpu->state);
    AfxZero(1, sizeof(dpu->activeRasterState), &dpu->activeRasterState);
    AfxZero(1, sizeof(dpu->nextRasterState), &dpu->nextRasterState);
    AfxZero(1, sizeof(dpu->activeResBind), &dpu->activeResBind);
    AfxZero(1, sizeof(dpu->nextResBind), &dpu->nextResBind);
    AfxZero(1, sizeof(dpu->activeXformState), &dpu->activeXformState);
    AfxZero(1, sizeof(dpu->nextXformState), &dpu->nextXformState);
    dpu->nextScissorUpdCnt = 0;
    dpu->nextViewportUpdCnt = 0;
    dpu->nextVinAttribUpdCnt = 0;
    dpu->nextVinBindingsCnt = 0;

    dpu->activeRasterState.depthCompareOp = afxCompareOp_LESS;
    dpu->activeRasterState.depthWriteEnabled = TRUE;
    dpu->activeRasterState.depthBounds[0] = 0.f;
    dpu->activeRasterState.depthBounds[1] = 1.f;
    dpu->activeRasterState.stencilFront.compareOp = afxCompareOp_ALWAYS;
    dpu->activeRasterState.stencilFront.reference = 0;
    dpu->activeRasterState.stencilFront.compareMask = (dpu->activeRasterState.stencilFront.writeMask = 0xFFFFFFFF);
    dpu->activeRasterState.stencilFront.failOp = (dpu->activeRasterState.stencilFront.passOp = (dpu->activeRasterState.stencilFront.depthFailOp = afxStencilOp_KEEP));
    dpu->activeRasterState.stencilBack.compareOp = afxCompareOp_ALWAYS;
    dpu->activeRasterState.stencilBack.reference = 0;
    dpu->activeRasterState.stencilBack.compareMask = (dpu->activeRasterState.stencilBack.writeMask = 0xFFFFFFFF);
    dpu->activeRasterState.stencilBack.failOp = (dpu->activeRasterState.stencilBack.passOp = (dpu->activeRasterState.stencilBack.depthFailOp = afxStencilOp_KEEP));
    dpu->activeRasterState.depthBiasConstFactor = 0.f;
    dpu->activeRasterState.depthBiasSlopeScale = 0.f;

    for (afxNat i = 0; i < 8; i++)
    {
        dpu->activeRasterState.outs[i].blendConfig.aBlendOp = afxBlendOp_ADD;
        dpu->activeRasterState.outs[i].blendConfig.rgbBlendOp = afxBlendOp_ADD;
        dpu->activeRasterState.outs[i].blendConfig.aSrcFactor = afxBlendFactor_ONE;
        dpu->activeRasterState.outs[i].blendConfig.rgbSrcFactor = afxBlendFactor_ONE;
        dpu->activeRasterState.outs[i].blendConfig.aDstFactor = afxBlendFactor_ZERO;
        dpu->activeRasterState.outs[i].blendConfig.rgbDstFactor = afxBlendFactor_ZERO;
        dpu->activeRasterState.outs[i].discardMask = NIL;
    }

    

    ///

    AfxTakeSlock(&dpu->deletionLock);
    AfxAllocateQueue(&dpu->deletionQueue, sizeof(_sglDeleteGlRes), 32);
    HMODULE opengl32;

    if (!(dpu->opengl32 = (opengl32 = LoadLibraryA("opengl32.dll")))) AfxError("");
    else
    {
        _SglDpuLoadBaseSymbols(ddev, unitIdx);

        HDC bkpHdc = dpu->GetCurrentDC();
        HGLRC bkpGlrc = dpu->GetCurrentContext();

        { // hide temp stuff
            afxNat32 wndStyles = WS_DISABLED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED;
            HWND tmpHwnd;
            HDC tmpHdc;
            HGLRC tmpHrc;

            if (!(tmpHwnd = CreateWindowA(ddev->wndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, ddev->wndClss.hInstance, ddev))) AfxThrowError();
            else
            {
                if (!(tmpHdc = GetDC(tmpHwnd))) AfxThrowError();
                else
                {
                    PIXELFORMATDESCRIPTOR pfd = { 0 };
                    pfd.nSize = sizeof(pfd);
                    pfd.nVersion = 1;
                    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL/* | PFD_DIRECT3D_ACCELERATED | PFD_SUPPORT_COMPOSITION | PFD_SWAP_EXCHANGE*/;
                    pfd.iPixelType = PFD_TYPE_RGBA;
                    pfd.cColorBits = GetDeviceCaps(tmpHdc, BITSPIXEL);
                    //pfd.cDepthBits = 24;
                    //pfd.cStencilBits = 8;

                    int pxlfmt;

                    //if (!(pxlfmt = dpu->ChoosePixelFormat(tmpHdc, &(pfd))))
                        if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &(pfd))))
                            AfxThrowError();

                    //if (!(dpu->DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                        if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                            AfxThrowError();

                    //if (!(dpu->SetPixelFormat(tmpHdc, pxlfmt, &(pfd))))
                        if (!SetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                            AfxThrowError();

                    AfxComment("Creating transient OpenGL context");

                    if (!(tmpHrc = dpu->CreateContext(tmpHdc))) AfxThrowError();
                    else
                    {
                        if (!(dpu->MakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                        else
                        {
                            _SglDpuLoadBaseSymbols(ddev, unitIdx);

                            afxString ver;
                            afxNat verMajor, verMinor, verPatch;
                            PFNGLGETSTRINGPROC _glGetString = (void*)dpu->GetProcAddress("glGetString");
                            AfxMakeString(&ver, (afxChar const*)_glGetString(GL_VERSION), 0);
                            AfxScanString(&ver, "%u.%u.%u", &verMajor, &verMinor, &verPatch);
                            dpu->verMajor = verMajor;
                            dpu->verMinor = verMinor;
                            dpu->verPatch = verPatch;
                            
                            if (!(dpu->wnd = CreateWindowA(ddev->wndClss.lpszClassName, "", wndStyles, 0, 0, 1, 1, NIL, NIL, ddev->wndClss.hInstance, dpu))) AfxThrowError();
                            else
                            {
                                if (!(dpu->dc = GetDC(dpu->wnd))) AfxThrowError();
                                else
                                {
                                    /*
                                    WGL_SWAP_METHOD_ARB
                                    If the pixel format supports a back buffer, then this indicates
                                    how they are swapped. If this attribute is set to
                                    WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back
                                    buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping
                                    copies the back buffer contents to the front buffer; if it is
                                    set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are
                                    copied to the front buffer but the back buffer contents are
                                    undefined after the operation. If the pixel format does not
                                    support a back buffer then this parameter is set to
                                    WGL_SWAP_UNDEFINED_ARB. The <iLayerPlane> parameter is ignored
                                    if this attribute is specified.
                                    */
                                    int pxlAttrPairs[][2] =
                                    {
                                        { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                                        { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                                        //{ WGL_SUPPORT_GDI_ARB, GL_TRUE },
                                        { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                                        { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                                        { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB }, // WGL_TYPE_RGBA_FLOAT_ARB
                                        { WGL_COLOR_BITS_ARB, GetDeviceCaps(ddev->dpus[unitIdx].dc, BITSPIXEL) },
                                        //{ WGL_ALPHA_BITS_ARB, 8 },
                                        //{ WGL_DEPTH_BITS_ARB, 0 },
                                        //{ WGL_STENCIL_BITS_ARB, 0 },
                                        //{ WGL_TRANSPARENT_ARB, GL_TRUE },
                                        { WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
                                        { WGL_SAMPLE_BUFFERS_ARB, GL_FALSE },
                                        //{ WGL_SAMPLE_BUFFERS_ARB, GL_TRUE },  // works on Intel, didn't work on Mesa
                                        //{ WGL_SAMPLES_ARB, 8 },  // works on Intel, didn't work on Mesa
                                        //{ WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT }, // WGL_COLORSPACE_LINEAR_EXT // works on Mesa, didn't work on Intel
                                        //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // works on Mesa, didn't work on Intel
                                        { NIL, NIL },
                                    };

                                    PFNWGLCHOOSEPIXELFORMATARBPROC _wglChoosePixelFormatARB = (void*)dpu->GetProcAddress("wglChoosePixelFormatARB");
                                    AfxAssert(_wglChoosePixelFormatARB);
                                    UINT formatCount;
                                    dpu->dcPxlFmt = 0;

                                    if (!_wglChoosePixelFormatARB(dpu->dc, &pxlAttrPairs[0][0], 0, 1, &(dpu->dcPxlFmt), &(formatCount))) AfxThrowError();
                                    else
                                    {
                                        AfxAssert(dpu->dcPxlFmt);
                                        AfxAssert(formatCount);
                                        AfxZero(1, sizeof(dpu->dcPfd), &dpu->dcPfd);

                                        if (!(dpu->DescribePixelFormat(dpu->dc, dpu->dcPxlFmt, sizeof(dpu->dcPfd), &dpu->dcPfd)))
                                            if (!(DescribePixelFormat(dpu->dc, dpu->dcPxlFmt, sizeof(dpu->dcPfd), &dpu->dcPfd)))
                                                AfxThrowError();

                                        if (!(dpu->SetPixelFormat(dpu->dc, dpu->dcPxlFmt, &(dpu->dcPfd))))
                                            if (!SetPixelFormat(dpu->dc, dpu->dcPxlFmt, &(dpu->dcPfd)))
                                                AfxThrowError();

                                        int ctxAttrPairs[][2] =
                                        {
                                            { WGL_CONTEXT_MAJOR_VERSION_ARB, verMajor },
                                            { WGL_CONTEXT_MINOR_VERSION_ARB, verMinor },
                                            { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB }, // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                                            { WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
    #ifdef _AFX_DEBUG
                                            | WGL_CONTEXT_DEBUG_BIT_ARB
    #endif
                                            },
                                            { NIL, NIL }
                                        };


                                        AfxComment("Creating definitive OpenGL context");

                                        PFNWGLCREATECONTEXTATTRIBSARBPROC _wglCreateContextAttribsARB = (void*)dpu->GetProcAddress("wglCreateContextAttribsARB");
                                        AfxAssert(_wglCreateContextAttribsARB);

                                        if (!(dpu->glrc = _wglCreateContextAttribsARB(dpu->dc, unitIdx ? ddev->dpus[0].glrc : NIL, (void*)ctxAttrPairs))) AfxThrowError();
                                        else
                                        {
                                            if (!(dpu->MakeCurrent(dpu->dc, dpu->glrc))) AfxThrowError();
                                            else
                                            {
                                                _SglDpuLoadBaseSymbols(ddev, unitIdx);
                                                //SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, (void**)&dpu->wgl.ptr, 0 == unitIdx);
                                                SglLoadOpenGlVmt(opengl32, 0, sizeof(glVmtNames) / sizeof(glVmtNames[0]), glVmtNames, (void**)&dpu->gl.ptr, 0 == unitIdx);

                                                if (0 == unitIdx)
                                                {
                                                    GLint n = 0;
                                                    gl->GetIntegerv(GL_NUM_EXTENSIONS, &n); _SglThrowErrorOccuried();
                                                    AfxEcho("Listing #%03u \"core\" GL supported extensions.", n);

                                                    for (GLint i = 0; i < n; i++)
                                                    {
                                                        const char* extension = (const char*)gl->GetStringi(GL_EXTENSIONS, i); _SglThrowErrorOccuried();
                                                        AfxEcho("%.3u %s ext found.", i, extension);
                                                    }
                                                }

                                                if (err)
                                                    dpu->MakeCurrent(tmpHdc, tmpHrc);
                                            }
                                            if (err)
                                                dpu->DeleteContext(dpu->glrc);
                                        }
                                    }
                                    if (err)
                                        ReleaseDC(dpu->wnd, dpu->dc);
                                }
                                if (err)
                                    DestroyWindow(dpu->wnd);
                            }

                            if (err)
                                dpu->MakeCurrent(bkpHdc, bkpGlrc);
                        }
                        dpu->DeleteContext(tmpHrc); // disable for detour of some bug caused by OBS
                    }
                    ReleaseDC(tmpHwnd, tmpHdc);
                }
                DestroyWindow(tmpHwnd);
            }
        }

        if (!err)
        {
            AfxAssert(dpu->dc == dpu->GetCurrentDC());
            AfxAssert(dpu->glrc = dpu->GetCurrentContext());

            afxNat aa = GetDeviceCaps(dpu->dc, HORZSIZE);
            afxNat ab = GetDeviceCaps(dpu->dc, VERTSIZE);
            afxNat ac = GetDeviceCaps(dpu->dc, HORZRES);
            afxNat ad = GetDeviceCaps(dpu->dc, VERTRES);
            afxNat ae = GetDeviceCaps(dpu->dc, LOGPIXELSX);
            afxNat af = GetDeviceCaps(dpu->dc, LOGPIXELSY);
            afxNat ag = GetDeviceCaps(dpu->dc, BITSPIXEL);
            afxNat ah = GetDeviceCaps(dpu->dc, PLANES);
            afxNat ai = GetDeviceCaps(dpu->dc, ASPECTX);
            afxNat aj = GetDeviceCaps(dpu->dc, ASPECTY);
            afxNat ak = GetDeviceCaps(dpu->dc, ASPECTXY);
            afxNat al = GetDeviceCaps(dpu->dc, VREFRESH);
            afxNat am = GetDeviceCaps(dpu->dc, BLTALIGNMENT);
            AfxAdvertise("%i %i %i %i %i %i %i %i %i %i %i %i %i ", aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am);

#if 0
            HWND dsk = GetDesktopWindow();
            HDC dskWndDc2 = GetWindowDC(dsk);
            HDC dskWndDc = GetWindowDC(NULL);
            HDC dskDc2 = GetDC(dsk);
            HDC dskDc = GetDC(NULL);
            int dskPf = SglGetPixelFormat(dskDc, wgl);
            int dskWndPf = SglGetPixelFormat(dskWndDc, wgl);
            PIXELFORMATDESCRIPTOR pfd, pfd2;
            SglDescribePixelFormat(dskDc, dskPf, sizeof(pfd), &pfd, wgl);
            SglDescribePixelFormat(dskWndDc, dskWndPf, sizeof(pfd2), &pfd2, wgl);
#endif
            //SglLoadOpenGlVmt(ddev->opengl32, 30, glVmtRanges[1][0] - 30, glVmtNames, &gl->ptr[0], 0); // only load get integer, string, etc.

            afxString ver;
            AfxMakeString(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxScanString(&ver, "%u.%u.%u", &dpu->verMajor, &dpu->verMinor, &dpu->verPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dpu->verMajor)); _SglThrowErrorOccuried();
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dpu->verMinor)); _SglThrowErrorOccuried();
            AfxMakeString(&dpu->subsysName, (afxChar const*)gl->GetString(GL_RENDERER), 0); _SglThrowErrorOccuried();
            AfxMakeString(&dpu->subsysVer, (afxChar const*)gl->GetString(GL_VERSION), 0); _SglThrowErrorOccuried();

            AfxLogMessageFormatted(0xFFFF0000, "\n Opening the SIGMA GL/2 Drawing Execution Port %.03u\n\t%.*s %.*s", unitIdx, AfxPushString(&dpu->subsysName), AfxPushString(&dpu->subsysVer));

            PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB = (void*)dpu->GetProcAddress("wglGetExtensionsStringARB");
            AfxAssert(_wglGetExtensionsStringARB);

            AfxAssist("%s", _wglGetExtensionsStringARB(dpu->dc));

            //gl->Enable(GL_FRAMEBUFFER_SRGB);
            gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS); _SglThrowErrorOccuried();
            //gl->ClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE); _SglThrowErrorOccuried();// set screen origin to top to bottm, and set depth to [ 0, 1 ]
            
#if _AFX_DEBUG
            gl->Enable(GL_DEBUG_OUTPUT);
            gl->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            gl->DebugMessageCallback(_glDbgMsgCb, NIL);
            //glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

            GLfloat dataf;
            GLfloat dataf2[3];
            GLint datai;
            GLint datai2[3];
            afxDrawDeviceLimits limits = { 0 };
            gl->GetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &datai); _SglThrowErrorOccuried();
            limits.maxSampleMaskWords = datai;
            gl->GetIntegerv(GL_MAX_CLIP_DISTANCES, &datai); _SglThrowErrorOccuried();
            limits.maxClipDistances = datai; // at least 8

            gl->GetIntegerv(GL_MAX_VIEWPORTS, &datai); _SglThrowErrorOccuried();
            limits.maxViewports = datai; // at least 16
            gl->GetIntegerv(GL_MAX_VIEWPORT_DIMS, datai2); _SglThrowErrorOccuried();
            limits.maxViewportDimensions[0] = datai2[0];
            limits.maxViewportDimensions[1] = datai2[1];
            gl->GetFloatv(GL_VIEWPORT_BOUNDS_RANGE, dataf2); _SglThrowErrorOccuried();
            limits.viewportBoundsRange[0] = dataf2[0];  // at least [-32768, 32767]
            limits.viewportBoundsRange[1] = dataf2[1];  // at least [-32768, 32767]
            gl->GetIntegerv(GL_VIEWPORT_SUBPIXEL_BITS, &datai); _SglThrowErrorOccuried();
            limits.viewportSubPixelBits = datai; // at least 0

            gl->GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &datai); _SglThrowErrorOccuried();
            limits.maxColorAttachments = datai; // at least 16384
            gl->GetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &datai); _SglThrowErrorOccuried();
            limits.maxFramebufferWidth = datai; // at least 16384
            gl->GetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &datai); _SglThrowErrorOccuried();
            limits.maxFramebufferHeight = datai; // at least 16384
            gl->GetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &datai); _SglThrowErrorOccuried();
            limits.maxFramebufferLayers = datai; // at least 2048
            gl->GetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &datai); _SglThrowErrorOccuried();
            limits.framebufferNoAttachmentsSampleCounts = datai; // at least 4

            gl->GetIntegerv(GL_MAX_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
            limits.maxImageDimension1D = datai; // at least 1024
            limits.maxImageDimension2D = datai; // at least 1024
            gl->GetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
            limits.maxImageDimension3D = datai; // at least 64
            gl->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &datai); _SglThrowErrorOccuried();
            limits.maxImageArrayLayers = datai; // at least 256
            gl->GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &datai); _SglThrowErrorOccuried();
            limits.maxImageDimensionCube = datai; // at least 1024
            gl->GetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &dataf); _SglThrowErrorOccuried();
            limits.maxSamplerLodBias = dataf; // at least 2.0

            gl->GetIntegerv(GL_MAX_INTEGER_SAMPLES, &datai); _SglThrowErrorOccuried();
            limits.sampledImageIntegerSampleCounts = datai;
            gl->GetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
            limits.sampledImageColorSampleCounts = datai;
            limits.framebufferColorSampleCounts = datai;
            gl->GetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &datai); _SglThrowErrorOccuried();
            limits.sampledImageDepthSampleCounts = datai;
            limits.sampledImageStencilSampleCounts = datai;
            limits.framebufferDepthSampleCounts = datai;
            limits.framebufferStencilSampleCounts = datai;

            gl->GetIntegerv(GL_MIN_MAP_BUFFER_ALIGNMENT, &datai); _SglThrowErrorOccuried();
            limits.minMemoryMapAlignment = datai; // at least 64
            gl->GetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
            limits.minStorageBufferOffsetAlignment = datai; // at least 64
            gl->GetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
            limits.minTexelBufferOffsetAlignment = datai; // at least 64
            gl->GetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &datai); _SglThrowErrorOccuried();
            limits.minUniformBufferOffsetAlignment = datai; // at least 64

            gl->GetFloatv(GL_POINT_SIZE_RANGE, dataf2); _SglThrowErrorOccuried();
            limits.pointSizeRange[0] = dataf2[0]; // at least 1
            limits.pointSizeRange[1] = dataf2[1]; // at least 1
            gl->GetFloatv(GL_POINT_SIZE_GRANULARITY, &dataf); _SglThrowErrorOccuried();
            limits.pointSizeGranularity = dataf;
            gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, dataf2); _SglThrowErrorOccuried();
            limits.lineWidthRange[0] = dataf2[0];
            limits.lineWidthRange[1] = dataf2[1];
            gl->GetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &dataf); _SglThrowErrorOccuried();
            limits.lineWidthGranularity = dataf;

            gl->GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &datai); _SglThrowErrorOccuried();
            limits.maxVertexInputAttributes = datai; // at least 16
            gl->GetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
            limits.maxVertexOutputComponents = datai; // at least 64
            gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &datai); _SglThrowErrorOccuried();
            limits.maxVertexInputBindings = datai;
            gl->GetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, &datai); _SglThrowErrorOccuried();
            limits.maxVertexInputAttributeOffset = datai;
            gl->GetIntegerv(GL_VERTEX_BINDING_STRIDE, &datai); _SglThrowErrorOccuried();
            limits.maxVertexInputBindingStride = datai;

            gl->GetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
            limits.maxFragmentInputComponents = datai; // at least 128
            gl->GetIntegerv(GL_MAX_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
            limits.maxFragmentOutputAttachments = datai; // at least 8
            gl->GetIntegerv(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &datai); _SglThrowErrorOccuried();
            limits.maxFragmentDualSrcAttachments = datai; // at least 1
            
            gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &datai); _SglThrowErrorOccuried();
            limits.maxComputeWorkGroupInvocations = datai;
            gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, datai2); _SglThrowErrorOccuried();
            limits.maxComputeWorkGroupCount[0] = datai2[0];
            limits.maxComputeWorkGroupCount[1] = datai2[1];
            limits.maxComputeWorkGroupCount[2] = datai2[2];
            gl->GetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, datai2); _SglThrowErrorOccuried();
            limits.maxComputeWorkGroupSize[0] = datai2[0];
            limits.maxComputeWorkGroupSize[1] = datai2[1];
            limits.maxComputeWorkGroupSize[2] = datai2[2];


            gl->GetIntegerv(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
            limits.maxGeometryInputComponents = datai; // at least 64
            gl->GetIntegerv(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &datai); _SglThrowErrorOccuried();
            limits.maxGeometryOutputComponents = datai; // at least 128

            limits.maxBoundDescriptorSets = 4;
            limits.maxPushConstantsSize = 0;

            ddev->base.limits = limits;

            //dthrD->thr.proc = _SglDthrProcCb;

            gl->GenVertexArrays(1, &dpu->emptyVao); _SglThrowErrorOccuried();
            gl->BindVertexArray(dpu->emptyVao); _SglThrowErrorOccuried();
            gl->BindVertexArray(0); _SglThrowErrorOccuried();

            dpu->instanced = TRUE;
            dpu->running = TRUE;
        }
        dpu->MakeCurrent(bkpHdc, bkpGlrc);

        if (err)
            FreeLibrary(dpu->opengl32);
    }
    return err;
}

_SGL afxError _SglDestroyDpu(afxDrawDevice ddev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    sglDpuIdd *dpu = &ddev->dpus[unitIdx];
    glVmt const* gl = &dpu->gl;

    afxMmu mmu = AfxGetDrawSystemMmu();

    _SglDdevProcessResDel(ddev, unitIdx);
    AfxReleaseSlock(&dpu->deletionLock);
    AfxDeallocateQueue(&dpu->deletionQueue);

    AfxDismountClass(&ddev->base.ports[unitIdx].scripts);
    AfxDismountClass(&ddev->base.ports[unitIdx].queues);

    gl->DeleteVertexArrays(1, &dpu->emptyVao); _SglThrowErrorOccuried();

    //wglVmt const* wgl = &dpu->wgl;

    HDC dc = dpu->GetCurrentDC();
    HGLRC rc = dpu->GetCurrentContext();

    if (dc == dpu->dc || rc == dpu->glrc)
        dpu->MakeCurrent(NIL, NIL);

    dpu->DeleteContext(dpu->glrc);

    ReleaseDC(dpu->wnd, dpu->dc);
    DestroyWindow(dpu->wnd);
    FreeLibrary(dpu->opengl32);
    return err;
}

_SGL afxError _SglDdevRelinkDinCb(afxDrawDevice ddev, afxDrawInput din, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);

    // TODO discard pull request submissions too.

    //AfxDiscardAllDrawInputSubmissions(din);
    afxDrawContext from = AfxGetLinker(&din->base.dctx);
    
    if (from)
    {
        AfxAssertObjects(1, &from, afxFcc_DCTX);
        AfxPopLinkage(&din->base.dctx);
        _SglDinFreeAllBuffers(din);
    }

    if (dctx)
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        AfxPushLinkage(&din->base.dctx, &dctx->base.inlinks);
    }

    //AfxDiscardAllDrawInputSubmissions(din);

    return err;
}

_SGL afxError _SglDdevRelinkDoutCb(afxDrawDevice ddev, afxDrawOutput dout, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);

    // TODO discard pull request submissions too.
    afxDrawContext from = AfxGetLinker(&dout->base.dctx);

    if (from)
    {
        AfxAssertObjects(1, &from, afxFcc_DCTX);
        AfxPopLinkage(&dout->base.dctx);
        AfxRegenerateDrawOutputBuffers(dout);
    }

    if (dctx)
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);
        AfxPushLinkage(&dout->base.dctx, &dctx->base.outlinks);
        AfxRegenerateDrawOutputBuffers(dout);
    }

    AfxFormatString(&dout->base.caption, "Draw Output %p (%s) --- OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build", dout, dctx && !err ? "On line" : "Off line");

    return err;
}

_SGL afxBool _SglProcessDctxCb(afxDrawContext dctx, void *udd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    if (dthr->ddev != AfxGetObjectProvider(dctx)) AfxThrowError();
    else
    {
        dthr->dctx = dctx;
        
        if (dctx->base.procCb(dctx, dthr))
            AfxThrowError();
    }
    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDdevProcCb(afxDrawDevice ddev, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);

    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

    if (unitIdx > 0)
        return err;

    dthr->portIdx = unitIdx;

    AfxCurateDrawContexts(ddev, 0, AFX_N32_MAX, _SglProcessDctxCb, (void*)dthr);

    _SglDdevProcessResDel(ddev, unitIdx); // delete after is safer?

    return err;
}

_SGL afxError _SglDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxMmu mmu = AfxGetDrawSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < ddev->dpuCnt; i++)
        if (_SglDestroyDpu(ddev, i))
            AfxThrowError();

    AfxDeallocate(mmu, ddev->dpus);

    _AfxUninstallChainedClasses(&ddev->base.classes);

    AfxDeallocate(mmu, ddev->base.ports);

    UnregisterClass(ddev->wndClss.lpszClassName, ddev->wndClss.hInstance);

    return err;
}

_SGL afxError _SglDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    AfxEntry("ddev=%p", ddev);
    afxError err = AFX_ERR_NONE;

    afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    afxMmu mmu = AfxGetDrawSystemMmu();

    //SglLoadOpenGlVmt(ddev->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, ddev->p);

    ddev->wndClss.cbSize = sizeof(ddev->wndClss); // only on EX
    ddev->wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    ddev->wndClss.lpfnWndProc = _SglWndHndlngPrcW32Callback;
    ddev->wndClss.cbClsExtra = sizeof(ddev);
    ddev->wndClss.cbWndExtra = sizeof((afxDrawOutput)0);
    ddev->wndClss.hInstance = GetModuleHandleA(NULL);
    ddev->wndClss.hIcon = NULL;
    ddev->wndClss.hCursor = NULL;
    ddev->wndClss.hbrBackground = NULL;
    ddev->wndClss.lpszMenuName = NULL;
    ddev->wndClss.lpszClassName = "OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build";
    ddev->wndClss.hIconSm = NULL;

    if (!(RegisterClassEx(&(ddev->wndClss)))) AfxThrowError();
    else
    {
        AfxComment("Listing mem available display devices...\n");

        //HDC hdc = NIL;
        afxNat idx = 0;
        while (1)
        {
            DISPLAY_DEVICE dispdev = { 0 };
            dispdev.cb = sizeof(dispdev);

            if (!(EnumDisplayDevices(NULL, idx, &dispdev, NIL))) break;
            else
            {
                AfxComment("#%u = %s (%s) %x", idx, dispdev.DeviceString, dispdev.DeviceName, dispdev.StateFlags);
                idx++;
            }
        };

        AfxReplicateString(&ddev->base.domain, info->domain);
        AfxReplicateString(&ddev->base.name, info->name);

        afxChain *classes = &ddev->base.classes;
        AfxTakeChain(classes, ddev);

        ddev->base.caps = info->caps ? *info->caps : (afxDrawDeviceCaps) { 0 };
        ddev->base.limits = info->limits ? *info->limits : (afxDrawDeviceLimits) { 0 };

        ddev->base.serving = FALSE;

        ddev->base.procCb = _SglDdevProcCb;
        ddev->base.relinkDin = _SglDdevRelinkDinCb;
        ddev->base.relinkDout = _SglDdevRelinkDoutCb;

        afxNat portCnt = AfxMax(1, AfxMin(AfxGetThreadingCapacity(), info->portCnt));
        ddev->base.portCnt = portCnt;
        AfxAssert(ddev->base.portCnt);

        if (!(ddev->base.ports = AfxAllocate(mmu, sizeof(ddev->base.ports[0]), portCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < portCnt; i++)
            {
                ddev->base.ports[i].portCaps = info->portCaps[i];

                afxClassConfig tmpClsCfg;
                tmpClsCfg = _SglDqueClsConfig;
                tmpClsCfg.mmu = mmu;
                AfxMountClass(&ddev->base.ports[i].queues, NIL, &ddev->base.classes, &tmpClsCfg);
                tmpClsCfg = _SglDscrClsConfig;
                tmpClsCfg.mmu = mmu;
                AfxMountClass(&ddev->base.ports[i].scripts, NIL, &ddev->base.classes, &tmpClsCfg);
            }

            afxDrawSystem dsys = _AfxGetDsysData();
            AfxAssertType(dsys, afxFcc_DSYS);

            // dctx must be after dque
            AfxMountClass(&ddev->base.contexts, &dsys->dcontexts, classes, info->dctxClsConfig);
            AfxMountClass(&ddev->base.outputs, &dsys->doutputs, classes, info->doutClsConfig);
            AfxMountClass(&ddev->base.inputs, &dsys->dinputs, classes, info->dinClsConfig);

            ddev->dpuCnt = portCnt;

            if (!(ddev->dpus = AfxAllocate(mmu, sizeof(ddev->dpus[0]), ddev->dpuCnt, 0, AfxHint()))) AfxThrowError();
            else
            {
                AfxZero(ddev->dpuCnt, sizeof(ddev->dpus[0]), ddev->dpus);

                for (afxNat i = 0; i < ddev->dpuCnt; i++)
                {
                    if (_SglBuildDpu(ddev, i))
                    {
                        AfxThrowError();

                        for (afxNat j = 0; j < i; j++)
                            if (_SglDestroyDpu(ddev, j))
                                AfxThrowError();
                    }
                }

                afxNat unitIdx;
                AfxGetThreadingUnit(&unitIdx);

                //wglVmt const* wgl = &ddev->dpus[unitIdx].wgl;

                if (!ddev->dpus[unitIdx].MakeCurrent(ddev->dpus[unitIdx].dc, ddev->dpus[unitIdx].glrc))
                    AfxThrowError();

                afxDrawDevice devInfo;

                //ddev->dpuCnt = 1;

                AfxAssert(ddev->base.procCb);
                ddev->base.serving = TRUE;
            }

            if (err)
                AfxDeallocate(mmu, ddev->base.ports);
        }

        if (err)
        {
            _AfxUninstallChainedClasses(&ddev->base.classes);
            UnregisterClass(ddev->wndClss.lpszClassName, ddev->wndClss.hInstance);
        }
    }

    return err;
}

_SGL afxError AfxIcdHookPoint(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;

    //afxUri file;
    //AfxMakeUri(&file, "e2draw.icd", 0);
    static afxString name, vendor, website, note;
    AfxMakeString(&icd->name, "OpenGL/Vulkan Continuous Integration --- SIGMA GL/2 --- Qwadro Execution Ecosystem", 0);
    AfxMakeString(&icd->vendor, "SIGMA Technology Group", 0);
    AfxMakeString(&icd->website, "www.sigmaco.org", 0);
    AfxMakeString(&icd->note, sigmaSignature, 0);
    icd->verMajor = 0;
    icd->verMinor = 7;
    icd->verPatch = 2;

    static afxClassConfig const devClsExt =
    {
        .fcc = afxFcc_DDEV,
        .name = "Draw Device",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxDrawDevice)),
        .mmu = NIL,
        .ctor = (void*)_SglDdevCtor,
        .dtor = (void*)_SglDdevDtor
    };
    

    afxUri file;
    AfxGetUriObject(&file, AfxGetModulePath(&icd->mdle));

    AfxLogMessageFormatted(0xFFFF0000, "\nInstalling '%.*s' ICD on draw system...\n\t%.*s %u.%u.%u\n\tVendor: %.*s <%.*s>\n\tNote: %.*s", AfxPushString(AfxGetUriString(&file)), AfxPushString(&icd->name), icd->verMajor, icd->verMinor, icd->verPatch, AfxPushString(&icd->vendor), AfxPushString(&icd->website), AfxPushString(&icd->note));

    //ddrv->mdle = info->mdle;
    //AfxAssertObjects(1, &ddrv->mdle, afxFcc_MDLE);
    //AfxReacquireObjects(1, (void*[]) { ddrv->mdle });

    afxChain *classes = &icd->classes;
    AfxTakeChain(classes, icd);

    if (AfxMountClass(&icd->devices, AfxGetDrawDeviceClass(), classes, &devClsExt)) AfxThrowError();
    else
    {
        afxDrawDeviceCaps const devCaps = { 0 };
        afxDrawPortCaps const portCaps[] =
        {
            { .queFlags = AFX_DQUE_DRAW | AFX_DQUE_COMPUTE | AFX_DQUE_TRANSFER | AFX_DQUE_VHS, .queCnt = 16 },
            { .queFlags = AFX_DQUE_COMPUTE | AFX_DQUE_TRANSFER, .queCnt = 2 },
        };
        static afxString devDomain, devName;
        AfxMakeString(&devDomain, "targa", 0);
        AfxMakeString(&devName, targaSigmaSignature, 0);
        afxDrawDeviceInfo const devInfo[] =
        {
            {
                .domain = &devDomain,
                .name = &devName,
                .caps = &devCaps,
                .portCnt = sizeof(portCaps) / sizeof(portCaps[0]),
                .portCaps = portCaps,
                .dctxClsConfig = &_SglDctxClsConfig,
                .doutClsConfig = &_SglDoutClsConfig,
                .dinClsConfig = &_SglDinClsConfig
            }
        };
        afxNat const devCnt = AFX_COUNTOF(devInfo);
        afxDrawDevice ddev[AFX_COUNTOF(devInfo)];

        if (AfxAcquireObjects(&icd->devices, devCnt, (afxObject*)ddev, (void const*[]) { NIL, devInfo })) AfxThrowError();
        else
        {
            AfxAssertObjects(devCnt, ddev, afxFcc_DDEV);

            for (afxNat i = 0; i < 1; i++)
            {

                AfxLogMessageFormatted(0xFFFF0000, "\nDraw device '%.*s' <'%.*s'> registered on draw system", AfxPushString(devInfo[i].name), AfxPushString(devInfo[i].domain));
            }

            if (err)
                AfxReleaseObjects(devCnt, (afxObject*)ddev);
        }

        if (err)
            _AfxUninstallChainedClasses(&icd->classes);
    }
    return err;
}