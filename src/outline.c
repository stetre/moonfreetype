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
#include FT_OUTLINE_H
#include FT_BBOX_H

/*------------------------------------------------------------------------------*
 | Check/test/push                                                              |
 *------------------------------------------------------------------------------*/

static FT_Outline* testoutline(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, OUTLINE_MT);
    return (FT_Outline*)(ud ? ud->obj : NULL);
    }

FT_Outline* checkoutline(lua_State *L, int arg)
    {
    FT_Outline *p = testoutline(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a outline");
    return NULL;
    }

int pushoutline(lua_State *L, FT_Outline *outline, FT_Library library)
    {
    ud_t *ud = userdata(outline);
    if(ud)
        return udata_push(L, outline);
    /* create userdata for outline  owned by FreeType */
    DBG("created outline %p (on push)\n", (void*)outline);
    newuserdata(L, (void*)outline, OUTLINE_MT);
    ud = userdata(outline);
    ud->library = library;
    ud->allocated = 0;
    return 1;
    }

static int pushpointtype(lua_State *L, unsigned int value) /* FT_CURVE_TAG_XXX */
    {
    if(value & 1)
        lua_pushstring(L, "on");
    else /* off (Bezier control point) */
        lua_pushstring(L, value & 2 ? "cubic" : "conic");
    return 1;
    }


static int pushscanmode(lua_State *L, unsigned int value) /* FT_CURVE_TAG_XXX */
    {
    if(!(value & FT_CURVE_TAG_HAS_SCANMODE))
        lua_pushstring(L, "none");
    else
        {
        value = value & FT_CURVE_TAG_TOUCH_BOTH;
        switch(value)
            {
            case FT_CURVE_TAG_TOUCH_X:  lua_pushstring(L, "touch x"); break;
            case FT_CURVE_TAG_TOUCH_Y:  lua_pushstring(L, "touch y"); break;
            default:
                lua_pushstring(L, "touch both");
            }
        }   
    return 1;
    }

int pushoutlinedata(lua_State *L, FT_Outline outline)
    {
    short i;
    lua_newtable(L);
#if 0
    lua_pushinteger(L, outline.n_contours); // #contours
    lua_setfield(L, -2, "n_contours");
    lua_pushinteger(L, outline.n_points); // #points
    lua_setfield(L, -2, "n_points");
#endif
    /* points */
    lua_newtable(L);
    for(i = 0; i < outline.n_points; i++)
        {
        pushvector(L, outline.points[i]);
        lua_seti(L, -2, i+1);
        }
    lua_setfield(L, -2, "points");
    /* tags */
    lua_newtable(L);
    for(i = 0; i < outline.n_points; i++)
        {
        lua_pushinteger(L, outline.tags[i]);
        lua_seti(L, -2, i+1);
        }
    lua_setfield(L, -2, "tags");
    lua_newtable(L);
    for(i = 0; i < outline.n_points; i++)
        {
        pushpointtype(L,  outline.tags[i]);
        lua_seti(L, -2, i+1);
        }
    lua_setfield(L, -2, "type");
    lua_newtable(L);
    for(i = 0; i < outline.n_points; i++)
        {
        pushscanmode(L,  outline.tags[i]);
        lua_seti(L, -2, i+1);
        }
    lua_setfield(L, -2, "scanmode");
    /* contours */
    lua_newtable(L);
    for(i = 0; i < outline.n_contours; i++)
        {
        pushindex(L, outline.contours[i]);
        lua_seti(L, -2, i+1);
        }
    lua_setfield(L, -2, "contours");
    pushflags(L, outline.flags); // outlineflags
    lua_setfield(L, -2, "flags");
    return 1;   
    }

/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int Outline_new(lua_State *L)
/* outline = outline_new(library, num_points, num_contours) 
 */
    {
    FT_Error ec;
    ud_t *ud;
    FT_Library *library = checklibrary(L, 1);
    FT_UInt num_points = luaL_checkinteger(L, 2);
    FT_Int num_contours = luaL_checkinteger(L, 3);
    FT_Outline *outline = (FT_Outline*)Malloc(L, sizeof(FT_Outline));

    ec = FT_Outline_New(*library, num_points, num_contours, outline);
    if(ec)
        {
        Free(L, outline);
        CheckError(L, ec); /* raises an error */
        return 0;
        }

    DBG("created outline %p\n", (void*)outline);
    newuserdata(L, (void*)outline, OUTLINE_MT);
    ud = userdata(outline);
    ud->library = *library;
    ud->allocated = 1;
    return 1;
    }

int freeoutline(lua_State *L, FT_Outline *outline)
    {
    ud_t *ud = userdata(outline);
    FT_Library library = ud->library;
    int allocated = ud->allocated;
    if(!freeuserdata(L, outline))
        return 0; /* double call */
    DBG("deleting outline %p\n", (void*)outline);
    if(!allocated)
        return 0;
    FT_Outline_Done(library, outline);
    Free(L, outline);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Outline *outline = testoutline(L, 1);
    if(outline)
        return freeoutline(L, outline);
    return 0;
    }

static int Outline_copy(lua_State *L)
    {
    FT_Error ec;
    ud_t *src_ud, *dst_ud;
    FT_Outline *src_outline = checkoutline(L, 1);
    FT_Outline *dst_outline = checkoutline(L, 2);
    src_ud = userdata(src_outline);
    dst_ud = userdata(dst_outline);
    //DBG("libraries: src %p dst %p\n", (void*)(src_ud->library), (void*)(dst_ud->library));
    if(src_ud->library != dst_ud->library)
        return luaL_error(L, "outlines do not belong to the same library");
    ec = FT_Outline_Copy(src_outline, dst_outline);
    CheckError(L, ec);
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/

static int Data(lua_State *L)
/* outlinedata = outline:data() */
    {
    FT_Outline *outline = checkoutline(L, 1);
    pushoutlinedata(L, *outline);
    return 1;
    }


static int Outline_check(lua_State *L) 
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_Error ec = FT_Outline_Check(outline);
    if(ec)
        { 
        lua_pushboolean(L, 0);
        lua_pushstring(L, fterrorstring(ec));
        return 2;
        }
    lua_pushboolean(L, 1);
    return 1;
    }


static int Outline_translate(lua_State *L) 
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_Pos xofs = luaL_checkinteger(L, 2);
    FT_Pos yofs = luaL_checkinteger(L, 3);
    FT_Outline_Translate(outline, xofs, yofs);
    return 0;
    }


