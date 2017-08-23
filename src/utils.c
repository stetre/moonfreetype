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

int noprintf(const char *fmt, ...) 
    { (void)fmt; return 0; }

int notavailable(lua_State *L, ...) 
    { NOT_AVAILABLE; }

/*------------------------------------------------------------------------------*
 | Malloc                                                                       |
 *------------------------------------------------------------------------------*/

/* We do not use malloc(), free() etc directly. Instead, we inherit the memory
 * allocator from the main Lua state instead (see lua_getallocf in the Lua manual)
 * and use that.
 *
 * By doing so, we can use an alternative malloc() implementation without recompiling
 * this library (we have needs to recompile lua only, or execute it with LD_PRELOAD
 * set to the path to the malloc library we want to use).
 */
static lua_Alloc Alloc = NULL;
static void* AllocUd = NULL;

static void malloc_init(lua_State *L)
    {
    if(Alloc) unexpected(L);
    Alloc = lua_getallocf(L, &AllocUd);
    }

static void* Malloc_(size_t size)
    { return Alloc ? Alloc(AllocUd, NULL, 0, size) : NULL; }

static void Free_(void *ptr)
    { if(Alloc) Alloc(AllocUd, ptr, 0, 0); }

void *Malloc(lua_State *L, size_t size)
    {
    void *ptr = Malloc_(size);
    if(ptr==NULL)
        { luaL_error(L, errstring(ERR_MEMORY)); return NULL; }
    memset(ptr, 0, size);
    //DBG("Malloc %p\n", ptr);
    return ptr;
    }

void *MallocNoErr(lua_State *L, size_t size) /* do not raise errors (check the retval) */
    {
    void *ptr = Malloc_(size);
    (void)L;
    if(ptr==NULL)
        return NULL;
    memset(ptr, 0, size);
    //DBG("MallocNoErr %p\n", ptr);
    return ptr;
    }

char *Strdup(lua_State *L, const char *s)
    {
    size_t len = strnlen(s, 256);
    char *ptr = (char*)Malloc(L, len + 1);
    if(len>0)
        memcpy(ptr, s, len);
    ptr[len]='\0';
    return ptr;
    }

void Free(lua_State *L, void *ptr)
    {
    (void)L;
    //DBG("Free %p\n", ptr);
    if(ptr) Free_(ptr);
    }


/*------------------------------------------------------------------------------*
 | userdata handling                                                            |
 *------------------------------------------------------------------------------*/

ud_t *newuserdata(lua_State *L, void *ptr, const char *mt)
    {
    ud_t *ud;
    ud = (ud_t*)udata_new(L, sizeof(ud_t), ptr, mt);
    memset(ud, 0, sizeof(ud_t));
    ud->obj = ptr ? ptr : (void*)ud;
    return ud;
    }

int freeuserdata(lua_State *L, void *ptr)
    {
    ud_t *ud = userdata(ptr);
    if(!ud)
        return 0; /* already deleted */
    ud->obj = NULL;
    udata_free(L, ptr);
    return 1;
    }

/*------------------------------------------------------------------------------*
 | Custom luaL_checkxxx() style functions                                       |
 *------------------------------------------------------------------------------*/

int checkboolean(lua_State *L, int arg)
    {
    if(!lua_isboolean(L, arg))
        return (int)luaL_argerror(L, arg, "boolean expected");
    return lua_toboolean(L, arg);
    }

int optboolean(lua_State *L, int arg, int d)
    {
    if(!lua_isboolean(L, arg))
        return d;
    return lua_toboolean(L, arg);
    }

/* 1-based index to 0-based ------------------------------------------*/


int checkindex(lua_State *L, int arg)
    {
    int val = luaL_checkinteger(L, arg);
    if(val < 1)
        return luaL_argerror(L, arg, "positive integer expected");
    return val - 1;
    }

int optindex(lua_State *L, int arg, int optval /* 0-based */)
    {
    int val = luaL_optinteger(L, arg, optval + 1);
    if(val < 1)
        return luaL_argerror(L, arg, "positive integer expected");
    return val - 1;
    }

