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

#ifndef AFX_MATERIAL_H
#define AFX_MATERIAL_H

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/draw/io/afxRaster.h"

// Graphics hardware is still undergoing major changes every year, and as such, there is still no standard way to represent material data for real-time rendering.
// Qwadro tries to avoid restricting your material rendering by keeping the standardized structure of materials to a minimum, and packing basically every material parameter provided by each art tool into the ExtendedData section of the awxMaterial structure.

// All professional art tools construct materials as "shader graphs", where materials are connected to each other to form a tree of shaders.
// Qwadro reproduces this data in its material data.
// Each material can have an arbitrary number of sub-materials that it uses as maps, which you can iterate over.
// Additionally, each material may be textured, and will have a reference to a afxRaster that provides the image data for the material.

AFX_DEFINE_STRUCT(awxMaterialMap)
{
    afxBufferedString   usage; // usage 16
    awxMaterial         sub; // awxMaterial
};

#ifdef _AFX_MATERIAL_C
AFX_OBJECT(awxMaterial)
{
    afxBufferedString   id; // 128
    afxRaster       tex;
    afxSampler      smp;
    afxNat          mapCnt;
    awxMaterialMap* maps;
    void            *extData;

    // non-Granny stuff
    afxInt          s[3];
    afxV4d          color; // albedo
    afxReal         shininess;
    
    afxV3d          Ka; // ambient color. The default is (0.2,0.2,0.2);
    afxV3d          Kd; // diffuse color. The default is (0.8,0.8,0.8);
    afxReal         Tr; // transparency. The default is 0.0 (not transparent at all)
    afxV3d          Ks; // specular color. This color shows up in highlights. The default is (1.0,1.0,1.0);
    afxReal         Ns; // specular exponent (aka shininess). The default is 0.0. A high exponent results in a tight, concentrated highlight.
    afxV3d          Ke; // 
    afxV3d          Tf; // Any light passing through the object is filtered by the transmission filter, which only allows the specifiec colors to pass through. For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue.
    afxReal         Ni; // optical density for the surface.  This is also known as index of refraction. The values can range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
};
#endif//_AFX_MATERIAL_C

//static_assert(offsetof(awxMaterial, color) % AFX_MTL_ALIGN == 0, "");

AFX afxString const*AfxGetMaterialId(awxMaterial mtl);
AFX afxNat          AfxCountMaterialMaps(awxMaterial mtl);

AFX awxMaterial     AfxFindSubmaterial(awxMaterial mtl, afxString const *usage);
AFX awxMaterial     AfxGetSubmaterial(awxMaterial mtl, afxNat mapIdx);
AFX void            AfxRebindSubmaterial(awxMaterial mtl, afxNat mapIdx, awxMaterial subMtl);

AFX void            AfxResetMaterialMap(awxMaterial mtl, afxNat mapIdx, afxString const* usage, awxMaterial subMtl);

AFX afxRaster       AfxGetMaterialTexture(awxMaterial mtl);
AFX void            AfxRebindMaterialTexture(awxMaterial mtl, afxRaster tex);
AFX void            AfxReloadMaterialTexture(awxMaterial mtl, afxUri const *tex);

// non-Granny
AFX void            AfxColorizeMaterial(awxMaterial mtl, afxV4d const color);
AFX void            AfxShineMaterial(awxMaterial mtl, afxReal shininess);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireMaterial(awxSimulation sim, afxString const* id, afxRaster tex, afxNat subCnt, awxMaterial *mtl);

#endif//AFX_MATERIAL_H