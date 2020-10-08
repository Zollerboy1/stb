//
//  stb_file.h
//  stb
//
//  Created by Josef Zoller on 08.10.20.
//

#ifndef stb_file_h
#define stb_file_h

#ifndef STB_FILE_NO_STDIO
#include <stdio.h>
#endif

#include <stdlib.h>
typedef char stb_c;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STBFDEF
#ifdef STBF_STATIC
#define STBFDEF static
#else
#define STBFDEF extern
#endif
#endif

#ifndef STBF_NO_STDIO
STBFDEF stb_c *stb_load(char const *filename);
STBFDEF void stb_free(void *retval_from_stb_load);
#endif


#ifdef __cplusplus
}
#endif


#ifdef STB_FILE_IMPLEMENTATION


#ifdef __cplusplus
#define STBF_EXTERN extern "C"
#else
#define STBF_EXTERN extern
#endif


#ifdef _MSC_VER
typedef   signed int   stb__int32;
#else
#include <stdint.h>
typedef int32_t  stb__int32;
#endif

#if defined(STBF_MALLOC) && defined(STBF_FREE) && (defined(STBF_REALLOC) || defined(STBF_REALLOC_SIZED))
// ok
#elif !defined(STBF_MALLOC) && !defined(STBF_FREE) && !defined(STBF_REALLOC) && !defined(STBF_REALLOC_SIZED)
// ok
#else
#error "Must define all or none of STBF_MALLOC, STBF_FREE, and STBF_REALLOC (or STBF_REALLOC_SIZED)."
#endif

#ifndef STBF_MALLOC
#define STBF_MALLOC(sz)           malloc(sz)
#define STBF_REALLOC(p,newsz)     realloc(p,newsz)
#define STBF_FREE(p)              free(p)
#endif

#ifndef STBF_REALLOC_SIZED
#define STBF_REALLOC_SIZED(p,oldsz,newsz) STBF_REALLOC(p,newsz)
#endif


STBFDEF void stb_free(void *retval_from_stbi_load)
{
   STBF_FREE(retval_from_stbi_load);
}


#ifndef STBF_NO_STDIO

#if defined(_MSC_VER) && defined(STBF_WINDOWS_UTF8)
STB_EXTERN __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char *str, int cbmb, wchar_t *widestr, int cchwide);
STB_EXTERN __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t *widestr, int cchwide, char *str, int cbmb, const char *defchar, int *used_default);
#endif

static FILE *stb__fopen(char const *filename, char const *mode)
{
   FILE *f;
#if defined(_MSC_VER) && defined(STBF_WINDOWS_UTF8)
   wchar_t wMode[64];
   wchar_t wFilename[1024];
    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, filename, -1, wFilename, sizeof(wFilename)))
      return 0;

    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, mode, -1, wMode, sizeof(wMode)))
      return 0;

#if _MSC_VER >= 1400
    if (0 != _wfopen_s(&f, wFilename, wMode))
        f = 0;
#else
   f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
   if (0 != fopen_s(&f, filename, mode))
      f=0;
#else
   f = fopen(filename, mode);
#endif
   return f;
}

STBFDEF stb_c *stb_load(char const *filename)
{
    stb_c * buffer;
    stb__int32 length;
    FILE *f = stb__fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = (stb_c *)STBF_MALLOC(length + 1);
        fread(buffer, 1, length, f);
        buffer[length] = 0;

        fclose(f);
    }

    return buffer;
}

#endif //!STBF_NO_STDIO

#endif // STB_FILE_IMPLEMENTATION


#endif /* stb_file_h */
