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

#include "afx/sim/afxSimDefs.h"
#include "afx/core/mem/afxArray.h"
#include "../../math/volume/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afxMaterial.h"
#include "afx/sim/modeling/afxSkeleton.h"
#include "afx/core/io/afxUrd.h"
#include "afx/sim/modeling/afxVertex.h"

/// O objeto afxMesh � a estrutura prim�ria para dado geom�trico no Qwadro.
/// Este referencia dados de v�rtice, dados de tri�ngulo, afxMaterial's, afxMeshMorph'es e afxMeshArticulation's.
/// Assets padr�es do Qwadro cont�m um arranjo de afxMesh na estrutura de informa��o do arquivo, e afxMesh'es s�o tamb�m referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh � separada em afxMeshSection's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSection � a estrutura prim�ria que voc� interage para obter estas por��es.
/// A estrutura afxMeshSection referencia o arranjo de �ndices para a afxMesh, a qual em sua vez referencia o arranjo de v�rtice.
/// Se a afxMesh � deform�vel, as v�rtices no arranjo de v�rtice referenciam as articula��es no arranjo de afxMeshArticulation's.
/// Os dados de v�rtice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, cont�m todos os v�rtices �nicos na afxMesh.

/// As v�rtices no Qwadro s�o r�gidos ou deform�veis.
/// afxMesh'es deform�veis s�o aquelas que est�o ligadas a m�ltiplas articula��es, onde afxMesh'es r�gidas s�o aquelas que est�o ligadas a uma singela articula��o (e portanto movem-se "rigidamente" com aquela articula��o).

/// Para determinar quais articula��es uma afxMesh est� ligada a (uma para afxMesh r�gida, muitas para afxMesh deform�vel), voc� pode acessar o arranjo de afxMeshArticulation's. 
/// Este arranjo cont�m nomes das articula��es as quais a afxMesh est� ligada, bem como par�metros de "oriented bounding box" para as partes da afxMesh que est�o ligadas �quela articula��o e outra informa��o pertinente a liga��o malha-a-articula��o.
/// Note que na maioria dos casos voc� n�o necessitar� de usar os nomes das articula��es no afxMeshArticulation diretamente, porque voc� pode usar um objeto afxMeshRigging para fazer este trabalho (e outro trabalho necess�rio de liga��o) para voc�.

/// Os dados de �ndice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, cont�m todos os �ndices para os tri�ngulos na afxMesh.
/// Estes �ndices sempre descrevem uma lista de tri�ngulo - isso �, cada grupo de tr�s �ndices descrevem um singelo tri�ngulo - os dados n�o s�o organizados em "strips" ou "fans".

/// Uma vez que voc� tem v�rtices e �ndices, voc� basicamente tem todos os dados da afxMesh que voc� necessita para quais por��es de �ndice v�o com quais afxMaterial's.
/// A estrutura afxMeshSection prov�m esta informa��o.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxFcc                  fcc;
    afxString               scalarName; // 16
    afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx;
    afxNat                  vtxCnt;
    afxBool                 dataIsDeltas;
};

AFX_DEFINE_STRUCT(afxMeshSection) // aka tri material group
{
    afxFcc                  fcc;
    afxNat                  mtlIdx;
    afxNat                  firstTriIdx;
    afxNat                  triCnt;
    afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
    afxString               name; // 32 // SIGMA added this field to handle Wavefront OBJect group names.
};

AFX_DEFINE_STRUCT(afxMeshTopology)
{
    // Mesh triangle topology is described by the afxMeshTopology structure, which is pointed to by the topology field. The afxMeshTopology structure provides a number of useful arrays of data, including the mesh triangulation, edge connectivity, and vertex relationships.

    afxFcc                  fcc;
    afxNat                  secCnt;
    afxMeshSection          *sections; // The 'sections' array specifies the actual triangulation of the mesh. Each afxMeshSection structure indexes into either the Indices array, for the indices that make up the triangles. The MaterialIndex field indexes into the MaterialBindings array, and specifies the material used for the group of triangles.
    afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
    afxIndexBuffer          ibuf; // afxIndexBuffer --- aka triangles
    afxNat                  idxRgn; // if ibuf is shared among other meshes, it should be a base for sections.    
    afxPrimTopology         primType; // added by SIGMA


    afxInt                  vtxToVtxCnt;
    afxInt                  *vtxToVtxMap; // The vtxToVtxMap array has, for each vertex in the corresponding vertex data, an index of the next vertex which was originally the same vertex. This is used to track vertices that are split during exporting, for example because of material boundaries. It is a circular list, so that each vertex points to the next coincident vertex, and then the final vertex points back to the first. If a vertex has no coincident vertices, then it simply points to itself.
    afxInt                  vtxToTriCnt;
    afxInt                  *vtxToTriMap; // The vtxToTriMap specifies, for each vertex, what triangle caused it to be created. So, for example, for the original vertices, it is simply the first triangle to reference it in the index list. But, for a vertex which was generated because of a material boundary or something similar, then it is the index of the first triangle who forced the vertex to be generated.
    afxInt                  sideToNeighborCnt;
    afxInt                  *sideToNeighborMap; // The sideToNeighborMap specifies, for each edge of a triangle, what the corresponding triangle and edge is across that edge. This array lines up with the Indices arrays such that, for each index, the "edge" corresponding to that index is the edge between that index and the next index in its triangle. The value stored in the sideToNeighborMap array is actually bit-packed. The two lowest-order bits of the value are the corresponding edge index in the corresponding triangle (ie., 00 would be the 0th edge, 01 the 1st, 10 the 2nd, and 11 is undefined). The rest of the bits (the high-order 30 bits) specify the actual triangle index - you would multiply it by three to get its location in the Indices arrays.
    afxInt                  bonesForTriCnt;
    afxInt                  *bonesForTri;
    afxInt                  triToBoneCnt;
    afxInt                  *triToBoneIndices;
};

