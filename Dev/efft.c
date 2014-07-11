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

#include "efft.h"

// WINDOW //
void ewindow_square(float* window, int size)
{
    for(int i = 0; i < size; i++)
		window[i] = 1.;
}

void ewindow_hanning(float* window, int size)
{
    for(int i = 0; i < size; i++)
		window[i] = 0.5 * (1.0 - cos(2.0 * EPD_PI * (double)i / (double)(size - 1)));
}

void ewindow_hamming(float* window, int size)
{
    for(int i = 0; i < size; i++)
		window[i] = 0.54 - (0.46 * cos(2.0 * EPD_PI * (double)i / (double)(size - 1)));
}

void ewindow_tukey(float* window, int size)
{
    float a0 = 0.5;
	for(int i = 0; i < size; i++)
    {
		if(i <= ((a0 * (double)(size - 1)) / 2.))
			window[i] = 0.5 * ( 1. + cos(EPD_PI * ( ((2 * (double)i) / (a0 * (double)(size - 1))) - 1.)));
		else if(i > ((a0 * (double)(size - 1)) / 2.) && i <= ((double)(size - 1) * (1. - ( a0 / 2.))))
			window[i] = 1.;
		else
			window[i] = 0.5 * ( 1. + cos(EPD_PI * ( ((2 *(double)i) / (a0 * (double)(size - 1))) + 1. - (2. / a0))));
    }
}

void ewindow_halfsinus(float* window, int size)
{
	for(int i = 0; i < size; i++)
		window[i] = sin((EPD_PI * (double)i) / (double)(size - 1));
}

void ewindow_sinus(float* window, int size)
{
	for(int i = 0; i < size; i++)
		window[i] = sin((EPD_2PI * (double)i) / (double)(size - 1));
}

void ewindow_lanczos(float* window, int size)
{
	for(int i = 0; i < size; i++)
		window[i] = sin(EPD_PI * (((2. * (double)i) / (double)(size - 1)) - 1.)) / (EPD_PI * (((2. * (double)i) / (double)(size - 1)) - 1.));
}

void ewindow_triangular(float* window, int size)
{
	for(int i = 0; i < size; i++)
		window[i] = (2. / (double)(size + 1)) * (((double)(size + 1) / 2.) - fabs((double)i - (double)(size - 1) / 2.));
}

void ewindow_bartlett(float* window, int size)
{
	for(int i = 0; i < size; i++)
		window[i] = (2. / (double)(size - 1)) * (((double)(size - 1) / 2.) - fabs((double)i - ((double)(size - 1) / 2.)));
}

void ewindow_gaussian(float* window, int size)
{
	float a0 = 0.5;
	for(int i = 0; i < size; i++)
		window[i] = exp((i - (double)(size - 1) / 2.)/(a0 * (double)(size - 1) / 2.) * (i - (double)(size - 1) / 2.)/(a0 * (double)(size - 1) / 2.) * -0.5);
}

void ewindow_bartlettHann(float* window, int size)
{
	float a0 = 0.62;
	float a1 = 0.48;
	float a2 = 0.38;
	for(int i = 0; i < size; i++)
		window[i] = a0 - a1 * fabs(((double)i / (double)(size - 1)) - 0.5) - a2 * cos((2.0 * EPD_PI *(double)i) / (double)(size - 1));
}

void ewindow_blackman(float* window, int size)
{
	float a0 = 0.16;
	for(int i = 0; i < size; i++)
		window[i] = ((1. - a0) / 2.) - (0.5 * cos(2.0 * EPD_PI * (double)i / (double)(size - 1))) + ((a0 / 2.) * cos(4.0 * EPD_PI * (double)i / (double)(size - 1)));
}

// SPECTRUM //

void espectrum_magnitude(float* spectrum, float* real, float* imag, int size)
{
    for(int i = 0; i < size; i++)
		spectrum[i] = sqrtf(real[i] * real[i] + imag[i] * imag[i]);
}

void espectrum_power(float* spectrum, float* real, float* imag, int size)
{
    for(int i = 0; i < size; i++)
		spectrum[i] = sqrtf(real[i] * real[i] + imag[i] * imag[i]);
    for(int i = 0; i < size; i++)
		spectrum[i] *= spectrum[i];
}

void espectrum_decibel(float* spectrum, float* real, float* imag,int size)
{
    for(int i = 0; i < size; i++)
        spectrum[i] = sqrtf(real[i] * real[i] + imag[i] * imag[i]);
    for(int i = 0; i < size; i++)
    {
        if(spectrum[i] > 0.000001f)
            spectrum[i] = pd_clip_min(20. * log10f(spectrum[i]), -90.f);
        else
            spectrum[i] = -90.;
    }
}

int efft_getlog2(long size)
{
    if(size <= 2)
        size = 2;
    else if(size <= 4)
        size = 4;
    else if(size <= 8)
        size = 8;
    else if(size <= 16)
        size = 16;
    else if(size <= 32)
        size = 32;
    else if(size <= 64)
        size = 64;
    else if(size <= 128)
        size = 128;
    else if(size <= 256)
        size = 256;
    else if(size <= 512)
        size = 512;
    else if(size <= 1024)
        size = 1024;
    else if(size <= 2048)
        size = 2048;
    else if(size <= 4096)
        size = 4096;
    else if(size <= 8192)
        size = 8192;
    else if(size <= 16384)
        size = 16384;
    else if(size <= 32768)
        size = 32768;
    else
        size = 65536;
    
    return size;
}




