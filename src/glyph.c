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

static FT_Glyph* testglyph(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, GLYPH_MT);
    return (FT_Glyph*)(ud ? ud->obj : NULL);
    }

static FT_Glyph* checkglyph(lua_State *L, int arg)
    {
    FT_Glyph *p = testglyph(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a glyph");
    return NULL;
    }

#if 0
static int pushglyph(lua_State *L, FT_Glyph *p)
    { return udata_push(L, p); }
#endif


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

static int Get_glyph(lua_State *L)
/* glyph = get_glyph(face) 
 * extracts the glyph image from the current face->glyph slot
 */
    {
    FT_Error ec;
    FT_Library library;
    ud_t *ud;
    FT_Face *face = checkface(L, 1);
    FT_Glyph *glyph = (FT_Glyph*)Malloc(L, sizeof(FT_Glyph));

    ud = userdata(face);
    library = ud->library;

    ec = FT_Get_Glyph((*face)->glyph, glyph);
    if(ec)
        {
        Free(L, glyph);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    DBG("created glyph %p\n", (void*)glyph);
    newuserdata(L, (void*)glyph, GLYPH_MT);
    ud = userdata(glyph);
    ud->library = library;
    return 1;
    }

static int Glyph_copy(lua_State *L)
    {
    FT_Error ec;
    FT_Library library;
    ud_t *ud;
    FT_Glyph *oldglyph = checkglyph(L, 1);
    FT_Glyph *glyph = (FT_Glyph*)Malloc(L, sizeof(FT_Glyph));

    ud = userdata(oldglyph);
    library = ud->library;
    ec = FT_Glyph_Copy(*oldglyph, glyph);
    if(ec)
        {
        Free(L, glyph);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    newuserdata(L, (void*)glyph, GLYPH_MT);
    ud = userdata(glyph);
    ud->library = library;
    DBG("created glyph %p (copy)\n", (void*)glyph);
    return 1;
    }


int freeglyph(lua_State *L, FT_Glyph *glyph)
    {
    if(!freeuserdata(L, glyph))
        return 0; /* double call */
    DBG("deleting glyph %p\n", (void*)glyph);
    FT_Done_Glyph(*glyph);
    Free(L, glyph);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Glyph *glyph = checkglyph(L, 1);
    return freeglyph(L, glyph);
    }

static int Glyph_get_cbox(lua_State *L) 
    {
    FT_BBox acbox;
    FT_Glyph *glyph = checkglyph(L, 1);
    FT_UInt bbox_mode = checkglyphbboxmode(L, 2);
    FT_Glyph_Get_CBox(*glyph, bbox_mode, &acbox);
    pushbbox(L, acbox);
    return 1;
    }

static int Glyph_to_bitmap(lua_State *L)
    {
    FT_Error ec;
    FT_Vector vec, *origin=0;
    FT_Bool destroy;
    FT_Glyph *glyph = checkglyph(L, 1);
    FT_Render_Mode render_mode = checkrendermode(L, 2);
    if(!lua_isnoneornil(L, 3))
        {
        checkvector(L, 3, &vec);
        origin = &vec;
        }
    destroy = optboolean(L, 4, 1); /* defaults to true */
    DBG("glyph_to_bitmap %p mode=%d origin=%p destroy=%s\n", 
            (void*)glyph,  render_mode, (void*)origin, destroy ? "true":"false");
    ec = FT_Glyph_To_Bitmap(glyph, render_mode, origin, destroy);
    DBG("glyph_to_bitmap %p ec=%d\n", (void*)glyph,  ec);
    CheckError(L, ec);
    return 0;
    }

static int Glyph_transform(lua_State *L)
    {
    FT_Error ec;
    FT_Matrix matrix;
    FT_Vector delta;
    FT_Glyph *glyph = checkglyph(L, 1);
    checkmatrix(L, 2, &matrix);
    checkvector(L, 3, &delta);
    ec = FT_Glyph_Transform(*glyph, &matrix, &delta);
    CheckError(L, ec);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/

static int Format(lua_State *L)
    {
    FT_Glyph *glyph = checkglyph(L, 1);
    pushglyphformat(L, (*glyph)->format);
    return 1;
    }

static int Advance(lua_State *L)
    {
    FT_Glyph *glyph = checkglyph(L, 1);
    pushvector(L, (*glyph)->advance);
    return 1;
    }

static int Bitmapdata(lua_State *L)
/* bitmapdata = glyph:bitmapdata() */
    {
    FT_BitmapGlyphRec *rec;
    FT_Glyph *glyph = checkglyph(L, 1);
    if((*glyph)->format != FT_GLYPH_FORMAT_BITMAP)
        return luaL_argerror(L, 1, "glyph format is not 'bitmap'");
    rec = (FT_BitmapGlyphRec*)(*glyph);
    pushbitmapdata(L, rec->bitmap, rec->left, rec->top, 1);
    return 1;
    }

static int Bitmap(lua_State *L)
/* bitmap = glyph:bitmap() */
    {
    FT_BitmapGlyphRec *rec;
    FT_Glyph *glyph = checkglyph(L, 1);
    if((*glyph)->format != FT_GLYPH_FORMAT_BITMAP)
        return luaL_argerror(L, 1, "glyph format is not 'bitmap'");
    rec = (FT_BitmapGlyphRec*)(*glyph);
    pushbitmap(L, &(rec->bitmap), rec->root.library);
    return 1;
    }

static int Outlinedata(lua_State *L)
    {
    FT_OutlineGlyphRec *rec;
    FT_Glyph *glyph = checkglyph(L, 1);
    if((*glyph)->format != FT_GLYPH_FORMAT_OUTLINE)
        return luaL_argerror(L, 1, "glyph format is not 'outline'");
    rec = (FT_OutlineGlyphRec *)(*glyph);
    pushoutlinedata(L, rec->outline);
    return 1;
    }

static int Outline(lua_State *L)
/* outline = glyph:outline() */
    {
    FT_OutlineGlyphRec *rec;
    FT_Glyph *glyph = checkglyph(L, 1);
    if((*glyph)->format != FT_GLYPH_FORMAT_OUTLINE)
        return luaL_argerror(L, 1, "glyph format is not 'outline'");
    rec = (FT_OutlineGlyphRec*)(*glyph);
    pushoutline(L, &(rec->outline), rec->root.library);
    return 1;
    }


static int Glyph_stroke(lua_State *L) //@@
    {
    FT_Glyph *glyph = checkglyph(L, 1);
    FT_Stroker *stroker = checkstroker(L, 2);
    FT_Bool destroy = optboolean(L, 3, 1);
    FT_Error ec = FT_Glyph_Stroke(glyph, *stroker, destroy);
    CheckError(L, ec);
    return 0;
    }

static int Glyph_strokeborder(lua_State *L) //@@
    {
    FT_Glyph *glyph = checkglyph(L, 1);
    FT_Stroker *stroker = checkstroker(L, 2);
    FT_Bool inside = optboolean(L, 3, 1);
    FT_Bool destroy = optboolean(L, 3, 1);
    FT_Error ec = FT_Glyph_StrokeBorder(glyph, *stroker, inside, destroy);
    CheckError(L, ec);
    return 0;
    }

/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        { "copy", Glyph_copy },
        { "get_cbox", Glyph_get_cbox },
        { "to_bitmap", Glyph_to_bitmap },
        { "transform", Glyph_transform },
        { "stroke", Glyph_stroke },
        { "strokeborder", Glyph_strokeborder },
        { "format", Format },
        { "advance", Advance },
        { "bitmap", Bitmap },
        { "bitmapdata", Bitmapdata },
        { "outline", Outline },
        { "outlinedata", Outlinedata },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "get_glyph", Get_glyph },
        { "done_glyph", Delete },
        { "glyph_copy", Glyph_copy },
        { "glyph_get_cbox", Glyph_get_cbox },
        { "glyph_to_bitmap", Glyph_to_bitmap },
        { "glyph_transform", Glyph_transform },
        { "glyph_stroke", Glyph_stroke },
        { "glyph_strokeborder", Glyph_strokeborder },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg FaceMethods[] = 
    {
        { "get_glyph", Get_glyph },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_glyph(lua_State *L)
    {
    udata_addmethods(L, FACE_MT, FaceMethods);
    udata_define(L, GLYPH_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

