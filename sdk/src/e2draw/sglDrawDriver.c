﻿/*
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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>

#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#include "sglDrawDriver.h"
#include "sgl.h"
#include "afx/core/afxSystem.h"
#pragma comment(lib, "opengl32")

HMODULE opengl32 = NIL;
HGLRC (WINAPI* _wglCreateContext)(HDC) = NIL;
BOOL  (WINAPI* _wglDeleteContext)(HGLRC) = NIL;
HGLRC (WINAPI* _wglGetCurrentContext)(VOID) = NIL;
HDC   (WINAPI* _wglGetCurrentDC)(VOID) = NIL;
PROC  (WINAPI* _wglGetProcAddress)(LPCSTR) = NIL;
BOOL  (WINAPI* _wglMakeCurrent)(HDC, HGLRC) = NIL;
int(WINAPI* _wglChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd) = NIL;
int(WINAPI* _wglDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd) = NIL;
BOOL(WINAPI* _wglSetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd) = NIL;
int(WINAPI* _wglGetPixelFormat)(HDC hdc) = NIL;
BOOL(WINAPI* _wglSwapBuffers)(HDC) = NIL;

afxBool _AfxLoadOpengl(void)
{
    if (!opengl32 && !(opengl32 = LoadLibraryA("opengl32.dll"))) AfxError("");
    else
    {
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
        _wglSwapBuffers = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
        return TRUE;
    }
    return FALSE;
}

static const char *glsyms[] =
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


afxError _SglLoadVmtSubset(sglVmt* vmt, afxNat base, afxNat cnt, afxChar const *names[])
{
    afxError err = AFX_ERR_NONE;
    afxString128 sym;
    AfxString128(&sym);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxFormatString(&sym.str, "%s", names[i]);

        if (!(vmt->ptr[base + i] = glcGetProcAddress(NIL, (void*)sym.buf)))
        {
            AfxFormatString(&sym.str, "%sARB", names[i]);

            if (!(vmt->ptr[base + i] = glcGetProcAddress(NIL, (void*)sym.buf)))
            {
                AfxFormatString(&sym.str, "%sEXT", names[i]);

                if (!(vmt->ptr[base + i] = glcGetProcAddress(NIL, (void*)sym.buf)))
                {
                    //AfxError("%s @ %i not found.", names[i], i);
                    //AfxThrowError();
                }
                else
                {
                    AfxEcho("%s @ %i found.", names[i], i);
                }
            }
            else
            {
                AfxEcho("%s @ %i found.", names[i], i);
            }
        }
        else
        {
            AfxEcho("%s @ %i found.", names[i], i);
        }
    }
    return err;
}

void _SglLoadVmt(sglVmt* vmt, afxNat verMajor, afxNat verMinor)
{
    AfxEntry("vmt=%p,verMajor=%u,verMinor=%u", vmt, verMajor, verMinor);
    int symCnt = 0;

    switch (verMajor)
    {
    case 1:
    {
        switch (verMinor)
        {
        case 0: symCnt = SGL_VMT_SIZE_100; break;
        case 1: symCnt = SGL_VMT_SIZE_110; break;
        case 2: symCnt = SGL_VMT_SIZE_120; break;
        case 3: symCnt = SGL_VMT_SIZE_130; break;
        case 4: symCnt = SGL_VMT_SIZE_140; break;
        case 5: symCnt = SGL_VMT_SIZE_150; break;
        default: break;
        }
        break;
    }
    case 2:
        switch (verMinor)
        {
        case 0: symCnt = SGL_VMT_SIZE_200; break;
        case 1: symCnt = SGL_VMT_SIZE_210; break;
        default: break;
        }
        break;
    case 3:
        switch (verMinor)
        {
        case 0: symCnt = SGL_VMT_SIZE_300; break;
        case 1: symCnt = SGL_VMT_SIZE_310; break;
        case 2: symCnt = SGL_VMT_SIZE_320; break;
        case 3: symCnt = SGL_VMT_SIZE_330; break;
        default: break;
        }
        break;
    case 4:
        switch (verMinor)
        {
        case 0: symCnt = SGL_VMT_SIZE_400; break;
        case 1: symCnt = SGL_VMT_SIZE_410; break;
        case 2: symCnt = SGL_VMT_SIZE_420; break;
        case 3: symCnt = SGL_VMT_SIZE_430; break;
        case 4: symCnt = SGL_VMT_SIZE_440; break;
        case 5: symCnt = SGL_VMT_SIZE_450; break;
        case 6: symCnt = SGL_VMT_SIZE_460; break;
        default: break;
        }
        break;
    default: break;
    }

    if (!opengl32)
        _AfxLoadOpengl();

    if (!opengl32) AfxError("");
    else
    {
        _SglLoadVmtSubset(vmt, 0, symCnt, glsyms);
    }
}

void* glcGetProcAddress(void *deviceHandle, const GLchar *funcName)
{
    (void)deviceHandle;
    afxError err = AFX_ERR_NONE;
    AfxAssert(funcName);
    PROC proc = NIL;
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");

    if (!(proc = f(funcName)))
        proc = GetProcAddress(opengl32, funcName);
            //AfxError("%s not found.", funcName);

    return proc;
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
    static GLenum const v[] =
    {
        NIL,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK
    };

    return v[cm];
};

_SGL GLenum AfxToGlPolygonMode(afxFillMode pm)
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
    static GLuint const v[] =
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

    return v[usage];
};

_SGL GLenum AfxToGlStencilOp(afxStencilOp so)
{
    static GLuint const v[] =
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

afxChar const sigglSigmaSignature[] =
{

                                                            
"\n ad88888ba   88    ,ad8888ba,     ,ad8888ba,   88           "
"\nd8\"     \"8b  88   d8\"'    `\"8b   d8\"'    `\"8b  88           "
"\nY8,          88  d8'            d8'            88           "
"\n`Y8aaaaa,    88  88             88             88           "
"\n  `\"\"\"\"\"8b,  88  88      88888  88      88888  88"
"\n        `8b  88  Y8,        88  Y8,        88  88           "
"\nY8a     a8P  88   Y8a.    .a88   Y8a.    .a88  88           "
"\n \"Y88888P\"   88    `\"Y88888P\"     `\"Y88888P\"   88888888888  "
};

afxChar const quadroGlSigmaSignature[] =
{
    "\n     .::::         :::::::::         ::::::::        :::        "
    "\n    :+:+:+         :+:    :+:       :+:    :+:       :+:        "
    "\n   +:+ +:+         +:+    +:+       +:+              +:+        "
    "\n  +#+  +:+         +#+    +:+       :#:              +#+        "
    "\n +#+#+#+#+#+       +#+    +#+       +#+   +#+#       +#+        "
    "\n       #+#         #+#    #+#       #+#    #+#       #+#        "
    "\n       ###         #########         ########        ########## "
};

_SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;
    
    if (!(rslt = _wglMakeCurrent(hdc, hrc)))
        //if (!(rslt = wglMakeCurrent(hdc, hrc)))
            AfxThrowError();

    return rslt;
}

_SGL afxError SglSwapBuffers(HDC hdc)
{
    afxError err = AFX_ERR_NONE;

    
    if (!(_wglSwapBuffers(hdc)))
        if (!(SwapBuffers(hdc)))
            AfxThrowError();

    //SetWindowTextA(dout->wglWnd, dout->title.buf); // deadlocks mem threads on exit
    //UpdateWindow(dout->wglWnd);
    //AfxYield();

    return err;
}

_SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd)
{
    afxError err = AFX_ERR_NONE;
    int fmt;

    if (!(fmt = _wglChoosePixelFormat(hdc, ppfd)))
        if (!(fmt = ChoosePixelFormat(hdc, ppfd)))
            AfxThrowError();

    return fmt;
}

_SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;

    if (!(rslt = _wglSetPixelFormat(hdc, format, ppfd)))
        if (!(rslt = SetPixelFormat(hdc, format, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = _wglDescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
        if (!(rslt = DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglGetPixelFormat(HDC hdc)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = _wglGetPixelFormat(hdc)))
        if (!(rslt = GetPixelFormat(hdc)))
            AfxThrowError();

    return rslt;
}

_SGL LRESULT WINAPI _SglWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (void*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (dout)
    {
        AfxAssertObject(dout, AFX_FCC_DOUT);
        sglDoutIdd *idd = dout->idd;
        AfxAssert(idd->wglWnd == hWnd);

        switch (message)
        {
        case WM_CLOSE:
        {

            // Sent as a signal that a window or an application should terminate.
            // wParam = This parameter is not used.
            // lParam = This parameter is not used.
            // If an application processes this message, it should return zero.

            //_SglDoutProcess(dout);

            //while (0 < AfxReleaseObject(&dout->obj));
#if 0
            afxDrawContext dctx = AfxGetDrawOutputConnectedContext(dout);

            if (!dctx) AfxReleaseDrawOutput(dout);
            else
            {
                struct msgdecl { afxSize tid; afxSize hwnd; afxSize wParam; afxSize lParam; } *msgdecl = AfxAllocate(NIL, sizeof(struct msgdecl), AfxSpawnHint());
                msgdecl->tid = GetThreadId(dctx->thread);
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
            //if (dout->resizable)
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

                afxWhd whdNew = { LOWORD(lParam), HIWORD(lParam), 1 };

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
            afxWhd const resolution = { GetDeviceCaps(idd->wglDc, HORZRES), GetDeviceCaps(idd->wglDc, VERTRES), GetDeviceCaps(idd->wglDc, PLANES) };
            dout->refreshRate = GetDeviceCaps(idd->wglDc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(idd->wglDc, HORZSIZE) / (afxReal64)GetDeviceCaps(idd->wglDc, VERTSIZE);
            
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
            
            RECT rect;
            GetClientRect(idd->wglWnd, &(rect));
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

            AfxV2dSub(dout->absCursorPos, curr, dout->absCursorMove);
            AfxV2dCopy(dout->absCursorPos, curr);

            afxV2d screen = { AfxScalar(dout->extent[0]), AfxScalar(dout->extent[1]) };

            AfxExtractV2dInNdc(dout->ndcCursorPos, dout->absCursorPos, screen);
            AfxExtractV2dInNdc(dout->ndcCursorMove, dout->absCursorMove, screen);

            //data2->breake = TRUE;
            break;
        }
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

            AfxAcquireArray(NIL, &fdrop.files, sizeof(afxChar const*), 0, AfxSpawnHint());

            afxChar* name = NIL;
            afxNat len = 0;
            afxNat i;

            afxDrawContext dctx = AfxGetDrawOutputConnectedContext(dout);
            AfxAssertObject(dctx, AFX_FCC_DCTX);
            afxMemory mem = AfxGetDrawContextMemory(dctx);
            AfxAssertObject(mem, AFX_FCC_MEM);

            for (i = 0; i < cnt; i++)
            {
                len = DragQueryFileA(hDrop, i, NIL, 0);

                if (!(name = AfxAllocate(mem, len + 1, 0, AfxSpawnHint()))) AfxError("");
                else
                {
                    DragQueryFileA(hDrop, i, name, len + 1);
                    afxNat arrIdx;
                    void *arrel = AfxInsertArrayUnit(&fdrop.files, &arrIdx);
                    AfxCopy(arrel, name, sizeof(name));
                }
            }

            afxEvent ev;
            AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &dout->obj, &fdrop);
            AfxObjectEmitEvent(&dout->obj, &ev);

            for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
            {
                AfxEcho("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
            }

            for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
            {
                AfxDeallocate(mem, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
            }

            AfxReleaseArray(&fdrop.files);

            DragFinish(hDrop);
            //data2->breake = TRUE;
            //return 0;
            break;
        }
        case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
        {
            dout->focused = FALSE;
            // TODO pause draw input to reduce resource usage?

            //BinkPause(Back_Bink, 1);
            //BinkPause(Alpha_Bink, 1);
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            dout->focused = TRUE;
            // TODO resume draw input of pause to reduce resource usage?

            //BinkPause(Back_Bink, 0);
            //BinkPause(Alpha_Bink, 0);
            break;
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

_SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc)
{
    AfxEntry("oglWndClss=%p,shareCtx=%p,phwnd=%p,phdc=%p,phrc=%p", oglWndClss, shareCtx, phwnd, phdc, phrc);
    afxError err = AFX_ERR_NONE;

    HDC bkpHdc = _wglGetCurrentDC();
    HGLRC bkpGlrc = _wglGetCurrentContext();

    HWND tmpHwnd = CreateWindowA(oglWndClss->lpszClassName, "", /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, oglWndClss->hInstance, NIL);

    if (!(tmpHwnd)) AfxThrowError();
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

            int pxlfmt = SglChoosePixelFormat(tmpHdc, &(pfd));
            SglDescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd);

            if (!SglSetPixelFormat(tmpHdc, pxlfmt, &(pfd))) AfxThrowError();
            else
            {
                HGLRC tmpHrc = _wglCreateContext(tmpHdc);

                if (!tmpHrc) AfxThrowError();
                else
                {
                    if (!(_wglMakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                    else
                    {
                        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)_wglGetProcAddress("wglChoosePixelFormatARB");
                        
                        if (!wglChoosePixelFormatARB) AfxThrowError();
                        else
                        {
                            HWND hwnd2 = CreateWindowA(oglWndClss->lpszClassName, "", /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, oglWndClss->hInstance, NIL);

                            if (!(hwnd2)) AfxThrowError();
                            else
                            {
                                HDC hdc2 = GetDC(hwnd2);

                                if (!(hdc2)) AfxThrowError();
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
                                    int pxlAttrPairs[16][2] = {0};
                            
                                    pxlAttrPairs[0][0] = WGL_DRAW_TO_WINDOW_ARB;
                                    pxlAttrPairs[0][1] = GL_TRUE;

                                    pxlAttrPairs[1][0] = WGL_SUPPORT_OPENGL_ARB;
                                    pxlAttrPairs[1][1] = GL_TRUE;

                                    pxlAttrPairs[2][0] = WGL_ACCELERATION_ARB;
                                    pxlAttrPairs[2][1] = WGL_FULL_ACCELERATION_ARB;

                                    pxlAttrPairs[3][0] = WGL_DOUBLE_BUFFER_ARB;
                                    pxlAttrPairs[3][1] = GL_TRUE;

                                    pxlAttrPairs[4][0] = WGL_PIXEL_TYPE_ARB;
                                    pxlAttrPairs[4][1] = WGL_TYPE_RGBA_ARB; // WGL_TYPE_RGBA_FLOAT_ARB

                                    pxlAttrPairs[5][0] = WGL_COLOR_BITS_ARB;
                                    pxlAttrPairs[5][1] = GetDeviceCaps(hdc2, BITSPIXEL);

                                    pxlAttrPairs[6][0] = WGL_ALPHA_BITS_ARB;
                                    pxlAttrPairs[6][1] = 8;

                                    //pxlAttrPairs[7][0] = WGL_DEPTH_BITS_ARB;
                                    //pxlAttrPairs[7][1] = 24;

                                    //pxlAttrPairs[8][0] = WGL_STENCIL_BITS_ARB;
                                    //pxlAttrPairs[8][1] = 8;

                                    pxlAttrPairs[7][0] = WGL_TRANSPARENT_ARB;
                                    pxlAttrPairs[7][1] = TRUE;

                                    pxlAttrPairs[8][0] = WGL_SWAP_METHOD_ARB;
                                    pxlAttrPairs[8][1] = WGL_SWAP_EXCHANGE_ARB;

                                    //pxlAttrPairs[10][0] = WGL_SAMPLE_BUFFERS_ARB; // works on Intel, didn't work on Mesa
                                    //pxlAttrPairs[10][1] = GL_TRUE;

                                    //pxlAttrPairs[11][0] = WGL_SAMPLES_ARB; // works on Intel, didn't work on Mesa
                                    //pxlAttrPairs[11][1] = 8;

                                    //[11][0] = WGL_COLORSPACE_EXT; // works on Mesa, didn't work on Intel
                                    //pxlAttrPairs[11][1] = WGL_COLORSPACE_SRGB_EXT; // WGL_COLORSPACE_LINEAR_EXT

                                    //pxlAttrPairs[12][0] = WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;  // works on Mesa, didn't work on Intel
                                    //pxlAttrPairs[12][1] = GL_TRUE;

                                    pxlAttrPairs[9][0] = NIL;
                                    pxlAttrPairs[9][1] = NIL;

                                    UINT formatCount;
                                    pxlfmt = 0;

                                    if (!wglChoosePixelFormatARB(hdc2, &pxlAttrPairs[0][0], 0, 1, &(pxlfmt), &(formatCount))) AfxThrowError();
                                    else
                                    {
                                        AfxAssert(pxlfmt);
                                        AfxAssert(formatCount);

                                        AFX_ZERO(&pfd);
                                        SglDescribePixelFormat(hdc2, pxlfmt, sizeof(pfd), &(pfd));

                                        if (!(SglSetPixelFormat(hdc2, pxlfmt, &(pfd)))) AfxThrowError();
                                        else
                                        {
                                            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

                                            if (!(wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)_wglGetProcAddress("wglCreateContextAttribsARB"))) AfxThrowError();
                                            else
                                            {
                                                int ctxAttrPairs[6][2] = { 0 };

                                                ctxAttrPairs[0][0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
                                                ctxAttrPairs[0][1] = 3;

                                                ctxAttrPairs[1][0] = WGL_CONTEXT_MINOR_VERSION_ARB;
                                                ctxAttrPairs[1][1] = 3;

                                                ctxAttrPairs[2][0] = WGL_CONTEXT_PROFILE_MASK_ARB;
                                                ctxAttrPairs[2][1] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
                                                //ctxAttrPairs[2][1] = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

                                                ctxAttrPairs[3][0] = WGL_CONTEXT_FLAGS_ARB;
                                                ctxAttrPairs[3][1] = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

#ifdef _AFX_DEBUG
                                                ctxAttrPairs[3][1] |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif
                                                ctxAttrPairs[3][1] |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;

                                                ctxAttrPairs[4][0] = NIL;
                                                ctxAttrPairs[4][1] = NIL;

                                                _wglMakeCurrent(bkpHdc, bkpGlrc);
                                                _wglDeleteContext(tmpHrc);
                                                tmpHrc = NIL;

                                                HGLRC  hrc2 = wglCreateContextAttribsARB(hdc2, shareCtx, (void*)ctxAttrPairs);

                                                if (!hrc2) AfxThrowError();
                                                else
                                                {
                                                    if (!_wglMakeCurrent(hdc2, hrc2)) AfxThrowError();
                                                    else
                                                    {
                                                        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)_wglGetProcAddress("wglSwapIntervalEXT");

                                                        if (wglSwapIntervalEXT)
                                                            wglSwapIntervalEXT(0);

                                                        AfxAssert(phwnd);
                                                        *phwnd = hwnd2;
                                                        AfxAssert(phdc);
                                                        *phdc = hdc2;
                                                        AfxAssert(phrc);
                                                        *phrc = hrc2;

                                                        if (err)
                                                            _wglMakeCurrent(bkpHdc, bkpGlrc);
                                                    }

                                                    if (err)
                                                        _wglDeleteContext(hrc2);
                                                }
                                            }
                                        }
                                    }

                                    if (err && hdc2)
                                        ReleaseDC(hwnd2, hdc2);
                                }

                                if (err && hwnd2)
                                    DestroyWindow(hwnd2);
                            }
                        }
                    }
                    _wglMakeCurrent(bkpHdc, bkpGlrc);

                    if (tmpHrc)
                        _wglDeleteContext(tmpHrc);
                }
            }

            if (tmpHdc)
                ReleaseDC(tmpHwnd, tmpHdc);
        }

        if (tmpHwnd)
            DestroyWindow(tmpHwnd);
    }
    return err;
}


_SGL afxError _SglDthrProcessDpuDeletionQueue(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *idd = ddrv->idd;
    sglDpuIdd *dpu = &idd->dpus[unitIdx];
    sglVmt const *gl = &dpu->wglVmt;

    //HDC dc = _wglGetCurrentDC();
    HGLRC rc = _wglGetCurrentContext();
    AfxAssert(rc == dpu->wglGlrc);

    if (/*dc != idd->wglDc || */rc != dpu->wglGlrc)
    {
        if (!_wglMakeCurrent(dpu->wglDc, dpu->wglGlrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            //_wglMakeCurrent(idd->wglDc, idd->wglGlrc);
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
                gl->DeleteBuffers(1, &delRes->gpuHandle);
                break;
            case 1: // tex
                gl->DeleteTextures(1, &delRes->gpuHandle);
                break;
            case 2: // surf
                gl->DeleteRenderbuffers(1, &delRes->gpuHandle);
                break;
            case 3: // canv
                gl->DeleteFramebuffers(1, &delRes->gpuHandle);
                break;
            case 4: // smp
                gl->DeleteSamplers(1, &delRes->gpuHandle);
                break;
            case 5: // pip
                gl->DeleteProgram(delRes->gpuHandle);
                break;
            case 6: // shd
                gl->DeleteShader(delRes->gpuHandle);
                break;
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

_SGL void _SglDeleteGlRes(afxDrawContext dctx, afxNat type, GLuint gpuHandle)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);    
    _sglDdrvIdd *idd = ddrv->idd;
    sglDpuIdd*dpu = &idd->dpus[AfxGetSystemProcessor()];
    AfxEnterSlockExclusive(&dpu->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandle = gpuHandle;
    delRes.type = type;
    AfxPushQueueUnit(&dpu->deletionQueue, &delRes);
    AfxExitSlockExclusive(&dpu->deletionLock);
};

_SGL afxError _SglBuildDpu(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *idd = ddrv->idd;
    //afxNat procUnitIdx = AfxGetSystemProcessor();
    sglDpuIdd *dpu = &idd->dpus[unitIdx];

    AfxAcquireSlock(&dpu->deletionLock);
    AfxAcquireQueue(&dpu->deletionQueue, sizeof(_sglDeleteGlRes), 32);
    
    if (_SglCreateCombinedDeviceContext(&idd->oglWndClss, idd->dpus[0].wglGlrc, &(dpu->wglWnd), &(dpu->wglDc), &(dpu->wglGlrc))) AfxThrowError();
    else
    {
        dpu->wglDcPxlFmt = SglGetPixelFormat(dpu->wglDc);

        if (!(_wglMakeCurrent(dpu->wglDc, dpu->wglGlrc))) AfxThrowError();
        else
        {
            afxNat aa = GetDeviceCaps(dpu->wglDc, HORZSIZE);
            afxNat ab = GetDeviceCaps(dpu->wglDc, VERTSIZE);
            afxNat ac = GetDeviceCaps(dpu->wglDc, HORZRES);
            afxNat ad = GetDeviceCaps(dpu->wglDc, VERTRES);
            afxNat ae = GetDeviceCaps(dpu->wglDc, LOGPIXELSX);
            afxNat af = GetDeviceCaps(dpu->wglDc, LOGPIXELSY);
            afxNat ag = GetDeviceCaps(dpu->wglDc, BITSPIXEL);
            afxNat ah = GetDeviceCaps(dpu->wglDc, PLANES);
            afxNat ai = GetDeviceCaps(dpu->wglDc, ASPECTX);
            afxNat aj = GetDeviceCaps(dpu->wglDc, ASPECTY);
            afxNat ak = GetDeviceCaps(dpu->wglDc, ASPECTXY);
            afxNat al = GetDeviceCaps(dpu->wglDc, VREFRESH);
            afxNat am = GetDeviceCaps(dpu->wglDc, BLTALIGNMENT);
            AfxAdvertise("%i %i %i %i %i %i %i %i %i %i %i %i %i ", aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am);


            HWND dsk = GetDesktopWindow();
            HDC dskWndDc2 = GetWindowDC(dsk);
            HDC dskWndDc = GetWindowDC(NULL);
            HDC dskDc2 = GetDC(dsk);
            HDC dskDc = GetDC(NULL);
            int dskPf = SglGetPixelFormat(dskDc);
            int dskWndPf = SglGetPixelFormat(dskWndDc);
            PIXELFORMATDESCRIPTOR pfd, pfd2;
            SglDescribePixelFormat(dskDc, dskPf, sizeof(pfd), &pfd);
            SglDescribePixelFormat(dskWndDc, dskWndPf, sizeof(pfd2), &pfd2);

            sglVmt* gl = (sglVmt*)&dpu->wglVmt;
            _SglLoadVmt(gl, 1, 0);

            afxString ver;
            AfxWrapStringLiteral(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxScanString(&ver, "%u.%u.%u", &dpu->wglGlrcVerMajor, &dpu->wglGlrcVerMinor, &dpu->wglGlrcVerPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dpu->wglGlrcVerMajor));
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dpu->wglGlrcVerMinor));

            _SglLoadVmt(gl, 4, 6);

            //afxString tmp;
            AfxWrapStringLiteral(&dpu->subsysName, (afxChar const*)gl->GetString(GL_RENDERER), 0);

            //gl->Enable(GL_FRAMEBUFFER_SRGB);
            gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            //dthr->thr.proc = _SglDthrProcCb;


            //HDC dcBkp = _wglGetCurrentDC();
            //HGLRC rcBkp = _wglGetCurrentContext();

            //if (!_wglMakeCurrent(idd->dpus[unitIdx].wglDc, idd->dpus[unitIdx].wglGlrc))
                //AfxThrowError();
        }

        if (err)
        {
            _wglMakeCurrent(NIL, NIL);
            _wglDeleteContext(dpu->wglGlrc);
            ReleaseDC(dpu->wglWnd, dpu->wglDc);
            DestroyWindow(dpu->wglWnd);
        }
    }

    return err;
}