AFX_DEFINE_STRUCT(afxMeshArticulation)
{
    afxFcc                  fcc;
    afxString               boneName; // 16
    afxAabb                 aabb; // originally oobb;
    afxNat                  triCnt;
    afxNat                  *triIdx;
};

AFX_DEFINE_STRUCT(afxMeshRigging)
{
    afxMesh                 msh;
    afxNat                  boneCnt;
    afxSkeleton             srcSkl;
    afxNat*                 srcBoneIndices;
    afxSkeleton             dstSkl;
    afxNat*                 dstBoneIndices;
};

AFX_DEFINE_STRUCT(afxMaterialSlot)
{
    afxMaterial             mtl;
};

AFX_DEFINE_STRUCT(afxFileBackedMesh)
{
    afxUri                  name;
    afxUrd                  *urd;
    afxUrdReference         primaryVtxData;
    afxUrdReference         *morphVtxData;
    struct
    {
        afxUrdReference     indexData;
        afxUrdReference     vtxToVtxMap;
        afxUrdReference     vtxToTriMap;
        afxUrdReference     sideToNeighborMap;
        afxUrdReference     bonesForTri;
        afxUrdReference     triToBoneIndices;
    }                       primaryTopology;
    afxUrdReference         *artTriIdx;
};

AFX_DEFINE_STRUCT(afxMeshVertexData)
{
    afxVertexLayout         layout;
    afxNat                  vtxCnt;
    void                    **vtxComps;
    afxString const         **vtxCompNames;
};

#ifdef _AFX_MESH_C
AFX_OBJECT(afxMesh)
{
    afxUri                  uri; // 128

    afxVertexBuffer         vbuf; // afxVertexBuffer --- aka vertex data
    afxNat                  baseVtx; // when vbuf is shared with other meshes it's not 0. Maybe that index buffer already does this?
    afxNat                  vtxCnt;
    afxNat                  morphCnt;
    afxMeshMorph            *morphs;
    afxMeshTopology         *topology;
    afxNat                  mtlSlotCnt;
    afxMaterialSlot         *mtlSlots;
    afxNat                  articulationCnt;
    afxMeshArticulation     *articulations;
    void                    *extData;
};
#endif

AFX afxUri const*           AfxMeshGetUri(afxMesh msh);
AFX afxVertexBuffer         AfxMeshGetVertexBuffer(afxMesh msh);
    
AFX afxNat                  AfxMeshGetMorphCount(afxMesh msh);
AFX afxMeshMorph*           AfxMeshGetMorph(afxMesh msh, afxNat morphIdx);

AFX afxNat                  AfxMeshGetSectionCount(afxMesh msh);
AFX afxMeshSection*         AfxMeshGetSection(afxMesh msh, afxNat secIdx);

AFX afxIndexBuffer          AfxMeshGetIndexBuffer(afxMesh msh);

AFX afxNat                  AfxMeshGetMaterialCount(afxMesh msh);
AFX afxMaterial             AfxMeshGetBoundMaterial(afxMesh msh, afxNat mtlIdx);
AFX afxMaterial             AfxMeshFindMaterial(afxMesh msh, afxUri const *name);
AFX void                    AfxMeshBindMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl);

AFX afxBool                 AfxMeshIsRigid(afxMesh msh);
AFX afxNat                  AfxMeshGetArticulationCount(afxMesh msh);
AFX afxMeshArticulation*    AfxMeshGetArticulation(afxMesh msh, afxNat artIdx);

AFX void                    AfxMeshTransform(afxMesh msh, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags);

AFX afxMeshRigging*         AfxMeshGenerateRig(afxMesh msh, afxSkeleton src, afxSkeleton dst);

#endif//AFX_MESH_H