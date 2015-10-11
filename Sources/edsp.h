/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

/**
 * \file eattr.h
 * \brief The header that declares all the methods for the t_eattr.
 * \author Pierre Guillot
 */

#ifndef DEF_EDSP
#define DEF_EDSP

#include "edefine.h"

/*! @defgroup groupdsppart The dsp part of the library.
 * @brief The dsp part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage digital signal processing.
 */

/*! @addtogroup groupdsppart
 *  @{
 */

/**
 * @struct _edsp
 * @brief The dsp manager object.
 * @details It contains the members and the methods for dsp management.
 */
EXTERN_STRUCT _edsp;
typedef struct _edsp t_edsp;

/*!< The signal processing can be inplace. */
#define EPD_INPLACE    (1<<0)
/*!< The signal processing can't be inplace. */
#define EPD_NOINPLACE  (1<<1)

//! The dsp perform method
typedef void (*t_perform_method)(t_object *x, t_object *dsp,
            t_sample **ins, long ni, t_sample **outs, long no, long nsamples, long f,void *up);

//! The dsp prepare method
typedef void (*t_dsp_method)(t_object *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);

/*!
 * \fn              t_edsp* edsp_new(void)
 * \brief           Allocates and initializes a new dsp manager.
 * \param owner     The owner of the dsp manager.
 * \param nins      The default number of inputs.
 * \param nouts     The default number of outputs.
 * \return          The new dsp manager.
 */
t_edsp* edsp_new(t_object* owner, size_t nins, size_t nouts);

/*!
 * \fn              t_edsp* edsp_findbyname(t_symbol* name)
 * \brief           Retrieves the dsp manager that matchs to the name.
 * \param name      The name of the dsp manager.
 * \return          The dsp manager that matchs to the name if it exists, othersise NULL.
 */
t_edsp* edsp_findbyname(t_symbol* name);

/** @} */

#endif



