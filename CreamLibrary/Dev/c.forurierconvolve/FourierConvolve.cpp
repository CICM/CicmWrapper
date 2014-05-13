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


#include "FourierConvolve.h"

namespace Cicm
{
    FourierConvolve::FourierConvolve(int numberOfInstances, int irSize, int irOffset)
    {
        m_number_of_instances = numberOfInstances;
        if(m_number_of_instances < 1)
            m_number_of_instances = 1;
        
        m_ir_size = irSize;
        if(m_ir_size < 2)
            m_ir_size = 2;
        else if(m_ir_size&((m_ir_size - 1) != 0))
        {
            while(m_ir_size&((m_ir_size - 1) != 0))
                m_ir_size--;
        }
        
        m_ir_offset = irOffset;
        if(m_ir_offset < 0)
            m_ir_offset = 0;
        
        m_window_size      =  m_ir_size * 2;
        m_scale            = 1.f / (float)(m_window_size * 8.f);
        
        m_input_real       = (float *)malloc_alligned(m_window_size * sizeof(float));
        m_output_real      = (float *)malloc_alligned(m_window_size * sizeof(float));
        m_temp_output      = (float *)malloc_alligned(m_window_size * sizeof(float));
        
        m_input_complex    = (float *)malloc_alligned(m_window_size * sizeof(float));
        m_output_complex   = (float *)malloc_alligned(m_window_size * sizeof(float));
        
        m_output_saved     = (float *)malloc_alligned(m_window_size * m_number_of_instances * sizeof(float));
        m_impulse_complex  = (float *)malloc_alligned(m_window_size * m_number_of_instances * sizeof(float));
        
        m_buffer_samples   = (float *)malloc_alligned(m_window_size * sizeof(float));
        
#ifdef __APPLE__
        m_order                = log2f(m_window_size);
        m_fft_handle           = vDSP_create_fftsetup(m_order, FFT_RADIX2);
        m_output_split.realp   = m_output_complex;
        m_output_split.imagp   = m_output_complex + m_ir_size;
        m_input_split.realp    = m_input_complex;
        m_input_split.imagp    = m_input_complex + m_ir_size;
#else
        m_handle_forward = fftwf_plan_r2r_1d(m_window_size,  m_input_real,  m_input_complex, FFTW_R2HC, FFTW_ESTIMATE);
        m_handle_inverse = fftwf_plan_r2r_1d(m_window_size,  m_input_complex,  m_output_real, FFTW_HC2R, FFTW_ESTIMATE);
#endif
        m_index = 0;
        m_can_process = 0;
    }
    
    FourierConvolve::~FourierConvolve()
    {
#ifdef __APPLE__
        vDSP_destroy_fftsetup(m_fft_handle);
#else
        fftwf_destroy_plan(m_handle_forward);
        fftwf_destroy_plan(m_handle_inverse);
#endif
        free_alligned(m_input_real);
        free_alligned(m_output_real);
        free_alligned(m_input_complex);
        free_alligned(m_output_complex);
        free_alligned(m_output_saved);
        free_alligned(m_impulse_complex);
        free_alligned(m_buffer_samples);
        free_alligned(m_temp_output);

    }
    
    void FourierConvolve::setIr(float *buffer, int size)
    {
        int i, j;
        if(size - m_ir_offset <= 0)
            m_can_process = 0;
        else
            m_can_process = 1;
#ifdef __APPLE__
        vDSP_vclr(m_impulse_complex, 1, m_window_size * m_number_of_instances);
        
        DSPSplitComplex impulse_split;
        for(i = 0; i < m_number_of_instances; i++)
        {
            vDSP_vclr(m_buffer_samples, 1, m_window_size);
            for(j = 0; j < m_ir_size && (m_ir_offset + m_ir_size * i + j) < size; j++)
                m_buffer_samples[j] = buffer[m_ir_offset + m_ir_size * i + j] * m_scale;
            
            impulse_split.realp = (m_impulse_complex + i * m_window_size);
            impulse_split.imagp = (m_impulse_complex + i * m_window_size + m_ir_size);
            vDSP_ctoz((DSPComplex *)(m_buffer_samples), 2, &impulse_split, 1, m_ir_size);
            vDSP_fft_zrip(m_fft_handle, &impulse_split, 1, m_order, FFT_FORWARD);
        }
#else
        memset(m_buffer_samples, 0, m_window_size * sizeof(float));
        memset(m_impulse_complex, 0, m_window_size * m_number_of_instances * sizeof(float));

        for(i = 0; i < m_ir_size * m_number_of_instances && i < (size - m_ir_offset); i++)
            m_buffer_samples[i] = buffer[m_ir_offset + i];
        
        fftwf_plan	handle;
        handle = fftwf_plan_r2r_1d(m_window_size, m_buffer_samples, m_impulse_complex, FFTW_R2HC, FFTW_ESTIMATE);
        fftwf_execute(m_handle_forward);
        fftwf_destroy_plan(handle);
#endif
    }
    
