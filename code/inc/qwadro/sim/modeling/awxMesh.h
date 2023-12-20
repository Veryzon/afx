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

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/core/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxInstance.h"
#include "qwadro/sim/awxMaterial.h"
#include "qwadro/sim/awxSkeleton.h"
#include "qwadro/core/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/sim/modeling/awxMeshTopology.h"

/// O objeto awxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
/// Este referencia dados de v�rtice, dados de tri�ngulo, awxMaterial's, awxMeshMorph'es e awxMeshVertebra's.
/// Assets padr�es do Qwadro cont�m um arranjo de awxMesh na estrutura de informa��o do arquivo, e awxMesh'es s�o tamb�m referenciadas pelos objetos awxModel que as usam.
/// Cada awxMesh � separada em awxMeshSurface's delitmitadas por awxMaterial's.
/// A estrutura awxMeshSurface � a estrutura prim�ria que voc� interage para obter estas por��es.
/// A estrutura awxMeshSurface referencia o arranjo de �ndices para a awxMesh, a qual em sua vez referencia o arranjo de v�rtice.
/// Se a awxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de awxMeshVertebra's.
/// Os dados de v�rtice para uma awxMesh, dados por um afxVertexBuffer referenciado pelo awxMesh, cont�m todos os v�rtices �nicos na awxMesh.

/// As v�rtices no Qwadro s�o r�gidos ou deform�veis.
/// awxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde awxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

/// Para determinar quais articula��es uma awxMesh est� ligada a (uma para awxMesh r�gida, muitas para awxMesh deform�vel), voc� pode acessar o arranjo de awxMeshVertebra's. 
/// Este arranjo cont�m nomes das articula��es as quais a awxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da awxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
/// Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no awxMeshVertebra diretamente, porque voc� pode usar um objeto awxMeshRig para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

/// Os dados de �ndice para uma awxMesh, dado por um awxMeshTopology referenciado pelo awxMesh, cont�m todos os �ndices para os tri�ngulos na awxMesh.
/// Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

/// Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da awxMesh que voc� necessita para quais por��es de �ndice v�o com quais awxMaterial's.
/// A estrutura awxMeshSurface prov�m esta informa��o.

AFX_DEFINE_STRUCT(awxMeshMorph) // aka morph target, blend shape
{
    _AFX_DBG_FCC;
    afxString           id; // 16
    awxVertexData       vtd;
    afxNat              baseVtxIdx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(awxMeshVertebra)
{
    _AFX_DBG_FCC;
    afxFixedString32         id;
    afxAabb             aabb; // originally oobb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

#ifdef _AFX_MESH_C
AFX_OBJECT(awxMesh)
{
    afxBufferedString   id; // 128

    awxVertexData       vtd;

    afxNat              morphCnt;
    awxMeshMorph*       morphs;

    awxMeshTopology     topology;
    
    afxNat              vertebraCnt;
    awxMeshVertebra*    vertebras;
    void*               extData;
};
#endif

AFX afxString const*    AwxGetMeshId(awxMesh msh);
AFX awxVertexData       AwxGetMeshVertices(awxMesh msh);
AFX afxNat              AwxCountMeshVertices(awxMesh msh);

AFX afxNat              AwxCountMeshMorphes(awxMesh msh);
AFX awxMeshMorph*       AwxGetMeshMorph(awxMesh msh, afxNat morphIdx);

AFX void                AwxRelinkMeshTopology(awxMesh msh, awxMeshTopology msht);
AFX afxBool             AwxGetMeshTopology(awxMesh msh, awxMeshTopology* msht);

AFX afxBool             AwxMeshIsDeformable(awxMesh msh);
AFX afxNat              AwxCountMeshVertebras(awxMesh msh);
AFX awxMeshVertebra*    AwxGetMeshVertebra(awxMesh msh, afxNat artIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX awxMesh             AwxAssembleMesh(awxSimulation sim, awxVertexData vtd, afxNat baseVtxIdx, afxNat vtxCnt, awxMeshTopology msht, afxNat baseSurfIdx, afxNat surfCnt, afxString const pivots[]);

AFX afxError            AwxBuildMeshes(awxSimulation sim, afxNat cnt, awxMeshBuilder const mshb[], awxMesh msh[]);

AFX void                AwxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxMesh msh[]);

AFX void                AwxRenormalizeMeshes(afxNat cnt, awxMesh msh[]);

#endif//AFX_MESH_H
