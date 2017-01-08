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
 | FT_Orientation                                                       |
 *----------------------------------------------------------------------*/

unsigned int checkorientation(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_ORIENTATION_FILL_RIGHT, "fill right"); /* FT_ORIENTATION_TRUETYPE */
        CASE(FT_ORIENTATION_FILL_LEFT, "fill left"); /* FT_ORIENTATION_POSTSCRIPT */
        CASE(FT_ORIENTATION_NONE, "none");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushorientation(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_ORIENTATION_FILL_RIGHT, "fill right"); /* FT_ORIENTATION_TRUETYPE */
        CASE(FT_ORIENTATION_FILL_LEFT, "fill left"); /* FT_ORIENTATION_POSTSCRIPT */
        CASE(FT_ORIENTATION_NONE, "none");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_StrokerBorder                                                     |
 *----------------------------------------------------------------------*/

unsigned int checkstrokerborder(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_STROKER_BORDER_LEFT, "left");
        CASE(FT_STROKER_BORDER_RIGHT, "right");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushstrokerborder(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_STROKER_BORDER_LEFT, "left");
        CASE(FT_STROKER_BORDER_RIGHT, "right");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Stroker_LineJoin                                                  |
 *----------------------------------------------------------------------*/

unsigned int checkstrokerlinejoin(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_STROKER_LINEJOIN_ROUND, "round");
        CASE(FT_STROKER_LINEJOIN_BEVEL, "bevel");
        CASE(FT_STROKER_LINEJOIN_MITER_VARIABLE, "miter variable");
        CASE(FT_STROKER_LINEJOIN_MITER_FIXED, "miter fixed");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushstrokerlinejoin(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_STROKER_LINEJOIN_ROUND, "round");
        CASE(FT_STROKER_LINEJOIN_BEVEL, "bevel");
        CASE(FT_STROKER_LINEJOIN_MITER_VARIABLE, "miter variable");
        CASE(FT_STROKER_LINEJOIN_MITER_FIXED, "miter fixed");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Stroker_LineCap                                                   |
 *----------------------------------------------------------------------*/

unsigned int checkstrokerlinecap(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_STROKER_LINECAP_BUTT, "butt");
        CASE(FT_STROKER_LINECAP_ROUND, "round");
        CASE(FT_STROKER_LINECAP_SQUARE, "square");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushstrokerlinecap(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_STROKER_LINECAP_BUTT, "butt");
        CASE(FT_STROKER_LINECAP_ROUND, "round");
        CASE(FT_STROKER_LINECAP_SQUARE, "square");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Glyph_BBox_Mode                                                   |
 *----------------------------------------------------------------------*/

unsigned int checkglyphbboxmode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_GLYPH_BBOX_UNSCALED, "unscaled");
        /* CASE(FT_GLYPH_BBOX_SUBPIXELS, "subpixels"); same as "unscaled" */
        CASE(FT_GLYPH_BBOX_GRIDFIT, "gridfit");
        CASE(FT_GLYPH_BBOX_TRUNCATE, "truncate");
        CASE(FT_GLYPH_BBOX_PIXELS, "pixels");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushbglyphboxmode(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_GLYPH_BBOX_UNSCALED, "unscaled");
        /* CASE(FT_GLYPH_BBOX_SUBPIXELS, "subpixels"); same as "unscaled" */
        CASE(FT_GLYPH_BBOX_GRIDFIT, "gridfit");
        CASE(FT_GLYPH_BBOX_TRUNCATE, "truncate");
        CASE(FT_GLYPH_BBOX_PIXELS, "pixels");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_TrueTypeEngineType                                                |
 *----------------------------------------------------------------------*/

unsigned int checktruetypeenginetype(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_TRUETYPE_ENGINE_TYPE_NONE, "none");
        CASE(FT_TRUETYPE_ENGINE_TYPE_PATENTED, "patented");
        CASE(FT_TRUETYPE_ENGINE_TYPE_UNPATENTED, "unpatented");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushtruetypeenginetype(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_TRUETYPE_ENGINE_TYPE_NONE, "none");
        CASE(FT_TRUETYPE_ENGINE_TYPE_PATENTED, "patented");
        CASE(FT_TRUETYPE_ENGINE_TYPE_UNPATENTED, "unpatented");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Size_Request_Type                                                 |
 *----------------------------------------------------------------------*/

unsigned int checksizerequesttype(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_SIZE_REQUEST_TYPE_BBOX, "bbox");
        CASE(FT_SIZE_REQUEST_TYPE_CELL, "cell");
        CASE(FT_SIZE_REQUEST_TYPE_NOMINAL, "nominal");
        CASE(FT_SIZE_REQUEST_TYPE_REAL_DIM, "real dim");
        CASE(FT_SIZE_REQUEST_TYPE_SCALES, "scales");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushsizerequesttype(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_SIZE_REQUEST_TYPE_BBOX, "bbox");
        CASE(FT_SIZE_REQUEST_TYPE_CELL, "cell");
        CASE(FT_SIZE_REQUEST_TYPE_NOMINAL, "nominal");
        CASE(FT_SIZE_REQUEST_TYPE_REAL_DIM, "real dim");
        CASE(FT_SIZE_REQUEST_TYPE_SCALES, "scales");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Kerning_Mode                                                      |
 *----------------------------------------------------------------------*/

