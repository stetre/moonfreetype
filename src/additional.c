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
 | Additional non-FreeType utilities                                            |
 *------------------------------------------------------------------------------*/

static int Fstypeflags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushfstypeflags(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L, checkfstypeflags(L, 1));
    return 1;
    }


static int Subglyphflags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsubglyphflags(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L, checksubglyphflags(L, 1));
    return 1;
    }


static int Outlineflags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushoutlineflags(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L, checkoutlineflags(L, 1));
    return 1;
    }


static int Loadflags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushloadflags(L, luaL_checkinteger(L, 1), 0);
    lua_pushinteger(L, checkloadflags(L, 1));
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "fstypeflags", Fstypeflags },
        { "subglyphflags", Subglyphflags },
        { "outlineflags", Outlineflags },
        { "loadflags", Loadflags },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_additional(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

