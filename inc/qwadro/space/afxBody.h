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

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "qwadro/cad/afxModel.h"
#include "qwadro/space/akxMotor.h"

/// O objeto afxBody aloja o estado de um afxModel no tempo de execu��o.
/// O afxModel � apenas dados estruturados para um modelo particular --- isto �, este n�o h� qualquer conceito de onde este est�, qual estado este est� em, ou quais anima��es est�o interagindo em si.
/// O afxBody mant�m rastreio disto e prov�m um n�mero de fun��es �teis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades id�nticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam id�nticos, cada um deveria ainda haver seu pr�prio afxBody.
/// Dito isto porque os dados do afxBody s�o todos din�micos, e n�o s�o compartilhados atrav�s de m�ltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto � comum colocar a hierarquia de estruturas do afxModel no cache e instanci�-las m�ltiplas vezes, voc� nunca deveria colocar objetos afxBody no cache e reus�-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da exist�ncia das entidades que eles representam na sua aplica��o.

/// Note, no entanto, que estes s�o simplesmente ponteiros para os dados originais que voc� passou durante a instancia��o.
/// O afxBody n�o mant�m c�pias dos dados (para minimizar uso de mem�ria), ent�o voc� n�o pode liberar os dados originais do modelo e esperar obter ponteiros v�lidos de volta aqui.
/// Para aquilo que importa, se voc� liberar os dados originais do modelo, nenhuma das chamadas para afxBody funcionar�o, desde que elas todas dependem daqueles dados, como voc� esperaria.

AFX_OBJECT(afxBody)
{
    afxModel            mdl; // afxModel
    afxSkeleton         cachedSkl;
    void*               cachedBones;
    afxNat              cachedBoneCnt;
    afxChain            motives;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxM4d matrix;
    afxM4d collWorldMtx;
    afxM4d invWorldInertiaMtx;
    afxQuat rot;

    afxV3d veloc;
    afxV3d omega;
    afxV3d accel;
    afxV3d alpha;
    afxV3d netForce;
    afxV3d netTorque;
    afxV3d prevExtlForce;
    afxV3d prevExtlTorque;

    afxV3d mass;
    afxV3d invMass;
    afxV3d aparentMass;
    afxV3d localCentreOfMass;
    afxV3d globalCentreOfMass;
    afxBox aabb;
    afxV3d dampCoef;

    int freeze : 1;
    int sleeping : 1;
    int autoSleep : 1;
    int isInWorld : 1;
    int equilibrium : 1;
    int continueCollisionMode : 1;
    int spawnnedFromCallback : 1;
    int collideWithLinkedBodies : 1;
    int solverInContinueCollision : 1;
    int inCallback : 1;

    akxPose             pose;
    afxM4d              placement;
};

AKX afxBool             AfxGetBodyModel(afxBody bod, afxModel* model);
AKX afxBool             AfxGetBodySkeleton(afxBody bod, afxSkeleton* skeleton);
    
AKX void                AkxResetBodyClock(afxBody bod);
AKX void                AfxUpdateBodyMatrix(afxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

AKX void                AfxRecenterLinkedMotorClocks(afxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AKX void                AfxReclockMotives(afxBody bod, afxReal newClock);

AKX void                AfxPurgeTerminatedMotives(afxBody bod);

AKX void                AfxGetBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

AKX akxMotor            AfxRunManipulatedPose(afxBody bod, afxReal currClock, afxReal duration, akxPose pose, akxTrackMask *ModelMask);
AKX akxMotor            AfxRunAnimationBinding(afxBody bod, afxReal startTime, akxAnimation ani, struct animation_binding *binding);
AKX akxMotor            AfxRunAnimation(afxBody bod, afxReal startTime, akxAnimation ani);

AKX void                AfxSampleBodyAnimationsLODSparse(afxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void                AfxSampleBodyAnimationsAcceleratedLOD(afxBody bod, afxNat pivotCnt, afxM4d const offset, akxPose scratch, akxPoseBuffer rslt, afxReal allowedErr);
AKX afxBool             AfxSampleSingleBodyAnimationLODSparse(afxBody bod, akxMotor moto, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void                AfxAccumulateBodyAnimationsLODSparse(afxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);

AKX void    AkxCmdBindPose(avxCmdb cmdb, akxPose pose);
AKX void    AkxCmdBindPoseBuffer(avxCmdb cmdb, akxPoseBuffer wp);
AKX void    AkxCmdBindSparsePivotMap(avxCmdb cmdb, afxNat const* sparseBoneArray);
AKX void    AkxCmdSetAllowedSamplingError(avxCmdb cmdb, afxReal allowedErr);
AKX void    AkxCmdSampleBodyMotions(avxCmdb cmdb, afxBody bod, afxNat basePivot, afxNat pivotCnt);

////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAcquireBodies(afxModel mdl, afxNat cnt, afxBody bod[]);

#endif//AFX_BODY_H
