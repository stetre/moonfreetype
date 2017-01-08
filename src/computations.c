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
#include FT_TRIGONOMETRY_H
#include FT_GLYPH_H


/*------------------------------------------------------------------------------*
 | Check & push                                                                 |
 *------------------------------------------------------------------------------*/

int checkvector(lua_State *L, int arg, FT_Vector *dst)
    {
    if(!lua_istable(L, arg))
        return luaL_argerror(L, arg, "table expected");
    lua_getfield(L, arg, "x"); dst->x = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "y"); dst->y = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return 1;
    }

int pushvector(lua_State *L, FT_Vector vector)
    {
    lua_newtable(L);
    lua_pushinteger(L, vector.x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, vector.y);
    lua_setfield(L, -2, "y");
    return 1;   
    }

int checkmatrix(lua_State *L, int arg, FT_Matrix *dst)
    {
    if(!lua_istable(L, arg))
        return luaL_argerror(L, arg, "table expected");
    lua_getfield(L, arg, "xx"); dst->xx = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "xy"); dst->xy = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "yx"); dst->yx = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "yy"); dst->yy = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return 0;
    }


int pushmatrix(lua_State *L, FT_Matrix matrix)
    {
    lua_newtable(L);
    lua_pushinteger(L, matrix.xx);
    lua_setfield(L, -2, "xx");
    lua_pushinteger(L, matrix.xy);
    lua_setfield(L, -2, "xy");
    lua_pushinteger(L, matrix.yx);
    lua_setfield(L, -2, "yx");
    lua_pushinteger(L, matrix.yy);
    lua_setfield(L, -2, "yy");
    return 1;   
    }

/*------------------------------------------------------------------------------*
 | Functions                                                                    |
 *------------------------------------------------------------------------------*/


static int Muldiv(lua_State *L)
    {
    FT_Long a = luaL_checkinteger(L, 1);
    FT_Long b = luaL_checkinteger(L, 2);
    FT_Long c = luaL_checkinteger(L, 3);
    lua_pushinteger(L, FT_MulDiv(a, b, c));
    return 1;
    }


static int Mulfix(lua_State *L)
    {
    FT_Long a = luaL_checkinteger(L, 1);
    FT_Long b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, FT_MulFix(a, b));
    return 1;
    }


static int Divfix(lua_State *L)
    {
    FT_Long a = luaL_checkinteger(L, 1);
    FT_Long b = luaL_checkinteger(L, 2);
    lua_pushinteger(L, FT_DivFix(a, b));
    return 1;
    }


static int Roundfix(lua_State *L)
    {
    FT_Fixed a = luaL_checkinteger(L, 1);
    lua_pushinteger(L, FT_RoundFix(a));
    return 1;
    }


static int Ceilfix(lua_State *L)
    {
    FT_Fixed a = luaL_checkinteger(L, 1);
    lua_pushinteger(L, FT_CeilFix(a));
    return 1;
    }


static int Floorfix(lua_State *L)
    {
    FT_Fixed a = luaL_checkinteger(L, 1);
    lua_pushinteger(L, FT_FloorFix(a));
    return 1;
    }


#define F(Func, func)                           \
static int Func(lua_State *L)                   \
    {                                           \
    FT_Angle angle = luaL_checkinteger(L, 1);   \
    lua_pushinteger(L, func(angle));            \
    return 1;                                   \
    }

F(Sin, FT_Sin)
F(Cos, FT_Cos)
F(Tan, FT_Tan)

#undef F


static int Atan2(lua_State *L)
    {
    FT_Fixed x = luaL_checkinteger(L, 1);
    FT_Fixed y = luaL_checkinteger(L, 2);
    lua_pushinteger(L, FT_Atan2(x, y));
    return 1;
    }


static int Angle_diff(lua_State *L)
    {
    FT_Angle a1 = luaL_checkinteger(L, 1);
    FT_Angle a2 = luaL_checkinteger(L, 2);
    lua_pushinteger(L, FT_Angle_Diff(a1, a2));
    return 1;
    }