unsigned int checkkerningmode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(FT_KERNING_DEFAULT, "default");
    CASE(FT_KERNING_UNFITTED, "unfitted");
    CASE(FT_KERNING_UNSCALED, "unscaled");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushkerningmode(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
    CASE(FT_KERNING_DEFAULT, "default");
    CASE(FT_KERNING_UNFITTED, "unfitted");
    CASE(FT_KERNING_UNSCALED, "unscaled");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Render_Mode                                                       |
 *----------------------------------------------------------------------*/

unsigned int checkrendermode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(FT_RENDER_MODE_NORMAL, "normal");
    CASE(FT_RENDER_MODE_LIGHT, "light");
    CASE(FT_RENDER_MODE_MONO, "mono");
    CASE(FT_RENDER_MODE_LCD, "lcd");
    CASE(FT_RENDER_MODE_LCD_V, "lcd v");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushrendermode(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
    CASE(FT_RENDER_MODE_NORMAL, "normal");
    CASE(FT_RENDER_MODE_LIGHT, "light");
    CASE(FT_RENDER_MODE_MONO, "mono");
    CASE(FT_RENDER_MODE_LCD, "lcd");
    CASE(FT_RENDER_MODE_LCD_V, "lcd v");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Encoding                                                          |
 *----------------------------------------------------------------------*/

unsigned int checkencoding(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
    CASE(FT_ENCODING_NONE, "none");
    CASE(FT_ENCODING_MS_SYMBOL, "ms symbol");
    CASE(FT_ENCODING_UNICODE, "unicode");
    CASE(FT_ENCODING_SJIS, "sjis");
    CASE(FT_ENCODING_GB2312, "gb2312");
    CASE(FT_ENCODING_BIG5, "big5");
    CASE(FT_ENCODING_WANSUNG, "wansung");
    CASE(FT_ENCODING_JOHAB, "johab");
    CASE(FT_ENCODING_ADOBE_STANDARD, "adobe standard");
    CASE(FT_ENCODING_ADOBE_EXPERT, "adobe expert");
    CASE(FT_ENCODING_ADOBE_CUSTOM, "adobe custom");
    CASE(FT_ENCODING_ADOBE_LATIN_1, "adobe latin 1");
    CASE(FT_ENCODING_APPLE_ROMAN, "apple roman");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushencoding(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
    CASE(FT_ENCODING_NONE, "none");
    CASE(FT_ENCODING_MS_SYMBOL, "ms symbol");
    CASE(FT_ENCODING_UNICODE, "unicode");
    CASE(FT_ENCODING_SJIS, "sjis");
    CASE(FT_ENCODING_GB2312, "gb2312");
    CASE(FT_ENCODING_BIG5, "big5");
    CASE(FT_ENCODING_WANSUNG, "wansung");
    CASE(FT_ENCODING_JOHAB, "johab");
    CASE(FT_ENCODING_ADOBE_STANDARD, "adobe standard");
    CASE(FT_ENCODING_ADOBE_EXPERT, "adobe expert");
    CASE(FT_ENCODING_ADOBE_CUSTOM, "adobe custom");
    CASE(FT_ENCODING_ADOBE_LATIN_1, "adobe latin 1");
    CASE(FT_ENCODING_APPLE_ROMAN, "apple roman");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }



/*----------------------------------------------------------------------*
 | FT_Glyph_Format                                                      |
 *----------------------------------------------------------------------*/

unsigned int checkglyphformat(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_GLYPH_FORMAT_NONE, "none");
        CASE(FT_GLYPH_FORMAT_COMPOSITE, "composite");
        CASE(FT_GLYPH_FORMAT_BITMAP, "bitmap");
        CASE(FT_GLYPH_FORMAT_OUTLINE, "outline");
        CASE(FT_GLYPH_FORMAT_PLOTTER, "plotter");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushglyphformat(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_GLYPH_FORMAT_NONE, "none");
        CASE(FT_GLYPH_FORMAT_COMPOSITE, "composite");
        CASE(FT_GLYPH_FORMAT_BITMAP, "bitmap");
        CASE(FT_GLYPH_FORMAT_OUTLINE, "outline");
        CASE(FT_GLYPH_FORMAT_PLOTTER, "plotter");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }


/*----------------------------------------------------------------------*
 | FT_Pixel_Mode                                                        |
 *----------------------------------------------------------------------*/

unsigned int checkpixelmode(lua_State *L, int arg)
    {
    const char *s = luaL_checkstring(L, arg);
#define CASE(CODE,str) if((strcmp(s, str)==0)) return CODE
        CASE(FT_PIXEL_MODE_NONE, "none");
        CASE(FT_PIXEL_MODE_MONO, "mono");
        CASE(FT_PIXEL_MODE_GRAY, "gray");
        CASE(FT_PIXEL_MODE_GRAY2, "gray2");
        CASE(FT_PIXEL_MODE_GRAY4, "gray4");
        CASE(FT_PIXEL_MODE_LCD, "lcd");
        CASE(FT_PIXEL_MODE_LCD_V, "lcd v");
        CASE(FT_PIXEL_MODE_BGRA, "bgra");
#undef CASE
    return (unsigned int)luaL_argerror(L, arg, badvalue(L,s));
    }

int pushpixelmode(lua_State *L, unsigned int value)
    {
    switch(value)
        {
#define CASE(CODE,str) case CODE: lua_pushstring(L, str); break
        CASE(FT_PIXEL_MODE_NONE, "none");
        CASE(FT_PIXEL_MODE_MONO, "mono");
        CASE(FT_PIXEL_MODE_GRAY, "gray");
        CASE(FT_PIXEL_MODE_GRAY2, "gray2");
        CASE(FT_PIXEL_MODE_GRAY4, "gray4");
        CASE(FT_PIXEL_MODE_LCD, "lcd");
        CASE(FT_PIXEL_MODE_LCD_V, "lcd v");
        CASE(FT_PIXEL_MODE_BGRA, "bgra");
#undef CASE
        default:
            return unexpected(L);
        }
    return 1;
    }

