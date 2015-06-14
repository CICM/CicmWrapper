/*
 * CicmWrapper - Pure Data Enhanced
 *
 * A wrapper for Pure Data
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

#ifdef _WIN32
#include <io.h>
#else
#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <unistd.h>
#endif

#ifdef _WINDOWS
	#pragma warning(disable:4091)
#endif

#ifdef PD_EXTENDED

#ifndef __m_pd_h_
#include "pd-extended/m_pd.h"
#include "pd-extended/m_imp.h"
#include "pd-extended/g_canvas.h"
#endif

#else

#ifndef __m_pd_h_
#include "pd-vanilla/m_pd.h"
#include "pd-vanilla/m_imp.h"
#include "pd-vanilla/g_canvas.h"
#endif

#endif
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#define EPD_PI  (3.141592653589793238462643383279502884)
#define EPD_2PI (6.283185307179586476925286766559005)
#define EPD_PI2 (1.57079632679489661923132169163975144)
#define EPD_PI4 (0.785398163397448309615660845819875721)
typedef long t_pd_err;
#define CLASS_OBJ           gensym("obj")
#define CLASS_BOX			gensym("box")

#define atom_setfloat(a, f) SETFLOAT(a, f)
#define atom_setlong(a, l)  SETFLOAT(a, (float)l)
#define atom_setsym(a, s)   SETSYMBOL(a, s)
#define atom_getlong(a)     (long)atom_getfloat(a)
#define atom_gettype(a)     (a)[0].a_type

//! @cond
typedef struct _namelist    /* element in a linked list of stored strings */
{
    struct _namelist *nl_next;  /* next in list */
    char *nl_string;            /* the string */
} t_namelist;

EXTERN t_namelist *sys_staticpath;
EXTERN t_namelist *sys_externlist;
EXTERN t_namelist *sys_searchpath;
EXTERN t_namelist *sys_helppath;
EXTERN t_namelist *namelist_append_files(t_namelist *listwas, const char *s);

//! @endcond

typedef void        (*method)(void* x, ...);
typedef void*       (*rmethod)(void* x, ...);
typedef t_pd_err    (*t_err_method)(void* x, ...);

void outlet_int(t_outlet* outlet, int val);
t_outlet* symbolout(void *x);
t_outlet *listout(void *x);
t_outlet *floatout(void *x);
t_outlet* bangout(void *x);
t_outlet* anythingout(void *x);

void* object_method(void* x, t_symbol* s, void* z, method method, long number, void* other);
void object_attr_setvalueof(t_object *x, t_symbol* s, int argc, t_atom *argv);
void object_attr_getvalueof(t_object *x, t_symbol *s, int *argc, t_atom **argv);

long binbuf_append_attribute(t_binbuf *d, t_symbol *key, int argc, t_atom *argv);

long atoms_get_attributes_offset(int ac, t_atom* av);
long binbuf_get_attributes_offset(t_binbuf *d);

long atoms_get_nattributes(int ac, t_atom* av);
long binbuf_get_nattributes(t_binbuf *d);

t_pd_err atoms_has_attribute(int ac, t_atom* av, t_symbol *key);
t_pd_err binbuf_has_attribute(t_binbuf *d, t_symbol *key);

long atoms_get_attribute_index(int ac, t_atom *av, t_symbol *key);
long binbuf_get_attribute_index(t_binbuf *d, t_symbol *key);

t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv);
t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, int *argc, t_atom **argv);
t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol *key, long *value);
t_pd_err binbuf_get_attribute_long(t_binbuf *d, t_symbol *key, long *value);
t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol *key, float *value);
t_pd_err binbuf_get_attribute_float(t_binbuf *d, t_symbol *key, float *value);
long atoms_get_keys(int ac, t_atom* av, t_symbol*** s);
long binbuf_get_keys(t_binbuf *d, t_symbol*** s);

t_binbuf* binbuf_via_atoms(int ac, t_atom *av);

double pd_clip_min(double aValue, double aMinimum);
double pd_clip_max(double aValue, double aMaximum);
double pd_clip_minmax(double aValue, double aMinimum, double aMaximum);
double pd_ordinate(double radius, double angle);
double pd_abscissa(double radius, double angle);
double pd_radius(double x, double y);
double pd_angle(double x, double y);

extern t_symbol* s_null;
extern t_symbol* s_obj;
extern t_symbol* s_atom;
extern t_symbol* s_attr_modified;
extern t_symbol* s_eboxbd;
extern t_symbol* s_eboxio;
extern t_symbol* s_size;
extern t_symbol* s_int;
extern t_symbol* s_long;
extern t_symbol* s_double;
extern t_symbol* s_eproxy1572;

void epd_init(void);
void pd_library_add_folder(char* libraryname, char* folder);

#endif
