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

#define _AFX_SIM_C
#define _AFX_MESH_C
#define _AFX_VERTEX_DATA_C
#define _AFX_VERTEX_BUILDER_C
#define _AFX_MESH_TOPOLOGY_C
#include "qwadro/sim/modeling/awxMeshBuilder.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/sim/modeling/awxMeshTopology.h"
#include "qwadro/sim/modeling/awxVertexData.h"

#define MaxNumWeights 8

struct WeightVertex
{
    afxNat8 BoneWeights[MaxNumWeights];
    afxNat8 BoneIndices[MaxNumWeights];
    afxNat BonesUsed;
};

struct TriWeightData
{
    afxNat jntCnt;
    afxNat8 jntIdx[MaxNumWeights * 3];
    afxIndexedTriangle vtxIdx;
};

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX void _AfxMesh113131(awxMesh msh)
{
    afxNat* OriginalIndices = msh->topology->vtxIdx;
    
    afxNat const NumMeshTris = AwxCountMeshTriangles(msh->topology);
    struct TriWeightData* TriWeights = AfxAllocate(NIL, sizeof(TriWeights[0]), NumMeshTris, 0, AfxHint());
    
    for (afxNat triIdx = 0; triIdx < NumMeshTris; ++triIdx)
    {
        struct TriWeightData* TriData = &TriWeights[triIdx];
        TriData->jntCnt = 0;
        
        for (afxNat vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
        {
            TriData->vtxIdx[vtxIdx] = OriginalIndices[triIdx * 3 + vtxIdx];
            
            awxVertexBias* bias = &msh->vtd->biases[OriginalIndices[triIdx * 3 + vtxIdx]];
            
            for (afxNat vtxWgtIdx = 0; vtxWgtIdx < bias->weightCnt; ++vtxWgtIdx)
            {
                afxVertexWeight* w = &msh->vtd->weights[bias->baseWeightIdx + vtxWgtIdx];
                
                if (AfxFind(TriData->jntIdx, TriData->jntIdx + TriData->jntCnt, w->pivotIdx) == TriData->jntIdx + TriData->jntCnt)
                {
                    TriData->jntIdx[TriData->jntCnt++] = w->pivotIdx;
                }
            }
        }
    }
}
#endif


#if 0
awxMesh AfxAddDomeToModel(awxModel mdl, afxReal radius, afxNat32 slices)
{
    afxNat32 numberParallels = slices / 4;
    afxNat32 numberVertices = (numberParallels + 1) * (slices + 1);
    afxNat32 numberIndices = numberParallels * slices * 6;
    afxReal angleStep = (2.0f * AFX_PI) / ((afxReal)slices);

    afxNat32 indexIndices;

    // used later to help us calculating tangents vectors
    afxReal helpVector[3] = { 1.0f, 0.0f, 0.0f };
    afxReal helpQuaternion[4];
    afxReal helpMatrix[16];

    slices = slices < 3 ? 3 : slices;

    awxEntity ent = AfxGetAspectOwner(&mdl->asp);

    awxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "parallelepiped");
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, xy_vertices, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, xy_tangents, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, xy_normals, 0);
    //AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, xy_texCoords, 0);
    
    afxReal* vertices = msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool;
    afxReal* normals = msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].pool;
    afxReal* tangents = msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].pool;
    afxReal* texCoords = msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].pool;

    msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_NRM].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_TAN].cap;
    msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].pop = msh->vbuf.attrs[AFX_VERTEX_ATTR_UV].cap;

    for (afxNat32 i = 0; i < numberParallels + 1; i++)
    {
        for (afxNat32 j = 0; j < slices + 1; j++)
        {
            afxNat32 vertexIndex = (i * (slices + 1) + j) * 4;
            afxNat32 normalIndex = (i * (slices + 1) + j) * 3;
            afxNat32 tangentIndex = (i * (slices + 1) + j) * 3;
            afxNat32 texCoordsIndex = (i * (slices + 1) + j) * 2;

            vertices[vertexIndex + 0] = radius * sinf(angleStep * (afxReal)i) * sinf(angleStep * (afxReal)j);
            vertices[vertexIndex + 1] = radius * cosf(angleStep * (afxReal)i);
            vertices[vertexIndex + 2] = radius * sinf(angleStep * (afxReal)i) * cosf(angleStep * (afxReal)j);
            vertices[vertexIndex + 3] = 1.0f;

            normals[normalIndex + 0] = vertices[vertexIndex + 0] / radius;
            normals[normalIndex + 1] = vertices[vertexIndex + 1] / radius;
            normals[normalIndex + 2] = vertices[vertexIndex + 2] / radius;

            texCoords[texCoordsIndex + 0] = (afxReal)j / (afxReal)slices;
            texCoords[texCoordsIndex + 1] = 1.0f - (afxReal)i / (afxReal)numberParallels;

            // use quaternion to get the tangent vector
            AfxRotationQuatFromAxial(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex + 0]);
            AfxRotateM4d_AxialFromQuat((afxV4d*)helpMatrix, helpQuaternion);

            AfxTransformPivotV3d(&tangents[tangentIndex], helpVector, (afxV4d*)helpMatrix);
        }
    }

    awxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    //AfxFillMeshSection(msh, msecIdx, numberIndices);
    afxNat32* indices = msec->indices.pool;
    msec->indices.pop = msec->indices.cap;

    indexIndices = 0;
    for (afxNat32 i = 0; i < numberParallels; i++)
    {
        for (afxNat32 j = 0; j < slices; j++)
        {
            indices[indexIndices++] = i * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + (j + 1);

            indices[indexIndices++] = i * (slices + 1) + j;
            indices[indexIndices++] = (i + 1) * (slices + 1) + (j + 1);
            indices[indexIndices++] = i * (slices + 1) + (j + 1);
        }
    }

    return msh;
}

