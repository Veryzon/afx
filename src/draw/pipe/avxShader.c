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

#define _AVX_DRAW_C
#define _AVX_SHADER_C
#include "../../dev/AvxImplKit.h"

_AVX afxError _AvxScanGlScript(afxString const* code, afxArray* fInOuts, afxArray* fResources, afxString* pushConstsName)
{
    afxError err = NIL;

    afxUri inc;
    afxChar buf[2048] = { 0 };
    afxUnit baseChar;
    afxString4096 src;
    AfxMakeString4096(&src, NIL);
    afxString2048 line;
    AfxMakeString2048(&line, NIL);
    
    afxUnit base = 0, strStep = 0;
    while ((strStep = AfxExcerptStringLine(code, base, &line.str)))
    {
        base += strStep;
        afxString excerpt;

        if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("OUT")))
        {
            afxUnit location = 0;
            afxString32 fmtName = { 0 };
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line.str, "OUT(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &location, fmtName.buf, varName.buf);

            if (fnd == 3)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&fmtName.str, 0, fmtName.buf, 0);
                AfxMakeString(&varName.str, 0, varName.buf, 0);
                afxVertexFormat fmt = AfxFindVertexFormat(&fmtName.str);

                afxUnit idx;
                avxShaderIoChannel* decl;

                if (!(decl = AfxInsertArrayUnit(fInOuts, &idx))) AfxThrowError();
                else
                {
                    AFX_ASSERT(location < 16); // hardcoded limitation
                    decl->location = location;
                    AFX_ASSERT(fmt < afxVertexFormat_TOTAL);
                    decl->fmt = fmt;
                    AfxMakeString8(&decl->semantic, &varName.str);
                }
            }
        }

        if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("IN")))
        {
            afxUnit location = 0;
            afxString32 fmtName = { 0 };
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line.str, "IN(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &location, fmtName.buf, varName.buf);

            if (fnd == 3)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&fmtName.str, 0, fmtName.buf, 0);
                AfxMakeString(&varName.str, 0, varName.buf, 0);

                afxVertexFormat fmt = AfxFindVertexFormat(&fmtName.str);

                afxUnit idx;
                avxShaderIoChannel* decl;

                if (!(decl = AfxInsertArrayUnit(fInOuts, &idx))) AfxThrowError();
                else
                {
                    AFX_ASSERT(location < 16); // hardcoded limitation
                    decl->location = location;
                    AFX_ASSERT(fmt < afxVertexFormat_TOTAL);
                    decl->fmt = fmt;
                    AfxMakeString8(&decl->semantic, &varName.str);
                }
            }
        }

        {
            if ((AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("SAMPLING_UNIT"))) ||
                (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("TEXTURE")))
                )
            {
                afxUnit set = 0, binding = 0, resCnt = 1;
                afxString32 typeName = { 0 };
                afxString32 varName = { 0 };
                afxUnit fnd = AfxScanString(&line.str, "SAMPLING_UNIT(%u, %u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &set, &binding, typeName.buf, varName.buf);

                if (!fnd)
                    fnd = AfxScanString(&line.str, "TEXTURE(%u, %u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &set, &binding, typeName.buf, varName.buf);

                if (fnd == 4)
                {
                    AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                    AfxMakeString(&typeName.str, 0, typeName.buf, 0);
                    AfxMakeString(&varName.str, 0, varName.buf, 0);

                    avxShaderParam resType = 0;

                    if ((0 == AfxCompareStrings(&typeName.str, FALSE, 1, &AfxString("sampler1D"))) ||
                        (0 == AfxCompareStrings(&typeName.str, FALSE, 1, &AfxString("sampler2D"))) ||
                        (0 == AfxCompareStrings(&typeName.str, FALSE, 1, &AfxString("sampler3D"))) ||
                        (0 == AfxCompareStrings(&typeName.str, FALSE, 1, &AfxString("samplerCube")))
                        )
                    {
                        resType = avxShaderParam_COMBINED_IMAGE_SAMPLER;
                    }

                    afxUnit idx;
                    avxShaderResource *decl;

                    if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                    else
                    {
                        AFX_ASSERT(set < 4);
                        decl->set = set;
                        AFX_ASSERT(binding < 10);
                        decl->binding = binding;
                        AFX_ASSERT(resType < avxShaderParam_TOTAL);
                        decl->type = resType;
                        AFX_ASSERT(decl->type != avxShaderParam_OUTPUT);
                        AFX_ASSERT(resCnt);
                        decl->cnt = AfxMax(resCnt, 1);
                        AfxMakeString16(&decl->name, &varName.str);
                    }
                }
            }

            if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("UNIFORM_UNIT")))
            {
                afxUnit set = 0, binding = 0, resCnt = 1;
                afxString32 varName = { 0 };
                afxUnit fnd = AfxScanString(&line.str, "UNIFORM_UNIT(%u, %u, %32[A-Za-z0-9_] )", &set, &binding, varName.buf);

                if (fnd == 3)
                {
                    AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                    AfxMakeString(&varName.str, 0, varName.buf, 0);

                    avxShaderParam resType = avxShaderParam_UNIFORM_UNIT;

                    afxUnit idx;
                    avxShaderResource *decl;

                    if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                    else
                    {
                        AFX_ASSERT(set < 4);
                        decl->set = set;
                        AFX_ASSERT(binding < 10);
                        decl->binding = binding;
                        AFX_ASSERT(resType < avxShaderParam_TOTAL);
                        decl->type = resType;
                        AFX_ASSERT(decl->type != avxShaderParam_OUTPUT);
                        AFX_ASSERT(resCnt);
                        decl->cnt = AfxMax(resCnt, 1);
                        AfxMakeString16(&decl->name, &varName.str);
                    }
                }
            }

            if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("STORAGE_UNIT")))
            {
                afxUnit set = 0, binding = 0, resCnt = 1;
                afxString32 accessName = { 0 };
                afxString32 varName = { 0 };
                afxUnit fnd = AfxScanString(&line.str, "STORAGE_UNIT(%u, %u, %32[A-Za-z0-9], %32[A-Za-z0-9_] )", &set, &binding, accessName.buf, varName.buf);

                if (fnd == 4)
                {
                    AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                    AfxMakeString(&accessName.str, 0, accessName.buf, 0);
                    AfxMakeString(&varName.str, 0, varName.buf, 0);

                    avxShaderParam resType = avxShaderParam_STORAGE_UNIT;

                    afxUnit idx;
                    avxShaderResource *decl;

                    if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                    else
                    {
                        AFX_ASSERT(set < 4);
                        decl->set = set;
                        AFX_ASSERT(binding < 10);
                        decl->binding = binding;
                        AFX_ASSERT(resType < avxShaderParam_TOTAL);
                        decl->type = resType;
                        AFX_ASSERT(decl->type != avxShaderParam_OUTPUT);
                        AFX_ASSERT(resCnt);
                        decl->cnt = AfxMax(resCnt, 1);
                        AfxMakeString16(&decl->name, &varName.str);
                    }
                }
            }


            if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("PUSHABLE")))
            {
                afxString32 varName = { 0 };
                afxUnit fnd = AfxScanString(&line.str, "PUSHABLE(%32[A-Za-z0-9_] )", varName.buf);

                if (fnd == 1)
                {
                    AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);
                    AfxMakeString(&varName.str, 0, varName.buf, 0);
                    AfxCopyString(pushConstsName, &varName.str);
                }
            }
        }
    }
    return err;
}

_AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode)
{
    afxError err = NIL;

    afxStream inc;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &inc);

    afxChar buf[2048] = { 0 };
    afxUnit baseChar;
    afxString4096 src;
    AfxMakeString4096(&src, NIL);
    afxString2048 line;
    AfxMakeString2048(&line, NIL);

    while (!AfxReadFileLine(file, &line.str))
    {
        afxString excerpt;

        if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("#include ")))
        {
            afxBool fnd = AfxScanString(&line.str, "#include <%128[^>]>\n", buf);

            afxUri incUri;
            AfxMakeUri(&incUri, 0, buf, 0);

            if (AfxReopenFile(inc, &incUri, afxFileFlag_R)) AfxThrowError();
            else
            {
                //afxChar* room = AfxInsertArrayUnits(&bp->codes, line.str.len + 3, &baseChar, NIL);
                //AfxDumpString(&AfxString("// "), 0, 3, room);
                //AfxDumpString(&line.str, 0, line.str.len, &room[3]);

                if (_AvxLoadGlScript(inc, fCode))
                    AfxThrowError();
            }
            continue;
        }

        void* room = AfxInsertArrayUnits(fCode, line.str.len, &baseChar, NIL);
        AfxDumpString(&line.str, 0, line.str.len, room);
    }
    AfxReleaseObjects(1, &inc);
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxSerializeShader(avxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOB);
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    AfxWriteStream(ios, shd->codeLen, 0, shd->code);

    return err;
}

