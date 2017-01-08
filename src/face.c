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

FT_Face* testface(lua_State *L, int arg)
    {
    ud_t *ud = (ud_t*)udata_test(L, arg, FACE_MT);
    return (FT_Face*)(ud ? ud->obj : NULL);
    }

FT_Face* checkface(lua_State *L, int arg)
    {
    FT_Face *p = testface(L, arg);
    if(p) return p;
    luaL_argerror(L, arg, "not a face");
    return NULL;
    }

#if 0
static int pushface(lua_State *L, FT_Face *p)
    { return udata_push(L, p); }
#endif


/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/

int newface(lua_State *L)
/* face = new_face(filepathname [, index=0 [, instance=0]]) */
    {
    FT_Error ec;
    ud_t *ud;
    FT_Library *library = checklibrary(L, 1);
    const char* filepathname = luaL_checkstring(L, 2);
    FT_Long index = luaL_optinteger(L, 3, 0);
    FT_Long instance = luaL_optinteger(L, 3, 0);
    FT_Face *face = (FT_Face*)Malloc(L, sizeof(FT_Face));
    FT_Long face_index = ((instance & 0x00007fff) << 16) | (index & 0x0000ffff);
    

    ec = FT_New_Face(*library, filepathname, face_index, face);
    DBG("created face %p %s\n", (void*)face, filepathname);
    DBG("ec=%d\n", ec);

    if(ec)
        {
        Free(L, face);
        //return nil_fterror(L, ec);
        CheckError(L, ec); /* raises an error */
        return 0;
        }
    DBG("face glyph %p\n", (void*)(*face)->glyph);
    
    newuserdata(L, (void*)face, FACE_MT);
    ud = userdata(face);
    ud->library = *library;
    return 1;
    }

static int Freesize(lua_State *L, const void *size, const void *face)
    {
    ud_t *ud = userdata(size);
    if(ud->face == (FT_Face)face)
        freesize(L, (FT_Size*)size);
    return 0;
    }

int freeface(lua_State *L, FT_Face *face)
    {
    FT_Error ec;
    /* delete all contained objects */
    udata_scan(L, SIZE_MT, face, Freesize);
    if(!freeuserdata(L, face))
        return 0; /* double call */
    DBG("deleting face %p\n", (void*)face);
    ec = FT_Done_Face(*face);
    Free(L, face);
    CheckError(L, ec);
    return 0;
    }

static int Delete(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    return freeface(L, face);
    }

static int Load_char(lua_State *L)
/* glyph = load_char(charcode [, load_flags = "default"]) */
    {
    FT_Error ec;
    FT_Int32 load_flags;
    FT_Face *face = checkface(L, 1);
    FT_ULong char_code = checkcharcode(L, 2);
    load_flags = checkloadflags(L, 3);
    ec = FT_Load_Char(*face, char_code, load_flags);
    CheckError(L, ec);
    return 0;
    }

static int Load_glyph(lua_State *L)
/* glyph = load_glyph(glyph_index [, load_flags = "default"]) */
    {
    FT_Error ec;
    FT_Int32 load_flags;
    FT_Face *face = checkface(L, 1);
    FT_UInt glyph_index = checkindex(L, 2);
    load_flags = checkloadflags(L, 3);
    ec = FT_Load_Glyph(*face, glyph_index, load_flags);
    CheckError(L, ec);
    return 0;
    }

static int Glyph(lua_State *L)
/* glyph = glyph() */
    {
    FT_Face *face = checkface(L, 1);
    pushglyphslot(L, (*face)->glyph);
    return 1;
    }

static int Glyph_bitmap(lua_State *L)
/* bitmap, left, top = glyph_bitmap() */
    {
    FT_Face *face = checkface(L, 1);
    FT_GlyphSlot glyph = (*face)->glyph;
    ud_t *ud = userdata(face);
    FT_Library library = ud->library;
    if(glyph->format != FT_GLYPH_FORMAT_BITMAP)
        return 0;
    pushbitmap(L, &(glyph->bitmap), library);
    lua_pushinteger(L, glyph->bitmap_left);
    lua_pushinteger(L, glyph->bitmap_top);
    return 3;
    }

