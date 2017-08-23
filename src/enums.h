/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Stefano Trettel
 *
 * Software repository: MoonFreeType, https://github.com/stetre/moonfreetype
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef enumsDEFINED
#define enumsDEFINED

/* enums.c */
#define enums_free_all moonfreetype_enums_free_all
void enums_free_all(lua_State *L);
#define enums_test moonfreetype_enums_test
uint32_t enums_test(lua_State *L, uint32_t domain, int arg, int *err);
#define enums_check moonfreetype_enums_check
uint32_t enums_check(lua_State *L, uint32_t domain, int arg);
#define enums_push moonfreetype_enums_push
int enums_push(lua_State *L, uint32_t domain, uint32_t code);
#define enums_values moonfreetype_enums_values
int enums_values(lua_State *L, uint32_t domain);
#define enums_checklist moonfreetype_enums_checklist
uint32_t* enums_checklist(lua_State *L, uint32_t domain, int arg, uint32_t *count, int *err);
#define enums_freelist moonfreetype_enums_freelist
void enums_freelist(lua_State *L, uint32_t *list);


/* Enum domains */
#define DOMAIN_RESULT                       0
#define DOMAIN_FT_ORIENTATION               1
#define DOMAIN_FT_STROKER_BORDER            2
#define DOMAIN_FT_STROKER_LINEJOIN          3
#define DOMAIN_FT_STROKER_LINECAP           4
#define DOMAIN_FT_GLYPH_BBOX                5
#define DOMAIN_FT_TRUETYPE_ENGINE_TYPE      6
#define DOMAIN_FT_SIZE_REQUEST_TYPE         7
#define DOMAIN_FT_KERNING                   8
#define DOMAIN_FT_RENDER_MODE               9
#define DOMAIN_FT_ENCODING                  10
#define DOMAIN_FT_GLYPH_FORMAT              11
#define DOMAIN_FT_PIXEL_MODE                12
/* NONFT additions */
#define DOMAIN_NONFT_TYPE                  101

/* Types for al.sizeof() & friends */
#define NONFT_TYPE_CHAR         1
#define NONFT_TYPE_UCHAR        2
#define NONFT_TYPE_BYTE         3
#define NONFT_TYPE_UBYTE        4
#define NONFT_TYPE_SHORT        5
#define NONFT_TYPE_USHORT       6
#define NONFT_TYPE_INT          7
#define NONFT_TYPE_UINT         8
#define NONFT_TYPE_LONG         9
#define NONFT_TYPE_ULONG        10
#define NONFT_TYPE_FLOAT        11
#define NONFT_TYPE_DOUBLE       12


#define testtype(L, arg, err) (ALenum)enums_test((L), DOMAIN_NONFT_TYPE, (arg), (err))
#define checktype(L, arg) (ALenum)enums_check((L), DOMAIN_NONFT_TYPE, (arg))
#define pushtype(L, val) enums_push((L), DOMAIN_NONFT_TYPE, (uint32_t)(val))
#define valuestype(L) enums_values((L), DOMAIN_NONFT_TYPE)

#define testorientation(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_ORIENTATION, (arg), (err))
#define checkorientation(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_ORIENTATION, (arg))
#define pushorientation(L, val) enums_push((L), DOMAIN_FT_ORIENTATION, (uint32_t)(val))
#define valuesorientation(L) enums_values((L), DOMAIN_FT_ORIENTATION)

#define teststrokerborder(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_STROKER_BORDER, (arg), (err))
#define checkstrokerborder(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_STROKER_BORDER, (arg))
#define pushstrokerborder(L, val) enums_push((L), DOMAIN_FT_STROKER_BORDER, (uint32_t)(val))
#define valuesstrokerborder(L) enums_values((L), DOMAIN_FT_STROKER_BORDER)

#define teststrokerlinejoin(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_STROKER_LINEJOIN, (arg), (err))
#define checkstrokerlinejoin(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_STROKER_LINEJOIN, (arg))
#define pushstrokerlinejoin(L, val) enums_push((L), DOMAIN_FT_STROKER_LINEJOIN, (uint32_t)(val))
#define valuesstrokerlinejoin(L) enums_values((L), DOMAIN_FT_STROKER_LINEJOIN)