static int Vector_unit(lua_State *L)
    {
    FT_Vector vec;
    FT_Angle a1 = luaL_checkinteger(L, 1);
    FT_Vector_Unit(&vec, a1);
    return pushvector(L, vec);
    }


static int Vector_rotate(lua_State *L)
    {
    FT_Vector vec;
    FT_Angle angle;
    checkvector(L, 1, &vec);
    angle = luaL_checkinteger(L, 2);
    FT_Vector_Rotate(&vec, angle);
    return pushvector(L, vec);
    }


static int Vector_length(lua_State *L)
    {
    FT_Vector vec;
    checkvector(L, 1, &vec);
    lua_pushinteger(L, FT_Vector_Length(&vec));
    return 1;
    }


static int Vector_polarize(lua_State *L)
    {
    FT_Vector vec;
    FT_Fixed length;
    FT_Angle angle;
    checkvector(L, 1, &vec);
    FT_Vector_Polarize(&vec, &length, &angle);
    lua_pushinteger(L, length);
    lua_pushinteger(L, angle);
    return 2;
    }


static int Vector_from_polar(lua_State *L)
    {
    FT_Vector vec;
    FT_Fixed length = luaL_checkinteger(L, 1);
    FT_Angle angle = luaL_checkinteger(L, 2);
    FT_Vector_From_Polar(&vec, length, angle);
    return pushvector(L, vec);
    }


static int Vector_transform(lua_State *L)
    {
    FT_Vector vec;
    FT_Matrix mat;
    checkvector(L, 1, &vec);
    checkmatrix(L, 2, &mat);
    FT_Vector_Transform(&vec, &mat);
    return pushvector(L, vec);
    }


static int Matrix_multiply(lua_State *L)
    {
    FT_Matrix a;
    FT_Matrix b;
    checkmatrix(L, 1, &a);
    checkmatrix(L, 1, &b);
    FT_Matrix_Multiply(&a, &b);
    return pushmatrix(L, b);
    }


static int Matrix_invert(lua_State *L)
    {
    FT_Error ec;
    FT_Matrix mat;
    checkmatrix(L, 1, &mat);
    ec = FT_Matrix_Invert(&mat);
    CheckError(L, ec);
    return pushmatrix(L, mat);
    }


static int AddConstants(lua_State *L)
    {
#define ADD(c) do { lua_pushinteger(L, FT_##c); lua_setfield(L, -2, #c); } while(0)
    /* FT_ANGLE_XX */
    ADD(ANGLE_PI);
    ADD(ANGLE_2PI);
    ADD(ANGLE_PI2);
    ADD(ANGLE_PI4);
#undef ADD
    return 0;
    }


/*------------------------------------------------------------------------------*
 | Registration                                                                 |
 *------------------------------------------------------------------------------*/

static const struct luaL_Reg Functions[] = 
    {
        { "muldiv", Muldiv },
        { "mulfix", Mulfix },
        { "divfix", Divfix },
        { "roundfix", Roundfix },
        { "ceilfix", Ceilfix },
        { "floorfix", Floorfix },
        { "sin", Sin },
        { "cos", Cos },
        { "tan", Tan },
        { "atan2", Atan2 },
        { "angle_diff", Angle_diff },
        { "vector_unit", Vector_unit },
        { "vector_rotate", Vector_rotate },
        { "vector_length", Vector_length },
        { "vector_polarize", Vector_polarize },
        { "vector_from_polar", Vector_from_polar },
        { "vector_transform", Vector_transform },
        { "matrix_multiply", Matrix_multiply },
        { "matrix_invert", Matrix_invert },
        { NULL, NULL } /* sentinel */
    };


void moonfreetype_open_computations(lua_State *L)
    {
    AddConstants(L);
    luaL_setfuncs(L, Functions, 0);
    }