static int Outline_transform(lua_State *L) 
    {
    FT_Matrix matrix;
    FT_Outline *outline = checkoutline(L, 1);
    checkmatrix(L, 2, &matrix);
    FT_Outline_Transform(outline, &matrix);
    return 0;
    }


static int Outline_embolden(lua_State *L)
    {
    FT_Error ec;
    FT_Outline *outline = checkoutline(L, 1);
    FT_Pos strength = luaL_checkinteger(L, 2);
    ec = FT_Outline_Embolden(outline, strength);
    CheckError(L, ec);
    return 0;
    }


static int Outline_emboldenxy(lua_State *L)
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_Pos xstrength = luaL_checkinteger(L, 2);
    FT_Pos ystrength = luaL_checkinteger(L, 3);
    FT_Error ec = FT_Outline_EmboldenXY(outline, xstrength, ystrength);
    CheckError(L, ec);
    return 0;
    }

static int Outline_reverse(lua_State *L) 
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_Outline_Reverse(outline);
    return 0;
    }



static int Outline_get_cbox(lua_State *L)
    {
    FT_BBox box;
    FT_Outline *outline = checkoutline(L, 1);
    FT_Outline_Get_CBox(outline, &box);
    pushbbox(L, box);
    return 1;
    }

static int Outline_get_bbox(lua_State *L)
    {
    FT_Error ec;
    FT_BBox box;
    FT_Outline *outline = checkoutline(L, 1);
    ec = FT_Outline_Get_BBox(outline, &box);
    CheckError(L, ec);
    pushbbox(L, box);
    return 1;
    }


static int Outline_get_bitmap(lua_State *L)
    {
    FT_Error ec;
    FT_Outline *outline = checkoutline(L, 1);
    FT_Bitmap *bitmap = checkbitmap(L, 2);
    ud_t *ud_o = userdata(outline);
    ud_t *ud_b = userdata(bitmap);
    if(ud_o->library != ud_b->library)
        return luaL_error(L, "outline and bitmap do not belong to the same library");
    ec = FT_Outline_Get_Bitmap(ud_o->library, outline, bitmap);
    CheckError(L, ec);
    return 0;
    }


static int Outline_render(lua_State *L) //@@FT_Outline_Render
    {
    FT_Outline *outline = checkoutline(L, 1);
    (void)outline;
    NOT_AVAILABLE;
    return 0;
    }


static int Outline_decompose(lua_State *L) //@@FT_Outline_Decompose
    {
    FT_Outline *outline = checkoutline(L, 1);
    (void)outline;
    NOT_AVAILABLE;
    return 0;
    }



static int Outline_get_orientation(lua_State *L)
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_Orientation orientation = FT_Outline_Get_Orientation(outline);
    pushorientation(L, orientation);
    return 1;
    }


static int Outline_getinsideborder(lua_State *L)
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_StrokerBorder border = FT_Outline_GetInsideBorder(outline);
    pushstrokerborder(L, border);
    return 1;
    }


static int Outline_getoutsideborder(lua_State *L)
    {
    FT_Outline *outline = checkoutline(L, 1);
    FT_StrokerBorder border = FT_Outline_GetOutsideBorder(outline);
    pushstrokerborder(L, border);
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "copy", Outline_copy },
        { "check", Outline_check },
        { "translate", Outline_translate },
        { "transform", Outline_transform },
        { "embolden", Outline_embolden },
        { "emboldenxy", Outline_emboldenxy },
        { "reverse", Outline_reverse },
        { "get_cbox", Outline_get_cbox },
        { "get_bbox", Outline_get_bbox },
        { "get_bitmap", Outline_get_bitmap },
        { "render", Outline_render },
        { "decompose", Outline_decompose },
        { "get_orientation", Outline_get_orientation },
        { "getinsideborder", Outline_getinsideborder },
        { "getoutsideborder", Outline_getoutsideborder },
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
        { "outline_new", Outline_new },
        { "outline_done", Delete },
        { "outline_copy", Outline_copy },
        { "outline_check", Outline_check },
        { "outline_translate", Outline_translate },
        { "outline_transform", Outline_transform },
        { "outline_embolden", Outline_embolden },
        { "outline_emboldenxy", Outline_emboldenxy },
        { "outline_reverse", Outline_reverse },
        { "outline_get_cbox", Outline_get_cbox },
        { "outline_get_bbox", Outline_get_bbox },
        { "outline_get_bitmap", Outline_get_bitmap },
        { "outline_render", Outline_render },
        { "outline_decompose", Outline_decompose },
        { "outline_get_orientation", Outline_get_orientation },
        { "outline_getinsideborder", Outline_getinsideborder },
        { "outline_getoutsideborder", Outline_getoutsideborder },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg LibraryMethods[] = 
    {
        { "outline_new", Outline_new },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_outline(lua_State *L)
    {
    udata_addmethods(L, LIBRARY_MT, LibraryMethods);
    udata_define(L, OUTLINE_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