#define teststrokerlinecap(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_STROKER_LINECAP, (arg), (err))
#define checkstrokerlinecap(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_STROKER_LINECAP, (arg))
#define pushstrokerlinecap(L, val) enums_push((L), DOMAIN_FT_STROKER_LINECAP, (uint32_t)(val))
#define valuesstrokerlinecap(L) enums_values((L), DOMAIN_FT_STROKER_LINECAP)

#define testglyphbboxmode(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_GLYPH_BBOX, (arg), (err))
#define checkglyphbboxmode(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_GLYPH_BBOX, (arg))
#define pushglyphbboxmode(L, val) enums_push((L), DOMAIN_FT_GLYPH_BBOX, (uint32_t)(val))
#define valuesglyphbboxmode(L) enums_values((L), DOMAIN_FT_GLYPH_BBOX)

#define testtruetypeenginetype(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_TRUETYPE_ENGINE_TYPE, (arg), (err))
#define checktruetypeenginetype(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_TRUETYPE_ENGINE_TYPE, (arg))
#define pushtruetypeenginetype(L, val) enums_push((L), DOMAIN_FT_TRUETYPE_ENGINE_TYPE, (uint32_t)(val))
#define valuestruetypeenginetype(L) enums_values((L), DOMAIN_FT_TRUETYPE_ENGINE_TYPE)

#define testsizerequesttype(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_SIZE_REQUEST_TYPE, (arg), (err))
#define checksizerequesttype(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_SIZE_REQUEST_TYPE, (arg))
#define pushsizerequesttype(L, val) enums_push((L), DOMAIN_FT_SIZE_REQUEST_TYPE, (uint32_t)(val))
#define valuessizerequesttype(L) enums_values((L), DOMAIN_FT_SIZE_REQUEST_TYPE)

#define testkerningmode(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_KERNING, (arg), (err))
#define checkkerningmode(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_KERNING, (arg))
#define pushkerningmode(L, val) enums_push((L), DOMAIN_FT_KERNING, (uint32_t)(val))
#define valueskerningmode(L) enums_values((L), DOMAIN_FT_KERNING)

#define testrendermode(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_RENDER_MODE, (arg), (err))
#define checkrendermode(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_RENDER_MODE, (arg))
#define pushrendermode(L, val) enums_push((L), DOMAIN_FT_RENDER_MODE, (uint32_t)(val))
#define valuesrendermode(L) enums_values((L), DOMAIN_FT_RENDER_MODE)

#define testencoding(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_ENCODING, (arg), (err))
#define checkencoding(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_ENCODING, (arg))
#define pushencoding(L, val) enums_push((L), DOMAIN_FT_ENCODING, (uint32_t)(val))
#define valuesencoding(L) enums_values((L), DOMAIN_FT_ENCODING)

#define testglyphformat(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_GLYPH_FORMAT, (arg), (err))
#define checkglyphformat(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_GLYPH_FORMAT, (arg))
#define pushglyphformat(L, val) enums_push((L), DOMAIN_FT_GLYPH_FORMAT, (uint32_t)(val))
#define valuesglyphformat(L) enums_values((L), DOMAIN_FT_GLYPH_FORMAT)

#define testpixelmode(L, arg, err) (uint32_t)enums_test((L), DOMAIN_FT_PIXEL_MODE, (arg), (err))
#define checkpixelmode(L, arg) (uint32_t)enums_check((L), DOMAIN_FT_PIXEL_MODE, (arg))
#define pushpixelmode(L, val) enums_push((L), DOMAIN_FT_PIXEL_MODE, (uint32_t)(val))
#define valuespixelmode(L) enums_values((L), DOMAIN_FT_PIXEL_MODE)

#if 0 /* scaffolding 6yy */
#define testxxx(L, arg, err) (uint32_t)enums_test((L), DOMAIN_XXX, (arg), (err))
#define checkxxx(L, arg) (uint32_t)enums_check((L), DOMAIN_XXX, (arg))
#define pushxxx(L, val) enums_push((L), DOMAIN_XXX, (uint32_t)(val))
#define valuesxxx(L) enums_values((L), DOMAIN_XXX)
    CASE(xxx);

#endif

#endif /* enumsDEFINED */


