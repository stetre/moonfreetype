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

#include "internal.h"


static void AddVersions(lua_State *L)
    {
    lua_pushstring(L, "_VERSION");
    lua_pushstring(L, "MoonFreeType " MOONFREETYPE_VERSION);
    lua_settable(L, -3);

    lua_pushstring(L, "_FREETYPE_VERSION");
    lua_pushfstring(L, "FreeType %d.%d.%d", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
    lua_settable(L, -3);
    }

lua_State *moonfreetype_L = NULL;

static void AtExit(void)
    {
    if(moonfreetype_L)
        {
        moonfreetype_L = NULL;
        }
    }

static int AddConstants(lua_State *L) /* pa.Xxxx constants for paXxxx defines */
    {
#define ADD(c) do { lua_pushinteger(L, FT_##c); lua_setfield(L, -2, #c); } while(0)
    /* FT_FSTYPE_XXX */
    ADD(FSTYPE_INSTALLABLE_EMBEDDING);
    ADD(FSTYPE_RESTRICTED_LICENSE_EMBEDDING);
    ADD(FSTYPE_PREVIEW_AND_PRINT_EMBEDDING);
    ADD(FSTYPE_EDITABLE_EMBEDDING);
    ADD(FSTYPE_NO_SUBSETTING);
    ADD(FSTYPE_BITMAP_EMBEDDING_ONLY);
    /* FT_SUBGLYPH_FLAG_XXX */
    ADD(SUBGLYPH_FLAG_ARGS_ARE_WORDS);
    ADD(SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES);
    ADD(SUBGLYPH_FLAG_ROUND_XY_TO_GRID);
    ADD(SUBGLYPH_FLAG_SCALE);
    ADD(SUBGLYPH_FLAG_XY_SCALE);
    ADD(SUBGLYPH_FLAG_2X2);
    ADD(SUBGLYPH_FLAG_USE_MY_METRICS);
    /* FT_OUTLINE_XXX */
    ADD(OUTLINE_NONE);
    ADD(OUTLINE_OWNER);
    ADD(OUTLINE_EVEN_ODD_FILL);
    ADD(OUTLINE_REVERSE_FILL);
    ADD(OUTLINE_IGNORE_DROPOUTS);
    ADD(OUTLINE_SMART_DROPOUTS);
    ADD(OUTLINE_INCLUDE_STUBS);
    ADD(OUTLINE_HIGH_PRECISION);
    ADD(OUTLINE_SINGLE_PASS);
    /* FT_LOAD_XXX */
    ADD(LOAD_DEFAULT);
    ADD(LOAD_NO_SCALE);
    ADD(LOAD_NO_HINTING);
    ADD(LOAD_RENDER);
    ADD(LOAD_NO_BITMAP);
    ADD(LOAD_VERTICAL_LAYOUT);
    ADD(LOAD_FORCE_AUTOHINT);
    ADD(LOAD_CROP_BITMAP);
    ADD(LOAD_PEDANTIC);
    ADD(LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH);
    ADD(LOAD_NO_RECURSE);
    ADD(LOAD_IGNORE_TRANSFORM);
    ADD(LOAD_MONOCHROME);
    ADD(LOAD_LINEAR_DESIGN);
    ADD(LOAD_NO_AUTOHINT);
    ADD(LOAD_COLOR);
    ADD(LOAD_COMPUTE_METRICS);
    ADD(ADVANCE_FLAG_FAST_ONLY); /* FT_ADVANCE_FLAG_XXX */
    ADD(LOAD_TARGET_NORMAL);
    ADD(LOAD_TARGET_LIGHT);
    ADD(LOAD_TARGET_MONO);
    ADD(LOAD_TARGET_LCD);
    ADD(LOAD_TARGET_LCD_V);
#undef ADD
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { NULL, NULL } /* sentinel */
    };



int luaopen_moonfreetype(lua_State *L)
/* Lua calls this function to load the module */
    {
    moonfreetype_L = L;
    
    moonfreetype_utils_init(L);
    lua_newtable(L); /* the freetype table */
    moonfreetype_open_enums(L);
    AddVersions(L);
    AddConstants(L);
    
    atexit(AtExit);

    /* add functions: */
    luaL_setfuncs(L, Functions, 0);
    moonfreetype_open_additional(L);
    moonfreetype_open_computations(L);
    moonfreetype_open_library(L);
    moonfreetype_open_face(L);
    moonfreetype_open_glyph(L);
    moonfreetype_open_size(L);
    moonfreetype_open_advance(L);
    moonfreetype_open_pfr(L);
    moonfreetype_open_variants(L);
    moonfreetype_open_bitmap(L);
    moonfreetype_open_outline(L);
    moonfreetype_open_stroker(L);

    return 1;
    }

