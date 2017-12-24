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
 | Check/test/push                                                              |
 *------------------------------------------------------------------------------*/

FT_Library* testlibrary(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, LIBRARY_MT);
    return (FT_Library*)(ud ? ud->obj : NULL);
    }

FT_Library* checklibrary(lua_State *L, int arg)
    {
    FT_Library *p = testlibrary(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a library");
    return NULL;
    }

#if 0
int pushlibrary(lua_State *L, FT_Library *p)
    { return udata_push(L, p); }
#endif


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int InitFreeType(lua_State *L)
    {
    FT_Error ec;
    FT_Library *library = (FT_Library*)Malloc(L, sizeof(FT_Library));

    ec = FT_Init_FreeType(library);

    if(ec)
        {
        Free(L, library);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    
    DBG("created library %p\n", (void*)library);
    newuserdata(L, (void*)library, LIBRARY_MT);
    return 1;
    }


static int Freeface(lua_State *L, const void *face, const void *library)
    {
    ud_t *ud = userdata(face);
    if(ud->library == (FT_Library)library)
        freeface(L, (FT_Face*)face);
    return 0;
    }

static int Freeglyph(lua_State *L, const void *glyph, const void *library)
    {
    ud_t *ud = userdata(glyph);
    if(ud->library == (FT_Library)library)
        freeglyph(L, (FT_Glyph*)glyph);
    return 0;
    }

static int Freestroker(lua_State *L, const void *stroker, const void *library)
    {
    ud_t *ud = userdata(stroker);
    if(ud->library == (FT_Library)library)
        freestroker(L, (FT_Stroker*)stroker);
    return 0;
    }

static int Freebitmap(lua_State *L, const void *bitmap, const void *library)
    {
    ud_t *ud = userdata(bitmap);
    if(ud->library == (FT_Library)library)
        freebitmap(L, (FT_Bitmap*)bitmap);
    return 0;
    }

static int Freeoutline(lua_State *L, const void *outline, const void *library)
    {
    ud_t *ud = userdata(outline);
    if(ud->library == (FT_Library)library)
        freeoutline(L, (FT_Outline*)outline);
    return 0;
    }


static int Delete(lua_State *L)
    {
    FT_Error ec;
    FT_Library *library = testlibrary(L, 1);
    if(!library) return 0; /* already deleted */
    /* delete all contained objects */
    udata_scan(L, BITMAP_MT, *library, Freebitmap);
    udata_scan(L, OUTLINE_MT, *library, Freeoutline);
    udata_scan(L, STROKER_MT, *library, Freestroker);
    udata_scan(L, GLYPH_MT, *library, Freeglyph);
    udata_scan(L, FACE_MT, *library, Freeface);

    if(!freeuserdata(L, library))
        return 0; /* double call */
    DBG("deleting library %p\n", (void*)library);
    ec = FT_Done_FreeType(*library);
    Free(L, library);
    CheckError(L, ec);
    return 0;
    }



static int Get_truetype_engine_type(lua_State *L) //@@DOC (o cassare?)
    {
    FT_Library* library = checklibrary(L, 1);
    FT_TrueTypeEngineType type = FT_Get_TrueType_Engine_Type(*library);
    pushtruetypeenginetype(L, type);
    return 1;
    }



/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/

static int LibraryVersion(lua_State *L) 
    {
    int major, minor, patch;
    FT_Library* library = checklibrary(L, 1);
    FT_Library_Version(*library, &major, &minor, &patch);
    lua_pushinteger(L, major);
    lua_pushinteger(L, minor);
    lua_pushinteger(L, patch);
    return 3;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

#define FUNCTIONS \
        { "new_face", newface },        \
        { "get_truetype_engine_type", Get_truetype_engine_type },

static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "version", LibraryVersion },
        FUNCTIONS
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "init_freetype", InitFreeType },
        { "done_freetype", Delete },
        { "library_version", LibraryVersion },
        FUNCTIONS
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_library(lua_State *L)
    {
    udata_define(L, LIBRARY_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }


