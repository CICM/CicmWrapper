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
#include "../../../Sources/cicm_wrapper.h"
}

#include "FourierConvolve.h"

typedef struct _fourierconvolve
{
	t_edspobj               j_box;
    Cicm::FourierConvolve*  f_convolve;
    t_symbol*               f_buffer_name;
    
} t_fourierconvolve;

t_eclass *fourierconvolve_class;

void *fourierconvolve_new(t_symbol *s, int argc, t_atom *argv);
void fourierconvolve_free(t_fourierconvolve *x);

void fourierconvolve_dsp(t_fourierconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);

void fourierconvolve_set(t_fourierconvolve *x, t_symbol *s);
void fourierconvolve_set_do(t_fourierconvolve *x, t_symbol *s, char dspmethod);
extern "C"  void setup_c0x2efourierconvolve_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.fourierconvolve~", (method)fourierconvolve_new, (method)fourierconvolve_free, (short)sizeof(t_fourierconvolve), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
	
    eclass_addmethod(c, (method) fourierconvolve_dsp,       "dsp",              A_CANT, 0);
    eclass_addmethod(c, (method) fourierconvolve_set,       "set",              A_SYM,  0);
    eclass_register(CLASS_BOX, c);
	fourierconvolve_class = c;
}

void *fourierconvolve_new(t_symbol *s, int argc, t_atom *argv)
{
	t_fourierconvolve *x =  NULL;
    int ir_size = 0;
    int ir_offset = 0;
    int n_instances = 0;
    
	x = (t_fourierconvolve *)eobj_new(fourierconvolve_class);
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_buffer_name= NULL;
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
        x->f_buffer_name = atom_getsym(argv);
    if(argc > 1 && argv && atom_gettype(argv+1) == A_FLOAT)
        n_instances = pd_clip_min(atom_getfloat(argv+1), 1);
    if(argc > 2 && argv && atom_gettype(argv+2) == A_FLOAT)
        ir_size = pd_clip_min(atom_getfloat(argv+2), 2);
    if(argc > 3 && argv && atom_gettype(argv+3) == A_FLOAT)
        ir_offset = pd_clip_min(atom_getfloat(argv+3), 0);
    
    x->f_convolve = new Cicm::FourierConvolve(n_instances, ir_size, ir_offset);
    
    fourierconvolve_set_do(x, x->f_buffer_name, 0);
    
	return (x);
}

void fourierconvolve_set_do(t_fourierconvolve *x, t_symbol *s, char dspmethod)
{
    t_garray *a = NULL;
    x->f_buffer_name = s;
    int buffer_size;
    float* buffer_sample;
    
    if (!(a = (t_garray *)pd_findbyclass(x->f_buffer_name, garray_class)))
    {
        object_error(x, "c.fourierconvolve~: %s no such array.", x->f_buffer_name->s_name);
        return;
    }
    else if (!garray_getfloatarray(a, &buffer_size, &buffer_sample))
    {
        object_error(x, "c.fourierconvolve~: %s array is empty.", x->f_buffer_name->s_name);
        return;
    }
    else
    {
        int dsp_state;
        if(!dspmethod)
            dsp_state = canvas_suspend_dsp();
        
        x->f_convolve->setIr(buffer_sample, buffer_size);
        
        if(!dspmethod)
            canvas_resume_dsp(dsp_state);
    }
}

void fourierconvolve_set(t_fourierconvolve *x, t_symbol *s)
{
    fourierconvolve_set_do(x, s, 0);
}

void fourierconvolve_free(t_fourierconvolve *x)
{
	eobj_dspfree((t_ebox *)x);
    delete x->f_convolve;
}

void fourierconvolve_perform(t_fourierconvolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    x->f_convolve->perform(ins[0], outs[0], sampleframes);
}

void fourierconvolve_dsp(t_fourierconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    fourierconvolve_set_do(x, x->f_buffer_name, 1);
    x->f_convolve->prepare(maxvectorsize);
    object_method(dsp, gensym("dsp_add"), x, (method)fourierconvolve_perform, 0, NULL);
}







