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

#define ADD(c) do { lua_pushinteger(L, FT_##c); lua_setfield(L, -2, #c); } while(0)

/* checkkkflags: accepts a list of strings starting from index=arg
 * pushxxxflags -> pushes a list of strings 
 */

/*----------------------------------------------------------------------*
 | FT_FSTYPE_XXX                                                        |
 *----------------------------------------------------------------------*/

static FT_Int32 checkfstypeflags(lua_State *L, int arg) 
    {
    const char *s;
    FT_Int32 flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
    CASE(FT_FSTYPE_INSTALLABLE_EMBEDDING, "installable embedding");
    CASE(FT_FSTYPE_RESTRICTED_LICENSE_EMBEDDING, "restricted license embedding");
    CASE(FT_FSTYPE_PREVIEW_AND_PRINT_EMBEDDING, "preview and print embedding");
    CASE(FT_FSTYPE_EDITABLE_EMBEDDING, "editable embedding");
    CASE(FT_FSTYPE_NO_SUBSETTING, "no subsetting");
    CASE(FT_FSTYPE_BITMAP_EMBEDDING_ONLY, "bitmap embedding only");
#undef CASE
        return (FT_Int32)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushfstypeflags(lua_State *L, FT_Int32 flags)
    {
    int n = 0;

    if(flags==FT_FSTYPE_INSTALLABLE_EMBEDDING) 
        { lua_pushstring(L, "installable embedding"); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
//  CASE(FT_FSTYPE_INSTALLABLE_EMBEDDING, "installable embedding");  zero
    CASE(FT_FSTYPE_RESTRICTED_LICENSE_EMBEDDING, "restricted license embedding");
    CASE(FT_FSTYPE_PREVIEW_AND_PRINT_EMBEDDING, "preview and print embedding");
    CASE(FT_FSTYPE_EDITABLE_EMBEDDING, "editable embedding");
    CASE(FT_FSTYPE_NO_SUBSETTING, "no subsetting");
    CASE(FT_FSTYPE_BITMAP_EMBEDDING_ONLY, "bitmap embedding only");
#undef CASE

    return n;
    }

static int FsTypeFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushfstypeflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkfstypeflags(L, 1));
    return 1;
    }

#define Add_FsTypeFlags(L) \
    ADD(FSTYPE_INSTALLABLE_EMBEDDING);\
    ADD(FSTYPE_RESTRICTED_LICENSE_EMBEDDING);\
    ADD(FSTYPE_PREVIEW_AND_PRINT_EMBEDDING);\
    ADD(FSTYPE_EDITABLE_EMBEDDING);\
    ADD(FSTYPE_NO_SUBSETTING);\
    ADD(FSTYPE_BITMAP_EMBEDDING_ONLY);\


/*----------------------------------------------------------------------*
 | FT_SUBGLYPH_FLAG_XXX                                                 |
 *----------------------------------------------------------------------*/

static FT_Int32 checksubglyphflags(lua_State *L, int arg) 
    {
    const char *s;
    FT_Int32 flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_WORDS, "args are words");
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES, "args are xy values");
        CASE(FT_SUBGLYPH_FLAG_ROUND_XY_TO_GRID, "round xy to grid");
        CASE(FT_SUBGLYPH_FLAG_SCALE, "scale");
        CASE(FT_SUBGLYPH_FLAG_XY_SCALE, "xy scale");
        CASE(FT_SUBGLYPH_FLAG_2X2, "2x2");
        CASE(FT_SUBGLYPH_FLAG_USE_MY_METRICS, "use my metrics");
#undef CASE
        return (FT_Int32)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushsubglyphflags(lua_State *L, FT_Int32 flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_WORDS, "args are words");
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES, "args are xy values");
        CASE(FT_SUBGLYPH_FLAG_ROUND_XY_TO_GRID, "round xy to grid");
        CASE(FT_SUBGLYPH_FLAG_SCALE, "scale");
        CASE(FT_SUBGLYPH_FLAG_XY_SCALE, "xy scale");
        CASE(FT_SUBGLYPH_FLAG_2X2, "2x2");
        CASE(FT_SUBGLYPH_FLAG_USE_MY_METRICS, "use my metrics");
#undef CASE

    return n;
    }

static int SubglyphFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushsubglyphflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checksubglyphflags(L, 1));
    return 1;
    }

#define Add_SubglyphFlags(L) \
        ADD(SUBGLYPH_FLAG_ARGS_ARE_WORDS);\
        ADD(SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES);\
        ADD(SUBGLYPH_FLAG_ROUND_XY_TO_GRID);\
        ADD(SUBGLYPH_FLAG_SCALE);\
        ADD(SUBGLYPH_FLAG_XY_SCALE);\
        ADD(SUBGLYPH_FLAG_2X2);\
        ADD(SUBGLYPH_FLAG_USE_MY_METRICS);\