awxMesh AfxAddParallelepipedToModel(awxModel mdl, afxReal width, afxReal height)
{
    static afxNat32 numberVertices = 4, numberIndices = 6;
    static afxReal const xy_vertices[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };
    static afxReal const xy_normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
    static afxReal const xy_tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    static afxReal const xy_texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
    static afxNat32 const xy_indices[] = { 0, 1, 2, 1, 3, 2 };

    awxEntity ent = AfxGetAspectOwner(&mdl->asp);

    awxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "parallelepiped");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, xy_vertices, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, xy_tangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, xy_normals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, xy_texCoords, 0);
    awxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "parallelepiped"));
    AfxFillMeshSection(msec, numberIndices, xy_indices);

    afxM4d m4d;
    AfxResetM4d(m4d);
    AfxScalingM4d(m4d, AfxSpawnV4d(width, 10.0, height, 1.0));

    for (afxNat32 i = 0; i < numberVertices; i++)
    {
        AfxPreMultiplyV4d(((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], ((afxV4d*)msh->vbuf.attrs[AFX_VERTEX_ATTR_XYZW].pool)[i], m4d);
    }
    return msh;
}

awxMesh AfxAddCubeToModel(awxModel mdl, afxReal scale)
{
    static afxNat32 numberVertices = 24, numberIndices = 36;

    static afxReal const cubeVertices[]=
    {
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f,
        -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f,
        -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f,
        -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f,
        +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f
    };

    static afxReal const cubeNormals[] =
    {
        0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f
    };

    static afxReal const cubeTangents[] =
    {
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f
    };

    static afxReal const cubeTexCoords[] =
    {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    static afxNat32 const cubeIndices[] = { 0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20, 23, 22, 20, 22, 21 };

    awxEntity ent = AfxGetAspectOwner(&mdl->asp);

    awxMesh msh = AfxAcquireMesh(mdl, numberVertices, 1, "cube");
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, numberVertices, 0, cubeVertices, 0);
    AfxRescaleVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_XYZW, scale);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_TAN, numberVertices, 0, cubeTangents, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_NRM, numberVertices, 0, cubeNormals, 0);
    AfxPopulateVertexBuffer(&msh->vbuf, AFX_VERTEX_ATTR_UV, numberVertices, 0, cubeTexCoords, 0);
    awxMeshSurface msec = AfxAcquireMeshSection(msh, afxPrimTopology_TRI_LIST, numberIndices, 0, AfxAcquireMaterial(mdl, "cube"));
    AfxFillMeshSection(msec, numberIndices, cubeIndices);
    return msh;
}
#endif//0

