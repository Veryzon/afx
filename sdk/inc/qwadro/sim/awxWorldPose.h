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

#ifndef AFX_WORLD_POSE_H
#define AFX_WORLD_POSE_H

#include "qwadro/math/afxMathDefs.h"

AFX_DEFINE_STRUCT(awxWorldPose)
{
    afxNat              xformCnt;
    afxM4d              *world;
    afxM4d              *composite;
};

/// O objeto awxWorldPose � um buffer usado para manter o estado de um awxSkeleton de awxModel como expressado no "world space". 
/// Voc� pode criar uma awxWorldPose assim: 

AFX afxError    AfxAcquireWorldPoses(void *sim, afxNat cnt, afxNat const artCnt[], afxBool const excludeComposite[], awxWorldPose *wp[]);

/// e quando voc� estiver satifeito de us�-lo, voc� deve liber�-lo assim: 

AFX void        AfxReleaseWorldPoses(afxNat cnt, awxWorldPose *wp[]);

/// Voc� pode encontrar o n�mero de articula��es representadas na awxWorldPose assim: 

AFX afxNat      AfxWorldPoseGetArticulationCount(awxWorldPose const *wp);

/// Em qualquer tempo, voc� pode inspecionar ou modificar o estado alojado de uma articula��o na awxWorldPose. 
/// Voc� acessa o estado da articula��o como um afxTransform assim:

AFX afxV4d*     AfxWorldPoseGetWorldMatrix(awxWorldPose const *wp, afxNat artIdx);

/// O transforme para a articula��o � uma afxM4d com componentes translacionais em afxReal[3][0], afxReal[3][1] e afxReal[3][2]. 
/// Desde que nunca h� quaisquer componentes projetivos, os componentes afxReal[0][3], afxReal[1][3] e afxReal[2][3] s�o garantidos a estarem zerados, e o componente afxReal[3][3] � garantido de ser 1.

/// Similarmente, voc� pode inspecionar ou modificar a afxM4d composta para uma articula��o: 

AFX afxV4d*     AfxWorldPoseGetCompositeMatrix(awxWorldPose const *wp, afxNat artIdx);

/// A afxM4d composta est� no mesmo layout como aquela retornada de GetWorldPose4x4. 
/// � igual ao "world space" --- afxM4d espacial para a articula��o (como dada por GetWorldPose4x4) multiplicada pelo transforme inverso de repouso no world-space --- para a articula��o (como alojada no awxSkeleton). 
/// Portanto, esta representa a diferen�a entre a atual posi��o no world-space da articula��o e sua posi��o de repouso no world-space.

/// As afxM4d de transforma��o no world-space e compostas s�o garantida a estarem cont�guas, assim sendo, voc� pode tamb�m obter o ponteiro para o in�cio da array e us�-lo para manualmente acessar articula��es individuais. 

AFX afxM4d*     AfxWorldPoseGetWorldMatrixArray(awxWorldPose const *wp);
AFX afxM4d*     AfxWorldPoseGetCompositeMatrixArray(awxWorldPose const *wp);

#endif//AFX_WORLD_POSE_H
