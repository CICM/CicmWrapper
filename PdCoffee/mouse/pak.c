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

#include "../../../PdEnhanced/Sources/pd_enhanced.h"

typedef struct  _pak
{
	t_ebox      j_box;
	t_outlet*   f_out;
    t_atom*     f_argv;
	long        f_argc;
    char*       f_selectors;
} t_pak;

t_eclass *pak_class;

void pak_setup(void);
void *pak_new(t_symbol *s, int argc, t_atom *argv);
void pak_anything(t_pak *x, t_symbol *s, int argc, t_atom *argv);
void pak_list(t_pak *x, t_symbol *s, int argc, t_atom *argv);
void pak_float(t_pak *x, float f);
void pak_symbol(t_pak *x, t_symbol *s);
void pak_free(t_pak *x);
void pak_assist(t_pak *x, void *b, long m, long a, char *s);

void pak_output(t_pak *x);

void pak_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("pak", (method)pak_new, (method)pak_free, (short)sizeof(t_pak), 0L, A_GIMME, 0);
    eclass_addmethod(c, (method)pak_anything,    "anything",       A_GIMME, 0);
    eclass_addmethod(c, (method)pak_list,        "list",           A_GIMME, 0);
    eclass_addmethod(c, (method)pak_float,       "float",          A_FLOAT, 0);
    eclass_addmethod(c, (method)pak_symbol,      "symbol",         A_SYMBOL,0);
    eclass_addmethod(c, (method)pak_output,      "bang",           A_CANT,  0);
    
    eclass_register(CLASS_BOX, c);
	pak_class = c;
}

void *pak_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_pak *x =  NULL;
    
    x = (t_pak *)ebox_alloc(pak_class);
    if(x)
    {
        if(argc < 2)
        {
            argc = 2;
        }
        x->f_argc = argc;
        x->f_argv = calloc(x->f_argc, sizeof(t_atom));
        x->f_selectors = calloc(x->f_argc, sizeof(char));
        if(atom_gettype(argv) == A_SYM && (atom_getsym(argv) == gensym("f") || atom_getsym(argv) == gensym("float")))
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, 0.);
        }
        else if(atom_gettype(argv) == A_FLOAT)
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, atom_getfloat(argv));
        }
        else if(atom_gettype(argv) == A_SYM && (atom_getsym(argv) == gensym("s") || atom_getsym(argv) == gensym("symbol")))
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, gensym("symbol"));
        }
        else if(atom_gettype(argv) == A_SYM)
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, atom_getsym(argv));
        }
        else
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, gensym("symbol"));
        }
        for(i = 1; i < x->f_argc; i++)
        {
            ebox_inletnew((t_ebox *)x);
            if(atom_gettype(argv+i) == A_SYM && (atom_getsym(argv+i) == gensym("f") || atom_getsym(argv+i) == gensym("float")))
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, 0.);
            }
            else if(atom_gettype(argv+i) == A_FLOAT)
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, atom_getfloat(argv));
            }
            else if(atom_gettype(argv+i) == A_SYM && (atom_getsym(argv+i) == gensym("s") || atom_getsym(argv+i) == gensym("symbol")))
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
            else if(atom_gettype(argv+i) == A_SYM)
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, atom_getsym(argv+i));
            }
            else
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
        }
        x->f_out = (t_outlet *)listout(x);
    }
    
    return (x);
}

void pak_list(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = ebox_getproxy((t_ebox *)x);
    if(argc && x->f_selectors[index] == 0 && atom_gettype(argv) == A_FLOAT)
    {
        atom_setfloat(x->f_argv+index, atom_getfloat(argv));
        pak_output(x);
    }
    else if(argc && x->f_selectors[index] == 1 && atom_gettype(argv) == A_SYM)
    {
        atom_setsym(x->f_argv+index, atom_getsym(argv));
        pak_output(x);
    }
}

void pak_float(t_pak *x, float f)
{
    int index = ebox_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 0)
    {
        atom_setfloat(x->f_argv+index, f);
        pak_output(x);
    }
}

void pak_anything(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = ebox_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

void pak_symbol(t_pak *x, t_symbol *s)
{
    int index = ebox_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

void pak_output(t_pak *x)
{
    outlet_list(x->f_out, &s_list, x->f_argc, x->f_argv);
}

void pak_free(t_pak *x)
{
	free(x->f_selectors);
    free(x->f_argv);
}

void pak_assist(t_pak *x, void *b, long m, long a, char *s)
{
	;
}