_AFXINL void AfxEndMeshBuilding(awxMeshBuilder* mshb)
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);

    AfxDeallocateArray(&mshb->biases);

    if (mshb->faces)
        AfxDeallocate(NIL, mshb->faces);

    if (mshb->vtx)
        AfxDeallocate(NIL, mshb->vtx);

    if (mshb->posn)
        AfxDeallocate(NIL, mshb->posn);

    if (mshb->nrm)
        AfxDeallocate(NIL, mshb->nrm);

    if (mshb->uv)
        AfxDeallocate(NIL, mshb->uv);

    if (mshb->pivots)
        AfxDeallocate(NIL, mshb->pivots);

    AfxAssignFcc(mshb, NIL);
}

_AFXINL afxError AfxBeginMeshBuilding(awxMeshBuilder* mshb, afxString const* id, afxNat vtxCnt, afxNat triCnt, afxNat surfCnt, afxNat artCnt)
{
    afxError err = NIL;
    AfxAssert(mshb);
    AfxAssert(vtxCnt);
    AfxAssert(triCnt);
    AfxAssert(surfCnt);
    AfxAssert(artCnt);
    mshb->vtxCnt = vtxCnt;
    mshb->artCnt = artCnt;
    mshb->triCnt = triCnt;
    mshb->surfCnt = surfCnt;
    mshb->faces = NIL;

    if (surfCnt && !(mshb->faces = AfxAllocate(NIL, sizeof(mshb->faces[0]), triCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        AfxZero(triCnt, sizeof(mshb->faces[0]), mshb->faces);        
    }

    AfxReplicateString(&mshb->id, id);

    AfxAllocateArray(&mshb->biases, artCnt, sizeof(awxVertexBias), (awxVertexBias[]) { 0 });

    mshb->vtx = AfxAllocate(NIL, sizeof(mshb->vtx[0]), vtxCnt, NIL, AfxHint());
    AfxZero(vtxCnt, sizeof(mshb->vtx[0]), mshb->vtx);

    mshb->pivots = AfxAllocate(NIL, sizeof(mshb->pivots[0]), artCnt, NIL, AfxHint());

    mshb->posn = AfxAllocate(NIL, sizeof(mshb->posn[0]), vtxCnt, AFX_SIMD_ALIGN, AfxHint());
    mshb->nrm = NIL;
    mshb->uv = NIL;

    AfxAssignFcc(mshb, afxFcc_MSHB);

    return err;
}

_AFXINL void AfxUpdateVertices(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxNat const baseBiasIdx[], afxNat const biasCnt[])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);

    for (afxNat i = 0; i < vtxCnt; i++)
    {
        afxVertex *v = &mshb->vtx[baseVtxIdx + i];
        //AfxAtv4FromAtv3(v->posn, posn[i]);
        v->baseBiasIdx = baseBiasIdx ? baseBiasIdx[i] : 0;
        v->biasCnt = biasCnt ? biasCnt[i] : 1;
        //AfxCopyV2d(v->uv, uv ? uv[i] : AfxSpawnV2d(0, 0));

        AfxAssertRange(mshb->biases.cnt, v->baseBiasIdx, v->biasCnt);
    }
}

_AFXINL void AfxUpdateVertexPositions(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][3])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(posn);

    for (afxNat i = 0; i < vtxCnt; i++)
        AfxAtv4FromAtv3(mshb->posn[baseVtxIdx + i], posn[i]);
}

_AFXINL void AfxUpdateVertexPositions4(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const posn[][4])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(posn);
    AfxCopyArrayedV4d(vtxCnt, posn, &mshb->posn[baseVtxIdx]);
}

_AFXINL afxError AfxUpdateVertexNormals(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const nrm[][3])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(nrm);

    if (!mshb->nrm && !(mshb->nrm = AfxAllocate(NIL, sizeof(mshb->nrm[0]), mshb->vtxCnt, AFX_SIMD_ALIGN, AfxHint())))
        AfxThrowError();

    if (!err)
        AfxCopyArrayedV3d(vtxCnt, nrm, &mshb->nrm[baseVtxIdx]);

    return err;
}

