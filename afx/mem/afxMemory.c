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


//#undef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
//#include "rpmalloc/rpmalloc.h"
#include <stdio.h>
#include <string.h>
#include <crtdbg.h>

#define _AFX_CORE_C
#define _AFX_MMU_C
#include "../dev/afxDevCoreBase.h"

typedef struct _afxArbitraryChunk
{
#ifndef _AFX_DONT_DEBUG_MEM_LEAK
    afxSize             siz;
    afxNat16            line;
    afxChar const       *file;
#endif
    _AFX_DBG_FCC;
    afxByte AFX_SIMD gap[16];
    afxByte AFX_SIMD data[];
} _afxArbitraryChunk;

_AFX afxError _AfxInitMmu(afxThread thr)
{
    afxError err = NIL;

    if (thr)
    {
        //rpmalloc_thread_initialize();
    }
    else
    {
        //rpmalloc_initialize(0);
    }
    return err;
}

_AFX afxError _AfxDeinitMmu(afxThread thr)
{
    afxError err = NIL;
    
    if (thr)
    {
        //rpmalloc_thread_finalize();
    }
    else
    {
        //rpmalloc_finalize();
    }
    return err;
}

_AFX void* AfxMemset(void* dst, afxInt val, afxSize siz)
{
    return memset(dst, val, siz);
}

_AFX void* AfxMemcpy(void* dst, void const* src, afxSize siz)
{
    return memcpy(dst, src, siz);
}

_AFX afxInt AfxMemcmp(void const* buf1, void const* buf2, afxSize siz)
{
    return memcmp(buf1, buf2, siz);
}

_AFX void const* AfxMemchr(void const* buf, afxInt val, afxNat cap)
{
    return memchr(buf, val, cap);
}

