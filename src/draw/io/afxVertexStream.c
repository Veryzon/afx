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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_VERTEX_BUFFER_C
#define _AVX_INDEX_BUFFER_C
#define _AVX_DRAW_INPUT_C
#include "../../dev/AvxImplKit.h"

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    afxBuffer               vbo;
    afxUnit32                offset;
    afxUnit32                size;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    afxBuffer               vbo;
};

AFX_DEFINE_STRUCT(StrideEntry)
{
    StrideEntry*            next;
    afxUnit32                stride;
    freeVBlistEntry*        freelist;
    createdVBlistEntry*     vblist;
};

AFX_DEFINE_STRUCT(DynamicVertexBuffer)
{
    DynamicVertexBuffer*    next;
    afxBuffer               vbo;
    afxUnit32                size;
    afxUnit32                used;
    afxBuffer*              variableAddress;
};

#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER       4
#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE (256*1024)

AFX_OBJECT(afxBufferizer)
{
    afxDrawInput        din;
    afxUnit32            DefaultVBSize;

    StrideEntry*        StrideList;

    afxSlabAllocator    StrideFreeList;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;

    DynamicVertexBuffer*DynamicVertexBufferList;
    afxSlabAllocator    DynamicVertexBufferFreeList;

    afxUnit32            CurrentDynamicVertexBufferManager;
    afxUnit32            OffSetDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    afxUnit32            SizeDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    afxBuffer           DynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];

};

afxBool CreateVertexBuffer(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, afxBuffer* vbo, afxUnit32* offset)
{
    // based on RwD3D9CreateVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo);
    AFX_ASSERT(offset);
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList) // Search for an available stride
    {
        if (stride == strideList->stride)
            break;

        strideList = strideList->next;
    }

    if (!strideList)
    {
        // create one if there isn't any
        strideList = AfxAllocateSlab(&vbMgr->StrideFreeList);
        AFX_ASSERT(strideList);
        strideList->stride = stride;
        strideList->freelist = NULL;
        strideList->vblist = NULL;

        // Add it to the begining
        strideList->next = vbMgr->StrideList;
        vbMgr->StrideList = strideList;
    }

    freeVBlistEntry* freelist = strideList->freelist;

    while (freelist) // Search for an available free space
    {
        if (freelist->size >= size)
            break;

        freelist = freelist->next;
    }

    if (!freelist)
    {
        // If there is not free space, create a new VB
        freelist = AfxAllocateSlab(&vbMgr->FreeVBFreeList);
        AFX_ASSERT(freelist);
        freelist->offset = 0;
        freelist->size = (((vbMgr->DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;
        
        afxDrawContext dctx;
        AfxGetDrawInputContext(vbMgr->din, &dctx);

        afxBuffer vbo;
        afxBufferInfo spec = { 0 };
        spec.bufCap = freelist->size;
        spec.flags = afxBufferFlag_W;
        spec.usage = afxBufferUsage_VERTEX;

        if (AfxAcquireBuffers(dctx, 1, &spec, &vbo))
        {
            AfxDeallocateSlab(&vbMgr->FreeVBFreeList, freelist);            
            return FALSE;
        }
        
        freelist->vbo = vbo;
        freelist->next = strideList->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (strideList->freelist)
            strideList->freelist->prev = freelist;

        strideList->freelist = freelist;

        // Add an entry to the vb list
        createdVBlistEntry* vblist = AfxAllocateSlab(&vbMgr->CreatedVBFreeList);
        vblist->vbo = vbo;
        vblist->next = strideList->vblist;
        strideList->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    *vbo = freelist->vbo;
    *offset = freelist->offset;
    afxUnit32 nextOffset = (*offset) + size;

    AFX_ASSERT(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;
    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev)
            freelist->prev->next = freelist->next;

        if (freelist->next)
            freelist->next->prev = freelist->prev;

        if (strideList->freelist == freelist)
            strideList->freelist = freelist->next;

        AfxDeallocateSlab(&vbMgr->FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void DestroyVertexBuffer(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, afxBuffer vbo, afxUnit32 offset)
{
    // based on RwD3D9DestroyVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo != NULL);

    // Search for the stride
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList )
    {
        StrideEntry* next = strideList->next;

        if (stride == strideList->stride)
            break;

        strideList = next;
    }

    if (strideList)
    {
        // Search for an available free space to add
        freeVBlistEntry* freelist = strideList->freelist;

        while (freelist)
        {
            freeVBlistEntry* nextFreelist = freelist->next;

            if (freelist->vbo == vbo)
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == offset) break;
                else
                {
                    // Could be added to the begin of the free space?

                    if (offset + size == freelist->offset)
                        break;
                }
            }
            freelist = nextFreelist;
        }

        if (freelist)
        {
            afxBool canCompactMore = FALSE;
            afxUnit32 newoffset = offset;
            afxUnit32 newSize = size;
            freeVBlistEntry* oldFreelist = freelist;

            do
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == newoffset) freelist->size += newSize;
                else
                {
                    // Could be added to the begin of the free space?

                    if (newoffset + newSize == freelist->offset)
                    {
                        freelist->offset = newoffset;
                        freelist->size += newSize;
                    }
                }

                // Could be compact more?
                newoffset = freelist->offset;
                newSize = freelist->size;
                oldFreelist = freelist;
                canCompactMore = FALSE;
                freelist = strideList->freelist;
                
                while (freelist)
                {
                    freeVBlistEntry* nextFreelist = freelist->next;

                    if (freelist->vbo == vbo)
                    {
                        // Could be added to the end of the free space?

                        if ((freelist->offset + freelist->size) == newoffset)
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                        else
                        {
                            // Could be added to the begin of the free space?

                            if (newoffset + newSize == freelist->offset)
                            {
                                canCompactMore = TRUE;
                                break;
                            }
                        }
                    }
                    freelist = nextFreelist;
                }

                // Remove old list

                if (canCompactMore)
                {
                    if (oldFreelist->prev)
                        oldFreelist->prev->next = oldFreelist->next;

                    if (oldFreelist->next)
                        oldFreelist->next->prev = oldFreelist->prev;

                    if (strideList->freelist == oldFreelist)
                        strideList->freelist = oldFreelist->next;

                    AfxDeallocateSlab(&vbMgr->FreeVBFreeList, oldFreelist);
                }
            } while (canCompactMore);
        }
        else
        {
            freelist = AfxAllocateSlab(&vbMgr->FreeVBFreeList);
            freelist->offset = offset;
            freelist->size = size;
            freelist->vbo = vbo;

            // Add it to the begining
            freelist->next = strideList->freelist;
            freelist->prev = NULL;

            if (strideList->freelist)
                strideList->freelist->prev = freelist;

            strideList->freelist = freelist;
        }

#if defined(RWDEBUG)
        BlocksCreated--;
        BytesReserved -= size;
#endif
    }
}

