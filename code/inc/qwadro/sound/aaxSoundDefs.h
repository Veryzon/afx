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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SOUND_DEFS_H
#define AFX_SOUND_DEFS_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxSimd.h"
#include "qwadro/math/afxTransform.h"


AFX_DEFINE_HANDLE(aaxSoundSystem);
AFX_DEFINE_HANDLE(aaxSoundDevice);
AFX_DEFINE_HANDLE(aaxSoundContext);
AFX_DEFINE_HANDLE(aaxSoundOutput);
AFX_DEFINE_HANDLE(aaxSoundInput);



typedef enum aaxCurveInterpolation // curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    aaxCurveInterpolation_Log3 = 0, // log3
    aaxCurveInterpolation_Sine = 1, // sine
    aaxCurveInterpolation_Log1 = 2, // log1
    aaxCurveInterpolation_InvSCurve = 3, // inversed S curve
    aaxCurveInterpolation_Linear = 4, // linear (default)
    aaxCurveInterpolation_SCurve = 5, // S curve
    aaxCurveInterpolation_Exp1 = 6, // exp1
    aaxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    aaxCurveInterpolation_Exp3 = 8, // exp3
    aaxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    aaxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} aaxCurveInterpolation;

AFX_DEFINE_STRUCT(aaxListener) // Listener information.
{
    afxTransform        pos; /// Listener position (see AK::SoundEngine::SetPosition()).

    afxReal32           scalingFactor; /// Listener scaling factor (see AK::SoundEngine::SetListenerScalingFactor()).
    afxBool             spatialized; /// Whether listener is spatialized or not (see AK::SoundEngine::SetListenerSpatialization()).
};

AFXINL void AaxAcquireListener(aaxListener *list)
{
    afxError err = NIL;
    AfxAssert(list);
    list->scalingFactor = (afxReal32)1;
    list->spatialized = TRUE;
}

AFX_DEFINE_STRUCT(aaxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

AFXINL void AaxTakeRamp(aaxRamp *ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AfxAssert(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

AFXINL void AaxResetRamp(aaxRamp *ramp)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AaxTakeRamp(ramp, 1, 1);
}

AFXINL void AaxCopyRamp(aaxRamp *ramp, aaxRamp const* in)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

AFXINL void AaxScaleRamp(aaxRamp *ramp, aaxRamp const *rhs)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

AFXINL aaxRamp AaxMergeRamps(aaxRamp const* lhs, aaxRamp const* rhs)
{
    aaxRamp rslt;
    AaxCopyRamp(&rslt, lhs);
    AaxScaleRamp(&rslt, rhs);
    return rslt;
}

AFX_DEFINE_STRUCT(aaxChannelConfig)
{
    afxNat32 uNumChannels : 8;   // the number of channels, identified (deduced from channel mask) or anonymous (set directly). 
    afxNat32 eConfigType : 4;    // a code that completes the identification of channels by uChannelMask.
    afxNat32 uChannelMask : 20; // a bit field, whose channel identifiers depend on aaxChannelConfigType (up to 20). Channel bits are defined in Speaker Config.
};

#endif//AFX_SOUND_DEFS_H