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
#include FT_PFR_H

static int Get_pfr_advance(lua_State *L) 
    {
    FT_Error ec;
    FT_Pos advance;
    FT_Face *face = checkface(L, 1);
    FT_UInt glyph_index = checkindex(L, 2);
    ec = FT_Get_PFR_Advance(*face, glyph_index, &advance);
    CheckError(L, ec);
    lua_pushinteger(L, advance);
    return 1;
    }

static int Get_pfr_metrics(lua_State *L) 
    {
    FT_Error ec;
    FT_UInt outline_res, metrics_res;
    FT_Fixed x_scale, y_scale;
    FT_Face *face = checkface(L, 1);
    ec = FT_Get_PFR_Metrics(*face, &outline_res, &metrics_res, &x_scale, &y_scale);
    CheckError(L, ec);
    lua_pushinteger(L, outline_res);
    lua_pushinteger(L, metrics_res);
    lua_pushinteger(L, x_scale);
    lua_pushinteger(L, y_scale);
    return 4;
    }

static int Get_pfr_kerning(lua_State *L)
    {
    FT_Error ec;
    FT_Vector vec;
    FT_Face *face = checkface(L, 1);
    FT_UInt left_glyph = checkindex(L, 2); /* glyph_index */
    FT_UInt right_glyph = checkindex(L, 3); /* glyph_index */
    ec = FT_Get_PFR_Kerning(*face, left_glyph, right_glyph, &vec);
    CheckError(L, ec);
    return pushvector(L, vec);
    }



/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "get_pfr_advance", Get_pfr_advance },
        { "get_pfr_metrics", Get_pfr_metrics },
        { "get_pfr_kerning", Get_pfr_kerning },
        { NULL, NULL } /* sentinel */
    };

#define Functions Methods

void moonfreetype_open_pfr(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    udata_addmethods(L, FACE_MT, Methods);
    }


