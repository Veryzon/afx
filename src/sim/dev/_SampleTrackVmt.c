#define _AMX_SKELETON_C
#include "../../dev/AmxImplKit.h"
#include "../../dev/AvxImplKit.h"

void SampleTrackUUUAtTime0(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4, afxM4d const ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult)
{
    if (CompositeResult)
        BuildFullWorldPoseComposite_Generic(&Track->lodTransform, ParentMatrix, InverseWorld4x4, CompositeResult, WorldResult);
    else
        BuildFullWorldPoseOnly_Generic(&Track->lodTransform, ParentMatrix, WorldResult);
}

void SampleTrackUUULocalAtTime0(afxMotionTransform const* SourceTrack, afxTransform *Result)
{
    Result->flags = 0;

    if (AfxIsCurveIdentity(SourceTrack->translation))
    {
        AfxV3dZero(Result->position);
    }
    else
    {
        Result->flags |= 1u;
        AfxExtractCurveKnotValue(SourceTrack->translation, 0, Result->position, AFX_V3D_ZERO);
    }

    if (AfxIsCurveIdentity(SourceTrack->transmission))
    {
        AfxQuatReset(Result->orientation);
    }
    else
    {
        Result->flags |= 2u;
        AfxExtractCurveKnotValue(SourceTrack->transmission, 0, Result->orientation, AFX_QUAT_IDENTITY);
    }

    if (AfxIsCurveIdentity(SourceTrack->transmutation))
    {
        AfxM3dReset(Result->scaleShear);
    }
    else
    {
        Result->flags |= 4u;
        
        AfxExtractCurveKnotValue(SourceTrack->transmutation, 0, Result->scaleShear, AFX_M3D_IDENTITY);
    }
}

void SampleTrackUUULocal(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxTransform *Result)
{
    afxBool v6; // zf

    afxByte v5 = Track->flags & akxBoundMotionTransformFlag_TranslationMask;

    if (v5 == akxBoundMotionTransformFlag_TranslationIsKeyframed)
    {
        Result->flags |= 1u;
        AfxExtractCurveKnotValue(SourceTrack->translation, Context->FrameIndex, Result->position, AFX_V3D_IDENTITY);
    }
    else if (v5)
    {
        v6 = v5 == akxBoundMotionTransformFlag_TranslationIsConstant;
        afxTransformFlags v7 = Result->flags;

        if (v6)
        {
            Result->flags = v7 | 1;
            AfxV3dCopy(Result->position, Track->lodTransform.position);
        }
        else
        {
            Result->flags = v7 | 1;
            Smt2.EvaluateCurve(Context, SourceTrack->translation, AFX_V3D_ZERO, 3, Result->position, 0);
        }
    }
    else
    {
        AfxV3dZero(Result->position);
    }

    afxByte v8 = Track->flags & akxBoundMotionTransformFlag_TransmissionMask;

    if (v8 == akxBoundMotionTransformFlag_TransmissionIsKeyframed)
    {
        Result->flags |= 2u;
        AfxExtractCurveKnotValue(SourceTrack->transmission, Context->FrameIndex, Result->orientation, AFX_QUAT_IDENTITY);
    }
    else if (v8)
    {
        v6 = v8 == akxBoundMotionTransformFlag_TransmissionIsConstant;
        afxTransformFlags v9 = Result->flags;

        if (v6)
        {
            Result->flags = v9 | 2;
            AfxV4dCopy(Result->orientation, Track->lodTransform.orientation);
        }
        else
        {
            Result->flags = v9 | 2;
            Smt2.EvaluateCurve(Context, SourceTrack->transmission, AFX_QUAT_IDENTITY, 4, Result->orientation, 1);
        }
    }
    else
    {
        AfxResetV4d(Result->orientation);
    }

    afxByte v10 = Track->flags & akxBoundMotionTransformFlag_TransmutationMask;

    if (v10 == akxBoundMotionTransformFlag_TransmutationIsKeyframed)
    {
        Result->flags |= 4u;
        AfxExtractCurveKnotValue(SourceTrack->transmutation, Context->FrameIndex, Result->scaleShear, AFX_M3D_IDENTITY);
    }
    else if (v10)
    {
        if (v10 == akxBoundMotionTransformFlag_TransmutationIsConstant)
        {
            Result->flags |= 4u;
            AfxM3dCopy(Result->scaleShear, Track->lodTransform.scaleShear);
        }
        else
        {
            Result->flags |= 4u;
            Smt2.EvaluateCurve(Context, SourceTrack->transmutation, &AFX_M3D_IDENTITY[0][0], 9, (afxReal *)Result->scaleShear, 0);
        }
    }
    else
    {
        AfxM3dReset(Result->scaleShear);
    }
}

