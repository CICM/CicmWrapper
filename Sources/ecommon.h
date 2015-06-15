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

/**
 * @file ecommon.h
 * @brief The header that declares all the common methods.
 * @author Pierre Guillot
 */

#ifndef DEF_EPD_COMMON
#define DEF_EPD_COMMON

#include "edefine.h"

void object_method(void* x, t_symbol* s, void* z, method method, long number, void* other);

/*! @addtogroup groupcommon The Common Part
 *  @{
 */

//! @cond
void epd_init(void);
//! @endcond

/*!
 * \fn          long atoms_get_attributes_offset(int ac, t_atom* av)
 * \brief       Retrieves the position of the first attribute in an array of t_atom.
 * \details     The function looks for the first t_symbol* that starts with the character @.
 * \param argc  The size of the array of t_atom.
 * \param argv  The array of t_atom.
 * \return The index of the first attribute or the maximum if there's no attribute.
 */
long atoms_get_attributes_offset(int argc, t_atom* argv);

/*!
 * \fn          long binbuf_get_attributes_offset(t_binbuf *d)
 * \brief       Retrieves the position of the first attribute in a t_binbuf.
 * \details     The function looks for the first t_symbol* that starts with the character @.
 * \param d     The t_binbuf.
 * \return The index of the first attribute or the maximum if there's no attribute.
 */
long binbuf_get_attributes_offset(t_binbuf *d);

/*!
 * \fn          long atoms_get_nattributes(int ac, t_atom* av)
 * \brief       Retrieves the number of attributes in an array of t_atom.
 * \details     The function looks for the symbols that start with the character @.
 * \param argc  The size of the array of t_atom.
 * \param argv  The array of t_atom.
 * \return The number of attributes.
 */
long atoms_get_nattributes(int argc, t_atom* argv);

/*!
 * \fn          long binbuf_get_nattributes(t_binbuf *d)
 * \brief       Retrieves the number of attributes in a t_binbuf.
 * \details     The function looks for the symbols that start with the character @.
 * \param d     The t_binbuf.
 * \return The number of attributes.
 */
long binbuf_get_nattributes(t_binbuf *d);

/*!
 * \fn          t_pd_err atoms_has_attribute(int ac, t_atom* av, t_symbol *key)
 * \brief       Retrieves if an attributes is in an array of t_atom.
 * \details     The function looks for the key t_symbol* of the attribute.
 * \param argc  The size of the array of t_atom.
 * \param argv  The array of t_atom.
 * \param key   The key of the attribute.
 * \return 1 if the attribute is in the array of t_atom, otherwise 0.
 */
t_pd_err atoms_has_attribute(int argc, t_atom* argv, t_symbol *key);

/*!
 * \fn          t_pd_err binbuf_has_attribute(t_binbuf *d, t_symbol *key)
 * \brief       Retrieves if an attributes is in a t_binbuf.
 * \details     The function looks for the key t_symbol* of the attribute.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \return 1 if the attribute is in the array of t_atom, otherwise 0.
 */
t_pd_err binbuf_has_attribute(t_binbuf *d, t_symbol *key);

/*!
 * \fn          long atoms_get_attribute_index(int argc, t_atom *argv, t_symbol *key)
 * \brief       Retrieves the position of an attribute in an array of t_atom.
 * \details     The function looks for position of the key t_symbol* of the attribute.
 * \param argc  The size of the array of t_atom.
 * \param argv  The array of t_atom.
 * \param key   The key of the attribute.
 * \return The position of the attribute or the maximum if the attribute isn't in the array.
 */
long atoms_get_attribute_index(int argc, t_atom *argv, t_symbol *key);

/*!
 * \fn          long binbuf_get_attribute_index(t_binbuf *d, t_symbol *key)
 * \brief       Retrieves the position of an attribute in a t_binbuf.
 * \details     The function looks for position of the key t_symbol* of the attribute.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \return The position of the attribute or the maximum if the attribute isn't in the array.
 */
long binbuf_get_attribute_index(t_binbuf *d, t_symbol *key);

/*!
 * \fn          t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv)
 * \brief       Retrieves the values of an attribute from an array of t_atom.
 * \details     The function looks for an attribute and retrieves its values. You should free the result memory if it has been allocated.
 * \param ac  The size of the array of t_atom.
 * \param av  The array of t_atom.
 * \param key   The key of the attribute.
 * \param argc  The pointer to the int that will be initialized with number of values of the attributes.
 * \param argv  The pointer to the array of t_atom that will be initalized with the values of the attributes.
 * \return 0 if the attribute values has been returned.
 */
t_pd_err atoms_get_attribute(int ac, t_atom* av, t_symbol *key, int *argc, t_atom **argv);

/*!
 * \fn          t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, int *argc, t_atom **argv)
 * \brief       Retrieves the values of an attribute from a t_binbuf.
 * \details     The function looks for an attribute and retrieves its values. You should free the result memory if it has been allocated.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \param argc  The pointer to the int that will be initialized with number of values of the attributes.
 * \param argv  The pointer to the array of t_atom that will be initalized with the values of the attributes.
 * \return 0 if the attribute values has been returned.
 */
