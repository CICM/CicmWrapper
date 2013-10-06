/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 *
 * Website  : http://www.mshparisnord.fr/HoaLibrary/
 * Contacts : cicm.mshparisnord@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef DEF_EPD_COMMON
#define DEF_EPD_COMMON


#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>



#ifdef _WIN32
#define snprintf _snprintf
#include <io.h>
#else
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <unistd.h>
#endif

extern "C"
{
#include "m_pd.h"
#include "m_imp.h"
#include "g_canvas.h"
#include "s_stuff.h"
}
#define EPD_PI  (3.141592653589793238462643383279502884)
#define EPD_2PI (6.283185307179586476925286766559005)
#define EPD_PI2 (1.57079632679489661923132169163975144)
#define EPD_PI4 (0.785398163397448309615660845819875721)
#define EPD_MAX_SIGS 256
#define A_LONG              A_FLOAT
#define A_SYM               A_SYMBOL
#define t_pd_err            long
#define ASSIST_INLET        1
#define ASSIST_OUTLET       2
#define E_NO_INPLACE 1
#define E_PUT_LAST 2
#define E_PUT_FIRST 4
#define E_IGNORE_DISABLE 8

#define atom_setfloat(atom, float)  SETFLOAT(atom, float)
#define atom_setlong(atom, long)    SETFLOAT(atom, (float)long)
#define atom_setsym(atom, sym)      SETSYMBOL(atom, sym)
#define atom_getlong(atom)          (long)atom_getfloat(atom)
#define atom_gettype(atom)          (atom)[0].a_type
#define atom_getsym(atom)           atom_getsymbol(atom)

#define object_error            pd_error
#define sys_getdspstate()       canvas_dspstate

#define layer_getname(layer) layer.c_name->s_name
#define layer_getsize(layer) layer.c_atom.size()

typedef void    (*method)(...);
typedef void*   (*rmethod)(...);
typedef t_pd_err (*t_err_method)(...);

union inletunion
{
    t_symbol *iu_symto;
    t_gpointer *iu_pointerslot;
    t_float *iu_floatslot;
    t_symbol **iu_symslot;
    t_blob **iu_blobslot; /* MP 20061226 blob type */
    t_float iu_floatsignalvalue;
};

struct _inlet
{
    t_pd i_pd;
    struct _inlet *i_next;
    t_object *i_owner;
    t_pd *i_dest;
    t_symbol *i_symfrom;
    union inletunion i_un;
};

struct _outconnect
{
    struct _outconnect *oc_next;
    t_pd *oc_to;
};

struct _outlet
{
    t_object *o_owner;
    struct _outlet *o_next;
    t_outconnect *o_connections;
    t_symbol *o_sym;
};

void outlet_int(void* outlet, int val);
void outlet_list(void* outlet, t_symbol *s, int argc, t_atom *argv);
void *listout(void *x);
void *floatout(void *x);
int obj_isfloatoutlet(t_object *x, int m);
int obj_isfloatinlet(t_object *x, int m);
void canvas_deletelines_for_io(t_canvas *x, t_text *text, t_inlet *inp, t_outlet *outp);

void* object_method(void* x, t_symbol* s);
void object_method(void* x, t_symbol* s, t_floatarg a, t_floatarg b,  t_floatarg c, t_floatarg d);
void object_method(void* x, t_symbol* s, void* z, method method, long number, void* other);
void object_attr_setvalueof(t_object *x, t_symbol* s, long argc, t_atom* argv);
void object_attr_getvalueof(t_object *x, t_symbol *s, long *argc, t_atom **argv);

t_pd_err binbuf_append_atoms(t_binbuf *d, t_symbol *key, long argc, t_atom *argv);
t_pd_err binbuf_copy_atoms(t_binbuf *d, t_symbol *key, long *argc, t_atom **argv);

double pd_clip_min(double aValue, double aMinimum);
double pd_clip_max(double aValue, double aMaximum);
double pd_clip_minmax(double aValue, double aMinimum, double aMaximum);
double pd_ordinate(double radius, double angle);
double pd_abscissa(double radius, double angle);
double pd_radius(double x, double y);
double pd_angle(double x, double y);

#endif