_AFX void* AfxMallocCb(afxSize siz, afxHere const hint)
{
    void* p;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p = _malloc_dbg(siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p = malloc(siz);
#endif
#else
    p = malloc(siz);
#endif
    return p;
}

_AFX void AfxFreeCb(void* block)
{
    void* p = block;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    _free_dbg(p, _NORMAL_BLOCK);
#else
    free(p);
#endif
#else
    rpfree(p);
#endif
}

_AFX void* AfxCallocCb(afxSize cnt, afxSize siz, afxHere const hint)
{
    void* p;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p = _calloc_dbg(cnt, siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p = calloc(cnt, siz);
#endif
#else
    p = rpcalloc(cnt, siz);
#endif
    return p;
}

_AFX void* AfxReallocCb(void* block, afxSize siz, afxHere const hint)
{
    void* p = block;

    void* p2;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p2 = _realloc_dbg(p, siz, _NORMAL_BLOCK, (const char*)hint[0], hint[1]);
#else
    p2 = realloc(p, siz);
#endif
#else
    p2 = rprealloc(p, siz);
#endif
    return p2;
}

_AFX void AfxFreeAlignedCb(void* block)
{
    void* p = block;

#ifndef SWAP_ALLOC
#ifdef _DEBUG
    _aligned_free_dbg(p);
#else
    _aligned_free(p);
#endif    
#else
    return rpfree(p);
#endif
}

_AFX void* AfxMallocAlignedCb(afxSize siz, afxSize align, afxHere const hint)
{
    void* p;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p = _aligned_malloc_dbg(siz, align, (const char*)hint[0], hint[1]);
#else
    p = _aligned_malloc(siz, align);
#endif
#else
    p = rpaligned_alloc(align, siz);
#endif
    return p;
}

_AFX void* AfxReallocAlignedCb(void* block, afxSize siz, afxSize align, afxHere const hint)
{
    void* p = block;
    void* p2;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p2 = _aligned_realloc_dbg(p, siz, align, (const char*)hint[0], hint[1]);
#else
    p2 = _aligned_realloc(p, siz, align);
#endif
#else
    p2 = rpaligned_realloc(p, align, siz, 0, 0);
#endif
    return p2;
}

_AFX void* AfxRecallocAlignedCb(void* block, afxSize cnt, afxSize siz, afxSize align, afxHere const hint)
{
    void* p = block;
    void* p2;
#ifndef SWAP_ALLOC
#ifdef _DEBUG
    p2 = _aligned_recalloc_dbg(p, cnt, siz, align, (const char*)hint[0], hint[1]);
#else
    p2 = _aligned_recalloc(p, cnt, siz, align);
#endif
#else
    p2 = rpaligned_realloc(p, align, siz * cnt, 0, 0);
#endif
    return p2;
}


_AFX void* AfxMalloc(afxSize siz)
{
    return AfxMallocCb(siz, AfxHere());
}

_AFX void AfxFree(void* block)
{
    AfxFreeCb(block);
}

_AFX void* AfxCalloc(afxSize cnt, afxSize siz)
{
    return AfxCallocCb(cnt, siz, AfxHere());
}

_AFX void* AfxRealloc(void* block, afxSize siz)
{
    return AfxReallocCb(block, siz, AfxHere());
}

_AFX void AfxFreeAligned(void* block)
{
    AfxFreeAlignedCb(block);
}

_AFX void* AfxMallocAligned(afxSize siz, afxSize align)
{
    return AfxMallocAlignedCb(siz, align, AfxHere());
}

_AFX void* AfxReallocAligned(void* block, afxSize siz, afxSize align)
{
    return AfxReallocAlignedCb(block, siz, align, AfxHere());
}

_AFX void* AfxRecallocAligned(void* block, afxSize cnt, afxSize siz, afxSize align)
{
    return AfxRecallocAlignedCb(block, cnt, siz, align, AfxHere());
}

_AFX void _AfxMemAllocNotStd(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    AfxAssert(siz);
    AfxAssert(cnt);
    AfxAssert(hint);
}

_AFX void _AfxMemDeallocNotStd(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    //AfxAssert(siz);
    AfxAssert(hint);
}

_AFX void _AfxMemDeallocStd(afxMmu mmu, void *p)
{
    //AfxEntry("p=%p", p);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssert(p);

    AfxFreeAligned(p);

    if (mmu->deallocNotCb)
        mmu->deallocNotCb(mmu, 0, 0, AfxHere());
}

_AFX void* _AfxMemReallocStd(afxMmu mmu, void* p, afxSize cnt, afxSize siz, afxSize align, afxHere const hint)
{
    //AfxEntry("ctx=%p,p=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    //AfxAssert(p);
    AfxAssert(siz);
    void *neo = NIL;

    // TODO Allow rememocate to an or another memocator.

    if (!(neo = AfxReallocAligned(p, cnt * siz, align ? align : AFX_SIMD_ALIGN)))
        AfxThrowError();

    return neo;
}

_AFX void* _AfxMemAllocStd(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxHere const hint)
{
    //AfxEntry("ctx=%p,siz=%u,hint=\"%s:%i!%s\"", ctx, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    AfxAssert(siz);
    AfxAssert(hint);
    void *p = NIL;

    if (!(p = AfxMallocAligned(cnt * siz, align ? align : AFX_SIMD_ALIGN)))
        AfxThrowError();

    return p;
}

// ALL API

_AFX afxSize AfxMemoryGetDefaultAlignment(afxMmu mmu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu->defAlign;
}

_AFX afxError AfxMemoryEnableDebugging(afxMmu mmu, afxNat level)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    mmu->dbgLevel = level;
    return err;
}

_AFX afxError _AfxMmuCtor(afxMmu mmu, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    afxAllocationStrategy const *as = args[0] ? ((afxAllocationStrategy const*)args[0]) + invokeNo : NIL;
    afxSize const *hint = ((afxSize const*)args[1]) + invokeNo;
    //AfxAssert(as);

    //AfxAssert(!as->align || as->align >= AFX_SIMD_ALIGN);
    mmu->defAlign = AFX_SIMD_ALIGN;

    mmu->dbgLevel = 3;

    mmu->fname = (char const *const)hint[0];
    mmu->fline = (int)hint[1];
    mmu->func = (char const *const)hint[2];

    if (as)
    {
        if (as->align)
            mmu->defAlign = as->align;
    }

    AfxDeploySlock(&mmu->memSlock);
    AfxDeployChain(&mmu->memChain, mmu);
    // Choose which memocation mechanism to be used. Actumemy there's just two: standard (arbitrary) and arena.

    //if (mmu->cap != (afxSize)0) _AfxArenaAllCtor(ctx, paradigm);
    //else
    {
        mmu->allocCb = _AfxMemAllocStd;
        mmu->reallocCb = _AfxMemReallocStd;
        mmu->deallocCb = _AfxMemDeallocStd;
        mmu->allocNotCb = _AfxMemAllocNotStd;
        mmu->deallocNotCb = _AfxMemDeallocNotStd;

        //_AfxMemCtorArena(ctx);
    }
    return err;
}

_AFX afxError _AfxMmuDtor(afxMmu mmu)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    mmu->dbgLevel = 3;

    //afxResult rslt = AfxMemoryExhaust(ctx);

    //if (rslt)
//        AfxLogAdvertence("Recovered. %i bytes orphaned @ (ctx)%p", rslt, ctx);

    AfxDismantleSlock(&mmu->memSlock);

    return err;
}