static int Glyph_outline(lua_State *L)
/* outline = glyph_outline() */
    {
    FT_Face *face = checkface(L, 1);
    FT_GlyphSlot glyph = (*face)->glyph;
    ud_t *ud = userdata(face);
    FT_Library library = ud->library;
    if(glyph->format != FT_GLYPH_FORMAT_OUTLINE)
        return 0;
    pushoutline(L, &(glyph->outline), library);
    return 1;
    }

static int Get_char_index(lua_State *L)
/* glyph_index = get_char_index(char_code) 
 * returns nil if character undefined */
    {
    FT_Face *face = checkface(L, 1);
    FT_ULong char_code = checkcharcode(L, 2);
    FT_UInt glyph_index = FT_Get_Char_Index(*face, char_code);
    if(!glyph_index)    
        return 0;
    pushindex(L, glyph_index);
    return 1;
    }

static int Get_name_index(lua_State *L)
/* glyph_index = get_name_index(name) 
 * returns nil if character undefined */
    {
    FT_Face *face = checkface(L, 1);
    FT_String *name = (FT_String*)luaL_checkstring(L, 2);
    FT_UInt glyph_index = FT_Get_Name_Index(*face, name);
    if(!glyph_index)    
        return 0;
    pushindex(L, glyph_index);
    return 1;
    }


static int Get_first_char(lua_State *L)
    {
    FT_ULong char_code;
    FT_UInt glyph_index;
    FT_Face *face = checkface(L, 1);
    char_code = FT_Get_First_Char(*face, &glyph_index);
    if(glyph_index==0)
        return 0;
    lua_pushinteger(L, char_code);
    lua_pushinteger(L, glyph_index);
    return 2;
    }

static int Get_next_char(lua_State *L)
    {
    FT_UInt glyph_index;
    FT_Face *face = checkface(L, 1);
    FT_ULong char_code = checkcharcode(L, 2);
    char_code = FT_Get_Next_Char(*face, char_code, &glyph_index);
    if(glyph_index==0)
        return 0;
    lua_pushinteger(L, char_code);
    lua_pushinteger(L, glyph_index);
    return 2;
    }


static int Select_size(lua_State *L)
    {
    FT_Error ec;
    FT_Face *face = checkface(L, 1);
    FT_Int index = checkindex(L, 2);
    ec = FT_Select_Size(*face, index);
    CheckError(L, ec);
    return 0;
    }

static int Request_size(lua_State *L)
    {
    FT_Error ec;
    FT_Size_RequestRec rec;
    FT_Face *face = checkface(L, 1);
    rec.type = (FT_Size_Request_Type)checksizerequesttype(L, 2);
    rec.width = luaL_checkinteger(L, 3);
    rec.height = luaL_checkinteger(L, 4);
    rec.horiResolution = luaL_checkinteger(L, 5);
    rec.vertResolution = luaL_checkinteger(L, 6);
    ec = FT_Request_Size(*face, &rec);
    CheckError(L, ec);
    return 0;
    }

static int Set_char_size(lua_State *L)
    {
    FT_Error ec;
    FT_Face *face = checkface(L, 1);
    FT_F26Dot6 char_width = luaL_checkinteger(L, 2);
    FT_F26Dot6 char_height = luaL_checkinteger(L, 3);
    FT_UInt horz_resolution = luaL_checkinteger(L, 4);
    FT_UInt vert_resolution = luaL_checkinteger(L, 5);
    ec = FT_Set_Char_Size(*face, char_width, char_height, horz_resolution, vert_resolution );
    CheckError(L, ec);
    return 0;
    }

