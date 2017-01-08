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
#include FT_SIZES_H

/*------------------------------------------------------------------------------*
 | Check/test/push                                                              |
 *------------------------------------------------------------------------------*/

static FT_Size* testsize(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, SIZE_MT);
    return (FT_Size*)(ud ? ud->obj : NULL);
    }

static FT_Size* checksize(lua_State *L, int arg)
    {
    FT_Size *p = testsize(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a size");
    return NULL;
    }

#if 0
static int pushsize(lua_State *L, FT_Size *p)
    { return udata_push(L, p); }
#endif


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int New_size(lua_State *L)
    {
    FT_Error ec;
    ud_t *ud;
    FT_Face *face = checkface(L, 1);
    FT_Size *size = (FT_Size*)Malloc(L, sizeof(FT_Size));

    ec = FT_New_Size(*face, size);
    if(ec)
        {
        Free(L, size);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    DBG("created size %p\n", (void*)size);
    newuserdata(L, (void*)size, SIZE_MT);
    ud = userdata(size);
    ud->face = *face;
    return 1;
    }

int freesize(lua_State *L, FT_Size *size)
    {
    FT_Error ec;
    if(!freeuserdata(L, size))
        return 0; /* double call */
    DBG("deleting size %p\n", (void*)size);
    ec = FT_Done_Size(*size);
    Free(L, size);
    CheckError(L, ec);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Size *size = checksize(L, 1);
    return freesize(L, size);
    }

static int Activate_size(lua_State *L)
    {
    FT_Size *size = checksize(L, 1);
    FT_Error ec = FT_Activate_Size(*size);
    CheckError(L, ec);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "activate", Activate_size },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "new_size", New_size },
        { "done_size", Delete },
        { "activate_size", Activate_size },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg FaceMethods[] = 
    {
        { "new_size", New_size },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_size(lua_State *L)
    {
    udata_addmethods(L, FACE_MT, FaceMethods);
    udata_define(L, SIZE_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

