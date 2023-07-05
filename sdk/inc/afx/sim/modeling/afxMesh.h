/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/mem/afxArray.h"
#include "../../math/volume/afxAabb.h"
#include "afx/core/afxObject.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afxMaterial.h"
#include "afx/sim/modeling/afxSkeleton.h"

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

AFX_DEFINE_HANDLE(afxMesh);

/// A serialization wrapper for the relationship between a Vertex buffer and a set of primitives.

AFX_DEFINE_STRUCT(afxMeshBlueprintArticulation)
{
    afxString*              boneName; // 32
    afxNat                  triCnt;
    void const              *tris;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintSection) // aka tri material group
{
    afxString*              name; // 32
    afxNat                  mtlIdx;
    afxNat                  firstTriIdx;
    afxNat                  triCnt;

    void const              *idxSrc;
    afxNat                  srcIdxSiz;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintVertexArrange)
{
    afxString*              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;

    void const              *src;
    afxVertexFormat         srcFmt;
};

/// Um blueprint foi idealizado para ser um meta-objeto quase-completo para gera��o de uma malha. Dependendo apenas das refer�ncias das fontes de v�rtices e �ndices, para evitar aloca��es desnecess�rias.

AFX_DEFINE_STRUCT(afxMeshBlueprint)
{
    afxFcc                  fcc;
    void*                   sim;
    afxUri128               uri;

    // vertex data
    afxNat                  vtxCnt; // must be greater than zero.
    afxArray                arranges; // afxMeshBlueprintVertexArrange

    // topology data
    afxNat                  idxCnt;
    void const              *idxSrc;
    afxNat                  srcIdxSiz;
    afxPrimTopology         prim;
    afxArray                sections; // afxMeshBlueprintSection

    // rigging data
    afxArray                articulations; // afxMeshBlueprintArticulation

    // material data
    afxArray                materials; // afxUri
};

#if 0
struct mesh_builder
{
    float NormalTolerance;
    float TangentTolerance;
    float BinormalTolerance;
    float TangentBinormalCrossTolerance;
    float TangentMergingMinCosine;
    float ChannelTolerance[256];
    const data_type_definition *VertexType;
    int VertexSize;
    int ChannelCount;
    int MaximumVertexCount;
    int VertexCount;
    stack_allocator VertexStack;
    stack_allocator NextCoincidentVertStack;
    stack_allocator FromTriangleStack;
    float Point[3];
    int BoneCount;
    float *BoneWeights;
    int WeightCount;
    vertex_weight_arrays WeightArrays;
    stack_allocator VertexIndexStack;
    stack_allocator NextInMaterialStack;
    int MaterialCount;
    int UsedMaterialCount;
    int *FirstMaterialTriangle;
    stack_allocator EdgeStack;
    int *EdgeHashHeads;
    int NextUnusedTriangle;
    int MaterialIndex;
    int VertexIndex[3];
    data_type_definition *BufferedVertexType;
    data_type_definition *ChannelTypes;
    int BufferedPrefixSize;
    int BufferedChannelSize;
    int BufferedVertexSize;
    buffered_vertex_prefix *BufferedVertex[3];
    buffered_vertex_prefix *ComparisonVertex;
    void *TruncatedVertex;
    stack_allocator TriangleTangentStack;
    stack_allocator VertexTangentStack;
    int VertexComponentNameCount;
    char **VertexComponentNames;
}
#endif

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxFcc                  fcc;
    afxString*              scalarName; // 16
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
    afxString*              name; // 32 // SIGMA added this field to handle Wavefront OBJect group names.
};

AFX_DEFINE_STRUCT(afxMeshTopology)
{
    afxFcc                  fcc;
    afxNat                  secCnt;
    afxMeshSection          *sections; // aka tri material group
    afxAabb                 aabb; // SIGMA added this field to ease occlusion culling.
    afxIndexBuffer          ibuf; // afxIndexBuffer --- aka triangles
    afxNat                  idxRgn; // if ibuf is shared among other meshes, it should be a base for sections.    
    afxPrimTopology         primType;

    // democratizado mas n�o sei o que fazer com isso k
    afxInt                  _vtx2vtxCnt;
    afxInt                  *_vtx2vtxMap;
    afxInt                  _vtx2triCnt;
    afxInt                  *_vtx2triMap;
    afxInt                  _side2neighborCnt;
    afxInt                  *_side2neighborMap;
    afxInt                  _bonesForTriCnt;
    afxInt                  *_bonesForTri;
    afxInt                  _tri2boneCnt;
    afxInt                  *_tri2boneIndices;
};

AFX_DEFINE_STRUCT(afxMeshArticulation)
{
    afxFcc                  fcc;
    afxString*              boneName; // 16
    afxAabb                 aabb; // originally oobb;
    afxNat                  triCnt;
    afxNat                  *triIndices;
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

AFX_OBJECT(afxMesh)
{
    afxObject               obj;
    afxUri*                 uri; // 128

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

AFX afxMeshRigging*             AfxMeshGenerateRig(afxMesh msh, afxSkeleton src, afxSkeleton dst);

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Inicializa uma blueprint, e reserva recursos.
AFX void                    AfxMeshBlueprintDeploy(afxMeshBlueprint *mshb, void *sim, afxUri const *name, afxNat vtxCnt, afxNat estVtxArrCnt, afxNat estSecCnt, afxNat estArtCnt, afxNat estMtlCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX void                    AfxMeshBlueprintDiscard(afxMeshBlueprint *mshb);

AFX void                    AfxMeshBlueprintErase(afxMeshBlueprint *mshb);

// Buscar por um nomeado material se j� inserido.
AFX afxBool                 AfxMeshBlueprintFindMaterial(afxMeshBlueprint *mshb, afxUri const *uri, afxNat *idx);

// Adiciona numa nova articula��o a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddArticulation(afxMeshBlueprint *mshb, afxString const *name, afxNat triCnt, void const *tris);

// Adiciona um novo material a malha.
AFX afxError                AfxMeshBlueprintAddMaterial(afxMeshBlueprint *mshb, afxMaterial mtl);

// Adiciona numa nova se��o a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddSection(afxMeshBlueprint *mshb, afxString const *name, afxNat mtlIdx, afxNat firstTriIdx, afxNat triCnt, void const *idxSrc, afxNat idxSrcSiz);

// Define dados de topologia da malha. Se um 'ibuf' n�o for oferecido, um novo ser� gerado com a capacidade de 'idxCnt'. Noutro caso, 'baseIdx' deve ser usado para delimitar o in�cio do espa�o utiliz�vel num 'ibuf' compartilhado.
AFX void                    AfxMeshBlueprintSetTopology(afxMeshBlueprint *mshb, afxPrimTopology prim, afxNat idxCnt, void const *idxSrc, afxNat idxSrcSiz);

// Adiciona um novo arranjo de v�rtice a malha. Ao menos uma, especificando posi��o, � necess�ria.
AFX afxError                AfxMeshBlueprintAddVertexArrange(afxMeshBlueprint *mshb, afxVertexRowSpecification const *spec);

#endif//AFX_MESH_H