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

#include "../dev/AvxImplKit.h"


_AVX afxString const g_str_Qwadro = AFX_STRING("Qwadro");
_AVX afxString const g_str_Draw = AFX_STRING("Draw");
_AVX afxString const g_str_Shader = AFX_STRING("Shader");
_AVX afxString const g_str_Include = AFX_STRING("Include");
_AVX afxString const g_str_Resource = AFX_STRING("Resource");
_AVX afxString const g_str_In = AFX_STRING("In");
_AVX afxString const g_str_Out = AFX_STRING("Out");
_AVX afxString const g_str_Stage = AFX_STRING("Stage");
_AVX afxString const g_str_stage = AFX_STRING("stage");
_AVX afxString const g_str_id = AFX_STRING("id");
_AVX afxString const g_str_name = AFX_STRING("name");
_AVX afxString const g_str_entry = AFX_STRING("entry");
_AVX afxString const g_str_Entry = AFX_STRING("Entry");
_AVX afxString const g_str_flag = AFX_STRING("flag");
_AVX afxString const g_str_Flag = AFX_STRING("Flag");
_AVX afxString const g_str_uri = AFX_STRING("uri");
_AVX afxString const g_str_set = AFX_STRING("set");
_AVX afxString const g_str_binding = AFX_STRING("binding");
_AVX afxString const g_str_type = AFX_STRING("type");
_AVX afxString const g_str_count = AFX_STRING("count");
_AVX afxString const g_str_location = AFX_STRING("location");
_AVX afxString const g_str_format = AFX_STRING("format");
_AVX afxString const g_str_topology = AFX_STRING("topology");
_AVX afxString const g_str_restart = AFX_STRING("restart");

_AVX afxString const g_str_DepthClampEnable = AFX_STRING("DepthClampEnable");
_AVX afxString const g_str_RasterizerDiscardEnable = AFX_STRING("RasterizerDiscardEnable");
_AVX afxString const g_str_FillMode = AFX_STRING("FillMode");
_AVX afxString const g_str_CullMode = AFX_STRING("CullMode");
_AVX afxString const g_str_FrontFace = AFX_STRING("FrontFace");
_AVX afxString const g_str_DepthBias = AFX_STRING("DepthBias");
_AVX afxString const g_str_enable = AFX_STRING("enable");
_AVX afxString const g_str_constant = AFX_STRING("constant");
_AVX afxString const g_str_slope = AFX_STRING("slope");
_AVX afxString const g_str_clamp = AFX_STRING("clamp");
_AVX afxString const g_str_LineWidth = AFX_STRING("LineWidth");

_AVX afxString const g_str_DepthTestEnable = AFX_STRING("DepthTestEnable");
_AVX afxString const g_str_DepthWriteEnable = AFX_STRING("DepthWriteEnable");
_AVX afxString const g_str_DepthCompareOp = AFX_STRING("DepthCompareOp");
_AVX afxString const g_str_DepthBoundsTest = AFX_STRING("DepthBoundsTest");
_AVX afxString const g_str_StencilTest = AFX_STRING("StencilTest");
_AVX afxString const g_str_Front = AFX_STRING("Front");
_AVX afxString const g_str_Back = AFX_STRING("Back");
_AVX afxString const g_str_fail = AFX_STRING("fail");
_AVX afxString const g_str_pass = AFX_STRING("pass");
_AVX afxString const g_str_depthFail = AFX_STRING("depthFail");
_AVX afxString const g_str_compare = AFX_STRING("compare");
_AVX afxString const g_str_min = AFX_STRING("min");
_AVX afxString const g_str_max = AFX_STRING("max");

_AVX afxString const g_str_true = AFX_STRING("true");
_AVX afxString const g_str_false = AFX_STRING("false");

