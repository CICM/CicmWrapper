/*
 * CicmWrapper
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

#include "../c.library.h"

#include <algorithm>
#include <cstring>
#include <numeric>
#include <vector>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "FFTConvolver.h"
#include "Utilities.h"

using namespace fftconvolver;

typedef struct _convolve
{
	t_edspobj     j_box;
    FFTConvolver* f_convolver;
    char          f_normalize;
    t_symbol*     f_buffer_name;
} t_convolve;

t_eclass *convolve_class;

void *convolve_new(t_symbol *s, int argc, t_atom *argv);
void convolve_free(t_convolve *x);
void convolve_dsp(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void convolve_set(t_convolve *x, t_symbol *s);
void convolve_set_do(t_convolve *x, t_symbol *s, char dsp);
void convolve_normalize(t_convolve *x, float f);

extern "C"  void setup_c0x2econvolve_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.convolve~", (method)convolve_new, (method)convolve_free, (short)sizeof(t_convolve), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
	cream_initclass(c);
    
    eclass_addmethod(c, (method) convolve_dsp,       "dsp",              A_CANT, 0);
    eclass_addmethod(c, (method) convolve_set,       "set",              A_SYM,  0);
    eclass_addmethod(c, (method) convolve_normalize, "normalize",        A_LONG, 0);
    eclass_register(CLASS_OBJ, c);
	convolve_class = c;
}

void *convolve_new(t_symbol *s, int argc, t_atom *argv)
{
	t_convolve *x =  NULL;

	x = (t_convolve *)eobj_new(convolve_class);
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_buffer_name        = NULL;
    x->f_convolver = new FFTConvolver();
    if(argc && argv && atom_gettype(argv) == A_SYM)
        x->f_buffer_name = atom_getsym(argv);
    if(argc > 1 && argv && atom_gettype(argv+1) == A_SYM && atom_getsym(argv+1) == gensym("normalize"))
        x->f_normalize = 1;
    else if(argc > 1 && argv && atom_gettype(argv+1) == A_LONG && atom_getlong(argv+1) > 0)
        x->f_normalize = 1;
    else
        x->f_normalize = 0;
    
    x->j_box.d_misc = E_NO_INPLACE;
    
	return (x);
}

void convolve_set_do(t_convolve *x, t_symbol *s, char dsp)
{
    t_garray *a = NULL;
    x->f_buffer_name = s;
    int buffer_size;
    t_word* buffer;
    float* temp;
    if(!s)
        return;
    
    if (!(a = (t_garray *)pd_findbyclass(x->f_buffer_name, garray_class)))
    {
        object_error(x, "c.convolve~: %s no such array.", x->f_buffer_name->s_name);
        return;
    }
    else if (!garray_getfloatwords(a, &buffer_size, &buffer))
    {
        object_error(x, "c.convolve~: %s array is empty.", x->f_buffer_name->s_name);
        return;
    }
    else
    {
        int dsp_state;
        if(!dsp)
            dsp_state = canvas_suspend_dsp();
        
        temp = (float *)malloc(buffer_size * sizeof(float));
        for(int i = 0; i < buffer_size; i++)
        {
            temp[i] = buffer[i].w_float;
        }
        if(x->f_normalize)
        {
            float max = 0;
            for(int i = 0; i < buffer_size; i++)
            {
                if(fabs(temp[i]) > max)
                    max = fabs(temp[i]);
            }
            if(max != 0)
            {
                max = 1.f / max;
                for(int i = 0; i < buffer_size; i++)
                {
                    temp[i] *= max;
                }
            }
        }
        x->f_convolver->reset();
        x->f_convolver->init(512, temp, buffer_size);
        
        free(temp);
        if(!dsp)
            canvas_resume_dsp(dsp_state);
    }
    
}

void convolve_set(t_convolve *x, t_symbol *s)
{
    convolve_set_do(x, s, 0);
}

void convolve_free(t_convolve *x)
{
	eobj_dspfree((t_ebox *)x);
}

void convolve_normalize(t_convolve *x, float f)
{
    x->f_normalize = f;
    if(x->f_normalize < 1)
        x->f_normalize = 0;
    else
        x->f_normalize = 1;
    convolve_set_do(x, x->f_buffer_name, 0);
}

void convolve_perform(t_convolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    x->f_convolver->process(ins[0], outs[0], sampleframes);
}

void convolve_dsp(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    convolve_set_do(x, x->f_buffer_name, 1);
    if(x->f_buffer_name)
        object_method(dsp, gensym("dsp_add"), x, (method)convolve_perform, 0, NULL);
}