_AFXINL afxError AfxUpdateVertexWraps(awxMeshBuilder* mshb, afxNat baseVtxIdx, afxNat vtxCnt, afxReal const uv[][2])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->vtxCnt, baseVtxIdx, vtxCnt);
    AfxAssert(uv);

    if (!mshb->uv && !(mshb->uv = AfxAllocate(NIL, sizeof(mshb->uv[0]), mshb->vtxCnt, AFX_SIMD_ALIGN, AfxHint())))
        AfxThrowError();

    if (!err)
        AfxCopyArrayedV2d(vtxCnt, uv, &mshb->uv[baseVtxIdx]);
    
    return err;
}

_AFXINL void AfxEmitTriangles(awxMeshBuilder* mshb, afxNat surfIdx, afxNat baseTriIdx, afxNat triCnt, afxNat const triVtxIdx[][3])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->surfCnt, surfIdx, 1);
    AfxAssertRange(mshb->triCnt, baseTriIdx, triCnt);

    for (afxNat i = 0; i < triCnt; i++)
    {
        mshb->faces[baseTriIdx + i].vtxIdx[0] = triVtxIdx[i][0];
        mshb->faces[baseTriIdx + i].vtxIdx[1] = triVtxIdx[i][1];
        mshb->faces[baseTriIdx + i].vtxIdx[2] = triVtxIdx[i][2];
        mshb->faces[baseTriIdx + i].surfIdx = surfIdx;
        mshb->faces[baseTriIdx + i].artCnt = 0;

        AfxAssert(surfIdx == mshb->faces[baseTriIdx + i].surfIdx);
        AfxAssertRange(mshb->surfCnt, mshb->faces[baseTriIdx + i].surfIdx, 1);
    }
}

_AFXINL afxNat AfxAddVertexBiases(awxMeshBuilder* mshb, afxNat cnt, afxNat const jntIdx[], afxReal const weight[])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxReserveArraySpace(&mshb->biases, mshb->biases.cnt + cnt);

    afxNat baseBiasIdx = AFX_INVALID_INDEX;
    AfxInsertArrayUnits(&mshb->biases, cnt, &baseBiasIdx, NIL);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(weight && 1.f >= weight[i]);
        AfxUpdateArrayRange(&mshb->biases, baseBiasIdx + i, 1, (const awxVertexBias[]) { { .pivotIdx = jntIdx ? jntIdx[i] : 0, .weight = weight ? weight[i] : 1.f } });
        AfxAssertRange(mshb->artCnt, ((awxVertexBias const*)AfxGetArrayUnit(&mshb->biases, baseBiasIdx + i))->pivotIdx, 1);
    }
    return baseBiasIdx;
}

_AFXINL void AfxRenameVertexPivots(awxMeshBuilder* mshb, afxNat basePivotIdx, afxNat cnt, afxString const name[])
{
    afxError err = NIL;
    AfxAssertType(mshb, afxFcc_MSHB);
    AfxAssertRange(mshb->artCnt, basePivotIdx, cnt);
    AfxAssert(name);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxReplicateString(&mshb->pivots[basePivotIdx + i], &name[i]);
    }
}