static int Set_transform(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    FT_Matrix *matrix = NULL;
    FT_Vector *delta = NULL;
    FT_Matrix mat;
    FT_Vector vec;
    if(!lua_isnoneornil(L, 2))
        {
        checkmatrix(L, 2, &mat);
        matrix = &mat;
        }
    if(!lua_isnoneornil(L, 3))
        {
        checkvector(L, 2, &vec);
        delta = &vec;
        }
    FT_Set_Transform(*face, matrix, delta);
    return 0;
    }

static int Render_glyph(lua_State *L)
/* render_glyph(face, render_mode)  (acts on face->glyph) */
    {
    FT_Error ec;
    FT_Face *face = checkface(L, 1);
    FT_Render_Mode render_mode = checkrendermode(L, 2);
    ec = FT_Render_Glyph((*face)->glyph, render_mode); 
    CheckError(L, ec);
    return 0;
    }

#define MAXNAMELEN 128
static int Get_glyph_name(lua_State *L)
    {
    FT_Error ec;
    char name[MAXNAMELEN];
    FT_Face *face = checkface(L, 1);
    FT_UInt glyph_index = checkindex(L, 2);
    name[0]='\0';
    ec = FT_Get_Glyph_Name(*face, glyph_index, name, MAXNAMELEN);
    CheckError(L, ec);
    lua_pushstring(L, name);
    return 1;
    }

static int Select_charmap(lua_State *L)
/* select_charmap(encoding | index) 
 * index = index in charmaps (1-based) */
    {
    FT_Encoding encoding;
    FT_Int charmap_index;
    FT_Error ec;
    FT_Face *face = checkface(L, 1);
    int t = lua_type(L, 2);
    if(t == LUA_TSTRING)
        {
        encoding = (FT_Encoding)checkencoding(L, 2);
        ec = FT_Select_Charmap(*face, encoding);
        }
    else
        {
        charmap_index = checkindex(L, 2);
        if(charmap_index >= (*face)->num_charmaps)
            return luaL_argerror(L, 2, "out of range");
        ec = FT_Set_Charmap(*face, (*face)->charmaps[charmap_index]);
        }
    CheckError(L, ec);
    return 0;
    }

static int Get_fstype_flags(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    FT_UShort flags = FT_Get_FSType_Flags(*face);
    pushfstypeflags(L, flags, 0);
    return 1;
    }

#include FT_FONT_FORMATS_H
static int Get_font_format(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    const char *fmt = FT_Get_Font_Format(*face);
    if(!fmt)
        return 0;
    lua_pushstring(L, fmt);
    return 1;
    }

#if 0
        { "",  },
static int (lua_State *L) //@@
    {
    FT_Face *face = checkface(L, 1);
    return 0;
    }


#endif


/*------------------------------------------------------------------------------*
 | Methods                                                                      |
 *------------------------------------------------------------------------------*/

static int Set_pixel_sizes(lua_State *L)
    {
    FT_Error ec;
    FT_Face *face = checkface(L, 1);
    FT_UInt width = luaL_checkinteger(L, 2);
    FT_UInt height = luaL_checkinteger(L, 3);
    DBG("set_pixel_sizes %p, %d ,%d\n",(void*)face, width, height);
    ec = FT_Set_Pixel_Sizes(*face, width, height);
    CheckError(L, ec);
    return 0;
    }

/* face_flags --------------------------------------------------------- */

#define F(Func, func)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    FT_Face *face = checkface(L, 1);                    \
    (void)face; /* to avoid warnings (some func do not use it) */\
    lua_pushboolean(L, func((*face)));                  \
    return 1;                                           \
    }

F(Has_horizontal, FT_HAS_HORIZONTAL)
F(Has_vertical, FT_HAS_VERTICAL)
F(Has_kerning, FT_HAS_KERNING)
F(Is_scalable, FT_IS_SCALABLE)
F(Is_sfnt, FT_IS_SFNT)
F(Is_fixed_width, FT_IS_FIXED_WIDTH)
F(Has_fixed_sizes, FT_HAS_FIXED_SIZES)
F(Has_fast_glyphs, FT_HAS_FAST_GLYPHS)
F(Has_glyph_names, FT_HAS_GLYPH_NAMES)
F(Has_multiple_masters, FT_HAS_MULTIPLE_MASTERS)
F(Is_cid_keyed, FT_IS_CID_KEYED)
F(Is_tricky, FT_IS_TRICKY)
F(Has_color, FT_HAS_COLOR)

