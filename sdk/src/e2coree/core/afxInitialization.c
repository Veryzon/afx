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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qwadro/core/afxInitialization.h"
#include "qwadro/core/afxFixedString.h"

AFX_DEFINE_STRUCT(_afxIniRecord)
{
    afxFixedString32 key;
    afxFixedString2048 value;
};

AFX_DEFINE_STRUCT(_afxIniPage)
{
    afxFixedString32 name;
    afxNat keyCnt;
    _afxIniRecord* keys;
};

_AFXINL _afxIniRecord* _AfxIniCreateRecord(_afxIniPage* page, afxString const* key, afxString const* value)
{
    afxError err;

    if ((!(page->keyCnt % 8)) && !(page->keys = AfxReallocate(NIL, page->keys, sizeof(_afxIniRecord), (8 + page->keyCnt), NIL, AfxHint()))) AfxThrowError();
    else
    {
        _afxIniRecord* entry = &page->keys[page->keyCnt++];
        AfxMakeFixedString32(&entry->key, key);
        AfxMakeFixedString2048(&entry->value, value);
        return entry;
    }
    return NIL;
}

_AFXINL _afxIniPage* _AfxIniCreatePage(afxIni* ini, afxString const* block)
{
    afxError err;

    if ((!(ini->pageCnt % 8)) && !(ini->pages = AfxReallocate(NIL, ini->pages, sizeof(_afxIniPage), (8 + ini->pageCnt), NIL, AfxHint()))) AfxThrowError();
    else
    {
        _afxIniPage* page = &ini->pages[ini->pageCnt++];
        page->keyCnt = 0;
        AfxMakeFixedString32(&page->name, block);

        if (!(page->keys = AfxAllocate(NIL, 8, sizeof(_afxIniRecord), 0, AfxHint())))
            AfxThrowError();

        return page;
    }
    return NIL;
}

_AFXINL _afxIniPage* _AfxIniFindPage(afxIni const* ini, afxString const* name)
{
    if (name)
    {
        for (afxNat i = 0; i < ini->pageCnt; i++)
            if (0 == AfxCompareString(&ini->pages[i].name.str.str, name))
                return &ini->pages[i];
    }
    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniFindRecord(_afxIniPage const* page, afxString const* key)
{
    if (page && key)
    {
        for (afxNat i = 0; i < page->keyCnt; i++)
            if (0 == AfxCompareString(&page->keys[i].key.str.str, key))
                return &page->keys[i];
    }
    return NIL;
}

_AFXINL _afxIniRecord* _AfxIniGetRecord(afxIni const* ini, afxString const* page, afxString const* key)
{
    return _AfxIniFindRecord(_AfxIniFindPage(ini, page), key);
}

_AFX afxError AfxSetUpIni(afxIni* ini)
{
    afxError err = NIL;
    ini->pageCnt = 0;
    
    if (!(ini->pages = AfxAllocate(NIL, 8, sizeof(_afxIniPage), 0, AfxHint())))
        AfxThrowError();

    return err;
}

_AFX void AfxCleanUpIni(afxIni* ini)
{
    for (afxNat i = 0; i < ini->pageCnt; i++)
    {
        _afxIniPage* page = &ini->pages[i];
        AfxDeallocate(NIL, page->keys);
    }
    AfxDeallocate(NIL, ini->pages);
}

_AFX afxError AfxIniLoadFromFile(afxIni* ini, afxUri const* uri)
{
    afxError err = NIL;
    FILE* f;

    if (!(f = fopen(AfxGetUriData(uri, 0), "r"))) AfxThrowError();
    else
    {
        enum { Section, Key, Value, Comment } state = Section;
        int c;
        afxNat posn = 0;
        afxNat spaces = 0;
        afxNat line = 0;
        afxNat bufSiz = 128 * sizeof(afxChar);
        afxChar* buf = malloc(bufSiz);
        afxChar* value = NIL;

        _afxIniPage* currPag = NIL;

        memset(buf, '\0', bufSiz);

        while ((c = getc(f)) != EOF)
        {
            if (posn > bufSiz - 2)
            {
                bufSiz += 128 * sizeof(char);
                size_t value_offset = value == NIL ? 0 : value - buf;
                buf = realloc(buf, bufSiz);
                memset(buf + posn, '\0', bufSiz - posn);
                if (value != NIL)
                    value = buf + value_offset;
            }

            switch (c)
            {
            case ' ':
                switch (state)
                {
                case Value: if (value[0] != '\0') spaces++; break;
                default: if (buf[0] != '\0') spaces++; break;
                }
                break;
            case ';':
                if (state == Value)
                {
                    buf[posn++] = c;
                    break;
                }
                else
                {
                    state = Comment;
                    buf[posn++] = c;

                    while (c != EOF && c != '\n')
                    {
                        c = getc(f);
                        if (c != EOF && c != '\n') buf[posn++] = c;
                    }
                }
            case '\n':
            case EOF:
                line++;
                if (state == Value)
                {
                    if (!currPag)
                        currPag = _AfxIniCreatePage(ini, &AfxString(""));

                    afxString bufStr, valueStr;
                    AfxMakeString(&bufStr, buf, 0);
                    AfxMakeString(&valueStr, value, 0);

                    _AfxIniCreateRecord(currPag, &bufStr, &valueStr);
                    value = NIL;
                }
                else if (state == Comment)
                {
                    if (!currPag)
                        currPag = _AfxIniCreatePage(ini, &AfxString(""));

                    afxString bufStr;
                    AfxMakeString(&bufStr, buf, 0);

                    _AfxIniCreateRecord(currPag, &bufStr, &AfxString(""));
                }
                else if (state == Section)
                {
                    AfxError("%u: Section '%s' missing ']' operator.", line, buf);
                }
                else if (state == Key && posn)
                {
                    AfxError("%u: Key '%s' missing '=' operator.", line, buf);
                }
                memset(buf, '\0', bufSiz);
                state = Key;
                posn = 0;
                spaces = 0;
                break;
            case '[':
                state = Section;
                break;
            case ']':
            {
                afxString bufStr;
                AfxMakeString(&bufStr, buf, 0);

                currPag = _AfxIniCreatePage(ini, &bufStr);
                memset(buf, '\0', bufSiz);
                posn = 0;
                spaces = 0;
                state = Key;
                break;
            }
            case '=':
                if (state == Key)
                {
                    state = Value;
                    buf[posn++] = '\0';
                    value = buf + posn;
                    spaces = 0;
                    continue;
                }
            default:
                for (; spaces > 0; spaces--) buf[posn++] = ' ';
                buf[posn++] = c;
                break;
            }
        }
        free(buf);
        fclose(f);
    }
    return err;
}

_AFX afxError AfxIniSaveToFile(afxIni const* ini, afxUri const* uri)
{
    afxError err = NIL;
    FILE* f;
    
    if (!(f = fopen(AfxGetUriData(uri, 0), "w+"))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < ini->pageCnt; i++)
        {
            _afxIniPage* pag = &ini->pages[i];
            fprintf(f, i > 0 ? "\n[%.*s]\n" : "[%.*s]\n", AfxPushString(&pag->name.str.str));

            for (afxNat q = 0; q < pag->keyCnt; q++)
            {
                _afxIniRecord* record = &pag->keys[q];

                if (record->key.buf[0] == ';')
                {
                    fprintf(f, "%.*s\n", AfxPushString(&record->key.str.str));
                }
                else
                {
                    fprintf(f, "%.*s = %.*s\n", AfxPushString(&record->key.str.str), AfxPushString(&record->value.str.str));
                }
            }
        }
        fclose(f);
    }
    return err;
}

