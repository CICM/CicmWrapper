/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_CICM_FOURIER_CONVOLVE
#define DEF_CICM_FOURIER_CONVOLVE

#ifdef __APPLE__

#include <Accelerate/Accelerate.h>
#define malloc_alligned malloc
#define free_alligned free
#else

#include <cblas.h>
#include <malloc.h>
#include "fftw3.h"

#define malloc_alligned(x)  _aligned_malloc(x, 16)
#define free_alligned(x)  _aligned_free(x)
#endif

extern "C" {
#include "../../../Sources/cicm_wrapper.h"
}
namespace Cicm
{
    class FourierConvolve
    {
    private:
        float*      m_buffer_samples;
        
        bool        m_can_process;
        
        int         m_number_of_instances;
        int         m_ir_offset;
        int			m_ir_size;
        int			m_window_size;
        
        float		m_scale;
        int         m_index;
        
        float*      m_input_real;
        float*      m_output_real;
        float*      m_output_saved;
        
        float*      m_input_complex;
        float*      m_impulse_complex;
        float*      m_output_complex;
        float*      m_temp_output;
        
#ifdef __APPLE__
        vDSP_Length     m_order;
        FFTSetup        m_fft_handle;
        
        DSPSplitComplex m_input_split;
        DSPSplitComplex m_output_split;
#else
        fftwf_plan	m_handle_forward;
        fftwf_plan  m_handle_inverse;
#endif
    
public:
        FourierConvolve(int numberOfInstances, int irSize, int irOffset);
    
        ~FourierConvolve();
    
        void setIr(float *buffer, int size);
        
        void prepare(int vectorsize);
        void perform(float* input, float* output, int sampleframes);
        void performAdd(float* input, float* output, int sampleframes);
        
        int getNumberOfInstances() const
        {
            return m_number_of_instances;
        }
        
        int getIrSize() const
        {
            return m_ir_size;
        }
        
        int getIrOffset() const
        {
            return m_ir_offset;
        }
        
        int canProcess() const
        {
            return m_can_process;
        }
    };
}


#endif