#undef F

/* style_flags --------------------------------------------------------- */

static int Is_italic(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    lua_pushboolean(L, (*face)->style_flags & FT_STYLE_FLAG_ITALIC);
    return 1;
    }

static int Is_bold(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    lua_pushboolean(L, (*face)->style_flags & FT_STYLE_FLAG_BOLD);
    return 1;
    }

/* names --------------------------------------------------------- */

static int Get_postscript_name (lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    const char *name = FT_Get_Postscript_Name(*face);
    if(!name) return 0;
    lua_pushstring(L, name);
    return 1;
    }


static int Family_name(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    if(!(*face)->family_name) return 0;
    lua_pushstring(L, (*face)->family_name);
    return 1;
    }

static int Style_name(lua_State *L)
    {
    FT_Face *face = checkface(L, 1);
    if(!(*face)->style_name) return 0;
    lua_pushstring(L, (*face)->style_name);
    return 1;
    }

/* integer attributes -------------------------------------------- */

#define F(Func, attr)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    FT_Face *face = checkface(L, 1);                    \
    lua_pushinteger(L, (*face)->attr);                  \
    return 1;                                           \
    }

F(Num_faces, num_faces)
F(Num_fixed_sizes, num_fixed_sizes)
F(Num_charmaps, num_charmaps)
F(Num_glyphs, num_glyphs)

F(Units_per_em, units_per_EM)
F(Ascender, ascender)
F(Descender, descender)
F(Height, height)
F(Max_advance_width, max_advance_width)
F(Max_advance_height, max_advance_height)
F(Underline_position, underline_position)
F(Underline_thickness, underline_thickness)
/*
        { "",  },
F(, )
*/

#undef F


static int Face_index(lua_State *L) 
/* index, instance = face_index() */
    {
    FT_Face *face = checkface(L, 1);
    FT_Long index = (*face)->face_index & 0x0000ffff;
    lua_pushinteger(L, index & 0x0000ffff);
    lua_pushinteger(L, (index & 0x7fff0000) >> 16);
    return 2;
    }

static int Bbox(lua_State *L) 
/* bbox = bbox() */
    {
    FT_Face *face = checkface(L, 1);
    pushbbox(L, (*face)->bbox);
    return 1;
    }

static int Charmap(lua_State *L) 
    {
    FT_Face *face = checkface(L, 1);
    pushcharmap(L, (*face)->charmap);
    return 1;
    }


static int Size(lua_State *L) 
/* size = size() */
    {
    FT_Face *face = checkface(L, 1);
    pushsizemetrics(L, (*face)->size);
    return 1;
    }


static int Available_sizes(lua_State *L) 
/* available_sizes = available_sizes () */
    {
    FT_Int i, n;
    FT_Face *face = checkface(L, 1);
    n = (*face)->num_fixed_sizes;
    if(n == 0)
        return 0;
    lua_newtable(L);
    for(i=0; i<n; i++)
        {
        pushbitmapsize(L, (*face)->available_sizes[i]);
        lua_seti(L, -2, i+1);
        }
    return 1;
    }