_AVX afxError AfxDumpShaderCode(avxShader shd, afxArray* arr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxArray fCode;

    if (shd->codeLen)
    {
        AfxWrapArray(&fCode, 1, shd->codeLen, shd->code, shd->codeLen);
        AfxAppendArray(arr, &fCode);
    }
    else
    {
        afxStream file;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxStreamUsage_FILE;
        iobi.flags = afxStreamFlag_READABLE;
        if (AfxAcquireStream(1, &iobi, &file)) AfxThrowError();
        else
        {
            afxUri* path = &shd->uri.uri;
            AFX_ASSERT(!AfxIsUriBlank(path));

            if (AfxReopenFile(file, path, afxFileFlag_R)) AfxThrowError();
            else
            {
                AfxMakeArray(&fCode, 4096, sizeof(afxByte), NIL);
                _AvxLoadGlScript(file, &fCode);
                AfxAppendArray(arr, &fCode);
                AfxCleanUpArray(&fCode);
            }
            AfxReleaseObjects(1, &file);
        }
    }

    return err;
}

_AVX afxError AfxPrintShader(avxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AFX_ASSERT(uri);
    AFX_ASSERT(!(AfxIsUriBlank(uri)));

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W)) AfxThrowError();
    else
    {
        if (AfxSerializeShader(shd, file)) AfxThrowError();
        else
        {
            // success
        }
        AfxReleaseObjects(1, &file);
    }
    return err;
}

_AVX afxUnit AfxCountShaderIoChannels(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->ioDeclCnt;
}

