#include <string.h>

#include "xssUtils.h"
#include "xssVm.h"

DEFINE_BUFFER(Byte, uint8_t);
DEFINE_BUFFER(Int, int);
DEFINE_BUFFER(String, ObjString*);

void xssSymbolTableInit(SymbolTable* symbols)
{
  xssStringBufferInit(symbols);
}

void xssSymbolTableClear(xssVm env, SymbolTable* symbols)
{
  xssStringBufferClear(env, symbols);
}

int xssSymbolTableAdd(xssVm env, SymbolTable* symbols,
                       const char* name, size_t length)
{
  ObjString* symbol = AS_STRING(xssNewStringLength(env, name, length));
  
  xssPushRoot(env, &symbol->obj);
  xssStringBufferWrite(env, symbols, symbol);
  xssPopRoot(env);
  
  return symbols->count - 1;
}

int xssSymbolTableEnsure(xssVm env, SymbolTable* symbols,
                          const char* name, size_t length)
{
  // See if the symbol is already defined.
  int existing = xssSymbolTableFind(symbols, name, length);
  if (existing != -1) return existing;

  // New symbol, so add it.
  return xssSymbolTableAdd(env, symbols, name, length);
}

int xssSymbolTableFind(const SymbolTable* symbols,
                        const char* name, size_t length)
{
  // See if the symbol is already defined.
  // TODO: O(n). Do something better.
  for (int i = 0; i < symbols->count; i++)
  {
    if (xssStringEqualsCString(symbols->data[i], name, length)) return i;
  }

  return -1;
}

void xssBlackenSymbolTable(xssVm env, SymbolTable* symbolTable)
{
  for (int i = 0; i < symbolTable->count; i++)
  {
    xssGrayObj(env, &symbolTable->data[i]->obj);
  }
  
  // Keep track of how much memory is still in use.
  env->bytesAllocated += symbolTable->capacity * sizeof(*symbolTable->data);
}

int xssUtf8EncodeNumBytes(int value)
{
  ASSERT(value >= 0, "Cannot encode a negative value.");
  
  if (value <= 0x7f) return 1;
  if (value <= 0x7ff) return 2;
  if (value <= 0xffff) return 3;
  if (value <= 0x10ffff) return 4;
  return 0;
}

int xssUtf8Encode(int value, uint8_t* bytes)
{
  if (value <= 0x7f)
  {
    // Single byte (i.e. fits in ASCII).
    *bytes = value & 0x7f;
    return 1;
  }
  else if (value <= 0x7ff)
  {
    // Two byte sequence: 110xxxxx 10xxxxxx.
    *bytes = 0xc0 | ((value & 0x7c0) >> 6);
    bytes++;
    *bytes = 0x80 | (value & 0x3f);
    return 2;
  }
  else if (value <= 0xffff)
  {
    // Three byte sequence: 1110xxxx 10xxxxxx 10xxxxxx.
    *bytes = 0xe0 | ((value & 0xf000) >> 12);
    bytes++;
    *bytes = 0x80 | ((value & 0xfc0) >> 6);
    bytes++;
    *bytes = 0x80 | (value & 0x3f);
    return 3;
  }
  else if (value <= 0x10ffff)
  {
    // Four byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx.
    *bytes = 0xf0 | ((value & 0x1c0000) >> 18);
    bytes++;
    *bytes = 0x80 | ((value & 0x3f000) >> 12);
    bytes++;
    *bytes = 0x80 | ((value & 0xfc0) >> 6);
    bytes++;
    *bytes = 0x80 | (value & 0x3f);
    return 4;
  }

  // Invalid Unicode value. See: http://tools.ietf.org/html/rfc3629
  UNREACHABLE();
  return 0;
}

int xssUtf8Decode(const uint8_t* bytes, uint32_t length)
{
  // Single byte (i.e. fits in ASCII).
  if (*bytes <= 0x7f) return *bytes;

  int value;
  uint32_t remainingBytes;
  if ((*bytes & 0xe0) == 0xc0)
  {
    // Two byte sequence: 110xxxxx 10xxxxxx.
    value = *bytes & 0x1f;
    remainingBytes = 1;
  }
  else if ((*bytes & 0xf0) == 0xe0)
  {
    // Three byte sequence: 1110xxxx	 10xxxxxx 10xxxxxx.
    value = *bytes & 0x0f;
    remainingBytes = 2;
  }
  else if ((*bytes & 0xf8) == 0xf0)
  {
    // Four byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx.
    value = *bytes & 0x07;
    remainingBytes = 3;
  }
  else
  {
    // Invalid UTF-8 sequence.
    return -1;
  }

  // Don't read past the end of the buffer on truncated UTF-8.
  if (remainingBytes > length - 1) return -1;

  while (remainingBytes > 0)
  {
    bytes++;
    remainingBytes--;

    // Remaining bytes must be of form 10xxxxxx.
    if ((*bytes & 0xc0) != 0x80) return -1;

    value = value << 6 | (*bytes & 0x3f);
  }

  return value;
}

int xssUtf8DecodeNumBytes(uint8_t byte)
{
  // If the byte starts with 10xxxxx, it's the middle of a UTF-8 sequence, so
  // don't count it at all.
  if ((byte & 0xc0) == 0x80) return 0;
  
  // The first byte's high bits tell us how many bytes are in the UTF-8
  // sequence.
  if ((byte & 0xf8) == 0xf0) return 4;
  if ((byte & 0xf0) == 0xe0) return 3;
  if ((byte & 0xe0) == 0xc0) return 2;
  return 1;
}

// From: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2Float
int xssPowerOf2Ceil(int n)
{
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  
  return n;
}

uint32_t xssValidateIndex(uint32_t count, int64_t value)
{
  // Negative indices count from the end.
  if (value < 0) value = count + value;

  // Check bounds.
  if (value >= 0 && value < count) return (uint32_t)value;

  return UINT32_MAX;
}