void SampleTrackUUUBlendWithTime0(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4, afxM4d const ParentMatrix, afxM4d WorldResult, afxM4d CompositeResult, afxReal BlendAmount)
{
    afxTransform Transform = { 0 };
    Transform.flags = 0;
    SampleTrackUUULocal(Context, SourceTrack, Track, &Transform);
    AfxMixTransform(&Transform, &Transform, &Track->lodTransform, BlendAmount);

    if (CompositeResult)
        BuildFullWorldPoseComposite_Generic(&Transform, ParentMatrix, InverseWorld4x4, CompositeResult, WorldResult);
    else
        BuildFullWorldPoseOnly_Generic(&Transform, ParentMatrix, WorldResult);
}

void SampleTrackUUU(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    afxTransform Transform = { 0 };
    SampleTrackUUULocal(Context, SourceTrack, Track, &Transform);

    if (CompositeResultAliased)
        BuildFullWorldPoseComposite_Generic(&Transform, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildFullWorldPoseOnly_Generic(&Transform, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackIII(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    if (CompositeResultAliased)
        BuildIdentityWorldPoseComposite_Generic(ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildIdentityWorldPoseOnly_Generic(ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackCII(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    if (CompositeResultAliased)
        BuildPositionWorldPoseComposite_Generic(Track->lodTransform.position, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionWorldPoseOnly_Generic(Track->lodTransform.position, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackCCI(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    if (CompositeResultAliased)
        BuildPositionOrientationWorldPoseComposite_Generic(Track->lodTransform.position, Track->lodTransform.orientation, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionOrientationWorldPoseOnly_Generic(Track->lodTransform.position, Track->lodTransform.orientation, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackCAI(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    afxReal *piBuffer; // [esp+Ch] [ebp-9Ch]
    afxReal *tiBuffer; // [esp+10h] [ebp-98h]
    afxReal Orientation[4] = {0}; // [esp+20h] [ebp-88h]
    afxReal tiBufferStack[6] = {0}; // [esp+30h] [ebp-78h]
    afxReal piBufferStack[24] = {0}; // [esp+48h] [ebp-60h]

    afxV3d position;
    AfxV3dCopy(position, Track->lodTransform.position);

    afxUnit v12 = AfxGetCurveDegree(SourceTrack->transmission);
    afxUnit v13 = AfxFindCurveKnot(SourceTrack->transmission, Context->LocalClock);

    if (Smt2.ConstructBSplineBuffers(
        4,
        (Context->UnderflowLoop != 0 ? SourceTrack->transmission : 0),
        SourceTrack->transmission,
        (Context->OverflowLoop != 0 ? SourceTrack->transmission : 0), 
        Context->LocalDuration, Context->LocalDuration, Context->LocalDuration, 
        v13, 
        tiBufferStack, piBufferStack, &tiBuffer, &piBuffer, 
        AFX_QUAT_IDENTITY))
    {
        AfxEnsureQuaternionContinuity(v12 + 1, piBufferStack);
    }
    Smt2.SampleBSpline(v12, 4, TRUE, tiBuffer, piBuffer, Context->LocalClock, Orientation);

    if (CompositeResultAliased)
        BuildPositionOrientationWorldPoseComposite_Generic(position, Orientation, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionOrientationWorldPoseOnly_Generic(position, Orientation, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackAII(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    afxReal Position[3]; // [esp+Ch] [ebp-6Ch]
    afxReal tiBufferStack[6]; // [esp+18h] [ebp-60h]
    afxReal piBufferStack[18]; // [esp+30h] [ebp-48h]

    afxUnit v9 = AfxGetCurveDegree(SourceTrack->translation);

    Smt2.ConstructBSplineBuffers(
        3,
        (Context->UnderflowLoop != 0 ? SourceTrack->translation : 0),
        SourceTrack->translation,
        (Context->OverflowLoop != 0 ? SourceTrack->translation : 0),
        Context->LocalDuration,
        Context->LocalDuration,
        Context->LocalDuration,
        AfxFindCurveKnot(SourceTrack->translation, Context->LocalClock),
        tiBufferStack,
        piBufferStack,
        (afxReal **)&Context,
        (afxReal **)&SourceTrack,
        AFX_V3D_ZERO);

    Smt2.SampleBSpline(v9, 3, FALSE, &Context->LocalClock, (const afxReal *)SourceTrack, Context->LocalClock, Position);

    if (CompositeResultAliased)
        BuildPositionWorldPoseComposite_Generic(Position, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionWorldPoseOnly_Generic(Position, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackACI(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    afxReal *tiBuffer; // [esp+Ch] [ebp-84h]
    afxReal *piBuffer; // [esp+10h] [ebp-80h]
    afxReal Orientation[4]; // [esp+14h] [ebp-7Ch]
    afxReal Position[3]; // [esp+24h] [ebp-6Ch]
    afxReal tiBufferStack[6]; // [esp+30h] [ebp-60h]
    afxReal piBufferStack[18]; // [esp+48h] [ebp-48h]

    afxUnit v8 = AfxGetCurveDegree(SourceTrack->translation);
    Smt2.ConstructBSplineBuffers(
        3,
        (Context->UnderflowLoop != 0 ? SourceTrack->translation : 0),
        SourceTrack->translation,
        (Context->OverflowLoop != 0 ? SourceTrack->translation : 0),
        Context->LocalDuration,
        Context->LocalDuration,
        Context->LocalDuration,
        AfxFindCurveKnot(SourceTrack->translation, Context->LocalClock),
        tiBufferStack,
        piBufferStack,
        &tiBuffer,
        &piBuffer,
        AFX_V3D_ZERO);

    Smt2.SampleBSpline(v8, 3, FALSE, tiBuffer, piBuffer, Context->LocalClock, Position);
    Orientation[0] = Track->lodTransform.orientation[0];
    Orientation[3] = Track->lodTransform.orientation[3];
    Orientation[1] = Track->lodTransform.orientation[1];
    Orientation[2] = Track->lodTransform.orientation[2];

    if (CompositeResultAliased)
        BuildPositionOrientationWorldPoseComposite_Generic(Position, Orientation, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionOrientationWorldPoseOnly_Generic(Position, Orientation, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackAAI(const sample_context *Context, const afxMotionTransform*SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    afxReal *piBuffer; // [esp+10h] [ebp-9Ch]
    afxReal *tiBuffer; // [esp+14h] [ebp-98h]
    afxReal Position[3]; // [esp+18h] [ebp-94h]
    afxReal Orientation[4]; // [esp+24h] [ebp-88h]
    afxReal tiBufferStack[6]; // [esp+34h] [ebp-78h]
    afxReal piBufferStack[24]; // [esp+4Ch] [ebp-60h]

    afxUnit v10 = AfxFindCurveKnot(SourceTrack->translation, Context->LocalClock);
    Smt2.ConstructBSplineBuffers(
        3,
        (Context->UnderflowLoop != 0 ? SourceTrack->translation : 0),
        SourceTrack->translation,
        (Context->OverflowLoop != 0 ? SourceTrack->translation : 0),
        Context->LocalDuration, Context->LocalDuration, Context->LocalDuration, v10, tiBufferStack, piBufferStack, &tiBuffer, &piBuffer, AFX_V3D_ZERO);


    Smt2.SampleBSpline(AfxGetCurveDegree(SourceTrack->translation), 3, FALSE, tiBuffer, piBuffer, Context->LocalClock, Position);
    afxUnit v11 = AfxGetCurveDegree(SourceTrack->transmission);

    if (Smt2.ConstructBSplineBuffers(
        4,
        (Context->UnderflowLoop != 0 ? SourceTrack->transmission : 0),
        SourceTrack->transmission,
        (Context->OverflowLoop != 0 ? SourceTrack->transmission : 0),
        Context->LocalDuration, Context->LocalDuration, Context->LocalDuration, AfxFindCurveKnot(SourceTrack->transmission, Context->LocalClock), tiBufferStack, piBufferStack, &piBuffer, &tiBuffer, AFX_QUAT_IDENTITY))
    {
        AfxEnsureQuaternionContinuity(v11 + 1, piBufferStack);
    }

    Smt2.SampleBSpline(v11, 4, TRUE, piBuffer, tiBuffer, Context->LocalClock, Orientation);

    if (CompositeResultAliased)
        BuildPositionOrientationWorldPoseComposite_Generic(Position, Orientation, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildPositionOrientationWorldPoseOnly_Generic(Position, Orientation, ParentMatrixAliased, WorldResultAliased);
}

void(__cdecl *Samplers[3][3][3])(const sample_context *, const afxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, afxM4d const, afxM4d const, afxM4d, afxM4d) =
{
  {
    { &SampleTrackIII, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackUUU, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackUUU, &SampleTrackUUU, &SampleTrackUUU }
  },
  {
    { &SampleTrackCII, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackCCI, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackCAI, &SampleTrackUUU, &SampleTrackUUU }
  },
  {
    { &SampleTrackAII, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackACI, &SampleTrackUUU, &SampleTrackUUU },
    { &SampleTrackAAI, &SampleTrackUUU, &SampleTrackUUU }
  }
};

void SampleTrackPOLocal(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, afxV3d ResultPosition, afxQuat ResultOrientation)
{
    if (AfxIsCurveIdentity(SourceTrack->translation))
    {
        ResultPosition[2] = 0.0;
        ResultPosition[1] = 0.0;
        *ResultPosition = 0.0;
    }
    else
    {
        Smt2.EvaluateCurve(Context, SourceTrack->translation, AFX_V3D_ZERO, 3, ResultPosition, 0);
    }

    if (AfxIsCurveIdentity(SourceTrack->transmission))
    {
        *ResultOrientation = 0.0;
        ResultOrientation[1] = 0.0;
        ResultOrientation[2] = 0.0;
        ResultOrientation[3] = 1.0;
    }
    else
    {
        Smt2.EvaluateCurve(Context, SourceTrack->transmission, AFX_QUAT_IDENTITY, 4, ResultOrientation, 1);
    }
}

static afxBool TransformTrackHasKeyframedCurves(const afxMotionTransform *Track)
{
    return AfxIsCurveKeyframed(Track->translation)
        || AfxIsCurveKeyframed(Track->transmission)
        || AfxIsCurveKeyframed(Track->transmutation);
}

int CurveKnotCharacter(afxCurve Curve)
{
    int result; // eax

    if (AfxIsCurveIdentity(Curve))
        result = 0;
    else
        result = 2 - (AfxIsCurveConstantOrIdentity(Curve) != 0);

    return result;
}

void(*GetTrackSamplerFor(const afxMotionTransform *Track))(const sample_context *, const afxMotionTransform *, akxBoundMotionTransform *, const afxTransform *, afxM4d const, afxM4d const, afxM4d, afxM4d)
{
    int v2; // esi
    int v3; // esi

    if (TransformTrackHasKeyframedCurves(Track))
        return SampleTrackUUU;

    v2 = 3 * CurveKnotCharacter(Track->translation);
    v3 = 3 * (CurveKnotCharacter(Track->transmission) + v2);
    return Samplers[0][0][CurveKnotCharacter(Track->transmutation) + v3];
}

void SampleTrackSSS(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    if (CompositeResultAliased)
        BuildFullWorldPoseComposite_Generic(RestTransform, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
    else
        BuildFullWorldPoseOnly_Generic(RestTransform, ParentMatrixAliased, WorldResultAliased);
}

void SampleTrackIIU(const sample_context *Context, const afxMotionTransform *SourceTrack, akxBoundMotionTransform *Track, const afxTransform *RestTransform, afxM4d const InverseWorld4x4Aliased, afxM4d const ParentMatrixAliased, afxM4d WorldResultAliased, afxM4d CompositeResultAliased)
{
    int v9; // ST08_4
    afxTransform Transform; // [esp+4h] [ebp-44h]

    afxCurve v8 = SourceTrack->transmutation;

    if (AfxIsCurveKeyframed(SourceTrack->transmutation))
    {
        v9 = Context->FrameIndex;
        Transform.flags = 4;
        AfxExtractCurveKnotValue(v8, v9, (afxReal *)Transform.scaleShear, AFX_M3D_IDENTITY);
        
        if (CompositeResultAliased)
            BuildFullWorldPoseComposite_Generic(&Transform, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
        else
            BuildFullWorldPoseOnly_Generic(&Transform, ParentMatrixAliased, WorldResultAliased);
    }
    else if (AfxIsCurveIdentity(v8))
    {
        if (CompositeResultAliased)
            BuildIdentityWorldPoseComposite_Generic(ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
        else
            BuildIdentityWorldPoseOnly_Generic(ParentMatrixAliased, WorldResultAliased);
    }
    else
    {
        Transform.flags = 4;
        Smt2.EvaluateCurve(Context, v8, AFX_M3D_IDENTITY, 9, (afxReal *)Transform.scaleShear, 0);

        if (CompositeResultAliased)
            BuildFullWorldPoseComposite_Generic(&Transform, ParentMatrixAliased, InverseWorld4x4Aliased, CompositeResultAliased, WorldResultAliased);
        else
            BuildFullWorldPoseOnly_Generic(&Transform, ParentMatrixAliased, WorldResultAliased);
    }
}

smt Smt = 
{
    .SampleTrackUUUAtTime0 = (void*)SampleTrackUUUAtTime0,
    .SampleTrackUUULocalAtTime0 = SampleTrackUUULocalAtTime0,
    .SampleTrackUUULocal = SampleTrackUUULocal,
    .SampleTrackUUUBlendWithTime0 = (void*)SampleTrackUUUBlendWithTime0,
    .SampleTrackUUU = SampleTrackUUU,
    .SampleTrackIII = SampleTrackIII,
    .SampleTrackCII = SampleTrackCII,
    .SampleTrackCCI = SampleTrackCCI,
    .SampleTrackCAI = SampleTrackCAI,
    .SampleTrackAII = SampleTrackAII,
    .SampleTrackACI = SampleTrackACI,
    .SampleTrackAAI = SampleTrackAAI,
    .Samplers = Samplers,
    .SampleTrackSSS = (void*)SampleTrackSSS,
    .GetTrackSamplerFor = GetTrackSamplerFor,
    .SampleTrackIIU = (void*)SampleTrackIIU,
    .SampleTrackPOLocal = SampleTrackPOLocal
};