_AVX afxUnit AfxGetShaderIoChannels(avxShader shd, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AFX_ASSERT_RANGE(shd->ioDeclCnt, first, cnt);
    AFX_ASSERT(channels);
    AFX_ASSERT(cnt);
    afxUnit hitCnt = 0;

    avxShaderIoChannel const* ins = shd->ioDecls;
    afxUnit cnt2 = AfxMin(shd->ioDeclCnt, cnt);

    for (afxUnit i = 0; i < cnt2; i++)
    {
        AfxCopy(&channels[i], &ins[first + i], sizeof(channels[0]));
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxUnit AfxCountShaderInterfaces(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->resDeclCnt;
}

_AVX afxResult AfxDescribeShaderInterfaces(avxShader shd, afxUnit first, afxUnit cnt, avxShaderResource rsrc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AFX_ASSERT_RANGE(shd->resDeclCnt, first, cnt);
    AFX_ASSERT(rsrc);
    AFX_ASSERT(cnt);

    afxResult rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        rsrc[i].set = shd->resDecls[first + i].set;
        rsrc[i].binding = shd->resDecls[first + i].binding;
        rsrc[i].type = shd->resDecls[first + i].type;
        rsrc[i].cnt = shd->resDecls[first + i].cnt;
        rsrc[i].name = shd->resDecls[first + i].name;
        ++rslt;
    }
    return rslt;
}

_AVX avxShaderStage AfxGetShaderStage(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->stage;
}

_AVX afxUri const* AfxShaderGetUri(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return &shd->uri.uri;
}

_AVX afxError _AvxShdStdDtorCb(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxObjectStash stashs[] =
    {
        {
            .cnt = shd->resDeclCnt,
            .siz = sizeof(shd->resDecls[0]),
            .var = (void**)&shd->resDecls
        },
        {
            .cnt = shd->ioDeclCnt,
            .siz = sizeof(shd->ioDecls[0]),
            .var = (void**)&shd->ioDecls
        },
        {
            .cnt = shd->codeLen,
            .siz = sizeof(shd->code[0]),
            .var = (void**)&shd->code
        }
    };

    if (AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
        AfxThrowError();

    //AfxDeallocateUri(&shd->uri);

    return err;
}

_AVX afxError _AvxShdStdCtorCb(avxShader shd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxDrawContext dctx = args[0];
    afxUri const* uri = args[1] ? ((afxUri const*)args[1]) + invokeNo : NIL;
    afxString const* code = args[2] ? ((afxString const*)args[2]) + invokeNo : NIL;
    
    AfxMakeUri128(&shd->uri, uri);

    //shd->verMajor = AfxElse(cfg->verMajor, 4);
    //shd->verMinor = AfxElse(cfg->verMajor, shd->verMajor == 4 ? 0 : (shd->verMinor == 3 ? 3 : (shd->verMinor == 2 ? 1 : 5)));
    //shd->extended = !!cfg->extended;
    shd->stage = 0;

    AfxMakeString32(&shd->pushConstName, NIL);

    afxArray fInOuts, fResources;
    AfxMakeArray(&fInOuts, 16, sizeof(avxShaderIoChannel), NIL);
    AfxMakeArray(&fResources, 16, sizeof(avxShaderResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    if (code && code->len)
    {
        _AvxScanGlScript(code, &fInOuts, &fResources, &shd->pushConstName.str);
        shd->codeLen = code->len + 1; // keep code in memory
    }
    else if (uri)
    {
        shd->codeLen = 0; // dont keep code in memory

        afxStream file;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxStreamUsage_FILE;
        iobi.flags = afxStreamFlag_READABLE;
        if (AfxAcquireStream(1, &iobi, &file)) AfxThrowError();
        else
        {
            AFX_ASSERT(!AfxIsUriBlank(uri));

            if (AfxReopenFile(file, uri, afxFileFlag_R)) AfxThrowError();
            else
            {
                afxArray fCode;
                afxString fCodeS;
                AfxMakeArray(&fCode, 4096, sizeof(afxByte), NIL);
                _AvxLoadGlScript(file, &fCode);
                AfxMakeString(&fCodeS, fCode.pop, fCode.data, fCode.pop);
                _AvxScanGlScript(&fCodeS, &fInOuts, &fResources, &shd->pushConstName.str);
                AfxCleanUpArray(&fCode);
            }
            AfxReleaseObjects(1, &file);
        }
    }
    else AfxThrowError();

    shd->resDeclCnt = fResources.pop;
    shd->resDecls = NIL;
    shd->ioDeclCnt = fInOuts.pop;
    shd->ioDecls = NIL;

    afxObjectStash stashs[] =
    {
        {
            .cnt = shd->resDeclCnt,
            .siz = sizeof(shd->resDecls[0]),
            .var = (void**)&shd->resDecls
        },
        {
            .cnt = shd->ioDeclCnt,
            .siz = sizeof(shd->ioDecls[0]),
            .var = (void**)&shd->ioDecls
        },
        {
            .cnt = shd->codeLen,
            .siz = sizeof(shd->code[0]),
            .var = (void**)&shd->code
        }
    };

    if (AfxAllocateInstanceData(shd, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
        if (shd->codeLen)
        {
            AfxCopy(shd->code, code->start, code->len);
            shd->code[shd->codeLen] = '\0';
        }

        for (afxUnit j = 0; j < shd->resDeclCnt; j++)
        {
            avxShaderResource const *decl = AfxGetArrayUnit(&fResources, j);
            shd->resDecls[j].set = decl->set;
            AFX_ASSERT(4/*_SGL_MAX_LEGO_PER_BIND*/ > shd->resDecls[j].set);
            shd->resDecls[j].binding = decl->binding;
            AFX_ASSERT(8/*_SGL_MAX_ENTRY_PER_LEGO*/ > shd->resDecls[j].binding);
            //shd->resDecls[j].visibility = decl->visibility;
            shd->resDecls[j].type = decl->type;
            shd->resDecls[j].cnt = decl->cnt;
            AfxMakeString16(&shd->resDecls[j].name, &decl->name.str);
        }

        for (afxUnit i = 0; i < shd->ioDeclCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fInOuts, i);
            shd->ioDecls[i] = *ioBp;
            AfxMakeString8(&shd->ioDecls[i].semantic, &ioBp->semantic.str);
        }

        shd->topology = avxTopology_TRI_LIST;

        if (err && AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
            AfxThrowError();
    }

    AfxCleanUpArray(&fInOuts);
    AfxCleanUpArray(&fResources);

    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_AVX afxClassConfig const _AvxShdStdImplementation =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxShader)),
    .ctor = (void*)_AvxShdStdCtorCb,
    .dtor = (void*)_AvxShdStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireShaders(afxDrawContext dctx, afxUnit cnt, afxUri const uris[], afxString const codes[], avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(uris || codes);
    AFX_ASSERT(shaders);
    AFX_ASSERT(cnt);

    afxClass* cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shaders, (void const*[]) { dctx, uris, codes }))
        AfxThrowError();

    return err;
}