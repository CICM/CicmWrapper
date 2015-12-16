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

#ifndef DEF_EPARAM
#define DEF_EPARAM

#include "edefine.h"

/*! @defgroup groupparampart The parameter part of the library.
 * @brief The parameter part of the library.
 * @details This part refers to the methods and structures that can be used
 * @details to manage proxy parameters.
 */

/*! @addtogroup groupparampart
 *  @{
 */

/**
 * @struct _eparam
 * @brief The parameter object.
 * @details It contains the members and the methods for a parameter.
 */

EXTERN_STRUCT _eparam;
typedef struct _eparam t_eparam;

//! The t_param_setter method
typedef void  (*t_param_setter)(t_object* x, t_eparam* p, float f);
//! The t_param_getter method
typedef float (*t_param_getter)(t_object* x, t_eparam* p);
//! The t_param_setter_t method
typedef void  (*t_param_setter_t)(t_object* x, int index, char const* text);
//! The t_param_getter_t method
typedef void (*t_param_getter_t)(t_object* x, int index, char* text);


void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv);
t_eparam* ebox_parameter_create(t_ebox *x, int index);
void ebox_parameter_destroy(t_ebox* x, int index);
float ebox_parameter_getvalue(t_ebox* x, int index);
void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify);
float ebox_parameter_getvalue_normalized(t_ebox* x, int index);
void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify);
float ebox_parameter_getmin(t_ebox* x, int index);
float ebox_parameter_getmax(t_ebox* x, int index);
int ebox_parameter_getnstep(t_ebox* x, int index);
t_symbol* ebox_parameter_getbind(t_ebox* x, int index);
char ebox_parameter_isinverted(t_ebox* x, int index);
void ebox_parameter_notify_changes(t_ebox *x, int index);
void ebox_parameter_begin_changes(t_ebox *x, int index);
void ebox_parameter_end_changes(t_ebox *x, int index);
void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name);
void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label);
void ebox_parameter_setminmax(t_ebox* x, int index, float min, float max);
void ebox_parameter_setnstep(t_ebox* x, int index, int nstep);
void ebox_parameter_setindex(t_ebox* x, int index, int pindex);
void ebox_parameter_setflags(t_ebox* x, int index, long flags);
void ebox_parameter_enable(t_ebox* x, int index);
void ebox_parameter_disable(t_ebox* x, int index);
void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_getter getter);
void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter);
t_eparam* eparameter_getbyindex(t_ebox* x, int index);

/*!
 * \fn              float eparameter_getvalue(t_eparam* param)
 * \brief           Retrieves the current (non-normalized) value of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \return          This function returns the current value of the parameter.
 */
float eparameter_getvalue(t_eparam* param);

/*!
 * \fn              float eparameter_getvalue_normalized(t_eparam* param)
 * \brief           Retrieves the current normalized value of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \return          This function returns the current value of the parameter.
 */
float eparameter_getvalue_normalized(t_eparam* param);

/*!
 * \fn              void eparameter_getvalue_text(t_eparam* param, char* text);
 * \brief           Retrieves the current value of the parameter as a string.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param text      The string to fill (the size of the string should be big enough to retrieve the value).
 */
void eparameter_getvalue_text(t_eparam* param, char* text);

/*!
 * \fn              void eparameter_setvalue(t_eparam* param, float value)
 * \brief           Sets the current (non-normalized) value of the parameter and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param value     The new value of the parameter.
 */
void eparameter_setvalue(t_eparam* param, float value);

/*!
 * \fn              void eparameter_setvalue_normalized(t_eparam* param, float value)
 * \brief           Sets the current normalized value of the parameter and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param value     The new value of the parameter.
 */
void eparameter_setvalue_normalized(t_eparam* param, float value);

/*!
 * \fn              void eparameter_setvalue_text(t_eparam* param, char* text)
 * \brief           Sets the current value of the parameter with a string and notify the owner that the value changed.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param text      The value as a string.
 */
void eparameter_setvalue_text(t_eparam* param, char const* text);

/*!
 * \fn              void eparameter_setname(t_eparam* param, t_symbol* name)
 * \brief           Sets name of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param name      The name of the parameter.
 */
void eparameter_setname(t_eparam* param, t_symbol* name);

/*!
 * \fn              void eparameter_setlabel(t_eparam* param, t_symbol* label)
 * \brief           Sets label of the parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param label     The label of the parameter.
 */
void eparameter_setlabel(t_eparam* param, t_symbol* label);

/*!
 * \fn              void eparameter_setindex(t_eparam* param, int index)
 * \brief           Sets the internal index of parameter.
 * \details         This function shoudl be called from outside the owner object.
 * \param param     The t_eparam pointer.
 * \param index     The index of the parameter.
 */
void eparameter_setindex(t_eparam* param, int index);

/*!
 * \fn          t_eparam* eparameter_getfromsymbol(t_symbol* name)
 * \brief       Retrieves a t_eparam from a symbol.
 * \param name  The binding symbol of the t_eparam.
 */
t_eparam* eparameter_getfromsymbol(t_symbol* name);




/** @} */

#endif



