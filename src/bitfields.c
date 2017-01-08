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


/*----------------------------------------------------------------------*
 | FT_FSTYPE_XXX                                                        |
 *----------------------------------------------------------------------*/

unsigned int checkfstypeflags(lua_State *L, int arg) 
/* Accepts an integer code or a list of strings starting from index=arg */
    {
    const char *s;
    unsigned int flags = 0;
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return (unsigned int)luaL_checkinteger(L, arg);

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
        return (unsigned int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

int pushfstypeflags(lua_State *L, unsigned int flags, int pushcode)
/* Pushes an integer code or a list of strings */
    {
    unsigned int n = 0;

    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

    if(flags==FT_FSTYPE_INSTALLABLE_EMBEDDING) 
        { lua_pushstring(L, "installable embedding"); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        /*CASE(FT_FSTYPE_INSTALLABLE_EMBEDDING, "installable embedding"); */
        CASE(FT_FSTYPE_RESTRICTED_LICENSE_EMBEDDING, "restricted license embedding");
        CASE(FT_FSTYPE_PREVIEW_AND_PRINT_EMBEDDING, "preview and print embedding");
        CASE(FT_FSTYPE_EDITABLE_EMBEDDING, "editable embedding");
        CASE(FT_FSTYPE_NO_SUBSETTING, "no subsetting");
        CASE(FT_FSTYPE_BITMAP_EMBEDDING_ONLY, "bitmap embedding only");
#undef CASE

    return n;
    }


/*----------------------------------------------------------------------*
 | FT_SUBGLYPH_FLAG_XXX                                                 |
 *----------------------------------------------------------------------*/

int checksubglyphflags(lua_State *L, int arg) 
/* Accepts an integer code or a list of strings starting from index=arg */
    {
    const char *s;
    int flags = 0;
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return (int)luaL_checkinteger(L, arg);

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
        return (int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

int pushsubglyphflags(lua_State *L, int flags, int pushcode)
/* Pushes an integer code or a list of strings */
    {
    int n = 0;

    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_WORDS, "args are words");
        CASE(FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES, "args are xy values");
        CASE(FT_SUBGLYPH_FLAG_ROUND_XY_TO_GRID, "round xy to grid");
        CASE(FT_SUBGLYPH_FLAG_SCALE, "scale");
        CASE(FT_SUBGLYPH_FLAG_XY_SCALE, "xy scale");
        CASE(FT_SUBGLYPH_FLAG_2X2, "2x2");
        CASE(FT_SUBGLYPH_FLAG_USE_MY_METRICS, "use my metrics");
#undef CASE
    if(flags==0) { lua_pushstring(L, "none"); n++; }

    return n;
    }


/*----------------------------------------------------------------------*
 | FT_OUTLINE_XXX                                                           |
 *----------------------------------------------------------------------*/

int checkoutlineflags(lua_State *L, int arg) 
/* Accepts an integer code or a list of strings starting from index=arg */
    {
    const char *s;
    int flags = FT_OUTLINE_NONE;
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return (int)luaL_checkinteger(L, arg);

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
        return (int)luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

int pushoutlineflags(lua_State *L, int flags, int pushcode)
/* Pushes an integer code or a list of strings */
    {
    int n = 0;

    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        /* CASE(FT_OUTLINE_NONE, "none"); */
        CASE(FT_OUTLINE_OWNER, "owner");
        CASE(FT_OUTLINE_EVEN_ODD_FILL, "even odd fill");
        CASE(FT_OUTLINE_REVERSE_FILL, "reverse fill");
        CASE(FT_OUTLINE_IGNORE_DROPOUTS, "ignore dropouts");
        CASE(FT_OUTLINE_SMART_DROPOUTS, "smart dropouts");
        CASE(FT_OUTLINE_INCLUDE_STUBS, "include stubs");
        CASE(FT_OUTLINE_HIGH_PRECISION, "high precision");
        CASE(FT_OUTLINE_SINGLE_PASS, "single pass");
#undef CASE
    if(flags==0) { lua_pushstring(L, "none"); n++; }

    return n;
    }

/*----------------------------------------------------------------------*
 | FT_LOAD_XXX                                                          |
 *----------------------------------------------------------------------*/

FT_Int32 checkloadflags(lua_State *L, int arg) 
/* Accepts an integer code or a list of strings starting from index=arg */
    {
    const char *s;
    FT_Int32 flags = FT_LOAD_DEFAULT;
    int target_set = 0;
    
    
    if(lua_type(L, arg) == LUA_TNUMBER)
        return (FT_Int32)luaL_checkinteger(L, arg);

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
#undef CASE
#define CASE(CODE,str) if((strcmp(s, str)==0)) do {                         \
            if(target_set)                                                  \
                return (FT_Int32)luaL_argerror(L, --arg, "duplicated target");  \
            target_set = 1;                                                 \
            flags |= CODE; goto done;                                       \
        } while(0)
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

int pushloadflags(lua_State *L, FT_Int32 flags_, int pushcode)
/* Pushes an integer code or a list of strings */
    {
#define TARGET_MASK (15 << 16)
    int n = 0;
    FT_Int32 flags = flags_ & ~TARGET_MASK;
    FT_Int32 target = flags_ & TARGET_MASK;

    if(pushcode)
        { lua_pushinteger(L, flags); return 1; }

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        /* CASE(FT_LOAD_DEFAULT,"default"); */
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
    if(flags==0) { lua_pushstring(L, "default"); n++; }

    switch(target)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); n++; break
        CASE(FT_LOAD_TARGET_NORMAL,"target normal");
        CASE(FT_LOAD_TARGET_LIGHT,"target light");
        CASE(FT_LOAD_TARGET_MONO,"target mono");
        CASE(FT_LOAD_TARGET_LCD,"target lcd");
        CASE(FT_LOAD_TARGET_LCD_V,"target lcd v");
#undef CASE
        default:
            return luaL_error(L, "invalid target");
        }
 
    return n;
#undef TARGET_MASK
    }


