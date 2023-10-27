/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "afxMeshTopology.h"
#include "afxMeshData.h"

/// O objeto afxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
/// Este referencia dados de v�rtice, dados de tri�ngulo, afxMaterial's, afxMeshMorph'es e afxMeshVertebra's.
/// Assets padr�es do Qwadro cont�m um arranjo de afxMesh na estrutura de informa��o do arquivo, e afxMesh'es s�o tamb�m referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh � separada em afxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSurface � a estrutura prim�ria que voc� interage para obter estas por��es.
/// A estrutura afxMeshSurface referencia o arranjo de �ndices para a afxMesh, a qual em sua vez referencia o arranjo de v�rtice.
/// Se a afxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de afxMeshVertebra's.
/// Os dados de v�rtice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, cont�m todos os v�rtices �nicos na afxMesh.

/// As v�rtices no Qwadro s�o r�gidos ou deform�veis.
/// afxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde afxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

/// Para determinar quais articula��es uma afxMesh est� ligada a (uma para afxMesh r�gida, muitas para afxMesh deform�vel), voc� pode acessar o arranjo de afxMeshVertebra's. 
/// Este arranjo cont�m nomes das articula��es as quais a afxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da afxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
/// Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no afxMeshVertebra diretamente, porque voc� pode usar um objeto afxMeshRig para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

/// Os dados de �ndice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, cont�m todos os �ndices para os tri�ngulos na afxMesh.
/// Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

/// Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da afxMesh que voc� necessita para quais por��es de �ndice v�o com quais afxMaterial's.
/// A estrutura afxMeshSurface prov�m esta informa��o.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    _AFX_DBG_FCC
    afxString               scalarName; // 16
    afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx;
    afxNat                  vtxCnt;
    afxBool                 dataIsDeltas;
};

AFX_DEFINE_STRUCT(afxMeshVertebra)
{
    _AFX_DBG_FCC
    afxString               name; // 16
    afxAabb                 aabb; // originally oobb;
    afxNat                  triCnt;
    afxNat*                 triIdx; // indices to vertices
};

AFX_DEFINE_STRUCT(afxMaterialSlot)
{
    afxMaterial             mtl;
};

AFX_OBJECT(afxMesh)
#ifdef _AFX_MESH_C
{
    afxUri                  uri; // 128

    afxMeshData             vtxData;
    //afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    //afxNat                  baseVtx; // when vbuf is shared with other meshes it's not 0. Maybe that index buffer already does this?
    //afxNat                  vtxCnt;
    afxNat                  morphCnt;
    afxMeshMorph*           morphs;
    afxMeshTopology         topology;
    afxNat                  mtlSlotCnt;
    afxMaterialSlot*        mtlSlots;
    afxNat                  vertebraCnt;
    afxMeshVertebra*        vertebras;
    void*                   extData;
}
#endif
;

AFX afxError            AfxBuildMeshes(afxSimulation sim, afxMeshBuilder const* mshb, afxNat cnt, void *data[], afxMesh msh[]);

AFX afxUri const*       AfxMeshGetUri(afxMesh msh);
AFX afxVertexBuffer     AfxGetMeshVertices(afxMesh msh);

AFX afxNat              AfxCountMeshMorphes(afxMesh msh);
AFX afxMeshMorph*       AfxGetMeshMorph(afxMesh msh, afxNat morphIdx);

AFX void                AfxSetMeshTopology(afxMesh msh, afxMeshTopology msht);
AFX afxBool             AfxGetMeshTopology(afxMesh msh, afxMeshTopology* msht);

AFX afxNat              AfxCountMeshMaterials(afxMesh msh);
AFX void                AfxSetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl);
AFX afxBool             AfxGetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial* mtl);
AFX afxBool             AfxFindMeshMaterial(afxMesh msh, afxUri const *name, afxMaterial* mtl);

AFX afxBool             AfxMeshIsRigid(afxMesh msh);
AFX afxNat              AfxCountMeshVertebras(afxMesh msh);
AFX afxMeshVertebra*    AfxGetMeshVertebra(afxMesh msh, afxNat artIdx);

AFX void                AfxTransformMesh(afxMesh msh, afxV3d const at, afxM3d const lt, afxM3d const ilt, afxReal atTol, afxReal ltTol, afxFlags flags);

#endif//AFX_MESH_H