_AFX afxBool AfxIniHasKey(afxIni const* ini, afxString const* page, afxString const* key)
{
    return (!!_AfxIniGetRecord(ini, page, key));
}

_AFX afxError AfxIniSetString(afxIni* ini, afxString const* page, afxString const* key, afxString* value)
{
    afxError err = NIL;
    _afxIniPage* pag;

    if ((!(pag = _AfxIniFindPage(ini, page))) && (!(pag = _AfxIniCreatePage(ini, page)))) AfxThrowError();
    else
    {
        _afxIniRecord* record;

        if ((!(record = _AfxIniFindRecord(pag, key))) && (!(record = _AfxIniCreateRecord(pag, key, value))))
            AfxThrowError();
        else
            AfxMakeFixedString2048(&record->value, value);
    }
    return err;
}

_AFX afxBool AfxIniGetString(afxIni const* ini, afxString const* page, afxString const* key, afxString* value)
{
    afxBool rslt;
    _afxIniRecord* entry;

    if ((rslt = ((rslt = !!(entry = _AfxIniGetRecord(ini, page, key))) && (!(AfxStringIsEmpty(&entry->value.str.str))))))
        AfxReflectString(&entry->value.str.str, value);

    return rslt;
}

afxBool AfxIniGetNat(afxIni const* ini, afxString const* page, afxString const* key, afxNat* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxNat)atoi(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetInt(afxIni const* ini, afxString const* page, afxString const* key, afxInt* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = atoi(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetReal(afxIni const* ini, afxString const* page, afxString const* key, afxReal* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetReal64(afxIni const* ini, afxString const* page, afxString const* key, afxReal64* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))))
        *value = (afxReal64)atof(val.start);

    return rslt;
}

_AFX afxBool AfxIniGetBool(afxIni const* ini, afxString const* page, afxString const* key, afxBool* value)
{
    afxBool rslt;
    afxString val;
    
    if ((rslt = ((rslt = AfxIniGetString(ini, page, key, &val)) && (!(AfxStringIsEmpty(&val))))) && ((0 == AfxCompareStringCil(&val, 0, "false", 0)) || (0 == AfxCompareStringCil(&val, 0, "0", 0))))
        *value = FALSE;

    return rslt;
}