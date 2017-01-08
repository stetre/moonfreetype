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

static int Get_advance(lua_State *L)
    {
    FT_Error ec;
    FT_Fixed advance;
    FT_Face *face = checkface(L, 1);
    FT_UInt glyph_index = checkindex(L, 2);
    FT_Int32 load_flags = checkloadflags(L, 3);
    ec = FT_Get_Advance(*face, glyph_index, load_flags, &advance);
    CheckError(L, ec);
    lua_pushinteger(L, advance);
    return 1;
    }

static int Get_advances(lua_State *L)
    {
    FT_Error ec;
    FT_UInt i;
    FT_Fixed *padvance;
    FT_Face *face = checkface(L, 1);
    FT_UInt start = checkindex(L, 2); /* glyph_index */
    FT_UInt count = checkindex(L, 3);
    FT_Int32 load_flags = checkloadflags(L, 4);
    if(count < 1)
        return luaL_argerror(L, 3, "positive number expected");
    padvance = (FT_Fixed*)Malloc(L, sizeof(FT_Fixed)*count);
    ec = FT_Get_Advances(*face, start, count, load_flags, padvance);
    if(ec) Free(L, padvance); /* CheckError() will return, so free the allocated memory */
    CheckError(L, ec);
    for(i=0; i<count; i++)
        lua_pushinteger(L, padvance[i]);
    Free(L, padvance);
    return count;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "get_advance", Get_advance },
        { "get_advances", Get_advances },
        { NULL, NULL } /* sentinel */
    };

#define Functions Methods

void moonfreetype_open_advance(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    udata_addmethods(L, FACE_MT, Methods);
    }


