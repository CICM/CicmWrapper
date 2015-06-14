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

#include "edefine.h"

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

void epd_init(void);
void pd_library_add_folder(char* libraryname, char* folder);

#endif
