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

#ifndef AFX_NODE_H
#define AFX_NODE_H

#include "qwadro/core/afxObject.h"
#include "qwadro/core/afxString.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/math/afxSphere.h"
#include "awxSimDefs.h"
#include "qwadro/sim/awxPose.h"
#include "qwadro/sim/awxWorldPose.h"

// Frames define the spatial relationships between the objects to which they are attached.
// The newly created frame has no parent and is its own root.
// To be of any use the frame should be attached to an object; frames canv be added to cameras and lights.
// In addition, frames may be used in a plugin attached to geometry.

// Frames contain position and orientation information. They define object hierarchies.
// The frame contains two matrices: the local transformation matrix (LTM) and the modeling matrix.
// The modeling matrix describes how the frame (and hence the object attached to it) is transformed relative to its local origin while the LTM describes the absolute transformation from local to world coordinates.
// If the frame is not part of a hierarchy, or forms the hierarchy's root, the modeling and LT matrices are identical. Otherwise, the modeling matrix is taken to be relative to the frame's parent and the LTM becomes the concatenation of all the modeling matrices up to the root frame. This ensures that any change in the parent's position or orientation will transfer through to the child frames.

// Handling with awxNodes, the programmer shouldn't care about matrix pre/post-multiplication. RenderWare chose don't do it.

// awxNode defines the base class for all scene objects in QWADRO AFX.



// 3D positions and transformations exist within coordinate systems called spaces.
// World space is the coordinate system for the entire scene.Its origin is at the center of the scene.The grid you see in view windows shows the world space axes.
// Object space is the coordinate system from an object's point of view.The origin of object space is at the object's pivot point, and its axes are rotated with the object.
// Local space is similar to object space, however it uses the origin and axes of the object's parent node in the hierarchy of objects.This is useful when you haven't transformed the object itself, but it is part of a group that is transformed.

enum blend_dag_node_type
{
    DagNodeType_Leaf_AnimationBlend = 0x0,
    DagNodeType_Leaf_LocalPose = 0x1,
    DagNodeType_Leaf_Callback = 0x2,
    DagNodeType_OnePastLastLeafType = 0x3,
    DagNodeType_Node_Crossfade = 0x4,
    DagNodeType_Node_WeightedBlend = 0x5,
    DagNodeType_OnePastLast = 0x6,
};

enum quaternion_mode
{
    BlendQuaternionDirectly = 0x0,
    BlendQuaternionInverted = 0x1,
    BlendQuaternionNeighborhooded = 0x2,
    BlendQuaternionAccumNeighborhooded = 0x3,
};

AFX_DEFINE_STRUCT(afxDagAnimation)
{
    awxBody bod;
    afxReal fillThreshold;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagPose)
{
    awxPose *lp;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagVmt)
{
    awxPose*(*sample)(void*, afxInt, afxReal, afxInt const*);
    void(*setClock)(void*, afxReal);
    void(*motionVectors)(void*, afxReal, afxReal*, afxReal*, afxBool);
    void *udd;
};

AFX_DEFINE_STRUCT(afxDagCrossfade)
{
    afxReal weightNone;
    afxReal weightAll;
    void/*track_mask*/ *trackMask;
    afxBool owned;
};

AFX_DEFINE_STRUCT(afxDagWeighted)
{
    awxSkeleton skl;
    enum quaternion_mode QuatMode;
    afxReal fillThreshold;
};

AFX_DEFINE_STRUCT(afxDagNode)
{
    afxDagNode *parent;
    afxNat childCnt;
    afxDagNode **children;
    afxReal Weight;
    void/*track_mask*/ *trackMask;
    afxBool ownedTrackMask;
    enum blend_dag_node_type Type;
    union
    {
        afxDagAnimation AnimationBlend;
        afxDagPose LocalPose;
        afxDagVmt Callback;
        afxDagCrossfade Crossfade;
        afxDagWeighted WeightedBlend;
    };
};


#endif//AFX_NODE_H