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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef AMX_CAD_IMPL_KIT_H
#define AMX_CAD_IMPL_KIT_H

//#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/sim/dev/afxComboSystem.h"

#ifdef _AMX_MATERIAL_C

AFX_DEFINE_STRUCT(akxMaterialMap)
{
    afxString     usage; // usage 16
    afxMaterial     sub; // afxMaterial
};

AFX_OBJECT(afxMaterial)
{
    afxString       urn; // 128
    afxRaster       tex;
    avxSampler      smp;
    afxUnit         mapCnt;
    akxMaterialMap* maps;
    void*           extData;

    // non-Granny stuff
    afxInt          s[3];
    afxV4d          color; // albedo
    afxReal         shininess;

    afxV3d          Ka; // ambient color. The default is (0.2,0.2,0.2);
    afxV3d          Kd; // diffuse color. The default is (0.8,0.8,0.8);
    afxReal         Tr; // transparency. The default is 0.0 (not transparent at all)
    afxV3d          Ks; // specular color. This color shows up in highlights. The default is (1.0,1.0,1.0);
    afxReal         Ns; // specular exponent (aka shininess). The default is 0.0. A high exponent results in a tight, concentrated highlight.
    afxV3d          Ke; // 
    afxV3d          Tf; // Any light passing through the object is filtered by the transmission filter, which only allows the specifiec colors to pass through. For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue.
    afxReal         Ni; // optical density for the surface.  This is also known as index of refraction. The values can range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
};

//AFX_STATIC_ASSERT(offsetof(afxMaterial, color) % AFX_MTL_ALIGN == 0, "");

#endif//_AMX_MATERIAL_C

#ifdef _AMX_CURVE_C
AFX_OBJECT(afxCurve)
{
    afxCurveFormat  fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxCurveFlags   flags;
    afxUnit         knotCnt;
    union
    {
        afxReal*    knots; // 32f
        afxReal     knot0;
    };
    afxUnit         ctrlCnt;
    union
    {
        afxV4d      ctrls4; // D4, Constant32f
        afxV3d      ctrls3;
        afxReal*    ctrls; // 32f
    };
};
#endif//_AMX_CURVE_C

#ifdef _AMX_POSE_C
AFX_DEFINE_STRUCT(akxArticulation)
{
    afxReal             weight;
    afxUnit             cnt;
    afxTransform        xform;
    afxUnit             traversalId;
};

AFX_OBJECT(afxPose)
{
    afxUnit             artCnt;
    akxArticulation*    arts;
    afxReal             fillThreshold;
    afxUnit             traversalId;
};
#endif//_AMX_POSE_C

#ifdef _AMX_POSE_BUFFER_C
AFX_OBJECT(afxPoseBuffer)
{
    afxUnit     matCnt;
    afxM4d*     world;
    afxM4d*     composite;
};
#endif//_AMX_POSE_BUFFER_C

#ifdef _AMX_SKELETON_C
AFX_OBJECT(afxSkeleton)
{
    afxUnit             jointCnt;
    afxUnit*            parentIdx; // [jointCnt]
    afxTransform*       local; // [jointCnt]
    afxM4d*             iw; // [jointCnt]
    afxReal*            lodError; // [jointCnt]
    afxString*          joints; // [jointCnt]
    void**              idd; // [jointCnt]
    void**              udd; // [jointCnt]
    
    afxSkeletonFlags    flags;
    afxUnit             lodType;
    afxReal             allowedLodErrFadingFactor;
    afxString           urn; // 32
};
#endif//_AMX_SKELETON_C

#ifdef _AMX_MODEL_C

AFX_DEFINE_STRUCT(afxMeshRig)
{
    afxMesh         msh;
    afxUnit*        biasToJointMap; // [biasCnt] --- to assembled skeleton.
    afxUnit*        biasFromJointMap; // [biasCnt] --- from original skeleton.
    afxSkeleton     sklOrig;
    afxMeshRigFlags flags;
    afxMaterial     txd;
    void**          idd;
    void**          udd;
};

AFX_OBJECT(afxModel)
{
    afxSkeleton     skl;
    afxTransform    displacement;
    afxUnit         rigCnt;
    afxMeshRig*     rigs;
    afxUnit         usedRigCnt;
    afxBox          aabb;
    afxModelFlags   flags;
    afxString       urn; // 32
    void**          idd;
    void**          udd;
};

#endif


typedef struct smt2
{
    void(*CubicCoefficients)(afxReal const a[3], afxReal const b[3], afxReal t, afxReal* ci_3, afxReal* ci_2, afxReal* ci_1, afxReal* ci);
    void(*SampleBSpline)(afxUnit Degree, afxUnit Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result);
    char(*ConstructBSplineBuffers)(afxInt dim, afxCurve prev, afxCurve c, afxCurve next, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec);
    void(*EvaluateCurve)(const sample_context *Context, afxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized);
} smt2;

AMX smt2 Smt2;

#endif//AMX_CAD_IMPL_KIT_H