_SGL afxError _SglDestroyDpu(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);    
    _sglDdrvIdd *idd = ddrv->idd;
    sglDpuIdd *dpu = &idd->dpus[unitIdx];

    _SglDthrProcessDpuDeletionQueue(ddrv, unitIdx);
    AfxReleaseSlock(&dpu->deletionLock);
    AfxReleaseQueue(&dpu->deletionQueue);

    HDC dc = _wglGetCurrentDC();
    HGLRC rc = _wglGetCurrentContext();

    if (dc == dpu->wglDc || rc == dpu->wglGlrc)
        _wglMakeCurrent(NIL, NIL);

    _wglDeleteContext(dpu->wglGlrc);
    ReleaseDC(dpu->wglWnd, dpu->wglDc);
    DestroyWindow(dpu->wglWnd);
    return err;
}

_SGL afxError _SglDdrvVmtDtorCb(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    _sglDdrvIdd *idd = ddrv->idd;

    UnregisterClass(idd->oglWndClss.lpszClassName, idd->oglWndClss.hInstance);
    
    afxMemory mem = AfxGetDrawMemory();
    AfxAssertObject(mem, AFX_FCC_MEM);

    for (afxNat i = 0; i < idd->dpuCnt; i++)
        if (_SglDestroyDpu(ddrv, i))
            AfxThrowError();

    AfxDeallocate(mem, idd->dpus);

    AfxDeallocate(mem, idd);
    ddrv->idd = NIL;

    return err;
}

