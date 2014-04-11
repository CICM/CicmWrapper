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

#include <Accelerate/Accelerate.h>
//#include <cblas.h>
#include "fftw3.h"

#endif

typedef struct _fourierconvolve
{
	t_edspobj   j_box;
    
    t_symbol*   f_buffer_name;
    float*      f_buffer_samples;
    
    char        f_should_process;
    char        f_in_dsp;
    
    int         f_index;
    int         f_array_offset;
    int			f_window_size;
	int			f_array_size;
	
	float		f_scale;
    
    float*      f_input_real;
    float*      f_output_real;
	float*      f_input_complex;
    float*      f_output_complex;
    float*      f_impulse_complex;
    float*      f_output_saved;
    
#ifdef __APPLE__
    vDSP_Length	f_order;
	FFTSetup    f_fft_handle;
    
    DSPSplitComplex f_input_split;
    DSPSplitComplex f_output_split;
    DSPSplitComplex f_impulse_split;
#else
	fftwf_plan	f_handle_forward;
	fftwf_plan	f_handle_inverse;
#endif
    
    
} t_fourierconvolve;

t_eclass *fourierconvolve_class;

void *fourierconvolve_new(t_symbol *s, int argc, t_atom *argv);
void fourierconvolve_free(t_fourierconvolve *x);
void fourierconvolve_assist(t_fourierconvolve *x, void *b, long m, long a, char *s);

void fourierconvolve_dsp(t_fourierconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);

void fourierconvolve_set(t_fourierconvolve *x, t_symbol *s);

extern "C"  void setup_c0x2efourierconvolve_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.fourierconvolve~", (method)fourierconvolve_new, (method)fourierconvolve_free, (short)sizeof(t_fourierconvolve), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
	
    eclass_addmethod(c, (method) fourierconvolve_dsp,       "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) fourierconvolve_assist,    "assist",           A_CANT, 0);
    eclass_addmethod(c, (method) fourierconvolve_set,       "set",              A_SYM,  0);
    eclass_register(CLASS_BOX, c);
	fourierconvolve_class = c;
}

void *fourierconvolve_new(t_symbol *s, int argc, t_atom *argv)
{
	t_fourierconvolve *x =  NULL;

	x = (t_fourierconvolve *)eobj_new(fourierconvolve_class);
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_buffer_name= NULL;
    x->f_array_offset = 0;
    x->f_array_size = 0;
    
    if(argc && argv && atom_gettype(argv) == A_SYM)
        x->f_buffer_name = atom_getsym(argv);
    if(argc > 1 && argv && atom_gettype(argv+1) == A_FLOAT)
        x->f_array_offset = pd_clip_min(atom_getfloat(argv+1), 0);
    if(argc > 2 && argv && atom_gettype(argv+2) == A_FLOAT)
        x->f_array_size = pd_clip_min(atom_getfloat(argv+2), 2);
    
    if(x->f_array_size < 2)
        x->f_array_size = 2;
    else if(x->f_array_size&((x->f_array_size-1) != 0))
    {
        while (x->f_array_size&((x->f_array_size-1) != 0))
            x->f_array_size--;
    }
    
    x->f_window_size= x->f_array_size * 2;
    x->f_scale		= 1.f / (float)(x->f_window_size * 8.f);
    
    x->f_input_real = (float *)malloc(x->f_window_size * sizeof(float));
    x->f_output_real = (float *)malloc(x->f_window_size * sizeof(float));
    x->f_input_complex = (float *)malloc(x->f_window_size * sizeof(float));
    x->f_output_complex = (float *)malloc(x->f_window_size * sizeof(float));
    x->f_impulse_complex = (float *)malloc(x->f_window_size * sizeof(float));
    x->f_output_saved = (float *)malloc(x->f_array_size * sizeof(float));
    x->f_buffer_samples = (float *)malloc(x->f_window_size * sizeof(float));
#ifdef __APPLE__
    x->f_order		= log2f(x->f_window_size);
    x->f_fft_handle = vDSP_create_fftsetup(x->f_order, FFT_RADIX2);
    
    x->f_impulse_split.realp = x->f_impulse_complex;
    x->f_impulse_split.imagp = x->f_impulse_complex + x->f_array_size;
    
    x->f_output_split.realp = x->f_output_complex;
    x->f_output_split.imagp = x->f_output_complex + x->f_array_size;
    x->f_input_split.realp = x->f_input_complex;
    x->f_input_split.imagp = x->f_input_complex + x->f_array_size;
#else
    x->f_handle_forward = fftwf_plan_r2r_1d(x->f_window_size, x->f_input_real, x->f_input_complex, FFTW_R2HC, FFTW_ESTIMATE);
    x->f_handle_inverse = fftwf_plan_r2r_1d(x->f_window_size, x->f_input_complex, x->f_output_real, FFTW_HC2R, FFTW_ESTIMATE);
#endif
    
    fourierconvolve_set(x, x->f_buffer_name);
    x->f_index = 0;
    x->f_in_dsp = 0;
	return (x);
}