_AVX afxString const g_str_DrawOperation = AFX_STRING("DrawOperation");
_AVX afxString const g_str_Operation = AFX_STRING("Operation");
_AVX afxString const g_str_Technique = AFX_STRING("Technique");
_AVX afxString const g_str_Pipeline = AFX_STRING("Pipeline");
_AVX afxString const g_str_Pass = AFX_STRING("Pass");
_AVX afxString const g_str_Rasterization = AFX_STRING("Rasterization");
_AVX afxString const g_str_DepthHandling = AFX_STRING("DepthHandling");
_AVX afxString const g_str_Input = AFX_STRING("Input");
_AVX afxString const g_str_PrimitiveTopology = AFX_STRING("PrimitiveTopology");

afxChar const *shdResTypeNames[] =
{
    "INPUT", // vertex input
    "OUTPUT", // color output?
    "SAMPLER",
    "COMBINED_IMAGE_SAMPLER", // when an image is bound without a sampler
    "SAMPLED_IMAGE",
    "STORAGE_IMAGE",
    "UNIFORM_TEXEL_BUFFER",
    "STORAGE_TEXEL_BUFFER",
    "CONSTANT_BUFFER",
    "STORAGE_BUFFER",
    "INPUT_ATTACHMENT",
    "PUSH_CONSTANT_BUFFER",
};

afxChar const *fmtNames[] =
{
    "REAL",
    "NAT",
    "INT",
    "V2D",
    "V2DN",
    "V2DI",
    "V3D",
    "V3DN",
    "V3DI",
    "V4D",
    "V4DN",
    "V4DI"
};

_AVX afxString const vtxFmtString[afxVertexFormat_TOTAL] =
{
    AFX_STRING(""),
    AFX_STRING("float"),
    AFX_STRING("vec2"),
    AFX_STRING("vec3"),
    AFX_STRING("vec4"),
    AFX_STRING("mat2"),
    AFX_STRING("mat3"),
    AFX_STRING("mat4"),

    //AFX_STRING("V4B"),
    //AFX_STRING("v8B"),
};

_AVX afxString const shaderStageString[] =
{
    AFX_STRING(""),
    AFX_STRING("VERTEX"),
    AFX_STRING("FRAGMENT"),
    AFX_STRING("GEOMETRY"),
    AFX_STRING("TESS_CTRL"),
    AFX_STRING("TESS_EVAL"),
    AFX_STRING("COMPUTE"),
};

_AVX afxString const fillModeString[] =
{
    AFX_STRING("SOLID"),
    AFX_STRING("EDGE"),
    AFX_STRING("POINT")
};

_AVX afxString const frontFaceString[] =
{
    AFX_STRING("CCW"),
    AFX_STRING("CW")
};

_AVX afxString const cullModeString[] =
{
    AFX_STRING("NONE"),
    AFX_STRING("FRONT"),
    AFX_STRING("BACK"),
    AFX_STRING("BOTH")
};

_AVX afxString const compareOpString[] =
{
    AFX_STRING("NEVER"),
    AFX_STRING("LESS"),
    AFX_STRING("LEQUAL"),
    AFX_STRING("EQUAL"),
    AFX_STRING("NOTEQUAL"),
    AFX_STRING("GEQUAL"),
    AFX_STRING("GREATER"),
    AFX_STRING("ALWAYS")
};

_AVX afxString const stencilOpString[] =
{
    AFX_STRING("KEEP"),
    AFX_STRING("ZERO"),
    AFX_STRING("REPLACE"),
    AFX_STRING("INC_AND_CLAMP"),
    AFX_STRING("DEC_AND_CLAMP"),
    AFX_STRING("INVERT"),
    AFX_STRING("INC_AND_WRAP"),
    AFX_STRING("DEC_AND_WRAP")
};

