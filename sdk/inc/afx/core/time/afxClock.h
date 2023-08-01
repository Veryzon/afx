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

#ifndef AFX_CLOCK_H
#define AFX_CLOCK_H

#include "afx/core/afxCoreDefs.h"

AFX_DEFINE_STRUCT(afxSystemClock)
{
    afxNat32 data[4];
};

AFX void        AfxGetSystemSeconds(afxSystemClock *clock);
AFX afxReal64   AfxGetSecondsElapsed(afxSystemClock const *start, afxSystemClock const *end);

/// Qwadro acessa ao rel�gio em tempo real do sistema para fins cronom�tricos internos. 
/// Conforme esperado que voc� a desejar solicitar esse rel�gio, Qwadro exp�e as fun��es que ele usa. 
/// Aqui est� um exemplo de como eles podem ser usados para cronometrar as itera��es de um loop.

/*
    afxSystemClock lastClock, currentClock;

    AfxGetSystemSeconds(&lastClock);

    while(1)
    {
        // Find out how many seconds have elapsed for the previous iteration of the loop
        AfxGetSystemSeconds(&currentClock);
        afxReal SecondsElapsed = AfxGetSecondsElapsed(&lastClock, &currentClock);
        lastClock = currentClock;

        // Do processing with SecondsElapsed here
    }
*/

#endif//AFX_CLOCK_H