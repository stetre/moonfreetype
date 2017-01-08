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

/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int Get_char_variant_index(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    FT_ULong charcode = luaL_checkinteger(L, 2);
    FT_ULong selector = luaL_checkinteger(L, 3);
    FT_UInt glyph_index = FT_Face_GetCharVariantIndex(*face, charcode, selector);
    pushindex(L, glyph_index);
    return 1;
    }

static int Get_char_variant_is_default(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    FT_ULong charcode = luaL_checkinteger(L, 2);
    FT_ULong selector = luaL_checkinteger(L, 3);
    FT_Int res = FT_Face_GetCharVariantIsDefault(*face, charcode, selector);
    if(res == -1)
        return 0; /* nil -> not a variant */
    lua_pushboolean(L, res);
    return 1;
    }

static int Get_variant_selectors(lua_State *L)
    {
    size_t i;
    FT_Face *face = checkface(L, 1);
    FT_UInt32 *selector = FT_Face_GetVariantSelectors(*face);
    if(selector == NULL)
        return 0;
    lua_newtable(L);
    i = 0;
    while(selector[i] != 0)
        {
        lua_pushinteger(L, selector[i]);
        i++;
        lua_seti(L, -2, i);
        }
    return 1;
    }

static int Get_variants_of_char(lua_State *L)
    {
    size_t i;
    FT_Face *face = checkface(L, 1);
    FT_ULong charcode = luaL_checkinteger(L, 2);
    FT_UInt32 *selector = FT_Face_GetVariantsOfChar(*face, charcode);
    if(selector == NULL)
        return 0;
    lua_newtable(L);
    i = 0;
    while(selector[i] != 0)
        {
        lua_pushinteger(L, selector[i]);
        i++;
        lua_seti(L, -2, i);
        }
    return 1;
    }


static int Get_chars_of_variant(lua_State *L)
    {
    size_t i;
    FT_Face *face = checkface(L, 1);
    FT_ULong selector = luaL_checkinteger(L, 2);
    FT_UInt32 *charcode = FT_Face_GetCharsOfVariant(*face, selector);
    if(charcode == NULL)
        return 0;
    lua_newtable(L);
    i = 0;
    while(charcode[i] != 0)
        {
        lua_pushinteger(L, charcode[i]);
        i++;
        lua_seti(L, -2, i);
        }
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "get_char_variant_index", Get_char_variant_index },
        { "get_char_variant_is_default", Get_char_variant_is_default },
        { "get_variant_selectors", Get_variant_selectors },
        { "get_variants_of_char", Get_variants_of_char },
        { "get_chars_of_variant", Get_chars_of_variant },
        { NULL, NULL } /* sentinel */
    };


#define Functions Methods

void moonfreetype_open_variants(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    udata_addmethods(L, FACE_MT, Methods);
    }