_AVX afxString const logicOpString[] =
{
    AFX_STRING("CLEAR"),
    AFX_STRING("AND"),
    AFX_STRING("AND_REVERSE"),
    AFX_STRING("COPY"),
    AFX_STRING("AND_INVERTED"),
    AFX_STRING("NO_OP"),
    AFX_STRING("XOR"),
    AFX_STRING("OR"),
    AFX_STRING("NOR"),
    AFX_STRING("EQUIVALENT"),
    AFX_STRING("INVERT"),
    AFX_STRING("OR_REVERSE"),
    AFX_STRING("COPY_INVERTED"),
    AFX_STRING("OR_INVERTED"),
    AFX_STRING("NAND"),
    AFX_STRING("SET")
};

_AVX afxString const primTopologyString[] =
{
    AFX_STRING("POINT_LIST"),
    AFX_STRING("LINE_LIST"),
    AFX_STRING("LINE_STRIP"),
    AFX_STRING("TRI_LIST"),
    AFX_STRING("TRI_STRIP"),
    AFX_STRING("TRI_FAN"),
    AFX_STRING("LINE_LIST_ADJACENT"),
    AFX_STRING("LINE_STRIP_ADJACENT"),
    AFX_STRING("TRI_LIST_ADJACENT"),
    AFX_STRING("TRI_STRIP_ADJACENT"),
    AFX_STRING("PATCH_LIST")
};

afxUnit const vtxFmtSizes[] =
{
    0,

    sizeof(afxReal),
    sizeof(afxV2d),
    sizeof(afxV3d),
    sizeof(afxV4d),
    sizeof(afxM2d),
    sizeof(afxM3d),
    sizeof(afxM4d),

    sizeof(afxByte),
    sizeof(afxUnit8[2]),
    sizeof(afxUnit8[3]),
    sizeof(afxUnit8[4]),


};

afxChar const * const vtxFmtNames[] =
{
    0,
    "X32R",
    "XY32R",
    "XYZ32R",
    "XYZW32R",
    "XYZW32Q"
};

_AVX afxUnit AfxVertexFormatGetSize(afxVertexFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_RANGE(afxVertexFormat_TOTAL, fmt, 1);
    return vtxFmtSizes[fmt];
}

_AVX afxResult AfxResolveFormat(afxFormat fmt, afxFormat *subfmt, afxSize *subSiz, afxSize *vecSiz, afxSize *siz)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_RANGE(afxVertexFormat_TOTAL, fmt, 1);

    static struct { afxFormat subfmt; afxSize vecSiz; afxSize siz; afxSize subsiz; } list[] =
    {
        { afxFormat_REAL, 1, sizeof(afxReal), sizeof(afxReal) }, // afxFormat_REAL,
        { afxFormat_NAT, 1, sizeof(afxUnit), sizeof(afxUnit) }, // afxFormat_NAT,
        { afxFormat_INT, 1, sizeof(afxInt), sizeof(afxInt) }, // afxFormat_INT,
        { afxFormat_REAL, 2, sizeof(afxV2d), sizeof(afxReal) }, // afxFormat_V2D,
        { afxFormat_NAT, 2, sizeof(afxV2d), sizeof(afxUnit) }, // afxFormat_V2DN,
        { afxFormat_INT, 2, sizeof(afxV2d), sizeof(afxInt) }, // afxFormat_V2DI,
        { afxFormat_REAL, 3, sizeof(afxV3d), sizeof(afxReal) }, // afxFormat_V3D,
        { afxFormat_NAT, 3, sizeof(afxV3d), sizeof(afxUnit) }, // afxFormat_V3DN,
        { afxFormat_INT, 3, sizeof(afxV3d), sizeof(afxInt) }, // afxFormat_V3DI,
        { afxFormat_REAL, 4, sizeof(afxV4d), sizeof(afxReal) }, // afxFormat_V4D,
        { afxFormat_NAT, 4, sizeof(afxV4d), sizeof(afxUnit) }, // afxFormat_V4DN,
        { afxFormat_INT, 4, sizeof(afxV4d), sizeof(afxInt) }, // afxFormat_V4DI,
    };

    if (subfmt)
        *subfmt = list[fmt].subfmt;

    if (vecSiz)
        *vecSiz = list[fmt].vecSiz;

    if (siz)
        *siz = list[fmt].siz;

    if (subSiz)
        *subSiz = list[fmt].subsiz;

    return 0;
}