t_pd_err binbuf_get_attribute(t_binbuf *d, t_symbol *key, int *argc, t_atom **argv);

/*!
 * \fn          t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol *key, long *value)
 * \brief       Retrieves the long value of an attribute from an array of t_atom.
 * \details     The function looks for an attribute and retrieves its long value.
 * \param ac  The size of the array of t_atom.
 * \param av  The array of t_atom.
 * \param key   The key of the attribute.
 * \param value  The pointer to the long value that will be initialized with number of values of the attributes.
 * \return 0 if the attribute value has been returned.
 */
t_pd_err atoms_get_attribute_long(int ac, t_atom* av, t_symbol *key, long *value);

/*!
 * \fn          t_pd_err binbuf_get_attribute_long(t_binbuf *d, t_symbol *key, long *value)
 * \brief       Retrieves the long value of an attribute from a t_binbuf.
 * \details     The function looks for an attribute and retrieves its long value.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \param value  The pointer to the long value that will be initialized with number of values of the attributes.
 * \return 0 if the attribute value has been returned.
 */
t_pd_err binbuf_get_attribute_long(t_binbuf *d, t_symbol *key, long *value);

/*!
 * \fn          t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol *key, float *value)
 * \brief       Retrieves the float value of an attribute from an array of t_atom.
 * \details     The function looks for an attribute and retrieves its float value.
 * \param ac  The size of the array of t_atom.
 * \param av  The array of t_atom.
 * \param key   The key of the attribute.
 * \param value  The pointer to the float value that will be initialized with number of values of the attributes.
 * \return 0 if the attribute value has been returned.
 */
t_pd_err atoms_get_attribute_float(int ac, t_atom* av, t_symbol *key, float *value);

/*!
 * \fn          t_pd_err binbuf_get_attribute_float(t_binbuf *d, t_symbol *key, float *value)
 * \brief       Retrieves the float value of an attribute from a t_binbuf.
 * \details     The function looks for an attribute and retrieves its float value.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \param value  The pointer to the float value that will be initialized with number of values of the attributes.
 * \return 0 if the attribute value has been returned.
 */
t_pd_err binbuf_get_attribute_float(t_binbuf *d, t_symbol *key, float *value);

/*!
 * \fn          long atoms_get_keys(int ac, t_atom* av, t_symbol*** s)
 * \brief       Retrieves the keys of attributes presents in an array of t_atom.
 * \details     The function retrieves all the t_symbol* that start with the character @. You should free the result memory if it has been allocated.
 * \param ac    The size of the array of t_atom.
 * \param av    The array of t_atom.
 * \param keys  The pointer to an array of t_symbol* that will be allocated if keys has been found.
 * \return The number of keys.
 */
long atoms_get_keys(int ac, t_atom* av, t_symbol*** keys);

/*!
 * \fn          long binbuf_get_keys(t_binbuf *d, t_symbol*** keys)
 * \brief       Retrieves the keys of attributes presents in a t_binbuf.
 * \details     The function retrieves all the t_symbol* that start with the character @. You should free the result memory if it has been allocated.
 * \param d     The t_binbuf.
 * \param keys  The pointer to an array of t_symbol* that will be allocated if keys has been found.
 * \return The number of keys.
 */
long binbuf_get_keys(t_binbuf *d, t_symbol*** keys);

/*!
 * \fn          t_binbuf* binbuf_via_atoms(int ac, t_atom *av)
 * \brief       Retrieves a t_binbuf from an array of t_atom.
 * \details     The function allocates a t_binbuf and intialized it with the array of t_atom.
 * \param ac    The size of the array of t_atom.
 * \param av    The array of t_atom.
 * \return The t_binbuf or NULL if the allocation goes wrong.
 */
t_binbuf* binbuf_via_atoms(int ac, t_atom *av);

/*!
 * \fn          long binbuf_append_attribute(t_binbuf *d, t_symbol *key, int argc, t_atom *argv)
 * \brief       Adds the key and the values of an attribute to a t_binbuf.
 * \details     The function format and append the key and the values of an attribute.
 * \param d     The t_binbuf.
 * \param key   The key of the attribute.
 * \param argc  The size of the array of t_atom.
 * \param argv  The array of t_atom that owns the values.
 * \return 0 if the attribute has been added.
 */
t_pd_err binbuf_append_attribute(t_binbuf *d, t_symbol *key, int argc, t_atom *argv);

double pd_clip_min(double aValue, double aMinimum);
double pd_clip_max(double aValue, double aMaximum);
double pd_clip_minmax(double aValue, double aMinimum, double aMaximum);
double pd_ordinate(double radius, double angle);
double pd_abscissa(double radius, double angle);
double pd_radius(double x, double y);
double pd_angle(double x, double y);

void pd_library_add_folder(char* libraryname, char* folder);

/** @} */
#endif
