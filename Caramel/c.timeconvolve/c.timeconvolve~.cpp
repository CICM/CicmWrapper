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

extern "C" {
#include "../../Sources/cicm_wrapper.h"
}

#ifdef __APPLE__

#include <Accelerate/Accelerate.h>

#else

#include <cblas.h>

#endif

typedef struct _timeconvolve
{
	t_edspobj   j_box;
    
    t_symbol*   f_buffer_name;
    
    float*      f_input_vector;
	float*      f_fir_vector;
	int         f_fir_size;
    int         f_fir_offset;
	int			f_index;
    
} t_timeconvolve;

t_eclass *timeconvolve_class;

void *timeconvolve_new(t_symbol *s, int argc, t_atom *argv);
void timeconvolve_free(t_timeconvolve *x);
void timeconvolve_assist(t_timeconvolve *x, void *b, long m, long a, char *s);

void timeconvolve_dsp(t_timeconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void timeconvolve_perform(t_timeconvolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);

void timeconvolve_set(t_timeconvolve *x, t_symbol *s);

extern "C"  void setup_c0x2etimeconvolve_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.timeconvolve~", (method)timeconvolve_new, (method)timeconvolve_free, (short)sizeof(t_timeconvolve), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
	
    eclass_addmethod(c, (method) timeconvolve_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) timeconvolve_assist,          "assist",           A_CANT, 0);
    eclass_addmethod(c, (method) timeconvolve_set,             "set",              A_SYM,  0);
    
    eclass_register(CLASS_BOX, c);
	timeconvolve_class = c;
}

void *timeconvolve_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_timeconvolve *x =  NULL;

	x = (t_timeconvolve *)eobj_new(timeconvolve_class);
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_buffer_name= NULL;
    x->f_fir_offset = 0;
    x->f_fir_size   = 0;
    
    x->f_fir_vector = (float *)malloc(8192 * sizeof(float));
    x->f_input_vector = (float *)malloc((8192 * 2 - 1 ) * sizeof(float));
    
    for(i = 0; i < 8192; i++)
        x->f_fir_vector[i] = 0.f;
    for(i = 0; i < (8192 * 2 - 1 ); i++)
        x->f_input_vector[i] = 0.f;
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
        x->f_buffer_name = atom_getsym(argv);
    if(argc > 1 && argv && atom_gettype(argv+1) == A_FLOAT)
        x->f_fir_offset = pd_clip_minmax(atom_getfloat(argv+1), 0, 8192);
    if(argc > 2 && argv && atom_gettype(argv+2) == A_FLOAT)
        x->f_fir_size = pd_clip_minmax(atom_getfloat(argv+2), 0, 8192);
    
    x->f_index = x->f_fir_size;
    timeconvolve_set(x, x->f_buffer_name);
    
	return (x);
}

void timeconvolve_set(t_timeconvolve *x, t_symbol *s)
{
    int i;
    t_garray *a = NULL;
    x->f_buffer_name = s;
    int buffer_size;
    float* buffer_sample;
    if(!x->f_buffer_name)
        return;
    
    if (!(a = (t_garray *)pd_findbyclass(x->f_buffer_name, garray_class)))
    {
        object_error(x, "c.timeconvolve~: %s no such array.", x->f_buffer_name->s_name);
        return;
    }
    else if (!garray_getfloatarray(a, &buffer_size, &buffer_sample))
    {
        object_error(x, "c.timeconvolve~: %s array is empty.", x->f_buffer_name->s_name);
        return;
    }
    else
    {
        for(i = 0; i < x->f_fir_size; i++)
        {
            if(i + x->f_fir_offset < buffer_size)
                x->f_fir_vector[i] = buffer_sample[i];
            else
                x->f_fir_vector[i] = 0.f;
        }
    }
}

void timeconvolve_free(t_timeconvolve *x)
{
	eobj_dspfree((t_ebox *)x);
    free(x->f_fir_vector);
    free(x->f_input_vector);
}

void timeconvolve_assist(t_timeconvolve *x, void *b, long m, long a, char *s)
{
	;
}

void timeconvolve_dsp(t_timeconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    timeconvolve_set(x, x->f_buffer_name);
    x->f_index = x->f_fir_size;
    object_method(dsp, gensym("dsp_add"), x, (method)timeconvolve_perform, 0, NULL);
}

void timeconvolve_perform(t_timeconvolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    float* input = ins[0];
    float* output = outs[0];
    float *fir = x->f_fir_vector;
	float *buffer = x->f_input_vector;
    int size = x->f_fir_size;
    int index = x->f_index;
#ifdef __APPLE__
    const vDSP_Stride Stride = 1;
#else
    const int Stride = 1;
#endif
    
    for(int i = 0; i < sampleframes; i++)
    {
        if(index < 0)
        {
            index = size;
            cblas_scopy(size, buffer, 1, buffer+size, 1);
        }
        buffer[index] = input[i];

#ifdef __APPLE__
        vDSP_dotpr(buffer+index, Stride, fir, Stride, output+i, (vDSP_Length)size);
#else
        output[i] = cblas_sdot(size, buffer+x->f_index, Stride, fir, Stride);
#endif
        index--;
    }
    x->f_index = index;
}






