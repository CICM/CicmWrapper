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

/*!
 * @file efft.h
 * @author Pierre Guillot
 * @version 0.1
 * @date 2013-2014
 */

#ifndef DEF_EFFT
#define DEF_EFFT

#include "estruct.h"

void ewindow_square(float* window, int size);
void ewindow_hanning(float* window, int size);
void ewindow_hamming(float* window, int size);
void ewindow_tukey(float* window, int size);
void ewindow_halfsinus(float* window, int size);
void ewindow_sinus(float* window, int size);
void ewindow_lanczos(float* window, int size);
void ewindow_triangular(float* window, int size);
void ewindow_bartlett(float* window, int size);
void ewindow_gaussian(float* window, int size);
void ewindow_bartlettHann(float* window, int size);
void ewindow_blackman(float* window, int size);

void espectrum_magnitude(float* spectrum, float* real, float* imag, int size);
void espectrum_power(float* spectrum, float* real, float* imag, int size);
void espectrum_decibel(float* spectrum, float* real, float* imag, int size);

int efft_getlog2(long size);
#endif



