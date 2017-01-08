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
#include FT_TRUETYPE_IDS_H


int pushcharmap(lua_State *L, FT_CharMap map)
    {
    lua_newtable(L);
    pushencoding(L, map->encoding);
    lua_setfield(L, -2, "encoding");
    lua_pushinteger(L, map->platform_id);
    lua_setfield(L, -2, "platform_id");
    lua_pushinteger(L, map->encoding_id);
    lua_setfield(L, -2, "encoding_id");
    lua_pushstring(L, platformstring(map->platform_id));
    lua_setfield(L, -2, "platform_name");
    lua_pushstring(L, encodingstring(map->platform_id, map->encoding_id));
    lua_setfield(L, -2, "encoding_name");
    return 1;
    }

const char* platformstring(unsigned int id)
    {
    switch(id)
        {
        case TT_PLATFORM_APPLE_UNICODE: return "apple unicode";
        case TT_PLATFORM_MACINTOSH: return "macintosh";
        case TT_PLATFORM_ISO: return "iso"; /* deprecated */
        case TT_PLATFORM_MICROSOFT: return "microsoft";
        case TT_PLATFORM_CUSTOM: return "custom";
        case TT_PLATFORM_ADOBE: return "adobe";
        default:
            return "unknown platform id";
        }
    return NULL; /* unreachable */
    }

static const char* encodingstring_apple(unsigned int id)
    {
    switch(id)
        {
        case TT_APPLE_ID_DEFAULT: return "default";
        case TT_APPLE_ID_UNICODE_1_1: return "unicode 1 1";
        case TT_APPLE_ID_ISO_10646: return "iso 10646";
        case TT_APPLE_ID_UNICODE_2_0: return "unicode 2 0";
        case TT_APPLE_ID_UNICODE_32: return "unicode 32";
        case TT_APPLE_ID_VARIANT_SELECTOR: return "variant selector";
        default:
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }

static const char* encodingstring_macintosh(unsigned int id)
    {
    switch(id)
        {
        default:
        case TT_MAC_ID_ROMAN: return "roman";
        case TT_MAC_ID_JAPANESE: return "japanese";
        case TT_MAC_ID_TRADITIONAL_CHINESE: return "traditional chinese";
        case TT_MAC_ID_KOREAN: return "korean";
        case TT_MAC_ID_ARABIC: return "arabic";
        case TT_MAC_ID_HEBREW: return "hebrew";
        case TT_MAC_ID_GREEK: return "greek";
        case TT_MAC_ID_RUSSIAN: return "russian";
        case TT_MAC_ID_RSYMBOL: return "rsymbol";
        case TT_MAC_ID_DEVANAGARI: return "devanagari";
        case TT_MAC_ID_GURMUKHI: return "gurmukhi";
        case TT_MAC_ID_GUJARATI: return "gujarati";
        case TT_MAC_ID_ORIYA: return "oriya";
        case TT_MAC_ID_BENGALI: return "bengali";
        case TT_MAC_ID_TAMIL: return "tamil";
        case TT_MAC_ID_TELUGU: return "telugu";
        case TT_MAC_ID_KANNADA: return "kannada";
        case TT_MAC_ID_MALAYALAM: return "malayalam";
        case TT_MAC_ID_SINHALESE: return "sinhalese";
        case TT_MAC_ID_BURMESE: return "burmese";
        case TT_MAC_ID_KHMER: return "khmer";
        case TT_MAC_ID_THAI: return "thai";
        case TT_MAC_ID_LAOTIAN: return "laotian";
        case TT_MAC_ID_GEORGIAN: return "georgian";
        case TT_MAC_ID_ARMENIAN: return "armenian";
        case TT_MAC_ID_MALDIVIAN: return "maldivian";
        //case TT_MAC_ID_SIMPLIFIED_CHINESE: return "simplified chinese";
        case TT_MAC_ID_TIBETAN: return "tibetan";
        case TT_MAC_ID_MONGOLIAN: return "mongolian";
        case TT_MAC_ID_GEEZ: return "geez";
        case TT_MAC_ID_SLAVIC: return "slavic";
        case TT_MAC_ID_VIETNAMESE: return "vietnamese";
        case TT_MAC_ID_SINDHI: return "sindhi";
        case TT_MAC_ID_UNINTERP: return "uninterp";
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }


static const char* encodingstring_iso(unsigned int id)
    {
    switch(id)
        {
        case TT_ISO_ID_7BIT_ASCII: return "7bit ascii";
        case TT_ISO_ID_10646: return "10646";
        case TT_ISO_ID_8859_1: return "8859 1";
        default:
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }


static const char* encodingstring_microsoft(unsigned int id)
    {
    switch(id)
        {
        case TT_MS_ID_SYMBOL_CS: return "symbol cs";
        case TT_MS_ID_UNICODE_CS: return "unicode cs";
        case TT_MS_ID_SJIS: return "sjis";
        case TT_MS_ID_GB2312: return "gb2312";
        case TT_MS_ID_BIG_5: return "big 5";
        case TT_MS_ID_WANSUNG: return "wansung";
        case TT_MS_ID_JOHAB: return "johab";
        case TT_MS_ID_UCS_4: return "ucs 4";
        default:
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }


static const char* encodingstring_custom(unsigned int id)
    {
    switch(id)
        {
        default:
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }


static const char* encodingstring_adobe(unsigned int id)
    {
    switch(id)
        {
        case TT_ADOBE_ID_STANDARD: return "standard";
        case TT_ADOBE_ID_EXPERT: return "expert";
        case TT_ADOBE_ID_CUSTOM: return "custom";
        case TT_ADOBE_ID_LATIN_1: return "latin 1";
        default:
            return "unknown encoding id";
        }
    return NULL; /* unreachable */
    }

const char* encodingstring(unsigned int platform_id, unsigned int id)
    {
    switch(platform_id)
        {
        case TT_PLATFORM_APPLE_UNICODE: return encodingstring_apple(id);
        case TT_PLATFORM_MACINTOSH: return encodingstring_macintosh(id);
        case TT_PLATFORM_ISO: return encodingstring_iso(id);
        case TT_PLATFORM_MICROSOFT: return encodingstring_microsoft(id);
        case TT_PLATFORM_CUSTOM: return encodingstring_custom(id);
        case TT_PLATFORM_ADOBE: return encodingstring_adobe(id);
        default:
            return "unknown platform id";
        }
    return NULL; /* unreachable */
    }

