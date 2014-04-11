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
#undef __APPLE__
#ifdef __APPLE__

#include <Accelerate/Accelerate.h>

#else

#include <Accelerate/Accelerate.h>
//#include <cblas.h>
#include "fftw3.h"

#endif


class TimeConvolve
{
protected:
	
	float*      m_input_vector;
	float*      m_fir_vector;
	int         m_fir_size;
	int         m_input_size;
	int			m_index;
    
public:
	TimeConvolve(int size)
    {
        m_fir_size		= size;
        m_input_size	= m_fir_size * 2 - 1;
        
        m_fir_vector = (float *)malloc(m_fir_size * sizeof(float));
        m_input_vector = (float *)malloc(m_input_size * sizeof(float));
        for(int i = 0; i < m_fir_size; i++)
            m_fir_vector[i] = 0.f;
        for(int i = 0; i < m_input_size; i++)
            m_input_vector[i] = 0.f;
        
        m_index = m_fir_size;
    }
    
	void setImpulseResponse(float* impulse)
    {
        if(impulse)
            cblas_scopy(m_fir_size, impulse, 1, m_fir_vector, 1);
        m_index = m_fir_size;
    }
    
	inline float process(float input)
    {
        if(m_index <= 0)
        {
            m_index = m_fir_size;
            cblas_scopy(m_fir_size, m_input_vector, 1, m_input_vector+m_fir_size, 1);
        }
        m_input_vector[--m_index] = input;
       
        return cblas_sdot(m_fir_size, m_input_vector+m_index, 1, m_fir_vector, 1);
    }
    
	~TimeConvolve()
    {
        free(m_input_vector);
        free(m_fir_vector);
    }
};

class FastFourierTransform
{
private:
	int				m_window_size;
	int				m_array_size;
	int				m_order;
	float			m_scale;
    
#ifdef __APPLE__
	FFTSetup            m_fft_handle;
#else
	fftwf_plan		m_handle_forward;
	fftwf_plan		m_handle_inverse;
	float*          m_real_vector;
	float*          m_complex_vector;
#endif
	
public:
	FastFourierTransform(int size)
    {
        if(size < 2)
            size = 2;
        else if(size&((size-1) != 0))
        {
            while (size&((size-1) != 0))
                size--;
        }
        m_window_size	= size * 2;
        m_array_size	= m_window_size / 2;
        m_order			= log2f(m_window_size);
        m_scale			= 1. / (double)(m_window_size * 2.);
        
#ifdef __APPLE__
        m_fft_handle = vDSP_create_fftsetup(m_order, FFT_RADIX2);
#else
        m_real_vector = (float *)malloc(m_window_size * sizeof(float));
        for(int i = 0; i < m_window_size; i++)
            m_real_vector[i] = 0.f;
        m_complex_vector = (float *)malloc(m_window_size * sizeof(float));
        for(int i = 0; i < m_window_size; i++)
            m_complex_vector[i] = 0.f;
        m_handle_forward = fftwf_plan_r2r_1d(m_window_size, m_real_vector, m_complex_vector, FFTW_R2HC, FFTW_ESTIMATE);
        m_handle_inverse = fftwf_plan_r2r_1d(m_window_size, m_complex_vector, m_real_vector, FFTW_HC2R, FFTW_ESTIMATE);
#endif
    }
#ifdef __APPLE__
    inline void forward(float *real, DSPSplitComplex *packed)
    {
        vDSP_ctoz((DSPComplex *)real, 2, packed, 1, m_array_size);
        vDSP_fft_zrip(m_fft_handle, packed, 1, m_order, FFT_FORWARD);
    }
    
    
    inline void inverse(DSPSplitComplex *packed, float *real)
    {
        vDSP_fft_zrip(m_fft_handle, packed, 1, m_order, FFT_INVERSE);
        vDSP_ztoc(anPackedVector, 1, (DSPComplex *)real, 2, m_array_size);
    }
#else
    inline void forward(float *real, float *packed)
    {
        memcpy(real, m_complex_vector, m_window_size * sizeof(float));
        fftwf_execute(m_handle_forward);
        memcpy(m_complex_vector, packed, m_window_size * sizeof(float));
    }
    
    
    inline void inverse(float *packed, float *real)
    {
        memcpy(packed, m_complex_vector, m_window_size * sizeof(float));
        fftwf_execute(m_handle_inverse);
        memcpy(m_complex_vector, real, m_window_size * sizeof(float));
    }
#endif
	int	getWindowSize() const
    {
        return m_window_size;
    }
    
