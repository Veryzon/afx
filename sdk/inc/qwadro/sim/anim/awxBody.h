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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "../modeling/afxModel.h"

/// O objeto awxBody aloja o estado de um afxModel no tempo de execu��o.
/// O afxModel � apenas dados estruturados para um modelo particular --- isto �, este n�o h� qualquer conceito de onde este est�, qual estado este est� em, ou quais anima��es est�o interagindo em si.
/// O awxBody mant�m rastreio disto e prov�m um n�mero de fun��es �teis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades id�nticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam id�nticos, cada um deveria ainda haver seu pr�prio awxBody.
/// Dito isto porque os dados do awxBody s�o todos din�micos, e n�o s�o compartilhados atrav�s de m�ltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto � comum colocar a hierarquia de estruturas do afxModel no cache e instanci�-las m�ltiplas vezes, voc� nunca deveria colocar objetos awxBody no cache e reus�-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da exist�ncia das entidades que eles representam na sua aplica��o.

/// Note, no entanto, que estes s�o simplesmente ponteiros para os dados originais que voc� passou durante a instancia��o.
/// O awxBody n�o mant�m c�pias dos dados (para minimizar uso de mem�ria), ent�o voc� n�o pode liberar os dados originais do modelo e esperar obter ponteiros v�lidos de volta aqui.
/// Para aquilo que importa, se voc� liberar os dados originais do modelo, nenhuma das chamadas para awxBody funcionar�o, desde que elas todas dependem daqueles dados, como voc� esperaria.

AFX_OBJECT(awxBody)
{
    afxModel            mdl; // afxModel
    afxSkeleton         cachedSkl;
    void*               cachedBones;
    afxNat              cachedBoneCnt;
    afxChain            motors;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxReal             mass; // totally of masses of parts. Additive to initial "fake" value. May be used to create physical behavoir on empty bodies.
    afxV4d              centre;
    afxV4d              velocity;
    afxV4d              force;
    afxV4d              torque;

    afxReal             linearDamping;
    afxReal             angularDamping;
    awxPose             pose;
};

AFX afxModel            AwxGetBodyModel(awxBody bod);
AFX afxSkeleton         AwxGetBodySkeleton(awxBody bod);
    
AFX void                AwxResetBodyClock(awxBody bod);
AFX void                AwxUpdateBodyModelMatrix(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal const mm[4][4], afxReal m[4][4]);

AFX void                AwxRecenterBodyAllClocks(awxBody bod, afxReal currClock);

AFX void                AwxSetBodyClock(awxBody bod, afxReal newClock);
AFX void                AwxFreeCompletedBodyControls(awxBody bod);
AFX void **             AwxGetBodyUserDataArray(awxBody bod);

AFX void                AwxGetBodyRootMotionVectors(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal translation[3], afxReal rotation[3]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AwxEmbodyModel(afxModel mdl, afxNat cnt, awxBody bod[]);

#endif//AFX_BODY_H