    void FourierConvolve::prepare(int sampleframes)
    {
        m_index = 0;
#ifdef __APPLE__
        vDSP_vclr(m_input_real, 1, m_window_size);
        vDSP_vclr(m_output_real, 1, m_window_size);
        vDSP_vclr(m_temp_output, 1, m_window_size);
        vDSP_vclr(m_input_complex, 1, m_window_size);
        vDSP_vclr(m_output_complex, 1, m_window_size);
        vDSP_vclr(m_output_saved, 1, m_window_size * m_number_of_instances);
#else
        memset(m_input_real, 0, m_window_size * sizeof(float));
        memset(m_output_real, 0, m_window_size * sizeof(float));
        memset(m_temp_output, 0, m_window_size * sizeof(float));
        memset(m_input_complex, 0, m_window_size * sizeof(float));
        memset(m_output_complex, 0, m_window_size * sizeof(float));
        memset(m_output_saved, 0, m_window_size * m_number_of_instances * sizeof(float));
#endif
    }
    
    void FourierConvolve::perform(float* input, float* output, int sampleframes)
    {
        int ir_size     = m_ir_size;
        int window_size = m_window_size;
        float* real_in  = m_input_real;
        float* real_out = m_output_real;
        int ninstance   = m_number_of_instances;
        int inc         = floor(sampleframes / (ir_size * ninstance));
        
        if(!m_can_process)
        {
            memset(output, 0, sampleframes * sizeof(float));
            return;
        }
        else if(sampleframes <= ir_size)
        {
            // Save the sample in the input real array
            cblas_scopy(sampleframes, input, 1, real_in + m_index, 1);
            // Write the sample from the output real array
            cblas_scopy(sampleframes, real_out + m_index, 1, output, 1);
            
            int index = m_index / inc;
            post("index %i", index);
            m_index += sampleframes;
            // If the input real array has been filled
            if(m_index >= ir_size)
            {
                m_index = 0;
                
                float* real_saved   = m_output_saved;
                float* temp_out     = m_temp_output;
                DSPSplitComplex impulse_split;
                impulse_split.realp =  m_impulse_complex;
                impulse_split.imagp =  m_impulse_complex + ir_size;
                // Zero-pad the input real array
                vDSP_vclr(real_in + ir_size, 1, ir_size);
                
                // Perform the FFT forward
                vDSP_ctoz((DSPComplex *)real_in, 2, &m_input_split, 1, ir_size);
                vDSP_fft_zrip(m_fft_handle, &m_input_split, 1, m_order, FFT_FORWARD);
                
                // For the 1st instance
                // Perform the multiplication
                float preserve_ir, preserve_si;
                preserve_ir = impulse_split.imagp[0];
                impulse_split.imagp[0] = 0;
                preserve_si = m_input_split.imagp[0];
                m_input_split.imagp[0] = 0;
                
                vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                
                m_output_split.imagp[0] = preserve_ir * preserve_si;
                impulse_split.imagp[0] = preserve_ir;
                
                // Perform the FFT inverse
                vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                vDSP_ztoc(&m_output_split, 1, (DSPComplex *)real_out, 2, ir_size);
                
                // Scale the result by 1 / (8 * window_size)
                //vDSP_vsmul(real_out, 1, &m_scale, real_out, 1, window_size);
                
                // Add the saved results to the output real array
                vDSP_vadd(real_out, 1, real_saved, 1, real_out, 1, ir_size);
                
                // Save the end of the result
                cblas_scopy(ir_size, real_out + ir_size, 1, real_saved, 1);
                
                // For each other instance
                for(int i = 1; i < ninstance; i++)
                {
                    real_saved = m_output_saved + (i - 1) * ir_size;
                    impulse_split.realp = m_impulse_complex + i * window_size;
                    impulse_split.imagp = impulse_split.realp + ir_size;
                    
                    // Perform the multiplication
                    preserve_ir = impulse_split.imagp[0];
                    impulse_split.imagp[0] = 0;
                    //preserve_si = m_input_split.imagp[0];
                    //m_input_split.imagp[0] = 0;
                    
                    vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                    
                    m_output_split.imagp[0] = preserve_ir * preserve_si;
                    impulse_split.imagp[0] = preserve_ir;
                    
                    // Perform the FFT inverse
                    vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                    
                    // Clear the output real array
                    vDSP_ztoc(&m_output_split, 1, (DSPComplex *)temp_out, 2, ir_size);
                    
                    // Scale the result by 1 / (8 * window_size)
                    //vDSP_vsmul(temp_out, 1, &m_scale, temp_out, 1, window_size);
                    
                    // Move the previous saved result to -irsize
                    vDSP_vadd(real_saved, 1, real_saved+ir_size, 1, real_saved, 1, ir_size);
                    
                    // Clear the previous saved result
                    vDSP_vclr(real_saved + ir_size, 1, ir_size);
                    
                    // Add the new result to the saved array
                    vDSP_vadd(real_saved, 1, temp_out, 1, real_saved, 1, window_size);
                }
            }
        }
        else
        {
            for(int i = 0; i < sampleframes; i += ir_size)
            {
                // Save the sample in the input real array
                cblas_scopy(ir_size, input+i, 1, real_in, 1);
                // Write the sample from the output real array
                cblas_scopy(ir_size, real_out, 1, output+i, 1);
                float* real_saved   = m_output_saved;
                float* temp_out     = m_temp_output;
                DSPSplitComplex impulse_split;
                impulse_split.realp =  m_impulse_complex;
                impulse_split.imagp =  m_impulse_complex + ir_size;
                // Zero-pad the input real array
                vDSP_vclr(real_in + ir_size, 1, ir_size);
                
                // Perform the FFT forward
                vDSP_ctoz((DSPComplex *)real_in, 2, &m_input_split, 1, ir_size);
                vDSP_fft_zrip(m_fft_handle, &m_input_split, 1, m_order, FFT_FORWARD);
                
                // For the 1st instance
                // Perform the multiplication
                float preserve_ir, preserve_si;
                preserve_ir = impulse_split.imagp[0];
                impulse_split.imagp[0] = 0;
                preserve_si = m_input_split.imagp[0];
                m_input_split.imagp[0] = 0;
                
                vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                
                m_output_split.imagp[0] = preserve_ir * preserve_si;
                impulse_split.imagp[0] = preserve_ir;
                
                // Perform the FFT inverse
                vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                vDSP_ztoc(&m_output_split, 1, (DSPComplex *)real_out, 2, ir_size);
                
                // Scale the result by 1 / (8 * window_size)
                vDSP_vsmul(real_out, 1, &m_scale, real_out, 1, window_size);
                
                // Add the saved results to the output real array
                vDSP_vadd(real_out, 1, real_saved, 1, real_out, 1, ir_size);
                
                // Save the end of the result
                cblas_scopy(ir_size, real_out + ir_size, 1, real_saved, 1);
                
                // For each other instance
                for(int i = 1; i < ninstance; i++)
                {
                    impulse_split.realp += window_size;
                    impulse_split.imagp += window_size;
                    
                    // Perform the multiplication
                    preserve_ir = impulse_split.imagp[0];
                    impulse_split.imagp[0] = 0;
                    preserve_si = m_input_split.imagp[0];
                    m_input_split.imagp[0] = 0;
                    
                    vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                    
                    m_output_split.imagp[0] = preserve_ir * preserve_si;
                    impulse_split.imagp[0] = preserve_ir;
                    
                    // Perform the FFT inverse
                    vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                    
                    // Clear the output real array
                    vDSP_ztoc(&m_output_split, 1, (DSPComplex *)temp_out, 2, ir_size);
                    
                    // Scale the result by 1 / (8 * window_size)
                    vDSP_vsmul(temp_out, 1, &m_scale, temp_out, 1, window_size);
                    
                    // Move the previous saved result to -irsize
                    vDSP_vadd(real_saved, 1, real_saved+ir_size, 1, real_saved, 1, ir_size);
                    
                    // Clear the previous saved result
                    vDSP_vclr(real_saved + ir_size, 1, ir_size);
                    
                    // Add the new result to the saved array
                    vDSP_vadd(real_saved, 1, temp_out, 1, real_saved, 1, window_size);
                    real_saved += ir_size;
                }
            }
        }
    }
    