_AFX awxMesh AfxBuildCubeMesh(afxSimulation sim, afxReal scale)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(scale);

    static afxV4d const cubeVertices[] =
    {
        {-1.0f,-1.0f,-1.0f, 1.0f }, {-1.0f,-1.0f, 1.0f, 1.0f }, { 1.0f,-1.0f, 1.0f, 1.0f }, { 1.0f,-1.0f,-1.0f, 1.0f },
        {-1.0f, 1.0f,-1.0f, 1.0f }, {-1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f,-1.0f, 1.0f },
        {-1.0f,-1.0f,-1.0f, 1.0f }, {-1.0f, 1.0f,-1.0f, 1.0f }, { 1.0f, 1.0f,-1.0f, 1.0f }, { 1.0f,-1.0f,-1.0f, 1.0f },
        {-1.0f,-1.0f, 1.0f, 1.0f }, {-1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f,-1.0f, 1.0f, 1.0f },
        {-1.0f,-1.0f,-1.0f, 1.0f }, {-1.0f,-1.0f, 1.0f, 1.0f }, {-1.0f, 1.0f, 1.0f, 1.0f }, {-1.0f, 1.0f,-1.0f, 1.0f },
        { 1.0f,-1.0f,-1.0f, 1.0f }, { 1.0f,-1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f,-1.0f, 1.0f }
    };

    static afxV3d const cubeNormals[] =
    {
        { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f,-1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }
    };

    static afxV3d const cubeTangents[] =
    {
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f }, {-1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }
    };

    static afxV2d const cubeTexCoords[] =
    {
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
        { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
        { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }
    };

    static afxIndexedTriangle const tris[] = { { 0, 2, 1 }, { 0, 3, 2 }, { 4, 5, 6 }, { 4, 6, 7 }, { 8, 9, 10 }, { 8, 10, 11 }, { 12, 15, 14 }, { 12, 14, 13 }, { 16, 17, 18 }, { 16, 18, 19 }, { 20, 23, 22 }, { 20, 22, 21 } };

    awxMeshBuilder mshb;
    AfxBeginMeshBuilding(&mshb, &AfxStaticString("cube"), AFX_COUNTOF(cubeVertices), AFX_COUNTOF(tris), 1, 1);
    AfxRenameVertexPivots(&mshb, 0, 1, &AfxStaticString("cube"));
    AfxAddVertexBiases(&mshb, 1, (afxNat[]) { 0 }, (afxReal[]) { 1.0 });
    AfxUpdateVertices(&mshb, 0, AFX_COUNTOF(cubeVertices), NIL, NIL);
    AfxUpdateVertexPositions4(&mshb, 0, AFX_COUNTOF(cubeVertices), cubeVertices);
    
    for (afxNat i = 0; i < mshb.vtxCnt; i++)
        AfxScaleV3d(mshb.posn[i], mshb.posn[i], scale);

    AfxUpdateVertexNormals(&mshb, 0, AFX_COUNTOF(cubeNormals), cubeNormals);
    AfxUpdateVertexWraps(&mshb, 0, AFX_COUNTOF(cubeTexCoords), cubeTexCoords);
    AfxEmitTriangles(&mshb, 0, 0, AFX_COUNTOF(tris), tris);
    
    awxMesh msh;
    AwxBuildMeshes(sim, 1, &mshb, &msh);
    AfxEndMeshBuilding(&mshb);

    return msh;
}

_AFX awxMesh AfxBuildParallelepipedMesh(afxSimulation sim, afxReal width, afxReal height)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(width);
    AfxAssert(height);

    static afxV4d const vertices[] = { { -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, -1.0f, 0.0f, 1.0f }, { -1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } };
    static afxV3d const normals[] = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } };
    static afxV3d const tangents[] = { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
    static afxV2d const texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
    static afxIndexedTriangle const tris[] = { { 0, 1, 2 }, { 1, 3, 2 } };

    awxMeshBuilder mshb;
    AfxBeginMeshBuilding(&mshb, &AfxStaticString("parallelepiped"), AFX_COUNTOF(vertices), AFX_COUNTOF(tris), 1, 1);
    AfxRenameVertexPivots(&mshb, 0, 1, &AfxStaticString("parallelepiped"));
    AfxAddVertexBiases(&mshb, 1, (afxNat[]) { 0 }, (afxReal[]) { 1.0 });
    AfxUpdateVertices(&mshb, 0, AFX_COUNTOF(vertices), NIL, NIL);
    AfxUpdateVertexPositions4(&mshb, 0, AFX_COUNTOF(vertices), vertices);

    afxM4d m;
    AfxResetM4d(m);
    AfxScalingM4d(m, AfxSpawnV4d(width, height, 1.0, 1.0));

    AfxPostMultiplyArrayedAtv4(m, AFX_COUNTOF(vertices), mshb.posn, mshb.posn);

    AfxUpdateVertexNormals(&mshb, 0, AFX_COUNTOF(normals), normals);
    AfxUpdateVertexWraps(&mshb, 0, AFX_COUNTOF(texCoords), texCoords);
    AfxEmitTriangles(&mshb, 0, 0, AFX_COUNTOF(tris), tris);
    
    awxMesh msh;
    AwxBuildMeshes(sim, 1, &mshb, &msh);
    AfxEndMeshBuilding(&mshb);

    return msh;
}