_AFX afxClassConfig const _AfxMmuMgrCfg =
{
    .fcc = afxFcc_MMU,
    .name = "Memory Management Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxMmu)),
    .ctor = (void*)_AfxMmuCtor,
    .dtor = (void*)_AfxMmuDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMmus(afxNat cnt, afxHere const hint, afxAllocationStrategy const strategy[], afxMmu mmus[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetMmuClass();
    AfxAssertClass(cls, afxFcc_MMU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)mmus, (void const*[]) { strategy, hint }))
        AfxThrowError();

    return err;
}

// MEMORY API

_AFX void AfxStream2(afxNat cnt, void const* src, afxSize srcStride, void* dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(cnt);
    //AfxAssert(srcStride);
    AfxAssert(dstStride);

    if ((srcStride == dstStride) && dstStride)
    {
        AfxCopy2(dst, src, dstStride, cnt);
    }
    else
    {
        afxByte* dst2 = dst;
        afxByte const *src2 = src;
        afxNat unitSiz = AfxMax(1, AfxMin(dstStride, srcStride)); // minor non-zero
        AfxAssert(unitSiz);
        //afxNat cnt = len / dstStride;
        //afxNat cnt2 = len % dstStride;
        afxNat majStride = AfxMax(1, AfxMax(dstStride, srcStride));
        //cnt = len / majStride;
        //AfxAssert(!(len % majStride));
        //AfxAssert(!cnt2);

        for (afxNat i = 0; i < cnt; i++)
        {
            AfxCopy(&(dst2[i * dstStride]), &(src2[i * srcStride]), unitSiz);
        }
    }
#if 0
    afxByte* dst2 = dst;
    afxByte const* src2 = src;
    afxSize unitSiz = AfxMinorNonZero(dstStride, srcStride);

    if ((dstStride == srcStride) && dstStride)
        AfxCopy2(len, unitSiz, src2, dst2);
    else for (afxNat i = 0; i < len; i++)
        AfxCopy2(1, unitSiz, &src2[i * srcStride], &dst2[i * dstStride]);
#endif
}

_AFX void AfxStream3(afxNat cnt, void const* src, afxNat srcOffset, afxSize srcStride, void* dst, afxNat dstOffset, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(cnt);
    //AfxAssert(srcStride);
    AfxAssert(dstStride);

    if ((srcStride == dstStride) && dstStride)
    {
        if ((srcOffset == dstOffset) && dstOffset)
            AfxStream2(cnt, src, srcStride, dst, dstStride);
        else
            AfxCopy2(dst, src, dstStride, cnt);
    }
    else
    {
        afxByte* dst2 = dst;
        afxByte const *src2 = src;
        afxNat unitSiz = AfxMax(1, AfxMin(dstStride, srcStride)); // minor non-zero
        AfxAssert(unitSiz);
        //afxNat cnt = len / dstStride;
        //afxNat cnt2 = len % dstStride;
        afxNat majStride = AfxMax(1, AfxMax(dstStride, srcStride));
        //cnt = len / unitSiz;
        //AfxAssert(!(len % majStride));
        //AfxAssert(!cnt2);

        for (afxNat i = 0; i < cnt; i++)
        {
            AfxCopy(&(dst2[(i * dstStride) + dstOffset]), &(src2[(i * srcStride) + srcOffset]), unitSiz);
        }
    }
}

_AFX void AfxCopy2(void* dst, void const* src, afxSize siz, afxSize cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxCatchError(err);
#if !0
    if (cnt * siz)
        AfxMemcpy(dst, src, cnt * siz);
#else
    register afxChar* dst2 = dst;
    register afxChar const* src2 = src;
    register afxSize t = (afxOffset)src2;

    if ((afxSize)dst2 < (afxSize)src2)
    {
        if ((t | (afxOffset)dst2) & (sizeof(afxSize) - 1))
        {
            t = ((t ^ (afxOffset)dst2) & (sizeof(afxSize) - 1) || range < sizeof(afxSize)) ? range : (sizeof(afxSize) - (t & (sizeof(afxSize) - 1)));
            range -= t;
            do { (*(dst2)++ = *(src2)++); } while (--(t));
        }

        if ((t = range / sizeof(afxSize)))
            do { *(afxSize*)dst2 = *(afxSize*)src2; src2 += sizeof(afxSize); dst2 += sizeof(afxSize); } while (--(t));

        if ((t = range & (sizeof(afxSize) - 1)))
            do { *(dst2)++ = *(src2)++; } while (--(t));
    }
    else
    {
        src2 += range;
        dst2 += range;

        if ((t | (afxOffset)dst2) & (sizeof(afxSize) - 1))
        {
            if ((t ^ (afxOffset)dst2) & (sizeof(afxSize) - 1) || range <= sizeof(afxSize)) t = range;
            else t &= (sizeof(afxSize) - 1);

            range -= t;
            do { *(--(dst2)) = *(--(src2)); } while (--(t));
        }

        if ((t = range / sizeof(afxSize)))
            do { src2 -= sizeof(afxSize); dst2 -= sizeof(afxSize); *(afxSize*)dst2 = *(afxSize*)src2; } while (--(t));

        if ((t = range & (sizeof(afxSize) - 1)))
            do { *(--(dst2)) = *(--(src2)); } while (--(t));
    }
#endif
}