_AVX avxTopology AfxFindPrimitiveTopology(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxTopology_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &primTopologyString[i]))
            return (avxTopology)i;
    }

    return (avxTopology)-1;
}

_AVX avxLogicOp AfxFindLogicOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxLogicOp_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &logicOpString[i]))
            return (avxLogicOp)i;
    }

    return (avxLogicOp)-1;
}

_AVX avxStencilOp AfxFindStencilOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxStencilOp_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &stencilOpString[i]))
            return (avxStencilOp)i;
    }

    return (avxStencilOp)-1;
}

_AVX avxCompareOp AfxFindCompareOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxCompareOp_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &compareOpString[i]))
            return (avxCompareOp)i;
    }

    return (avxCompareOp)-1;
}

_AVX avxCullMode AfxFindCullMode(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxCullMode_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &cullModeString[i]))
            return (avxCullMode)i;
    }

    return (avxCullMode)-1;
}

_AVX avxFrontFace AfxFindFrontFace(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxFrontFace_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &frontFaceString[i]))
            return (avxFrontFace)i;
    }

    return (avxFrontFace)-1;
}

_AVX avxFillMode AfxFindFillMode(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxFillMode_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &fillModeString[i]))
            return (avxFillMode)i;
    }

    return (avxFillMode)-1;
}

_AVX avxShaderStage AfxFindShaderStage(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxShaderStage_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &shaderStageString[i]))
            return (avxShaderStage)i;
    }

    return (avxShaderStage)0;
}

_AVX afxVertexFormat AfxFindVertexFormat(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < afxVertexFormat_TOTAL; i++)
    {
        if (0 == AfxCompareStrings(str, TRUE, 1, &vtxFmtString[i]))
            return (afxVertexFormat)i;
    }

    return (afxVertexFormat)-1;
}

_AVX afxString const* AfxStringifyShaderStage(avxShaderStage ss, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ss < avxShaderStage_TOTAL);
    AfxCopyString(out, &shaderStageString[ss]);
    return out;
}

_AVX afxString const* AfxStringifyFillMode(avxFillMode fm, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(fm < avxFillMode_TOTAL);
    AfxCopyString(out, &fillModeString[fm]);
    return out;
}

_AVX afxString const* AfxStringifyFrontFace(avxFrontFace ff, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ff < avxFrontFace_TOTAL);
    AfxCopyString(out, &frontFaceString[ff]);
    return out;
}

_AVX afxString const* AfxStringifyCullMode(avxCullMode cm, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cm < avxCullMode_TOTAL);
    AfxCopyString(out, &cullModeString[cm]);
    return out;
}

_AVX afxString const* AfxStringifyCompareOp(avxCompareOp co, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(co < avxCompareOp_TOTAL);
    AfxCopyString(out, &compareOpString[co]);
    return out;
}

_AVX afxString const* AfxStringifyStencilOp(avxStencilOp so, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(so < avxStencilOp_TOTAL);
    AfxCopyString(out, &stencilOpString[so]);
    return out;
}

_AVX afxString const* AfxStringifyLogicOp(avxLogicOp lo, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lo < avxLogicOp_TOTAL);
    AfxCopyString(out, &logicOpString[lo]);
    return out;
}

_AVX afxString const* AfxStringifyPrimitiveTopology(avxTopology pt, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pt < avxTopology_TOTAL);
    AfxCopyString(out, &primTopologyString[pt]);
    return out;
}

_AVX afxString const* AfxStringifyVertexFormat(afxVertexFormat fmt, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(fmt < afxVertexFormat_TOTAL);
    AfxCopyString(out, &vtxFmtString[fmt]);
    return out;
}