/*----------------------------------------------------------------------*
 | FT_OUTLINE_XXX                                                       |
 *----------------------------------------------------------------------*/

static FT_Int32 checkoutlineflags(lua_State *L, int arg) 
    {
    const char *s;
    FT_Int32 flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FT_OUTLINE_NONE, "none");
        CASE(FT_OUTLINE_OWNER, "owner");
        CASE(FT_OUTLINE_EVEN_ODD_FILL, "even odd fill");
        CASE(FT_OUTLINE_REVERSE_FILL, "reverse fill");
        CASE(FT_OUTLINE_IGNORE_DROPOUTS, "ignore dropouts");
        CASE(FT_OUTLINE_SMART_DROPOUTS, "smart dropouts");
        CASE(FT_OUTLINE_INCLUDE_STUBS, "include stubs");
        CASE(FT_OUTLINE_HIGH_PRECISION, "high precision");
        CASE(FT_OUTLINE_SINGLE_PASS, "single pass");
#undef CASE
        return (FT_Int32)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushoutlineflags(lua_State *L, FT_Int32 flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
//      CASE(FT_OUTLINE_NONE, "none"); zero
        CASE(FT_OUTLINE_OWNER, "owner");
        CASE(FT_OUTLINE_EVEN_ODD_FILL, "even odd fill");
        CASE(FT_OUTLINE_REVERSE_FILL, "reverse fill");
        CASE(FT_OUTLINE_IGNORE_DROPOUTS, "ignore dropouts");
        CASE(FT_OUTLINE_SMART_DROPOUTS, "smart dropouts");
        CASE(FT_OUTLINE_INCLUDE_STUBS, "include stubs");
        CASE(FT_OUTLINE_HIGH_PRECISION, "high precision");
        CASE(FT_OUTLINE_SINGLE_PASS, "single pass");
#undef CASE

    return n;
    }

static int OutlineFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushoutlineflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkoutlineflags(L, 1));
    return 1;
    }

#define Add_OutlineFlags(L) \
        ADD(OUTLINE_NONE);\
        ADD(OUTLINE_OWNER);\
        ADD(OUTLINE_EVEN_ODD_FILL);\
        ADD(OUTLINE_REVERSE_FILL);\
        ADD(OUTLINE_IGNORE_DROPOUTS);\
        ADD(OUTLINE_SMART_DROPOUTS);\
        ADD(OUTLINE_INCLUDE_STUBS);\
        ADD(OUTLINE_HIGH_PRECISION);\
        ADD(OUTLINE_SINGLE_PASS);\


/*----------------------------------------------------------------------*
 | FT_LOAD_XXX                                                          |
 *----------------------------------------------------------------------*/

static FT_Int32 checkloadflags(lua_State *L, int arg) 
    {
    const char *s;
    FT_Int32 flags = 0; // = FT_LOAD_DEFAULT
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FT_LOAD_DEFAULT,"default");
        CASE(FT_LOAD_NO_SCALE,"no scale");
        CASE(FT_LOAD_NO_HINTING,"no hinting");
        CASE(FT_LOAD_RENDER,"render");
        CASE(FT_LOAD_NO_BITMAP,"no bitmap");
        CASE(FT_LOAD_VERTICAL_LAYOUT,"vertical layout");
        CASE(FT_LOAD_FORCE_AUTOHINT,"force autohint");
        CASE(FT_LOAD_CROP_BITMAP,"crop bitmap");
        CASE(FT_LOAD_PEDANTIC,"pedantic");
        CASE(FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH,"ignore global advance width");
        CASE(FT_LOAD_NO_RECURSE,"no recurse");
        CASE(FT_LOAD_IGNORE_TRANSFORM,"ignore transform");
        CASE(FT_LOAD_MONOCHROME,"monochrome");
        CASE(FT_LOAD_LINEAR_DESIGN,"linear design");
        CASE(FT_LOAD_NO_AUTOHINT,"no autohint");
        CASE(FT_LOAD_COLOR,"color");
        CASE(FT_LOAD_COMPUTE_METRICS,"compute metrics");
        CASE(FT_ADVANCE_FLAG_FAST_ONLY, "fast only");
        CASE(FT_LOAD_TARGET_NORMAL,"target normal");
        CASE(FT_LOAD_TARGET_LIGHT,"target light");
        CASE(FT_LOAD_TARGET_MONO,"target mono");
        CASE(FT_LOAD_TARGET_LCD,"target lcd");
        CASE(FT_LOAD_TARGET_LCD_V,"target lcd v");
