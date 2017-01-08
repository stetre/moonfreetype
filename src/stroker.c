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

static FT_Stroker* teststroker(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, STROKER_MT);
    return (FT_Stroker*)(ud ? ud->obj : NULL);
    }

FT_Stroker* checkstroker(lua_State *L, int arg)
    {
    FT_Stroker *p = teststroker(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a stroker");
    return NULL;
    }

#if 0
static int pushstroker(lua_State *L, FT_Stroker *p)
    { return udata_push(L, p); }
#endif


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int Stroker_new(lua_State *L)
/* stroker = stroker_new(face) 
 */
    {
    FT_Error ec;
    ud_t *ud;
    FT_Library *library = checklibrary(L, 1);
    FT_Stroker *stroker = (FT_Stroker*)Malloc(L, sizeof(FT_Stroker));

    ec = FT_Stroker_New(*library, stroker);
    if(ec)
        {
        Free(L, stroker);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    DBG("created stroker %p\n", (void*)stroker);
    newuserdata(L, (void*)stroker, STROKER_MT);
    ud = userdata(stroker);
    ud->library = *library;
    return 1;
    }

int freestroker(lua_State *L, FT_Stroker *stroker)
    {
    if(!freeuserdata(L, stroker))
        return 0; /* double call */
    DBG("deleting stroker %p\n", (void*)stroker);
    FT_Stroker_Done(*stroker);
    Free(L, stroker);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Stroker *stroker = checkstroker(L, 1);
    return freestroker(L, stroker);
    }

/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/



static int Stroker_beginsubpath(lua_State *L)
    {
    FT_Error ec;
    FT_Vector to;
    FT_Bool open;
    FT_Stroker *stroker = checkstroker(L, 1);
    checkvector(L, 2, &to);
    open = checkboolean(L, 3);
    ec = FT_Stroker_BeginSubPath(*stroker, &to, open);
    CheckError(L, ec);
    return 0;
    }


static int Stroker_endsubpath(lua_State *L) 
    {
    FT_Error ec;
    FT_Stroker *stroker = checkstroker(L, 1);
    ec = FT_Stroker_EndSubPath(*stroker);
    CheckError(L, ec);
    return 0;
    }


static int Stroker_rewind(lua_State *L) 
    {
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_Stroker_Rewind(*stroker);
    return 0;
    }


static int Stroker_set(lua_State *L) 
    {
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_Fixed radius = luaL_checkinteger(L, 2);
    FT_Stroker_LineCap line_cap = checkstrokerlinecap(L, 3);
    FT_Stroker_LineJoin line_join = checkstrokerlinejoin(L, 4);
    FT_Fixed miter_limit = luaL_checkinteger(L, 5);
    FT_Stroker_Set(*stroker, radius, line_cap, line_join, miter_limit); 
    return 0;
    }

static int Stroker_conicto(lua_State *L) 
    {
    FT_Error ec;
    FT_Vector control, to;
    FT_Stroker *stroker = checkstroker(L, 1);
    checkvector(L, 2, &control);
    checkvector(L, 3, &to);
    ec = FT_Stroker_ConicTo(*stroker, &control, &to);
    CheckError(L, ec);
    return 0;
    }


static int Stroker_cubicto(lua_State *L) 
    {
    FT_Error ec;
    FT_Vector control1, control2, to;
    FT_Stroker *stroker = checkstroker(L, 1);
    checkvector(L, 2, &control1);
    checkvector(L, 3, &control2);
    checkvector(L, 4, &to);
    ec = FT_Stroker_CubicTo(*stroker, &control1, &control2, &to);
    CheckError(L, ec);
    return 0;
    }

static int Stroker_lineto(lua_State *L)
    {
    FT_Error ec;
    FT_Vector to;
    FT_Stroker *stroker = checkstroker(L, 1);
    checkvector(L, 2, &to);
    ec = FT_Stroker_LineTo(*stroker, &to);
    CheckError(L, ec);
    return 0;
    }


static int Stroker_getcounts(lua_State *L)
    {
    FT_Error ec;
    FT_UInt num_points, num_contours;
    FT_Stroker *stroker = checkstroker(L, 1);
    ec = FT_Stroker_GetCounts(*stroker, &num_points, &num_contours);
    CheckError(L, ec);
    lua_pushinteger(L, num_points);
    lua_pushinteger(L, num_contours);
    return 2;
    }


static int Stroker_parseoutline(lua_State *L)
    {
    ud_t *ud1, *ud2;
    FT_Error ec;
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_Outline *outline = checkoutline(L, 2);
    FT_Bool opened = checkboolean(L, 3);
    ud1 = userdata(stroker);
    ud2 = userdata(outline);
    if(ud1->library != ud2->library)
        return luaL_error(L, "stroker and outline do not belong to the same library");
    ec = FT_Stroker_ParseOutline(*stroker, outline, opened);
    CheckError(L, ec);
    return 0;
    }


static int Stroker_export(lua_State *L) 
    {
    ud_t *ud1, *ud2;
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_Outline *outline = checkoutline(L, 2);
    ud1 = userdata(stroker);
    ud2 = userdata(outline);
    if(ud1->library != ud2->library)
        return luaL_error(L, "stroker and outline do not belong to the same library");
    FT_Stroker_Export(*stroker, outline);
    return 0;
    }


static int Stroker_exportborder(lua_State *L)
    {
    ud_t *ud1, *ud2;
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_StrokerBorder border = checkstrokerborder(L, 2);
    FT_Outline *outline = checkoutline(L, 3);
    ud1 = userdata(stroker);
    ud2 = userdata(outline);
    if(ud1->library != ud2->library)
        return luaL_error(L, "stroker and outline do not belong to the same library");
    FT_Stroker_ExportBorder(*stroker, border, outline);
    return 0;
    }


static int Stroker_getbordercounts(lua_State *L)
    {
    FT_Error ec;
    FT_UInt num_points, num_contours;
    FT_Stroker *stroker = checkstroker(L, 1);
    FT_StrokerBorder border = checkstrokerborder(L, 2);
    ec = FT_Stroker_GetBorderCounts(*stroker, border, &num_points, &num_contours);
    CheckError(L, ec);
    lua_pushinteger(L, num_points);
    lua_pushinteger(L, num_contours);
    return 2;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/


static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "beginsubpath", Stroker_beginsubpath },
        { "endsubpath", Stroker_endsubpath },
        { "rewind", Stroker_rewind },
        { "set", Stroker_set },
        { "conicto", Stroker_conicto },
        { "cubicto", Stroker_cubicto },
        { "lineto", Stroker_lineto },
        { "getcounts", Stroker_getcounts },
        { "parseoutline", Stroker_parseoutline },
        { "export", Stroker_export },
        { "exportborder", Stroker_exportborder },
        { "getbordercounts", Stroker_getbordercounts },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "stroker_new", Stroker_new },
        { "stroker_done", Delete },
        { "stroker_beginsubpath", Stroker_beginsubpath },
        { "stroker_endsubpath", Stroker_endsubpath },
        { "stroker_rewind", Stroker_rewind },
        { "stroker_set", Stroker_set },
        { "stroker_conicto", Stroker_conicto },
        { "stroker_cubicto", Stroker_cubicto },
        { "stroker_lineto", Stroker_lineto },
        { "stroker_getcounts", Stroker_getcounts },
        { "stroker_parseoutline", Stroker_parseoutline },
        { "stroker_export", Stroker_export },
        { "stroker_exportborder", Stroker_exportborder },
        { "stroker_getbordercounts", Stroker_getbordercounts },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg LibraryMethods[] = 
    {
        { "stroker_new", Stroker_new },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_stroker(lua_State *L)
    {
    udata_addmethods(L, LIBRARY_MT, LibraryMethods);
    udata_define(L, STROKER_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

