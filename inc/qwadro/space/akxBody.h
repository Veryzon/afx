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

/// O objeto akxBody aloja o estado de um afxModel no tempo de execu��o.
/// O afxModel � apenas dados estruturados para um modelo particular --- isto �, este n�o h� qualquer conceito de onde este est�, qual estado este est� em, ou quais anima��es est�o interagindo em si.
/// O akxBody mant�m rastreio disto e prov�m um n�mero de fun��es �teis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades id�nticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam id�nticos, cada um deveria ainda haver seu pr�prio akxBody.
/// Dito isto porque os dados do akxBody s�o todos din�micos, e n�o s�o compartilhados atrav�s de m�ltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto � comum colocar a hierarquia de estruturas do afxModel no cache e instanci�-las m�ltiplas vezes, voc� nunca deveria colocar objetos akxBody no cache e reus�-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da exist�ncia das entidades que eles representam na sua aplica��o.

/// Note, no entanto, que estes s�o simplesmente ponteiros para os dados originais que voc� passou durante a instancia��o.
/// O akxBody n�o mant�m c�pias dos dados (para minimizar uso de mem�ria), ent�o voc� n�o pode liberar os dados originais do modelo e esperar obter ponteiros v�lidos de volta aqui.
/// Para aquilo que importa, se voc� liberar os dados originais do modelo, nenhuma das chamadas para akxBody funcionar�o, desde que elas todas dependem daqueles dados, como voc� esperaria.

#ifndef AKX_BODY_H
#define AKX_BODY_H

#include "qwadro/cad/afxModel.h"
#include "qwadro/space/akxMotor.h"

AKX afxBool     AfxGetBodyModel(akxBody bod, afxModel* model);
AKX afxBool     AfxGetBodySkeleton(akxBody bod, afxSkeleton* skeleton);
    
AKX void        AkxResetBodyClock(akxBody bod);
AKX void        AfxUpdateBodyMatrix(akxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

AKX void        AfxRecenterLinkedMotorClocks(akxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AKX void        AfxReclockMotives(akxBody bod, afxReal newClock);

AKX void        AfxPurgeTerminatedMotives(akxBody bod);

AKX void        AfxGetBodyRootMotionVectors(akxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

AKX void        AfxSampleBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void        AfxSampleBodyAnimationsAcceleratedLOD(akxBody bod, afxNat pivotCnt, afxM4d const offset, akxPose scratch, akxPoseBuffer rslt, afxReal allowedErr);
AKX afxBool     AfxSampleSingleBodyAnimationLODSparse(akxBody bod, akxMotor moto, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void        AfxAccumulateBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);

AKX void        AkxCmdBindPose(avxCmdb cmdb, akxPose pose);
AKX void        AkxCmdBindPoseBuffer(avxCmdb cmdb, akxPoseBuffer wp);
AKX void        AkxCmdBindSparsePivotMap(avxCmdb cmdb, afxNat const* sparseBoneArray);
AKX void        AkxCmdSetAllowedSamplingError(avxCmdb cmdb, afxReal allowedErr);
AKX void        AkxCmdSampleBodyMotions(avxCmdb cmdb, akxBody bod, afxNat basePivot, afxNat pivotCnt);

AKX void        AfxSetBodyMass(akxBody bod, afxV3d mass);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AfxSpawnBodies(afxModel proto, afxNat cnt, akxBody bod[]);

AKX afxError    AfxAcquireBodies(afxModel mdl, afxNat cnt, akxBody bod[]);

#endif//AKX_BODY_H