#undef CASE
        return (FT_Int32)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushloadflags(lua_State *L, FT_Int32 flags_)
    {
#define TARGET_MASK (15 << 16)
    int n = 0;
    FT_Int32 flags = flags_ & ~TARGET_MASK;
    FT_Int32 target = flags_ & TARGET_MASK;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
//      CASE(FT_LOAD_DEFAULT,"default");
        CASE(FT_LOAD_NO_SCALE,"no scale");
        CASE(FT_LOAD_NO_HINTING,"no hinting");
        CASE(FT_LOAD_RENDER,"render");
        CASE(FT_LOAD_NO_BITMAP,"no bitmap");
        CASE(FT_LOAD_VERTICAL_LAYOUT,"vertical layout");
        CASE(FT_LOAD_FORCE_AUTOHINT,"force autohint");
        CASE(FT_LOAD_CROP_BITMAP,"crop bitmap");
        CASE(FT_LOAD_PEDANTIC,"pedantic");
        CASE(FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH,"ignore global advance width");
        CASE(FT_LOAD_NO_RECURSE,"no recurse");
        CASE(FT_LOAD_IGNORE_TRANSFORM,"ignore transform");
        CASE(FT_LOAD_MONOCHROME,"monochrome");
        CASE(FT_LOAD_LINEAR_DESIGN,"linear design");
        CASE(FT_LOAD_NO_AUTOHINT,"no autohint");
        CASE(FT_LOAD_COLOR,"color");
        CASE(FT_LOAD_COMPUTE_METRICS,"compute metrics");
        CASE(FT_ADVANCE_FLAG_FAST_ONLY, "fast only");
#undef CASE
    if(flags==FT_LOAD_DEFAULT) { lua_pushstring(L, "default"); n++; }

#define CASE(CODE,str) do { if( target & CODE) { lua_pushstring(L, str); n++; } } while(0)
//      CASE(FT_LOAD_TARGET_NORMAL,"target normal");
        CASE(FT_LOAD_TARGET_LIGHT,"target light");
        CASE(FT_LOAD_TARGET_MONO,"target mono");
        CASE(FT_LOAD_TARGET_LCD,"target lcd");
        CASE(FT_LOAD_TARGET_LCD_V,"target lcd v");
#undef CASE
    if(target==FT_LOAD_TARGET_NORMAL) { lua_pushstring(L, "target normal"); n++; }
    return n;
    }

static int LoadFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushloadflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkloadflags(L, 1));
    return 1;
    }

#define Add_LoadFlags(L) \
        ADD(LOAD_DEFAULT);\
        ADD(LOAD_NO_SCALE);\
        ADD(LOAD_NO_HINTING);\
        ADD(LOAD_RENDER);\
        ADD(LOAD_NO_BITMAP);\
        ADD(LOAD_VERTICAL_LAYOUT);\
        ADD(LOAD_FORCE_AUTOHINT);\
        ADD(LOAD_CROP_BITMAP);\
        ADD(LOAD_PEDANTIC);\
        ADD(LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH);\
        ADD(LOAD_NO_RECURSE);\
        ADD(LOAD_IGNORE_TRANSFORM);\
        ADD(LOAD_MONOCHROME);\
        ADD(LOAD_LINEAR_DESIGN);\
        ADD(LOAD_NO_AUTOHINT);\
        ADD(LOAD_COLOR);\
        ADD(LOAD_COMPUTE_METRICS);\
        ADD(ADVANCE_FLAG_FAST_ONLY);\
        ADD(LOAD_TARGET_NORMAL);\
        ADD(LOAD_TARGET_LIGHT);\
        ADD(LOAD_TARGET_MONO);\
        ADD(LOAD_TARGET_LCD);\
        ADD(LOAD_TARGET_LCD_V);\


/*------------------------------------------------------------------------------*
 | Additional utilities                                                         |
 *------------------------------------------------------------------------------*/

static int AddConstants(lua_State *L) /* ft.XXX constants for FT_XXX values */
    {
    Add_FsTypeFlags(L);
    Add_SubglyphFlags(L);
    Add_OutlineFlags(L);
    Add_LoadFlags(L);
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "subglyphflags", SubglyphFlags },
        { "fstypeflags", FsTypeFlags },
        { "outlineflags", OutlineFlags },
        { "loadflags", LoadFlags },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_flags(lua_State *L)
    {
    AddConstants(L);
    luaL_setfuncs(L, Functions, 0);
    }


#if 0 // scaffolding

/*----------------------------------------------------------------------*
 | 
 *----------------------------------------------------------------------*/

static FT_Int32 checkzzz(lua_State *L, int arg) 
    {
    const char *s;
    FT_Int32 flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(FT_ZZZ_, "");
#undef CASE
        return (FT_Int32)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushzzz(lua_State *L, FT_Int32 flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(FT_ZZZ_, "");
#undef CASE

    return n;
    }

static int Zzz(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushzzz(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkzzz(L, 1));
    return 1;
    }

    Add_Zzz(L);
        { "zzz", Zzz },
#define Add_Zzz(L) \
    ADD(ZZZ_);\

#endif