void pushindex(lua_State *L, int val)
    { lua_pushinteger((L), (val) + 1); }

/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

FT_ULong checkcharcode(lua_State *L, int arg)
/* a charcode may be given as an integer code, or as a character (e.g. 'X') */
    {
    const char *c;
    size_t len;
    FT_ULong char_code;
    if(lua_type(L, arg) == LUA_TNUMBER)
        char_code = luaL_checkinteger(L, arg);
    else
        {
        c = luaL_checklstring(L, arg, &len);
        if(len != 1)
            return luaL_argerror(L, arg, "invalid length, must be 1");
        char_code = c[0];
        }
    return char_code;
    }



static int pushsubglyph(lua_State *L, FT_GlyphSlot glyph, FT_UInt sub_index)
    {
    FT_Error ec;
    FT_Int p_index, p_arg1, p_arg2;
    FT_UInt p_flags;
    FT_Matrix p_transform;
    ec = FT_Get_SubGlyph_Info(glyph,sub_index,&p_index,&p_flags,&p_arg1,&p_arg2, &p_transform);
    CheckError(L, ec);
    lua_newtable(L);
    pushindex(L, p_index); /* glyph_index */
    lua_setfield(L, -2, "index");
    pushsubglyphflags(L, p_flags, 1);
    lua_setfield(L, -2, "flags");
    lua_pushinteger(L, p_arg1);
    lua_setfield(L, -2, "arg1");
    lua_pushinteger(L, p_arg2);
    lua_setfield(L, -2, "arg2");
    pushmatrix(L, p_transform);
    lua_setfield(L, -2, "transform");
    return 1;
    }


int pushglyphslot(lua_State *L, FT_GlyphSlot glyph)
    {
    FT_UInt i;

    lua_newtable(L);
    pushglyphmetrics(L, glyph->metrics);
    lua_setfield(L, -2, "metrics");
    lua_pushinteger(L, glyph->linearHoriAdvance);
    lua_setfield(L, -2, "linear_hori_advance");
    lua_pushinteger(L, glyph->linearVertAdvance);
    lua_setfield(L, -2, "linear_vert_advance");
    pushvector(L, glyph->advance);
    lua_setfield(L, -2, "advance");
    pushglyphformat(L, glyph->format);
    lua_setfield(L, -2, "format");
    if(glyph->format == FT_GLYPH_FORMAT_BITMAP)
        {
        pushbitmapdata(L, glyph->bitmap, glyph->bitmap_left, glyph->bitmap_top, 1);
        lua_setfield(L, -2, "bitmap");
        }
    else if((glyph->format == FT_GLYPH_FORMAT_OUTLINE)||(glyph->format == FT_GLYPH_FORMAT_PLOTTER))
        {
        pushoutlinedata(L, glyph->outline);
        lua_setfield(L, -2, "outline");
        }
    else if(glyph->format == FT_GLYPH_FORMAT_COMPOSITE)
        {
#if 0
        lua_pushinteger(L, glyph->num_subglyphs);
        lua_setfield(L, -2, "num_subglyphs");
#endif
        lua_newtable(L);
        for(i = 0; i <  glyph->num_subglyphs; i++)
            {
            pushsubglyph(L, glyph, i);
            lua_seti(L, -2, i+1);
            }
        lua_setfield(L, -2, "subglyphs");
        }

    if(glyph->control_len > 0)
        {
#if 0
        lua_pushinteger(L, glyph->control_len );
        lua_setfield(L, -2, "control_len");
#endif
        lua_pushlstring(L, (char*)glyph->control_data, (size_t)glyph->control_len);
        lua_setfield(L, -2, "control_data");
        }
    lua_pushinteger(L, glyph->lsb_delta);
    lua_setfield(L, -2, "lsb_delta");
    lua_pushinteger(L, glyph->rsb_delta);
    lua_setfield(L, -2, "rsb_delta");
    // glyph->other: not supported (need the size...)
    return 1;
    }