afxBool DynamicVertexBufferCreate(afxBufferizer vbMgr, afxUnit size, afxBuffer *vertexBuffer)
{
    afxError err = NIL;
    DynamicVertexBuffer *freeVertexBuffer = NULL;
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (!currentVertexBuffer->used)
        {
            if (currentVertexBuffer->size == size)
            {
                break;
            }
            else if (freeVertexBuffer == NULL || freeVertexBuffer->size < currentVertexBuffer->size)
            {
                freeVertexBuffer = currentVertexBuffer;
            }
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = TRUE;

        *vertexBuffer = currentVertexBuffer->vbo;

        currentVertexBuffer->variableAddress = vertexBuffer;
    }
    else
    {
        /* Destroy a free vertex buffer to make room for the new one */
        if (freeVertexBuffer)
        {
            AfxReleaseObjects(1, &freeVertexBuffer->vbo);
            freeVertexBuffer->vbo = NULL;

            currentVertexBuffer = freeVertexBuffer;

#if defined(RWDEBUG)
            NumDynamicVertexBuffer--;
#endif /* defined(RWDEBUG) */
        }
        else
        {
            /* Create a new item */
            currentVertexBuffer = AfxAllocateSlab(&vbMgr->DynamicVertexBufferFreeList);

            /* Add it to the beginnig of the list */
            currentVertexBuffer->next = vbMgr->DynamicVertexBufferList;

            vbMgr->DynamicVertexBufferList = currentVertexBuffer;
        }

        afxDrawContext dctx;
        AfxGetDrawInputContext(vbMgr->din, &dctx);

        afxBufferInfo spec = { 0 };
        spec.bufCap = size;
        spec.usage = afxBufferUsage_VERTEX;
        spec.flags = afxBufferFlag_W;
        
        if (AfxAcquireBuffers(dctx, 1, &spec, vertexBuffer))
        {
            AfxThrowError();
            return FALSE;
        }

        /* Fill item info */
        currentVertexBuffer->size = size;
        currentVertexBuffer->used = TRUE;
        currentVertexBuffer->vbo = (*vertexBuffer);
        currentVertexBuffer->variableAddress = (vertexBuffer);

#if defined(RWDEBUG)
        NumDynamicVertexBuffer++;
#endif /* defined(RWDEBUG) */
    }

    return TRUE;
}

void DynamicVertexBufferDestroy(afxBufferizer vbMgr, void *vertexBuffer)
{
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (currentVertexBuffer->vbo == (vertexBuffer))
        {
            break;
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = FALSE;
        currentVertexBuffer->variableAddress = NULL;
    }
}