void fourierconvolve_set(t_fourierconvolve *x, t_symbol *s)
{
    t_garray *a = NULL;
    x->f_buffer_name = s;
    int buffer_size;
    float* buffer_sample;
    if(!x->f_buffer_name)
        return;
    
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
        if(buffer_size - x->f_array_offset <= 0)
        {
            x->f_should_process = 0;
            return;
        }
       
        int dsp_state;
        if(!x->f_in_dsp)
            dsp_state = canvas_suspend_dsp();
        
        memset(x->f_buffer_samples, 0, x->f_window_size * sizeof(float));
        if(buffer_size - x->f_array_offset > x->f_array_size)
            memcpy(x->f_buffer_samples, buffer_sample + x->f_array_offset, x->f_array_size * sizeof(float));
        else
            memcpy(x->f_buffer_samples, buffer_sample + x->f_array_offset, (buffer_size - x->f_array_offset) * sizeof(float));
        
#ifdef __APPLE__
        vDSP_ctoz((DSPComplex *)x->f_buffer_samples, 2, &x->f_impulse_split, 1, x->f_array_size);
        vDSP_fft_zrip(x->f_fft_handle, &x->f_impulse_split, 1, x->f_order, FFT_FORWARD);
#else
        fftwf_plan	handle;
        handle = fftwf_plan_r2r_1d(x->f_window_size, x->f_buffer_samples, x->f_impulse_complex, FFTW_R2HC, FFTW_ESTIMATE);
        fftwf_execute(x->f_handle_forward);
        fftwf_destroy_plan(handle);
#endif
        x->f_should_process = 1;
        
        if(!x->f_in_dsp)
            canvas_resume_dsp(dsp_state);
        
    }
    
}

void fourierconvolve_free(t_fourierconvolve *x)
{
	eobj_dspfree((t_ebox *)x);
#ifdef __APPLE__
    vDSP_destroy_fftsetup(x->f_fft_handle);
#else
    fftwf_destroy_plan(x->f_handle_forward);
    fftwf_destroy_plan(x->f_handle_inverse);
#endif
    free(x->f_input_real);
    free(x->f_output_real);
    free(x->f_input_complex);
    free(x->f_output_complex);
    free(x->f_output_saved);
    free(x->f_impulse_complex);
    free(x->f_buffer_samples);
}

void fourierconvolve_assist(t_fourierconvolve *x, void *b, long m, long a, char *s)
{
	;
}

void fourierconvolve_perform_down(t_fourierconvolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    int array_size = x->f_array_size;
    cblas_scopy(sampleframes, ins[0], 1, x->f_input_real + x->f_index, 1);
    cblas_scopy(sampleframes, x->f_output_real + x->f_index, 1, outs[0], 1);
    x->f_index += sampleframes;
    if(x->f_index >= array_size)
    {
        x->f_index = 0;
        memset(x->f_input_real + array_size, 0, array_size * sizeof(float));
        memset(x->f_output_real, 0, x->f_window_size * sizeof(float));
        
#ifdef __APPLE__
        vDSP_ctoz((DSPComplex *)x->f_input_real, 2, &x->f_input_split, 1, array_size);
        vDSP_fft_zrip(x->f_fft_handle, &x->f_input_split, 1, x->f_order, FFT_FORWARD);
        
        float preserve_ir = x->f_impulse_split.imagp[0];
        x->f_impulse_split.imagp[0] = 0;
        float preserve_si = x->f_input_split.imagp[0];
        x->f_input_split.imagp[0] = 0;
        vDSP_zvmul(&x->f_input_split, 1, &x->f_impulse_split, 1, &x->f_output_split, 1, array_size, 1);
        x->f_output_split.imagp[0] = preserve_ir * preserve_si;
        x->f_impulse_split.imagp[0] = preserve_ir;
        
        vDSP_fft_zrip(x->f_fft_handle, &x->f_output_split, 1, x->f_order, FFT_INVERSE);
        vDSP_ztoc(&x->f_output_split, 1, (DSPComplex *)x->f_output_real, 2, array_size);
        vDSP_vsmul(x->f_output_real, 1, &x->f_scale, x->f_output_real, 1, x->f_window_size);
        
        vDSP_vadd(x->f_output_real, 1, x->f_output_saved, 1, x->f_output_real, 1, array_size);
        cblas_scopy(array_size, x->f_output_real + array_size, 1, x->f_output_saved, 1);
        
#else
        fftwf_execute(x->f_handle_forward);
        fftwf_execute(x->f_handle_inverse);
#endif
    }
}

void fourierconvolve_perform_zero(t_fourierconvolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    memset(outs[0], 0, sampleframes * sizeof(float));
}

void fourierconvolve_dsp(t_fourierconvolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_in_dsp = 1;
    fourierconvolve_set(x, x->f_buffer_name);
    x->f_in_dsp = 0;
    if(!x->f_should_process)
    {
        object_method(dsp, gensym("dsp_add"), x, (method)fourierconvolve_perform_zero, 0, NULL);
        return;
    }
    
    memset(x->f_input_real, 0, x->f_window_size * sizeof(float));
    memset(x->f_output_real, 0, x->f_window_size * sizeof(float));
    memset(x->f_input_complex, 0, x->f_window_size * sizeof(float));
    memset(x->f_output_complex, 0, x->f_window_size * sizeof(float));
    memset(x->f_output_saved, 0, x->f_array_size * sizeof(float));
    
    if(maxvectorsize <= x->f_array_size)
        object_method(dsp, gensym("dsp_add"), x, (method)fourierconvolve_perform_down, 0, NULL);/*
    else if(maxvectorsize > x->f_array_size)
        object_method(dsp, gensym("dsp_add"), x, (method)fourierconvolve_perform_up, 0, NULL);
    else
        object_method(dsp, gensym("dsp_add"), x, (method)fourierconvolve_perform_equal, 0, NULL);*/
}