    void FourierConvolve::performAdd(float* input, float* output, int sampleframes)
    {
        int ir_size     = m_ir_size;
        int window_size = m_window_size;
        float* real_in  = m_input_real;
        float* real_out = m_output_real;
        
        if(!m_can_process)
            return;
        
        if(sampleframes > ir_size)
            goto next;
        
        // Save the sample in the input real array
        cblas_scopy(sampleframes, input, 1, real_in + m_index, 1);
        // Add the sample from the output real array
#ifdef __APPLE__
        vDSP_vadd(output, 1, real_out + m_index, 1, output, 1, sampleframes);
#else
#endif
        m_index += sampleframes;
        
        // If the input real array has been filled
        if(m_index >= ir_size)
        {
            m_index = 0;
            int ninstance       = m_number_of_instances;
            float* real_saved   = m_output_saved;
            float* temp_out     = m_temp_output;
#ifdef __APPLE__
            DSPSplitComplex impulse_split;
            impulse_split.realp =  m_impulse_complex;
            impulse_split.imagp =  m_impulse_complex + ir_size;
            // Zero-pad the input real array
            vDSP_vclr(real_in + ir_size, 1, ir_size);
            
            // Perform the FFT forward
            vDSP_ctoz((DSPComplex *)real_in, 2, &m_input_split, 1, ir_size);
            vDSP_fft_zrip(m_fft_handle, &m_input_split, 1, m_order, FFT_FORWARD);
            
            // For the 1st instance
            // Perform the multiplication
            float preserve_ir, preserve_si;
            preserve_ir = impulse_split.imagp[0];
            impulse_split.imagp[0] = 0;
            preserve_si = m_input_split.imagp[0];
            m_input_split.imagp[0] = 0;
            
            vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
            
            m_output_split.imagp[0] = preserve_ir * preserve_si;
            impulse_split.imagp[0] = preserve_ir;
            
            // Perform the FFT inverse
            vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
            vDSP_ztoc(&m_output_split, 1, (DSPComplex *)real_out, 2, ir_size);
            
            // Scale the result by 1 / (8 * window_size)
            vDSP_vsmul(real_out, 1, &m_scale, real_out, 1, window_size);
            
            // Add the saved results to the output real array
            vDSP_vadd(real_out, 1, real_saved, 1, real_out, 1, ir_size);
            
            // Save the end of the result
            cblas_scopy(ir_size, real_out + ir_size, 1, real_saved, 1);
            
            // For each other instance
            for(int i = 1; i < ninstance; i++)
            {
                impulse_split.realp += window_size;
                impulse_split.imagp += window_size;
                
                // Perform the multiplication
                preserve_ir = impulse_split.imagp[0];
                impulse_split.imagp[0] = 0;
                preserve_si = m_input_split.imagp[0];
                m_input_split.imagp[0] = 0;
                
                vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                
                m_output_split.imagp[0] = preserve_ir * preserve_si;
                impulse_split.imagp[0] = preserve_ir;
                
                // Perform the FFT inverse
                vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                
                // Clear the output real array
                vDSP_ztoc(&m_output_split, 1, (DSPComplex *)temp_out, 2, ir_size);
                
                // Scale the result by 1 / (8 * window_size)
                vDSP_vsmul(temp_out, 1, &m_scale, temp_out, 1, window_size);
                
                // Move the previous saved result to -irsize
                vDSP_vadd(real_saved, 1, real_saved+ir_size, 1, real_saved, 1, ir_size);
                
                // Clear the previous saved result
                vDSP_vclr(real_saved + ir_size, 1, ir_size);
                
                // Add the new result to the saved array
                vDSP_vadd(real_saved, 1, temp_out, 1, real_saved, 1, window_size);
                real_saved += ir_size;
            }
#else
            // Zero-pad the input real array
            memset(real_in + ir_size, 0, ir_size * sizeof(float));
            // Zero init the output real array
            memset(real_out, 0, window_size * sizeof(float));
            
            fftwf_execute(m_handle_forward);
            fftwf_execute(m_handle_inverse);
#endif
        }
        
        return;
        
    next:
        
        for(int i = 0; i < sampleframes; i += ir_size)
        {
            // Save the sample in the input real array
            cblas_scopy(ir_size, input+i, 1, real_in, 1);
            // Add the sample from the output real array
#ifdef __APPLE__
            vDSP_vadd(output+i, 1, real_out, 1, output+i, 1, ir_size);
#else
#endif
#ifdef __APPLE__
            int ninstance       = m_number_of_instances;
            float* real_saved   = m_output_saved;
            float* temp_out     = m_temp_output;
            DSPSplitComplex impulse_split;
            impulse_split.realp =  m_impulse_complex;
            impulse_split.imagp =  m_impulse_complex + ir_size;
            // Zero-pad the input real array
            vDSP_vclr(real_in + ir_size, 1, ir_size);
            
            // Perform the FFT forward
            vDSP_ctoz((DSPComplex *)real_in, 2, &m_input_split, 1, ir_size);
            vDSP_fft_zrip(m_fft_handle, &m_input_split, 1, m_order, FFT_FORWARD);
            
            // For the 1st instance
            // Perform the multiplication
            float preserve_ir, preserve_si;
            preserve_ir = impulse_split.imagp[0];
            impulse_split.imagp[0] = 0;
            preserve_si = m_input_split.imagp[0];
            m_input_split.imagp[0] = 0;
            
            vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
            
            m_output_split.imagp[0] = preserve_ir * preserve_si;
            impulse_split.imagp[0] = preserve_ir;
            
            // Perform the FFT inverse
            vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
            vDSP_ztoc(&m_output_split, 1, (DSPComplex *)real_out, 2, ir_size);
            
            // Scale the result by 1 / (8 * window_size)
            vDSP_vsmul(real_out, 1, &m_scale, real_out, 1, window_size);
            
            // Add the saved results to the output real array
            vDSP_vadd(real_out, 1, real_saved, 1, real_out, 1, ir_size);
            
            // Save the end of the result
            cblas_scopy(ir_size, real_out + ir_size, 1, real_saved, 1);
            
            // For each other instance
            for(int i = 1; i < ninstance; i++)
            {
                impulse_split.realp += window_size;
                impulse_split.imagp += window_size;
                
                // Perform the multiplication
                preserve_ir = impulse_split.imagp[0];
                impulse_split.imagp[0] = 0;
                preserve_si = m_input_split.imagp[0];
                m_input_split.imagp[0] = 0;
                
                vDSP_zvmul(&m_input_split, 1, &impulse_split, 1, &m_output_split, 1, ir_size, 1);
                
                m_output_split.imagp[0] = preserve_ir * preserve_si;
                impulse_split.imagp[0] = preserve_ir;
                
                // Perform the FFT inverse
                vDSP_fft_zrip(m_fft_handle, &m_output_split, 1, m_order, FFT_INVERSE);
                
                // Clear the output real array
                vDSP_ztoc(&m_output_split, 1, (DSPComplex *)temp_out, 2, ir_size);
                
                // Scale the result by 1 / (8 * window_size)
                vDSP_vsmul(temp_out, 1, &m_scale, temp_out, 1, window_size);
                
                // Move the previous saved result to -irsize
                vDSP_vadd(real_saved, 1, real_saved+ir_size, 1, real_saved, 1, ir_size);
                
                // Clear the previous saved result
                vDSP_vclr(real_saved + ir_size, 1, ir_size);
                
                // Add the new result to the saved array
                vDSP_vadd(real_saved, 1, temp_out, 1, real_saved, 1, window_size);
                real_saved += ir_size;
            }
            
#else
            fftwf_execute(m_handle_forward);
            fftwf_execute(m_handle_inverse);
#endif
        }
        return;
    }
    
}

