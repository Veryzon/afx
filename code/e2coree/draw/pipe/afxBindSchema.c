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

#define _AFX_DRAW_C
#define _AFX_BIND_SCHEMA_C
#include "qwadro/draw/afxDrawContext.h"

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxLegoBlueprintBegin(afxPipelineRigBlueprint *blueprint, afxNat estBindCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    blueprint->dctx = NIL;
    //afxMmu mmu = AfxGetDrawContextMmu(blueprint->dctx);
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAllocateArray(&blueprint->bindings, AfxMax(estBindCnt, 10), sizeof(afxPipelineRigBlueprintBinding), NIL);
}

_AFXINL void AfxLegoBlueprintErase(afxPipelineRigBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        afxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->bindings);
}

_AFXINL afxError AfxLegoBlueprintEnd(afxPipelineRigBlueprint *blueprint, afxBindSchema *lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    if (lego)
    {
        if (AfxCountArrayElements(&blueprint->bindings))
        {
            if (AfxBuildBindSchemas(blueprint->dctx, 1, lego, blueprint))
            {
                AfxThrowError();
            }
        }
        else
        {
            *lego = NIL;
        }
    }

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        afxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxDeallocateArray(&blueprint->bindings);

    blueprint->fcc = NIL;
    return err;
}

_AFXINL afxError AfxLegoBlueprintAddBinding(afxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    afxNat idx;
    afxPipelineRigBlueprintBinding *decl;

    if (!(decl = AfxInsertArrayUnit(&blueprint->bindings, &idx))) AfxThrowError();
    else
    {
        AfxAssert(point < 10); // hardcoded limitation
        decl->binding = point;
        AfxAssert(visibility);
        decl->visibility = visibility;
        AfxAssert(type < AFX_SHD_RES_TYPE_TOTAL);
        decl->type = type;
        AfxAssert(decl->type != AFX_SHD_RES_TYPE_OUTPUT);

        AfxAssert(cnt);
        decl->cnt = AfxMax(cnt, 1);
         AfxCloneString(&decl->name, name);
    }
    return err;
}

_AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, afxShaderBlueprint shdb[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(cnt);
    AfxAssert(shdb);
    AfxAssert(set < 4);

    for (afxNat i = 0; i < cnt; i++)
    {
        //AfxAssertObjects(1, &shdb[i], afxFcc_SHD);
        afxNat resCnt = shdb->resources.cnt;
        afxBool incompatible = FALSE;

        for (afxNat k = 0; k < resCnt; k++)
        {
            afxShaderBlueprintResource rsrc = *(afxShaderBlueprintResource*)AfxGetArrayUnit(&shdb->resources, k);
            //AfxDescribeShaderInterfaces(shd[i], k, 1, &rsrc);

            if (rsrc.set == set)
            {
                afxBool entryExisting = FALSE;

                afxArray *bindings = &blueprint->bindings;

                for (afxNat j = 0; j < AfxCountArrayElements(bindings); j++)
                {
                    afxPipelineRigBlueprintBinding *binding = AfxGetArrayUnit(bindings, j);

                    if ((entryExisting |= (rsrc.binding == binding->binding)))
                    {
                        if ((incompatible |= ((rsrc.type != binding->type) || (rsrc.cnt != binding->cnt)))) AfxThrowError();
                        else
                        {
                            binding->visibility |= /*entry->visibility |*/ AfxGetBitOffset(shdb->stage);
                        }
                        break;
                    }
                }

                if (!err && !entryExisting)
                {
                    if (AfxLegoBlueprintAddBinding(blueprint, rsrc.binding, AfxGetBitOffset(shdb->stage), rsrc.type, rsrc.cnt, &rsrc.name.str))
                    {
                        AfxThrowError();
                    }
                }

                if (incompatible)
                    break;
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// LEGO                                                                       //
////////////////////////////////////////////////////////////////////////////////

_AFX afxResult AfxGetBindSchemaEntry(afxBindSchema lego, afxNat first, afxNat cnt, afxPipelineRigBindingDecl decl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);
    AfxAssert(cnt);
    AfxAssertRange(lego->entryCnt, first, cnt);
    AfxAssert(decl);
    afxResult rslt = 0;

    for (afxNat i = 0; i < AfxMin(lego->entryCnt, cnt); i++)
    {
        if (i >= cnt) break;
        else
        {
            decl[i].binding = lego->entries[i].binding;
            decl[i].visibility = lego->entries[i].visibility;
            decl[i].type = lego->entries[i].type;
            decl[i].cnt = lego->entries[i].cnt;
            decl[i].name = &lego->entries[i].name.str;
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxNat32 AfxGetBindSchemaHash(afxBindSchema lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);
    return lego->crc32;
}

_AFX afxError AfxBuildBindSchemas(afxDrawContext dctx, afxNat cnt, afxBindSchema lego[], afxPipelineRigBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetBindSchemaClass(dctx);
    AfxAssertClass(cls, afxFcc_LEGO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)lego, (void const*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}