afxBool _VertexBufferManagerOpen(afxBufferizer vbMgr)
{
    afxError err = NIL;
    //avxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxSetUpSlabAllocator(&vbMgr->StrideFreeList, sizeof(StrideEntry), 16);
    AfxSetUpSlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxSetUpSlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    AfxSetUpSlabAllocator(&vbMgr->DynamicVertexBufferFreeList, sizeof(DynamicVertexBuffer), 4);

    afxBool created;
    afxUnit32 n;

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

        created = DynamicVertexBufferCreate(vbMgr, MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE, &(vbMgr->DynamicVertexBufferManager[n]));

        if (!created)
        {
            AfxThrowError();
        }
    }

    return TRUE;
}

void _VertexBufferManagerClose(afxBufferizer vbMgr)
{
    //avxVertexBufferizer* vbMgr = *vbMgrPtr;
#if defined(RWDEBUG)
    AFX_ASSERT(BlocksCreated == 0);
    AFX_ASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    // D3D9DynamicVertexBufferManagerDestroy();

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (afxUnit n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = 0;

        if (vbMgr->DynamicVertexBufferManager[n])
        {
            DynamicVertexBufferDestroy(vbMgr, vbMgr->DynamicVertexBufferManager[n]);

            vbMgr->DynamicVertexBufferManager[n] = NULL;
        }
    }

    // D3D9DynamicVertexBufferListDestroy();

    while (vbMgr->DynamicVertexBufferList)
    {
        DynamicVertexBuffer *next = vbMgr->DynamicVertexBufferList->next;

        if (vbMgr->DynamicVertexBufferList->vbo)
        {
            AfxReleaseObjects(1, &vbMgr->DynamicVertexBufferList->vbo);

            vbMgr->DynamicVertexBufferList->vbo = NULL;

            if (vbMgr->DynamicVertexBufferList->variableAddress)
            {
                *vbMgr->DynamicVertexBufferList->variableAddress = NULL;
            }
        }

        AfxDeallocateSlab(&vbMgr->DynamicVertexBufferFreeList, vbMgr->DynamicVertexBufferList);

        vbMgr->DynamicVertexBufferList = next;
    }

    AfxCleanUpSlabAllocator(&vbMgr->DynamicVertexBufferFreeList);

#if defined(RWDEBUG)
    NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

    // ---

    while (vbMgr->StrideList != NULL)
    {
        StrideEntry  *next = vbMgr->StrideList->next;

        /*  Destroy the free list */
        while (vbMgr->StrideList->freelist != NULL)
        {
            freeVBlistEntry   *nextFreelist = vbMgr->StrideList->freelist->next;

            AfxDeallocateSlab(&vbMgr->FreeVBFreeList, vbMgr->StrideList->freelist);

            vbMgr->StrideList->freelist = nextFreelist;
        }

        /*  Destroy the VB list */
        while (vbMgr->StrideList->vblist != NULL)
        {
            createdVBlistEntry   *nextvblist = vbMgr->StrideList->vblist->next;

            if (vbMgr->StrideList->vblist->vbo != NULL)
            {
                AfxReleaseObjects(1, &(vbMgr->StrideList->vblist->vbo));
            }

            AfxDeallocateSlab(&vbMgr->CreatedVBFreeList, vbMgr->StrideList->vblist);

            vbMgr->StrideList->vblist = nextvblist;
        }

        AfxDeallocateSlab(&vbMgr->StrideFreeList, vbMgr->StrideList);

        vbMgr->StrideList = next;
    }

    AfxCleanUpSlabAllocator(&vbMgr->CreatedVBFreeList);
    AfxCleanUpSlabAllocator(&vbMgr->FreeVBFreeList);
    AfxCleanUpSlabAllocator(&vbMgr->StrideFreeList);

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */
}

_AVX afxError _AvxBufzDtor(afxBufferizer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    
    _VertexBufferManagerClose(vbuf);
    
    return err;
}

_AVX afxError _AvxBufzCtor(afxBufferizer vbuf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);

    afxDrawInput din = args[0];
    afxBufferizerInfo const* info = ((afxBufferizerInfo const *)args[1]) + invokeNo;

    _VertexBufferManagerOpen(vbuf);

    return err;
}

_AVX afxClassConfig const _AvxVbufMgrCfg =
{
    .fcc = afxFcc_VBUF,
    .name = "Bufferizer",
    .name = "Bufferizer",
    .fixedSiz = sizeof(AFX_OBJECT(afxBufferizer)),
    .ctor = (void*)_AvxBufzCtor,
    .dtor = (void*)_AvxBufzDtor
};

_AVX afxError AfxAcquireBufferizer(afxDrawInput din, afxBufferizerInfo const* info, afxBufferizer bufferizer[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(bufferizer);
    AFX_ASSERT(info);

    afxClass* cls = AfxGetVertexBufferClass(din);
    AfxAssertClass(cls, afxFcc_VBUF);

    if (AfxAcquireObjects(cls, 1, (afxObject*)bufferizer, (void const*[]) { din, info }))
        AfxThrowError();

    return err;
}