	int	getArraySize() const
    {
        return m_array_size;
    }
    
	int	getOrder() const
    {
        return m_order;
    }
    
	float getScale() const
    {
        return m_scale;
    }
    
	~FastFourierTransform()
    {
#ifdef __APPLE__
        vDSP_destroy_fftsetup(m_fft_handle);
#else
        fftwf_destroy_plan(m_handle_forward);
        fftwf_destroy_plan(m_handle_inverse);
        free(m_real_vector);
        free(m_complex_vector);
#endif
    }
};

class FastFourierConvolution
{
private:
	
	int                    m_window_size;
	int                    m_array_size;
	int                    m_log2_size;
	int                    m_number_of_instances;
	int                    m_ramp;
	int                    m_offset;
	int                    m_hope_size;
    
	FastFourierTransform*	m_fft_instance;
	float**                 m_real_vector;
	float*                  m_buffer;

#ifdef __APPLE__
	DSPSplitComplex*		m_input_complexes;
	DSPSplitComplex**		m_impul_complexes;
	DSPSplitComplex*		m_output_complexes;
#else
    float*                  m_input_complexes;
	float**                 m_impul_complexes;
	float*                  m_output_complexes;
#endif
	float                   m_result;
	
public:
	FastFourierConvolution(int size, int number)
    {
        m_fft_instance	= new FastFourierTransform(size * 2);
        m_window_size	= m_fft_instance->getWindowSize();
        m_array_size	= m_fft_instance->getArraySize();
        m_number_of_instances = number;
        m_hope_size		= m_array_size / m_number_of_instances;
        m_ramp = 0;
        
        m_real_vector		= new float*[m_number_of_instances];
        m_impul_complexes	= new DSPSplitComplex*[m_number_of_instances];
        
        Cicm_packed_malloc(m_input_complexes, m_window_size);
        Cicm_packed_malloc(m_output_complexes, m_window_size);
        Cicm_signal_malloc(m_buffer, m_array_size);
        
        Cicm_packed_clear(m_input_complexes, m_window_size);
        Cicm_packed_clear(m_output_complexes, m_window_size);
        Cicm_signal_clear(m_buffer, m_array_size);
        
        for(int i = 0; i < m_number_of_instances; i++)
        {
            Cicm_signal_malloc(m_real_vector[i], m_window_size);
            Cicm_packed_malloc(m_impul_complexes[i], m_window_size);
            Cicm_signal_clear(m_real_vector[i], m_window_size);
            Cicm_packed_clear(m_impul_complexes[i], m_window_size);
    }
	void loadImpulseResponse(float* anImpulseResponse, long aSize);
        inline float process(float anInput)
        {
            m_result = m_real_vector[0][m_ramp];
            m_real_vector[0][m_ramp] = anInput;
            
            if(m_ramp % m_hope_size == 0 && m_ramp != 0)
            {
                int index = m_ramp / m_hope_size;
                if(index < m_number_of_instances)
                {
                    vDSP_zvmul(m_input_complexes, 1, m_impul_complexes[index], 1, m_output_complexes, 1, m_array_size, 1);
                    m_fft_instance->inverse(m_output_complexes, m_real_vector[index]);
                    
                    if(index < m_number_of_instances - 1)
                        vDSP_vadd(m_real_vector[index+1], 1, m_real_vector[index], 1, m_real_vector[index], 1,  m_window_size);
                }
            }
            
            if(++m_ramp >= m_array_size)
            {
                Cicm_signal_copy(m_real_vector[0]+m_array_size, m_buffer, m_array_size);
                Cicm_signal_clear(m_real_vector[0]+m_array_size, m_array_size);
                m_fft_instance->forward(m_real_vector[0], m_input_complexes);
                vDSP_zvmul(m_input_complexes, 1, m_impul_complexes[0], 1, m_output_complexes, 1, m_array_size, 1);
                
                m_fft_instance->inverse(m_output_complexes, m_real_vector[0]);
                if(m_number_of_instances > 1)
                    Cicm_signal_add(m_real_vector[1], m_real_vector[0], m_real_vector[0], m_window_size);
                Cicm_signal_add(m_buffer, m_real_vector[0], m_real_vector[0], m_array_size);
                m_ramp = 0;
            }
            
            return m_result;
        }

	~FastFourierConvolution();
};


typedef struct _convolve
{
	t_edspobj   j_box;
    
    int         f_number_of_samples;
    t_float*    f_buffer;
    t_symbol*   f_buffer_name;
    
    TimeConvolve*  f_fir_filter;
} t_convolve;

t_eclass *convolve_class;

void *convolve_new(t_symbol *s, int argc, t_atom *argv);
void convolve_free(t_convolve *x);
void convolve_assist(t_convolve *x, void *b, long m, long a, char *s);

void convolve_dsp(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);
void convolve_perform(t_convolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sf, long f,void *up);

void convolve_buffer_set(t_convolve *x, t_symbol *s);

extern "C"  void setup_c0x2econvolve_tilde(void)
{
	t_eclass *c;
    
	c = eclass_new("c.convolve~", (method)convolve_new, (method)convolve_free, (short)sizeof(t_convolve), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
	
    eclass_addmethod(c, (method) convolve_dsp,             "dsp",              A_CANT, 0);
	eclass_addmethod(c, (method) convolve_assist,          "assist",           A_CANT, 0);
    eclass_register(CLASS_BOX, c);
	convolve_class = c;
}

void *convolve_new(t_symbol *s, int argc, t_atom *argv)
{
	t_convolve *x =  NULL;

	x = (t_convolve *)eobj_new(convolve_class);
    eobj_dspsetup((t_ebox *)x, 1, 1);
    
    x->f_fir_filter = new TimeConvolve(128);
    if(argc && argv && atom_gettype(argv) == A_SYM)
        convolve_buffer_set(x, atom_getsym(argv));
    
	return (x);
}

void convolve_buffer_set(t_convolve *x, t_symbol *s)
{
    t_garray *a = NULL;
    x->f_buffer_name = s;
    if (!(a = (t_garray *)pd_findbyclass(x->f_buffer_name, garray_class)))
    {
        x->f_buffer = 0;
        x->f_number_of_samples = 0;
    }
    else if (!garray_getfloatarray(a, &x->f_number_of_samples, &x->f_buffer))
    {
        x->f_buffer = 0;
        x->f_number_of_samples = 0;
    }
    else
    {
        if(x->f_number_of_samples >= 128)
            x->f_fir_filter->setImpulseResponse(x->f_buffer);

    }
}

void convolve_free(t_convolve *x)
{
	eobj_dspfree((t_ebox *)x);
    delete x->f_fir_filter;
}

void convolve_assist(t_convolve *x, void *b, long m, long a, char *s)
{
	;
}

void convolve_dsp(t_convolve *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp, gensym("dsp_add"), x, (method)convolve_perform, 0, NULL);
}

void convolve_perform(t_convolve *x, t_object *d, float **ins, long ni, float **outs, long no, long sampleframes, long f,void *up)
{
    for(int i = 0; i < sampleframes; i++)
    {
        outs[0][i] = x->f_fir_filter->process(ins[0][i]);
    }
}