int pushglyphmetrics(lua_State *L, FT_Glyph_Metrics metrics)
    {
    lua_newtable(L);
    lua_pushinteger(L, metrics.width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, metrics.height);
    lua_setfield(L, -2, "height");
    lua_pushinteger(L, metrics.horiBearingX);
    lua_setfield(L, -2, "hori_bearing_x");
    lua_pushinteger(L, metrics.horiBearingY);
    lua_setfield(L, -2, "hori_bearing_y");
    lua_pushinteger(L, metrics.horiAdvance);
    lua_setfield(L, -2, "hori_advance");
    lua_pushinteger(L, metrics.vertBearingX);
    lua_setfield(L, -2, "vert_bearing_x");
    lua_pushinteger(L, metrics.vertBearingY);
    lua_setfield(L, -2, "vert_bearing_y");
    lua_pushinteger(L, metrics.vertAdvance);
    lua_setfield(L, -2, "vert_advance");
    return 1;   
    }


int pushbbox(lua_State *L, FT_BBox bbox)
    {
    lua_newtable(L);
    lua_pushinteger(L, bbox.xMin);
    lua_setfield(L, -2, "xmin");
    lua_pushinteger(L, bbox.xMax);
    lua_setfield(L, -2, "xmax");
    lua_pushinteger(L, bbox.yMin);
    lua_setfield(L, -2, "ymin");
    lua_pushinteger(L, bbox.yMax);
    lua_setfield(L, -2, "ymax");
    return 1;   
    }


int pushsizemetrics(lua_State *L, FT_Size size)
    {
    lua_newtable(L);
    lua_pushinteger(L, size->metrics.x_ppem);
    lua_setfield(L, -2, "x_ppem");
    lua_pushinteger(L, size->metrics.y_ppem);
    lua_setfield(L, -2, "y_ppem");
    lua_pushinteger(L, size->metrics.x_scale);
    lua_setfield(L, -2, "x_scale");
    lua_pushinteger(L, size->metrics.y_scale);
    lua_setfield(L, -2, "y_scale");
    lua_pushinteger(L, size->metrics.ascender);
    lua_setfield(L, -2, "ascender");
    lua_pushinteger(L, size->metrics.descender);
    lua_setfield(L, -2, "descender");
    lua_pushinteger(L, size->metrics.height);
    lua_setfield(L, -2, "height");
    lua_pushinteger(L, size->metrics.max_advance);
    lua_setfield(L, -2, "max_advance");
    return 1;
    }

int pushbitmapsize(lua_State *L, FT_Bitmap_Size size)
    {
    lua_newtable(L);
    lua_pushinteger(L, size.height);
    lua_setfield(L, -2, "height");
    lua_pushinteger(L, size.width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, size.size);
    lua_setfield(L, -2, "size");
    lua_pushinteger(L, size.x_ppem);
    lua_setfield(L, -2, "x_ppem");
    lua_pushinteger(L, size.y_ppem);
    lua_setfield(L, -2, "y_ppem");
    return 1;
    }


/*------------------------------------------------------------------------------*
 | Internal error codes                                                         |
 *------------------------------------------------------------------------------*/

const char* errstring(int err)
    {
    switch(err)
        {
        case 0: return "success";
        case ERR_GENERIC: return "generic error";
        case ERR_TABLE: return "not a table";
        case ERR_EMPTY: return "empty list";
        case ERR_TYPE: return "invalid type";
        case ERR_VALUE: return "invalid value";
        case ERR_NOTPRESENT: return "missing";
        case ERR_MEMORY: return "out of memory";
        case ERR_LENGTH: return "invalid length";
        case ERR_POOL: return "elements are not from the same pool";
        case ERR_BOUNDARIES: return "invalid boundaries";
        case ERR_UNKNOWN: return "unknown field name";
        default:
            return "???";
        }
    return NULL; /* unreachable */
    }

/*------------------------------------------------------------------------------*
 | Inits                                                                        |
 *------------------------------------------------------------------------------*/

void moonfreetype_utils_init(lua_State *L)
    {
    malloc_init(L);
    }




