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

/// O objeto afxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
/// Este referencia dados de v�rtice, dados de tri�ngulo, afxMaterial's, akxMeshMorph'es e akxMeshBias's.
/// Assets padr�es do Qwadro cont�m um arranjo de afxMesh na estrutura de informa��o do arquivo, e afxMesh'es s�o tamb�m referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh � separada em akxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura akxMeshSurface � a estrutura prim�ria que voc� interage para obter estas por��es.
/// A estrutura akxMeshSurface referencia o arranjo de �ndices para a afxMesh, a qual em sua vez referencia o arranjo de v�rtice.
/// Se a afxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de akxMeshBias's.
/// Os dados de v�rtice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, cont�m todos os v�rtices �nicos na afxMesh.

/// As v�rtices no Qwadro s�o r�gidos ou deform�veis.
/// afxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde afxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

/// Para determinar quais articula��es uma afxMesh est� ligada a (uma para afxMesh r�gida, muitas para afxMesh deform�vel), voc� pode acessar o arranjo de akxMeshBias's. 
/// Este arranjo cont�m nomes das articula��es as quais a afxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da afxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
/// Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no akxMeshBias diretamente, porque voc� pode usar um objeto akxMeshRig para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

/// Os dados de �ndice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, cont�m todos os �ndices para os tri�ngulos na afxMesh.
/// Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

/// Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da afxMesh que voc� necessita para quais por��es de �ndice v�o com quais afxMaterial's.
/// A estrutura akxMeshSurface prov�m esta informa��o.

#ifndef AKX_MESH_H
#define AKX_MESH_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxBox.h"
#include "qwadro/base/afxObject.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/cad/afxMeshTopology.h"
#include "qwadro/cad/akxVertexData.h"
#include "qwadro/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxMeshMorph) // aka morph target, blend shape
{
    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(akxMeshBias)
{
    afxBox              obb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

AFX_DEFINE_STRUCT(akxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxString32         id;
    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxNat              extraMorphCnt;
    akxMeshMorph const* extraMorphs;
    afxString const*    extraMorphTagMap;
    afxMeshTopology     topology;
    afxNat              mtlCnt;
    afxNat              biasCnt;
    afxString const*    biases;
};

AKX afxBool             AfxGetMeshUrn(afxMesh msh, afxString* id);

AKX afxNat              AfxCountMeshVertices(afxMesh msh);
AKX afxBool             AfxGetMeshVertices(afxMesh msh, afxNat morphIdx, akxVertexData* data, afxNat* baseVtx, afxNat* vtxCnt);

AKX afxNat              AfxCountMeshMorphs(afxMesh msh);
AKX akxMeshMorph*       AfxGetMeshMorphs(afxMesh msh, afxNat baseMorphIdx);

AKX afxError            AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht);
AKX afxMeshTopology     AfxGetMeshTopology(afxMesh msh);

AKX afxBool             AfxMeshIsDeformable(afxMesh msh);

AKX afxNat              AfxCountMeshBiases(afxMesh msh);
AKX akxMeshBias*        AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AKX afxString*          AfxGetMeshBiasTags(afxMesh msh, afxNat biasIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleMeshes(afxSimulation sim, afxNat cnt, akxMeshBlueprint const blueprints[], afxMesh meshes[]);

AKX afxError            AfxBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBuilder const mshb[], afxMesh meshes[]);

AKX void                AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[]);

AKX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[]);

#endif//AKX_MESH_H
