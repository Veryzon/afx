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

#ifndef AFX_WORLD_POSE_H
#define AFX_WORLD_POSE_H

#include "qwadro/math/afxMathDefs.h"

AFX_DEFINE_HANDLE(akxPoseBuffer);

AFX_OBJECT(akxPoseBuffer)
{
    afxNat      matCnt;
    afxM4d*     world;
    afxM4d*     composite;
};

/// O objeto akxPoseBuffer � um buffer usado para manter o estado de um afxSkeleton de afxModel como expressado no "world space". 
/// Voc� pode criar uma akxPoseBuffer assim: 

AKX afxError    AfxAcquirePoseBuffers(afxSimulation sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], akxPoseBuffer wp[]);

/// Voc� pode encontrar o n�mero de articula��es representadas na akxPoseBuffer assim: 

AKX afxNat      AfxGetPoseBufferCapacity(akxPoseBuffer const wp);

/// Em qualquer tempo, voc� pode inspecionar ou modificar o estado alojado de uma articula��o na akxPoseBuffer. 
/// Voc� acessa o estado da articula��o como um afxTransform assim:

AKX afxM4d*     AfxGetWorldMatrices(akxPoseBuffer const wp, afxNat baseArtIdx);

/// O transforme para a articula��o � uma afxM4d com componentes translacionais em afxReal[3][0], afxReal[3][1] e afxReal[3][2]. 
/// Desde que nunca h� quaisquer componentes projetivos, os componentes afxReal[0][3], afxReal[1][3] e afxReal[2][3] s�o garantidos a estarem zerados, e o componente afxReal[3][3] � garantido de ser 1.

/// Similarmente, voc� pode inspecionar ou modificar a afxM4d composta para uma articula��o: 

AKX afxM4d*     AfxGetCompositeMatrices(akxPoseBuffer const wp, afxNat baseArtIdx);

/// A afxM4d composta est� no mesmo layout como aquela retornada de GetPoseBuffer4x4. 
/// � igual ao "world space" --- afxM4d espacial para a articula��o (como dada por GetPoseBuffer4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articula��o (como alojada no afxSkeleton). 
/// Portanto, esta representa a diferen�a entre a atual posi��o no world-space da articula��o e sua posi��o de repouso no world-space.

/// As afxM4d de transforma��o no world-space e compostas s�o garantida a estarem cont�guas, assim sendo, voc� pode tamb�m obter o ponteiro para o in�cio da array e us�-lo para manualmente acessar articula��es individuais. 

AKX afxM4d*     AfxPoseBufferGetWorldMatrixArray(akxPoseBuffer const wp);
AKX afxM4d*     AfxPoseBufferGetCompositeMatrixArray(akxPoseBuffer const wp);

#endif//AFX_WORLD_POSE_H
