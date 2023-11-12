#pragma once
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
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_ANIMATION_H
#define AFX_ANIMATION_H

#include "afx/sim/afxSimDefs.h"
#include "afxCurve2.h"
#include "afx/math/afxTransform.h"

AFX_DEFINE_STRUCT(afxMotionData)
{
    afxString32         id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve*           valueCurve;
};

AFX_DEFINE_STRUCT(afxMotionNoteEntry)
{
    afxReal             timeStamp;
    afxString128        text;
};

AFX_DEFINE_STRUCT(afxMotionNote)
{
    afxString8          id;
    afxNat              entryCnt;
    afxMotionNoteEntry* entries;
};

AFX_DEFINE_STRUCT(afxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

AFX_DEFINE_STRUCT(afxMotionTransform)
{
    afxString32         id;
    afxMask             flags;
    afxCurve*           orienCurve;
    afxCurve*           posnCurve;
    afxCurve*           scaleShearCurve;
};

#ifdef _AFX_MOTION_C
AFX_OBJECT(afxMotion)
{
    afxString32         id;
    afxMask             flags;
    afxNat              xformSeqCnt;
    afxMotionTransform* xformSeqs; // uma arranjo de registros da mo��o de junta de afxSkeleton do correspondente afxModel.
    afxNat              xformLodErrorCnt;
    afxReal*            xformLodErrors;
    afxNat              vecSeqCnt;
    afxMotionData*      vecSeqs;
    afxNat              noteSeqCnt;
    afxMotionNote*      noteSeqs;
    afxTransform        init;
    afxMotionTransform* rootMotion;
    afxV3d              loopTranslation;
    afxPeriodicLoop*    periodicLoop;    
};
#endif

/// Qwadro armazena anima��es em partes baseadas em qu�o muitos modelos est�o envolvidos numa anima��o.
/// Assim sendo, uma afxAnimation � a mo��o de um conjunto de afxModel's animando sobre tempo.
/// A afxAnimation cont�m um ou mais afxMotion's, cada do qual correspondendo � mo��o de um modelo espec�fico (desde que uma anima��o pode envolver m�ltiplos modelos, se o autor assim escolher).
/// O afxMotion � constitu�do de curvas, cada da qual especifica a transla��o, rota��o e escala de uma junta no afxSkeleton do correspondente afxModel.

#ifdef _AFX_ANIMATION_C
AFX_OBJECT(afxAnimation)
{
    afxString32         id;

    afxReal             dur; /// especifica qu�o demorado, em segundos, a anima��o leva para executar a completa��o.
    afxReal             timeStep; /// especifica qu�o muitos segundos h� entre cada frame.
    afxReal             oversampling; /// 1.f

    /// @timeStep � o intervalo ap�s oversampling.
    /// Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma anima��o foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    /// Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    /// Tanto @timeStep quanto @oversampling n�o s�o usadas pelo Qwadro.
    /// Est�o aqui de forma informativa preservando conhecimento sobre a inten��o original do autor.

    afxNat              motionCnt;
    afxMotion*          motions;
};
#endif

AFX void        AfxTransformAnimations(afxReal const lt[3][3], afxReal const ilt[3][3], afxReal const at[3], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxAnimation ani[]);

#endif//AFX_ANIMATION_H