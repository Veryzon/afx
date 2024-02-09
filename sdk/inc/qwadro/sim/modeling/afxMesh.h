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

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/sim/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/sim/modeling/afxMeshTopology.h"
#include "qwadro/core/afxFixedString.h"

/// O objeto afxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
/// Este referencia dados de v�rtice, dados de tri�ngulo, afxMaterial's, afxMeshMorph'es e afxMeshPivot's.
/// Assets padr�es do Qwadro cont�m um arranjo de afxMesh na estrutura de informa��o do arquivo, e afxMesh'es s�o tamb�m referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh � separada em afxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSurface � a estrutura prim�ria que voc� interage para obter estas por��es.
/// A estrutura afxMeshSurface referencia o arranjo de �ndices para a afxMesh, a qual em sua vez referencia o arranjo de v�rtice.
/// Se a afxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de afxMeshPivot's.
/// Os dados de v�rtice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, cont�m todos os v�rtices �nicos na afxMesh.

/// As v�rtices no Qwadro s�o r�gidos ou deform�veis.
/// afxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde afxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

/// Para determinar quais articula��es uma afxMesh est� ligada a (uma para afxMesh r�gida, muitas para afxMesh deform�vel), voc� pode acessar o arranjo de afxMeshPivot's. 
/// Este arranjo cont�m nomes das articula��es as quais a afxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da afxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
/// Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no afxMeshPivot diretamente, porque voc� pode usar um objeto afxMeshRig para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

/// Os dados de �ndice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, cont�m todos os �ndices para os tri�ngulos na afxMesh.
/// Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

/// Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da afxMesh que voc� necessita para quais por��es de �ndice v�o com quais afxMaterial's.
/// A estrutura afxMeshSurface prov�m esta informa��o.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    _AFX_DBG_FCC;
    afxString           id; // 16
    afxNat              idStrIdx;
    awxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(afxMeshPivot)
{
    _AFX_DBG_FCC;
    afxFixedString32    id;
    afxNat              idStrIdx;
    afxAabb             aabb; // originally oobb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

#ifdef _AFX_MESH_C
AFX_OBJECT(afxMesh)
{
    afxNat              idStrIdx;
    afxFixedString32    id;

    awxVertexData       vtd;

    // EXP ---
    struct
    {
        awxVertexData   vtd;
        afxNat          baseVtx;
        afxNat          vtxCnt;
    }*                  lods;
    afxNat              lodCnt;
    // --- EXP

    afxNat              morphCnt;
    afxMeshMorph*       morphs;

    afxMeshTopology     topology;
    
    afxNat              pivotCnt;
    afxMeshPivot*       pivots;
    void*               extData;

    afxStringCatalog    strc;
};
#endif

AFX_DEFINE_STRUCT(afxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxFixedString32    id;
    afxNat              vtxDataIdx;
    afxNat              topologyIdx;
    afxNat              mtlCnt;
    afxNat              pivotCnt;
    afxString const*    pivots;
};

AFX afxBool             AfxGetMeshId(afxMesh msh, afxString* id);

AFX awxVertexData       AfxGetMeshVertices(afxMesh msh);
AFX afxNat              AfxCountMeshVertices(afxMesh msh);

AFX afxNat              AfxCountMeshMorphes(afxMesh msh);
AFX afxMeshMorph*       AfxGetMeshMorph(afxMesh msh, afxNat morphIdx);

AFX void                AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht);
AFX afxMeshTopology     AfxGetMeshTopology(afxMesh msh);

AFX afxBool             AfxMeshIsDeformable(afxMesh msh);

AFX afxNat              AfxCountMeshVertebras(afxMesh msh);
AFX afxMeshPivot*       AfxGetMeshPivot(afxMesh msh, afxNat pvtIdx);
AFX afxBool             AfxGetMeshPivotId(afxMesh msh, afxNat pvtIdx, afxString* id);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAssembleMeshes(afxSimulation sim, afxStringCatalog strc, awxVertexData const datas[], afxMeshTopology const topologies[], afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

AFX afxError            AfxBuildMeshes(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxMeshBuilder const mshb[], afxMesh meshes[]);

AFX void                AfxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal linearTol, afxReal const atv[4], afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[]);

AFX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[]);

#endif//AFX_MESH_H
