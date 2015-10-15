/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_ETEXTLAYOUT
#define DEF_ETEXTLAYOUT

#include "ecommon.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!
 * \fn          t_epath * epath_new(void)
 * \brief       Allocates a new path
 * \return      The pointer of the path.
 */

/*!
 * \fn          t_etextlayout* etextlayout_new(void)
 * \brief       Allocates a new t_etextlayout layout.
 * \return      The new t_etextlayout pointer.
 */
t_etextlayout* etextlayout_new(void);

/*!
 * \fn          void etextlayout_destroy(t_etextlayout* textlayout)
 * \brief       Frees a t_etextlayout layout.
 * \param textlayout The t_etextlayout pointer.
 */
void etextlayout_destroy(t_etextlayout* textlayout);

/*!
 * \fn          void etextlayout_set(t_etextlayout* textlayout, const char* text, t_efont *jfont,  float x, float y, float width,  float height, etextanchor_flags anchor, etextjustify_flags justify, etextwrap_flags wrap)
 * \brief       Initializes a t_etextlayout.
 * \param textlayout The t_etextlayout pointer.
 * \param text The text to draw.
 * \param font The t_efont to use.
 * \param x The absicssa position of the t_etextlayout.
 * \param y The ordinate position of the t_etextlayout.
 * \param width The width of the t_etextlayout.
 * \param height The height of the t_etextlayout.
 * \param justify The justification of the text.
 * \param wrap if the text should be wrap or not.
 */
void etextlayout_set(t_etextlayout* textlayout, const char* text, t_efont *font,  float x, float y, float width,  float height,etextjustify_flags justify, etextwrap_flags wrap);

/*!
 * \fn                  void etextlayout_settextcolor(t_etextlayout* textlayout, t_rgba* color)
 * \brief               Sets the color of a t_etextlayout layout.
 * \param textlayout    The t_etextlayout pointer.
 * \param color         The color pointer.
 */
void etextlayout_settextcolor(t_etextlayout* textlayout, t_rgba const* color);



/** @} */

#endif