static int Charmaps(lua_State *L) 
    {
    FT_Int i, n;
    FT_Face *face = checkface(L, 1);
    n = (*face)->num_charmaps;
    if(n == 0)
        return 0;
    lua_newtable(L);
    for(i=0; i<n; i++)
        {
        pushcharmap(L, (*face)->charmaps[i]);
        lua_seti(L, -2, i+1);
        }
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Kerning                                                                      |
 *------------------------------------------------------------------------------*/

static int Get_kerning(lua_State *L)
    {
    FT_Error ec;
    FT_Vector vec;
    FT_Face *face = checkface(L, 1);
    FT_UInt left_glyph = checkindex(L, 2); /* glyph_index */
    FT_UInt right_glyph = checkindex(L, 3); /* glyph_index */
    FT_UInt kern_mode = checkkerningmode(L, 4);
    ec = FT_Get_Kerning(*face, left_glyph, right_glyph, kern_mode, &vec);
    CheckError(L, ec);
    return pushvector(L, vec);
    }

static int Get_track_kerning(lua_State *L)
    {
    FT_Error ec;
    FT_Fixed kerning;
    FT_Face *face = checkface(L, 1);
    FT_Fixed point_size = luaL_checkinteger(L, 2);
    FT_Int degree = luaL_checkinteger(L, 3);
    ec = FT_Get_Track_Kerning(*face, point_size, degree, &kerning);
    CheckError(L, ec);
    lua_pushinteger(L, kerning);
    return 1;
    }



/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

/* ft.xxx() functions also available as face:xxx() methods */
#define FUNCTIONS \
        { "set_pixel_sizes", Set_pixel_sizes }, \
        { "load_char", Load_char },             \
        { "load_glyph", Load_glyph },           \
        { "get_char_index", Get_char_index },   \
        { "get_name_index", Get_name_index },   \
        { "get_first_char", Get_first_char },   \
        { "get_next_char", Get_next_char },     \
        { "select_size", Select_size },         \
        { "request_size", Request_size },       \
        { "set_char_size", Set_char_size },     \
        { "set_transform", Set_transform },     \
        { "render_glyph", Render_glyph },       \
        { "get_glyph_name", Get_glyph_name },   \
        { "select_charmap", Select_charmap },   \
        { "get_fstype_flags", Get_fstype_flags },\
        { "get_font_format", Get_font_format }, \
        { "get_postscript_name", Get_postscript_name }, \
        { "get_kerning", Get_kerning }, \
        { "get_track_kerning", Get_track_kerning }, \


static const struct luaL_Reg Methods[] = 
    {
        { "done", Delete },
        FUNCTIONS
        { "glyph", Glyph },
        { "glyph_bitmap", Glyph_bitmap },
        { "glyph_outline", Glyph_outline },
        { "has_horizontal", Has_horizontal },
        { "has_vertical", Has_vertical },
        { "has_kerning", Has_kerning },
        { "is_scalable", Is_scalable },
        { "is_sfnt", Is_sfnt },
        { "is_fixed_width", Is_fixed_width },
        { "has_fixed_sizes", Has_fixed_sizes },
        { "has_fast_glyphs", Has_fast_glyphs },
        { "has_glyph_names", Has_glyph_names },
        { "has_multiple_masters", Has_multiple_masters },
        { "is_cid_keyed", Is_cid_keyed },
        { "is_tricky", Is_tricky },
        { "has_color",  Has_color},
        { "is_italic", Is_italic },
        { "is_bold", Is_bold },
        { "family_name", Family_name },
        { "style_name", Style_name },
        { "num_faces", Num_faces },
        { "num_fixed_sizes", Num_fixed_sizes },
        { "num_charmaps", Num_charmaps },
        { "num_glyphs", Num_glyphs },
        { "units_per_em", Units_per_em },
        { "ascender", Ascender },
        { "descender", Descender },
        { "height", Height },
        { "max_advance_width", Max_advance_width },
        { "max_advance_height", Max_advance_height },
        { "underline_position", Underline_position },
        { "underline_thickness", Underline_thickness },
        { "face_index", Face_index },
        { "bbox", Bbox },
        { "size", Size },
        { "available_sizes", Available_sizes },
        { "charmaps", Charmaps },
        { "charmap", Charmap },
        { NULL, NULL } /* sentinel */
    };


static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Delete },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "done_face", Delete },
        FUNCTIONS
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_face(lua_State *L)
    {
    udata_define(L, FACE_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }


