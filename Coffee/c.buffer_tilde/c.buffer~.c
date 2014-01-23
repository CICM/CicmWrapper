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

#include "../../../PdEnhanced/Sources/cicm_wrapper.h"

typedef struct  _buffer
{
	t_edspobj   j_box;
    
    t_symbol*   f_name;
    float       f_ms;
    long        f_channels;
    
	t_outlet*   f_out;
    t_array*    f_array;

} t_buffer;

t_eclass *buffer_class;

void *buffer_new(t_symbol *s, int argc, t_atom *argv);
void buffer_anything(t_buffer *x, t_symbol *s, int argc, t_atom *argv);
void buffer_list(t_buffer *x, t_symbol *s, int argc, t_atom *argv);
void buffer_set(t_buffer *x, t_symbol *s, int argc, t_atom *argv);
void buffer_float(t_buffer *x, float f);
void buffer_symbol(t_buffer *x, t_symbol *s);
void buffer_output(t_buffer *x, long size);
void buffer_free(t_buffer *x);
void buffer_assist(t_buffer *x, void *b, long m, long a, char *s);

void buffer_bang(t_buffer *x);

void setup_c0x2ebuffer_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.buffer~", (method)buffer_new, (method)buffer_free, (short)sizeof(t_buffer), 0L, A_GIMME, 0);
    eclass_dspinit(c);
    
    eclass_addmethod(c, (method)buffer_anything,    "anything",       A_GIMME, 0);
    eclass_addmethod(c, (method)buffer_list,        "list",           A_GIMME, 0);
    eclass_addmethod(c, (method)buffer_set,         "set",            A_GIMME, 0);
    eclass_addmethod(c, (method)buffer_float,       "float",          A_FLOAT, 0);
    eclass_addmethod(c, (method)buffer_symbol,      "symbol",         A_SYMBOL,0);
    eclass_addmethod(c, (method)buffer_bang,        "bang",           A_CANT,  0);
    
    CLASS_ATTR_FLOAT                (c, "ms", 0, t_buffer, f_ms);
    CLASS_ATTR_FILTER_CLIP          (c, "ms", 0, 1);
    CLASS_ATTR_DEFAULT              (c, "ms", 0, "0");
    CLASS_ATTR_SAVE                 (c, "ms", 1);
    
    CLASS_ATTR_LONG                 (c, "channels", 0, t_buffer, f_channels);
    CLASS_ATTR_FILTER_CLIP          (c, "channels", 0, 1);
    CLASS_ATTR_DEFAULT              (c, "channels", 0, "1");
    CLASS_ATTR_SAVE                 (c, "channels", 1);
    
    eclass_register(CLASS_OBJ, c);
    cicm_post();
	buffer_class = c;
}

void *buffer_new(t_symbol *s, int argc, t_atom *argv)
{
	t_buffer *x =  NULL;
    t_binbuf* d;
    if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
        x = (t_buffer *)eobj_new(buffer_class);
        if(x)
        {
            x->f_name = atom_getsym(argv);
            x->f_out = (t_outlet *)listout(x);
            ebox_attrprocess_viabinbuf(x, d);
        }
        return (x);
    }
    else
    {
        post("c.buffer~ needs a name.");
        return NULL;
    }
    
}

void buffer_set(t_buffer *x, t_symbol *s, int argc, t_atom *argv)
{
    ;
}

void buffer_list(t_buffer *x, t_symbol *s, int argc, t_atom *argv)
{
    ;
}

void buffer_anything(t_buffer *x, t_symbol *s, int argc, t_atom *argv)
{
    post(x->f_name->s_name);
}

void buffer_float(t_buffer *x, float f)
{
    ;
}

void buffer_symbol(t_buffer *x, t_symbol *s)
{
    ;
}

void buffer_bang(t_buffer *x)
{
    post(x->f_name->s_name);
}

void buffer_free(t_buffer *x)
{
	eobj_free(x);
    eobj_dspfree(x);
}

void buffer_assist(t_buffer *x, void *b, long m, long a, char *s)
{
	;
}