_AFX awxMesh AfxBuildDomeMesh(afxSimulation sim, afxReal radius, afxNat slices)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(radius);
    AfxAssert(slices);
    AfxAssert(slices >= 3);

    afxNat numberParallels = slices / 4;
    afxNat numberVertices = (numberParallels + 1) * (slices + 1);
    afxNat numberIndices = numberParallels * slices * 6;
    afxReal angleStep = (2.f * AFX_PI) / ((afxReal)slices);

    // used later to help us calculating tangents vectors
    afxQuat helpQuaternion;
    afxM4d helpMatrix;

    slices = slices < 3 ? 3 : slices;

    awxMeshBuilder mshb;
    AfxBeginMeshBuilding(&mshb, &AfxStaticString("dome"), numberVertices, numberIndices / 3, 1, 1);
    AfxRenameVertexPivots(&mshb, 0, 1, &AfxStaticString("dome"));
    AfxAddVertexBiases(&mshb, 1, (afxNat[]) { 0 }, (afxReal[]) { 1.0 });
    AfxUpdateVertices(&mshb, 0, numberVertices, NIL, NIL);

    AfxUpdateVertexNormals(&mshb, 0, 1, &AFX_V3D_ZERO);

    AfxUpdateVertexWraps(&mshb, 0, 1, &AFX_V2D_ZERO);

    afxV4d* vertices = mshb.posn;
    afxV3d* normals = mshb.nrm;
    //afxV3d* tangents = mshb.tan;
    afxV2d* texCoords = mshb.uv;

    for (afxNat i = 0; i < numberParallels + 1; i++)
    {
        for (afxNat j = 0; j < slices + 1; j++)
        {
            afxNat vertexIndex = (i * (slices + 1) + j) * 4;
            afxNat normalIndex = (i * (slices + 1) + j) * 3;
            afxNat tangentIndex = (i * (slices + 1) + j) * 3;
            afxNat texCoordsIndex = (i * (slices + 1) + j) * 2;

            vertices[vertexIndex][0] = radius * AfxSin(angleStep * (afxReal)i) * AfxSin(angleStep * (afxReal)j);
            vertices[vertexIndex][1] = radius * AfxCos(angleStep * (afxReal)i);
            vertices[vertexIndex][2] = radius * AfxSin(angleStep * (afxReal)i) * AfxCos(angleStep * (afxReal)j);
            vertices[vertexIndex][3] = 1.0f;

            normals[normalIndex][0] = vertices[vertexIndex][0] / radius;
            normals[normalIndex][1] = vertices[vertexIndex][1] / radius;
            normals[normalIndex][2] = vertices[vertexIndex][2] / radius;

            texCoords[texCoordsIndex][0] = (afxReal)j / (afxReal)slices;
            texCoords[texCoordsIndex][1] = 1.0f - (afxReal)i / (afxReal)numberParallels;

            // use quaternion to get the tangent vector
            AfxQuatFromAxisAngle(helpQuaternion, AfxSpawnV3d(0, 1, 0), 360.0f * texCoords[texCoordsIndex][0]);
            AfxRotationM4dFromQuat(helpMatrix, helpQuaternion);
            //AfxPostMultiplyV3d(&tangents[tangentIndex], helpMatrix, AFX_V3D_X);
        }
    }

    afxNat baseTriIdx = 0;
    afxIndexedTriangle tri;

    for (afxNat32 i = 0; i < numberParallels; i++)
    {
        for (afxNat32 j = 0; j < slices; j++)
        {
            tri[0] = i * (slices + 1) + j;
            tri[1] = (i + 1) * (slices + 1) + j;
            tri[2] = (i + 1) * (slices + 1) + (j + 1);

            AfxEmitTriangles(&mshb, 0, baseTriIdx, 1, &tri);
            ++baseTriIdx;

            tri[0] = i * (slices + 1) + j;
            tri[1] = (i + 1) * (slices + 1) + (j + 1);
            tri[2] = i * (slices + 1) + (j + 1);

            AfxEmitTriangles(&mshb, 0, baseTriIdx, 1, &tri);
            ++baseTriIdx;
        }
    }

    awxMesh msh;
    AwxBuildMeshes(sim, 1, &mshb, &msh);
    //AfxEndMeshBuilding(&mshb);

    return msh;
}