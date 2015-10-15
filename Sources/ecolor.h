/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_ECOLOR
#define DEF_ECOLOR

#include "ecommon.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!
 * \fn          void rgba_set(t_rgba *color, const float red, const float green, const float blue, const float alpha)
 * \brief       Intializes a t_rgba color.
 * \param color The t_rgba color.
 * \param red   The red component.
 * \param green The green component.
 * \param blue  The blue component.
 * \param alpha The alpha component.
 */
void rgba_set(t_rgba *color, const float red, const float green, const float blue, const float alpha);

/*!
 * \fn          void rgb_set(t_rgb *color, const float red, const float green, const float blue)
 * \brief       Intializes a t_rgb color.
 * \param color The t_rgb color.
 * \param red   The red component.
 * \param green The green component.
 * \param blue  The blue component.
 */
void rgb_set(t_rgb *color, const float red, const float green, const float blue);

/*!
 * \fn                  void hsla_set(t_hsla *color, const float hue, const float saturation, const float lightness, const float alpha)
 * \brief               Intializes a t_hsla color.
 * \param color         The t_hsla color.
 * \param hue           The hue component.
 * \param saturation    The saturation component.
 * \param lightness     The lightness component.
 * \param alpha         The alpha component.
 */
void hsla_set(t_hsla *color, const float hue, const float saturation, const float lightness, const float alpha);

/*!
 * \fn                  void hsl_set(t_hsl *color, const float hue, const float saturation, const float lightness)
 * \brief               Intializes a t_hsl color.
 * \param color         The t_hsl color.
 * \param hue           The hue component.
 * \param saturation    The saturation component.
 * \param lightness     The lightness component.
 */
void hsl_set(t_hsl *color, const float hue, const float saturation, const float lightness);

/*!
 * \fn          char* rgba_to_hex(t_rgba color)
 * \brief       Converts a t_rgba color to an hexadecimal color.
 * \param color The t_rgba color.
 * \return The hexadecimal color.
 */
char* rgba_to_hex(t_rgba const* color);

/*!
 * \fn          char* rgb_to_hex(t_rgb color)
 * \brief       Converts a t_rgb color to an hexadecimal color.
 * \param color The t_rgb color.
 * \return The hexadecimal color.
 */
char* rgb_to_hex(t_rgb const*color);

/*!
 * \fn          char* hsla_to_hex(t_hsla color)
 * \brief       Converts a t_hsla color to an hexadecimal color.
 * \param color The t_hsla color.
 * \return The hexadecimal color.
 */
char* hsla_to_hex(t_hsla const* color);

/*!
 * \fn          char* hsl_to_hex(t_hsl color)
 * \brief       Converts a t_hsl color to an hexadecimal color.
 * \param color The t_hsl color.
 * \return The hexadecimal color.
 */
char* hsl_to_hex(t_hsl const* color);

/*!
 * \fn          t_hsla rgba_to_hsla(t_rgba color)
 * \brief       Converts a t_rgba color to a t_hsla color.
 * \param color The t_rgba color.
 * \return The t_hsla color.
 */
t_hsla rgba_to_hsla(t_rgba const* color);

/*!
 * \fn          t_hsl rgb_to_hsl(t_rgb color)
 * \brief       Converts a t_rgb color to a t_hsl color.
 * \param color The t_rgb color.
 * \return The t_hsl color.
 */
t_hsl rgb_to_hsl(t_rgb const* color);

/*!
 * \fn          t_rgba hex_to_rgba(char* color)
 * \brief       Converts an hexadecimal color to a t_rgba color.
 * \param color The hexadecimal color.
 * \return The t_rgba color.
 */
t_rgba hex_to_rgba(char const* color);

/*!
 * \fn          t_rgb hex_to_rgb(char* color)
 * \brief       Converts an hexadecimal color to a t_rgb color.
 * \param color The hexadecimal color.
 * \return The t_rgb color.
 */
t_rgb hex_to_rgb(char const* color);

/*!
 * \fn          t_rgba hsla_to_rgba(t_hsla color)
 * \brief       Converts a t_hsla color to a t_rgba color.
 * \param color The t_hsla color.
 * \return The t_rgba color.
 */
t_rgba hsla_to_rgba(t_hsla const* color);

/*!
 * \fn          t_rgb hsl_to_rgb(t_hsl color)
 * \brief       Converts a t_hsl color to a t_rgb color.
 * \param color The t_hsl color.
 * \return The t_rgb color.
 */
t_rgb hsl_to_rgb(t_hsl const* color);

/*!
 * \fn              t_rgba rgba_add_contrast(t_rgba color, float contrast)
 * \brief           Adds some constrast to a t_rgba color.
 * \details         Adds the constrast value to all the component of the color.
 * \param color     The t_rgba color.
 * \param contrast  The contrast value.
 * \return The new t_rgba color.
 */
t_rgba rgba_add_contrast(t_rgba const* color, float contrast);

/*!
 * \fn              t_rgb rgb_add_contrast(t_rgb color, float contrast)
 * \brief           Adds some constrast to a t_rgb color.
 * \details         Adds the constrast value to all the component of the color.
 * \param color     The t_rgb color.
 * \param contrast  The contrast value.
 * \return The new t_rgb color.
 */
t_rgb rgb_add_contrast(t_rgb const* color, float contrast);

/*!
 * \fn          t_rgba rgba_is_equal(t_rgba const& color, t_rgba const& other);
 * \brief       Compares two t_rgba colors.
 * \param color The t_rgba color.
 * \param other The other t_rgba color.
 * \return a non negative value if the colors are equals.
 */
char rgba_is_equal(t_rgba const* color, t_rgba const* other);



/** @} */

#endif
