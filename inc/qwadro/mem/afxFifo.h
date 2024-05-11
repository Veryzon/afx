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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

/// afxFifo is a lock-less, thread-safe, multi-producer-multi-consumer queue data structure. It is designed to hold copyable values.

#ifndef AFX_FIFO_H
#define AFX_FIFO_H

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxAtomic.h"

AFX_DEFINE_STRUCT(afxFifo)
{
    struct
    {
        afxAtom64   seqIdx; /// Global index of the queue entry in the sequence, to detect when we are at a valid read or write pos
        afxByte     value[]; /// Value actually contained in the queue
    }*              entries; // Buffer of entries.    
    afxNat          unitSiz;
    afxInt64        queIdxMask; /// Mask to apply to the read/write position to clamp it to array bounds
    afxAtom64       readPosn; /// readIndex of where we are in the sequence
    afxAtom64       writePosn; /// writeIndex of where we are in the sequence
};

/// Inicializa a fila afxFifo e aloca mem�ria para o n�mero especificado de entradas.
/// O n�mero de entradas n�o � mut�vel ap�s inicializa��o.
AFX afxError    AfxSetUpFifo(afxFifo* fifo, afxNat unitSiz, afxNat cap);

/// Libera a mem�ria reservado para a fila e redefine o estado interno.
/// A fila deve estar vazia quando esta fun��o for chamada.
AFX afxError    AfxCleanUpFifo(afxFifo* fifo);

/// Checa se h� um valor dispon�vel para ser pipocado
AFX afxBool     AfxFifoIsEmpty(afxFifo* fifo);

/// Enfileira o valor provido. O valor ser� copiado para o buffer interno da fila.
/// Retorna TRUE se o enfileiramente foi desempenhado sucesivamente.
/// Retorna FALSE se o enfileiramento n�o pode ser realizado.
/// Isto pode acontecer se a fila estiver cheia, e algumas opera��es de desenfileiramento h� de ocorrer.
AFX afxBool     AfxEnqueueFifo(afxFifo* fifo, void* src);

/// Desenfilera um valor da file especificada, copiando-o para para a vari�vel argumentada.
/// Retorna TRUE se um valor foi sucessivamente desenfileirado, FALSE caso contr�rio (se FALSE, data n�o ser� escrito.
AFX afxBool     AfxDequeueFifo(afxFifo* fifo, void* value);

#endif//AFX_FIFO_H
