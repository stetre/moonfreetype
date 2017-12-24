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
#include FT_BITMAP_H

/*------------------------------------------------------------------------------*
 | Check/test/push                                                              |
 *------------------------------------------------------------------------------*/

static FT_Bitmap* testbitmap(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, BITMAP_MT);
    return (FT_Bitmap*)(ud ? ud->obj : NULL);
    }

FT_Bitmap* checkbitmap(lua_State *L, int arg)
    {
    FT_Bitmap *p = testbitmap(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a bitmap");
    return NULL;
    }

int pushbitmap(lua_State *L, FT_Bitmap *bitmap, FT_Library library)
    {
    ud_t *ud = userdata(bitmap);
    if(ud)
        return udata_push(L, bitmap);
    /* create userdata for bitmap  owned by FreeType */
    DBG("created bitmap %p (on push)\n", (void*)bitmap);
    newuserdata(L, (void*)bitmap, BITMAP_MT);
    ud = userdata(bitmap);
    ud->library = library;
    ud->allocated = 0;
    return 1;
    }

int pushbitmapdata(lua_State *L, FT_Bitmap bitmap, FT_Int left, FT_Int top, int leftandtop)
/* pushes bitmap contents in a table */
    {
    size_t len; 
    lua_newtable(L);
    if(leftandtop)
        {
        lua_pushinteger(L, left);
        lua_setfield(L, -2, "left");
        lua_pushinteger(L, top);
        lua_setfield(L, -2, "top");
        }
    lua_pushinteger(L, bitmap.rows);
    lua_setfield(L, -2, "rows");
    lua_pushinteger(L, bitmap.width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, bitmap.pitch);
    lua_setfield(L, -2, "pitch");
    pushpixelmode(L, bitmap.pixel_mode);
    lua_setfield(L, -2, "pixel_mode");
    if(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
        {
        lua_pushinteger(L, bitmap.num_grays); 
        lua_setfield(L, -2, "num_grays");
        }
    len = bitmap.rows * (FT_ULong)bitmap.pitch; /* +/- buffer length */
    if(len)
        {
        len = len > 0 ? len : -len; /* pich may be negative */
        lua_pushlstring(L, (char*)bitmap.buffer, len);
        lua_setfield(L, -2, "buffer");
        }
    /* palette_mode: not used currently 
     * palette:      not used currently 
     */
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int New_bitmap(lua_State *L)
/* bitmap = new_bitmap(library) 
 */
    {
    ud_t *ud;
    FT_Library *library = checklibrary(L, 1);
    FT_Bitmap *bitmap = (FT_Bitmap*)Malloc(L, sizeof(FT_Bitmap));

    FT_Bitmap_Init(bitmap);
    DBG("created bitmap %p\n", (void*)bitmap);
    newuserdata(L, (void*)bitmap, BITMAP_MT);
    ud = userdata(bitmap);
    ud->library = *library;
    ud->allocated = 1;
    return 1;
    }

int freebitmap(lua_State *L, FT_Bitmap *bitmap)
    {
    ud_t *ud = userdata(bitmap);
    FT_Library library = ud->library;
    int allocated = ud->allocated;
    if(!freeuserdata(L, bitmap))
        return 0; /* double call */
    DBG("deleting bitmap %p\n", (void*)bitmap);
    if(!allocated)
        return 0;
    FT_Bitmap_Done(library, bitmap);
    Free(L, bitmap);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Bitmap *bitmap = testbitmap(L, 1);
    if(!bitmap) return 0; /* already deleted */
    return freebitmap(L, bitmap);
    }

static int Bitmap_copy(lua_State *L)
    {
    FT_Error ec;
    ud_t *src_ud, *dst_ud;
    FT_Bitmap *src_bitmap = checkbitmap(L, 1);
    FT_Bitmap *dst_bitmap = checkbitmap(L, 2);
    src_ud = userdata(src_bitmap);
    dst_ud = userdata(dst_bitmap);
    //DBG("libraries: src %p dst %p\n", (void*)(src_ud->library), (void*)(dst_ud->library));
    if(src_ud->library != dst_ud->library)
        return luaL_error(L, "bitmaps do not belong to the same library");
    ec = FT_Bitmap_Copy(src_ud->library, src_bitmap, dst_bitmap);
    CheckError(L, ec);
    return 0;
    }


static int Bitmap_convert(lua_State *L)
    {
    FT_Error ec;
    ud_t *src_ud, *dst_ud;
    FT_Bitmap *src_bitmap = checkbitmap(L, 1);
    FT_Bitmap *dst_bitmap = checkbitmap(L, 2);
    FT_Int alignment = luaL_checkinteger(L, 3);
    src_ud = userdata(src_bitmap);
    dst_ud = userdata(dst_bitmap);
    //DBG("libraries: src %p dst %p\n", (void*)(src_ud->library), (void*)(dst_ud->library));
    if(src_ud->library != dst_ud->library)
        return luaL_error(L, "bitmaps do not belong to the same library");
    ec = FT_Bitmap_Convert(src_ud->library, src_bitmap, dst_bitmap, alignment);
    CheckError(L, ec);
    return 0;
    }

static int Bitmap_embolden(lua_State *L)
//@@ GlyphSlot_Own_Bitmap ?
    {
    FT_Error ec;
    FT_Bitmap *bitmap = checkbitmap(L, 1);
    FT_Pos xstrength = luaL_checkinteger(L, 2);
    FT_Pos ystrength = luaL_checkinteger(L, 3);
    ud_t *ud = userdata(bitmap);
    ec = FT_Bitmap_Embolden(ud->library, bitmap, xstrength, ystrength);
    CheckError(L, ec);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/

static int Data(lua_State *L)
/* bitmapdata = bitmap:data([left, top]) 
 * left and top default to 0 */
    {
    FT_Bitmap *bitmap = checkbitmap(L, 1);
    FT_Int left = luaL_optinteger(L, 2, 0);
    FT_Int top = luaL_optinteger(L, 3, 0);
    pushbitmapdata(L, *bitmap, left, top, 1);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "copy", Bitmap_copy },
        { "convert", Bitmap_convert },
        { "embolden", Bitmap_embolden },
        { "data", Data },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "new_bitmap", New_bitmap },
        { "done_bitmap", Delete },
        { "bitmap_copy", Bitmap_copy },
        { "bitmap_convert", Bitmap_convert },
        { "bitmap_embolden", Bitmap_embolden },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg LibraryMethods[] = 
    {
        { "new_bitmap", New_bitmap },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_bitmap(lua_State *L)
    {
    udata_addmethods(L, LIBRARY_MT, LibraryMethods);
    udata_define(L, BITMAP_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