_SGL afxBool _SglProcessContextCb(afxDrawContext dctx, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawThread dthr = udd;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    dthr->dctx = dctx;

    if (dctx->vmt->proc(dctx, dthr))
        AfxThrowError();

    dthr->dctx = NIL;
    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDdrvVmtProcCb(afxDrawDriver ddrv, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    afxNat unitIdx = AfxGetSystemProcessor();

    _sglDdrvIdd *idd = ddrv->idd;

    if (unitIdx >= idd->dpuCnt)
        return err;

    if (!_wglMakeCurrent(idd->dpus[unitIdx].wglDc, idd->dpus[unitIdx].wglGlrc))
        AfxThrowError();

    dthr->portIdx = unitIdx;

    AfxCurateDrawContexts(ddrv, 0, AfxGetDrawContextCount(ddrv), _SglProcessContextCb, dthr);

    _SglDthrProcessDpuDeletionQueue(ddrv, unitIdx); // delete after is safer?

    return err;
}

_SGL _afxDdrvVmt SglDrvVmt =
{
    _SglDdrvVmtDtorCb,
    _SglDdrvVmtProcCb,
    _SglDdrvVmtDqueCb,
    _SglDdrvVmtDctxCb,
    _SglDdrvVmtDoutCb,
    _SglDdrvVmtDinCb
};

_SGL afxError _SglDdrvCtorCb(afxDrawDriver ddrv, afxDrawDriverSpecification const *spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    (void)spec;

    AfxAssert(mainThreadId == AfxGetTid());

    afxMemory mem = AfxGetDrawMemory();
    AfxAssertObject(mem, AFX_FCC_MEM);

    ddrv->vmt = &SglDrvVmt;

    _sglDdrvIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());

    if (!idd) AfxThrowError();
    else
    {
        ddrv->idd = idd;

        if (!opengl32)
            _AfxLoadOpengl();

        idd->oglWndClss.cbSize = sizeof(idd->oglWndClss); // only on EX
        idd->oglWndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        idd->oglWndClss.lpfnWndProc = _SglWndHndlngPrcW32Callback;
        idd->oglWndClss.cbClsExtra = sizeof(ddrv);
        idd->oglWndClss.cbWndExtra = sizeof((afxDrawOutput)0);
        idd->oglWndClss.hInstance = GetModuleHandleA(NULL);
        idd->oglWndClss.hIcon = NULL;
        idd->oglWndClss.hCursor = NULL;
        idd->oglWndClss.hbrBackground = NULL;
        idd->oglWndClss.lpszMenuName = NULL;
        idd->oglWndClss.lpszClassName = "SIGMA Future Draw System --- Public Test Build --- QWADRO (c) 2017 SIGMA";
        idd->oglWndClss.hIconSm = NULL;

        if (!(RegisterClassEx(&(idd->oglWndClss)))) AfxThrowError();
        else
        {
            AfxComment("Listing mem available display devices...\n");

            //HDC hdc = NIL;
            afxNat idx = 0;
            DISPLAY_DEVICE ddev;
            while (1)
            {
                ddev.cb = sizeof(ddev);

                if (!(EnumDisplayDevices(NULL, idx, &ddev, NIL))) break;
                else
                {
                    AfxComment("#%u = %s (%s) %x", idx, ddev.DeviceString, ddev.DeviceName, ddev.StateFlags);
                    idx++;
                }
            };

#if 0
            for (afxNat i = 0; i < AfxGetSystemConcurrencyCapacity(); i++)
            {
                afxDrawThreadSpecification dthrSpec = { 0 };
                //dthrSpec.base.affinityMask = AFX_FLAG(0);

                afxDrawThread dthr;

                if (!(dthr = _AfxAcquireDrawThread(ddrv, &dthrSpec))) AfxThrowError();
                else
                {
                    AfxAssertObject(dthr, AFX_FCC_DTHR);
                    AfxSetThreadAffinity(&dthr->thr, i);
                    AfxRunThread(&dthr->thr);
                }
            }
#endif
            
            idd->dpuCnt = 1;// AfxGetSystemConcurrencyCapacity();

            idd->dpus = AfxAllocate(mem, idd->dpuCnt * sizeof(idd->dpus[0]), 0, AfxSpawnHint());

            AfxZero(idd->dpus, idd->dpuCnt * sizeof(idd->dpus[0]));

            for (afxNat i = 0; i < idd->dpuCnt; i++)
            {
                if (_SglBuildDpu(ddrv, i))
                {
                    AfxThrowError();

                    for (afxNat j = 0; j < i; j++)
                        if (_SglDestroyDpu(ddrv, j))
                            AfxThrowError();
                }
            }

            afxNat unitIdx = AfxGetSystemProcessor();

            if (!_wglMakeCurrent(idd->dpus[unitIdx].wglDc, idd->dpus[unitIdx].wglGlrc))
                AfxThrowError();

            if (err)
            {
                UnregisterClass(idd->oglWndClss.lpszClassName, idd->oglWndClss.hInstance);
            }
        }

        if (err)
            AfxDeallocate(mem, idd);
    }

    return err;
}

_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;

    static afxString name, author, website, note;
    AfxWrapStringLiteral(&name, "4DGL", 0);
    AfxWrapStringLiteral(&author, "SIGMA Technology Group", 0);
    AfxWrapStringLiteral(&website, "www.sigmaco.org", 0);
    AfxWrapStringLiteral(&note, /*sigglSigmaSignature*/quadroGlSigmaSignature, 0);
    static afxDrawDriverFeatures const features = { 0 };
    static afxDrawDriverSpecification spec = { 0 };
    spec.mdle = mdle;
    spec.name = &name;
    spec.author = &author;
    spec.website = &website;
    spec.note = &note;
    spec.verMajor = 0;
    spec.verMinor = 7;
    spec.verPatch = 2;
    spec.features = &features;
    spec.ctor = _SglDdrvCtorCb;
    
    afxDrawDriver ddrv;

    if (AfxRegisterDrawDriver(&spec, &ddrv)) AfxThrowError();
    else
    {
        AfxAssert(ddrv->vmt);
    }
    return err;
}