_AFX void AfxCopy(void* dst, void const* src, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(src);
    AfxAssert(dst);
    AfxAssert(src != dst);
    AfxAssert(siz);
    AfxCopy2(dst, src, siz, 1);
}

_AFX void AfxFill2(void* p, void const* value, afxSize siz, afxSize cnt)
{
    // This function fills the first 'cnt' sets of 'unitSiz' bytes of memory area pointed by 'p' with the constant 'value'.

    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(cnt);
    //AfxAssert(value);
    AfxAssert(siz);

    if (value)
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            afxByte* ptr = (afxByte*)p;
            AfxCopy(&ptr[i * siz], value, siz);
        }
    }
    else
    {
        if (cnt)
            AfxMemset(p, 0, cnt * siz);
    }
}

_AFX void AfxFill(void* p, void const* value, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    //AfxAssert(value);
    AfxAssert(siz);
    AfxFill2(p, value, siz, 1);
}

_AFX void AfxZero2(void* p, afxSize siz, afxSize cnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxMemset(p, 0, cnt * siz);
}

_AFX void AfxZero(void *p, afxSize siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(p);
    AfxAssert(siz);
    AfxZero2(p, siz, 1);
}

_AFX void AfxDeallocate(void *p)
{
    afxError err = AFX_ERR_NONE;

    if (p)
    {
        AfxAssert(p);
        afxSize freedSiz = 0;

        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            mmu->deallocCb(mmu, p);
        }
        else
        {
            AfxFreeAlignedCb(p);
        }
    }
}

_AFX void* AfxReallocate(void *p, afxSize siz, afxSize cnt, afxNat align, afxHere const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(siz);
    AfxAssert(cnt);
    AfxAssert(hint);
    //AfxEntry("p=%p,siz=%u,hint=\"%s:%i!%s\"", p, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    void *out = NIL;

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

    if (p)
    {
        if (!mmu)
        {
            if (!(out = AfxReallocAlignedCb(p, cnt * siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return out;
        }
        else
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(out = mmu->reallocCb(mmu, p, cnt, siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return out;
        }
    }
    else
    {
        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(out = mmu->reallocCb(mmu, p, cnt, siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return out;
        }
        else
        {
            if (!(out = AfxReallocAlignedCb(NIL, cnt * siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return out;
        }
    }
    return out;
}

_AFX void* AfxCoallocate(afxSize cnt, afxSize siz, afxNat align, afxHere const hint)
{
    //AfxEntry("ctx=%p,cnt=%u,siz=%u,hint=\"%s:%i!%s\"", ctx, cnt, siz, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(siz);
    AfxAssert(hint);
    void *p = NIL;

    afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

    if (mmu)
    {
        AfxAssertObjects(1, &mmu, afxFcc_MMU);

        if (!(p = mmu->allocCb(mmu, cnt, siz, AfxMax(sizeof(void*), align), hint)))
            AfxThrowError();
    }
    else
    {
        if (!(p = AfxRecallocAlignedCb(NIL, cnt, siz, AfxMax(sizeof(void*), align), hint)))
            AfxThrowError();
    }
    return p;
}

_AFX void* AfxAllocate(afxSize cnt, afxSize siz, afxNat align, afxHere const hint)
{
    //AfxEntry("ctx=%p,siz=%u,cnt=%u,hint=\"%s:%i!%s\"", ctx, siz, cnt, _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
    afxError err = AFX_ERR_NONE;
    AfxAssert(hint);
    AfxAssert(siz);
    AfxAssert(cnt);
    void *p = NIL;

    if (cnt * siz)
    {
        afxMmu mmu = /*AfxGetSystem() ? AfxGetSystemContext() :*/ NIL;

        if (mmu)
        {
            AfxAssertObjects(1, &mmu, afxFcc_MMU);

            if (!(p = mmu->allocCb(mmu, cnt, siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return p;
        }
        else
        {
            if (!(p = AfxMallocAlignedCb(cnt * siz, AfxMax(sizeof(void*), align), hint)))
                AfxThrowError();

            return p;
        }
    }
    return p;
}



