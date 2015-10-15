/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_EPATH
#define DEF_EPATH

#include "ecommon.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!
 * \fn          t_epath* epath_new(void)
 * \brief       Allocates a new path
 * \return      The pointer of the path.
 */
t_epath* epath_new(void);

/*!
 * \fn          void epath_free(t_epath *p);
 * \brief       Frees a path
 * \param p     The pointer of the path.
 */
void epath_free(t_epath *p);

/*!
 * \fn          void epath_clear(t_epath *p);
 * \brief       Clears a path
 * \param p     The pointer of the path.
 */
void epath_clear(t_epath *p);

/*!
 * \fn          void epath_move_to(t_epath *p, const float x, const float y)
 * \brief       Adds a new starting point to a path.
 * \param p     The pointer of the path.
 * \param x     The abscissa of the point.
 * \param y     The ordinate of the point.
 */
void epath_move_to(t_epath *p, const float x, const float y);

/*!
 * \fn          void epath_line_to(t_epath *p, const float x, const float y)
 * \brief       Adds a new point to a path.
 * \param p     The pointer of the path.
 * \param x     The abscissa of the point.
 * \param y     The ordinate of the point.
 */
void epath_line_to(t_epath *p, const float x, const float y);

/*!
 * \fn          void epath_line(t_epath *p, const float x0, const float y0, const float x1, const float y1)
 * \brief       Adds a line to a path.
 * \param p     The pointer of the path.
 * \param x0    The abscissa of the start point.
 * \param y0    The ordinate of the start point.
 * \param x1    The abscissa of the end point.
 * \param y1    The ordinate of the end point.
 */
void epath_line(t_epath *p, const float x0, const float y0, const float x1, const float y1);

/*!
 * \fn          void epath_curve_to(t_epath *p, const float c1x, const float c1y, 
                                                const float c2x, const float c2y, 
                                                const float ex, const float ey)
 * \brief       Adds a bezier curve to a path.
 * \param p     The pointer of the path.
 * \param c1x   The abscissa of the first control point.
 * \param c1y   The ordinate of the first control point.
 * \param c2x   The abscissa of the second control point.
 * \param c2y   The ordinate of the second control point.
 * \param ex    The abscissa of the end point.
 * \param ey    The ordinate of the end point.
 */
void epath_curve_to(t_epath *p, const float c1x, const float c1y,
                                const float c2x, const float c2y,
                                const float ex, const float ey);

/*!
 * \fn          void epath_curve(t_epath *p, const float sx, const float sy,
                                             const float c1x, const float c1y,
                                             const float c2x, const float c2y,
                                             const float ex, const float ey)
 * \brief       Adds a bezier curve to a path.
 * \param p     The pointer of the path.
 * \param sx    The abscissa of the start point.
 * \param sy    The ordinate of the start point.
 * \param c1x   The abscissa of the first control point.
 * \param c1y   The ordinate of the first control point.
 * \param c2x   The abscissa of the second control point.
 * \param c2y   The ordinate of the second control point.
 * \param ex    The abscissa of the end point.
 * \param ey    The ordinate of the end point.
 */
void epath_curve(t_epath *p, const float sx, const float sy,
                 const float c1x, const float c1y,
                 const float c2x, const float c2y,
                 const float ex, const float ey);

/*!
 * \fn          void epath_copy_path(t_epath *p, t_epath const* other)
 * \brief       Copies a path to the current one.
 * \param p     The pointer of the path.
 * \param other The pointer of the path to copy.
 */
void epath_copy_path(t_epath *p, t_epath const* other);

/*!
 * \fn          void epath_swap_path(t_epath *p, t_epath const* other)
 * \brief       Swaps a the content of two paths.
 * \param p     The pointer of the first path.
 * \param other The pointer of the second path.
 */
void epath_swap_path(t_epath *p, t_epath *other);

/*!
 * \fn          void epath_add_path(t_epath *p, t_epath const* other);
 * \brief       Adds a path to the current one.
 * \param p     The pointer of the path.
 * \param other The pointer of the path to add.
 */
void epath_add_path(t_epath *p, t_epath const* other);

/*!
 * \fn              void epath_rectangle_rounded(t_epath *p, const float x, const float y,
                                                             const float width, const float height,
                                                             const float roundness)
 * \brief           Adds a rectangle to a path.
 * \param p         The pointer of the path.
 * \param x         The abscissa of the rectangle.
 * \param y         The ordinate of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param roundness The roundness of the corner.
 */
void epath_rectangle_rounded(t_epath *p, const float x, const float y,
                                         const float width, const float height,
                                         const float roundness);

/*!
 * \fn              void epath_rectangle(t_epath *p, const float x, const float y,
                                                     const float width, const float height)
 * \brief           Adds a rectangle to a path.
 * \param p         The pointer of the path.
 * \param x         The abscissa of the rectangle.
 * \param y         The ordinate of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 */
void epath_rectangle(t_epath *p, const float x, const float y,
                                 const float width, const float height);

/*!
 * \fn              void epath_circle(t_epath *p, float xc, float yc, float radius);
 * \brief           Adds a circle to a path.
 * \param p         The pointer of the path.
 * \param xc        The abscissa of the center of the circle.
 * \param yc        The ordiante of the center of the circle.
 * \param radius    The radius of the circle.
 */
void epath_circle(t_epath *p, const float xc, const float yc, const float radius);

/*!
 * \fn              void epath_oval(t_epath *p, const float xc, const float yc, 
                                                const float radiusx, const float radiusy);
 * \brief           Adds an oval to a path.
 * \param p         The pointer of the path.
 * \param xc        The abscissa of the center of the oval.
 * \param yc        The ordiante of the center of the oval.
 * \param radiusx   The horizontal radius of the circle.
 * \param radiusy   The vertical radius of the circle.
 */
void epath_oval(t_epath *p, const float xc, const float yc, const float radiusx, const float radiusy);

/*!
 * \fn              void epath_arc(t_epath *p, float xc, float yc, float radius,
                                               float angle1, float angle2)
 * \brief           Adds an arc to a path.
 * \param p         The pointer of the path.
 * \param xc        The abscissa of the center of the oval.
 * \param yc        The ordiante of the center of the oval.
 * \param radius    The radius of the circle.
 * \param angle1    The first angle of the oval arc in radian.
 * \param angle2    The second angle of the oval arc in radian.
 */
void epath_arc(t_epath *p, const float xc, const float yc, const float radius,
                           const float angle1, const float angle2);

/*!
 * \fn              void epath_arc_oval(t_epath *p, const float xc, const float yc,
                                                    const float radiusx, const float radiusy,
                                                    const float angle1, const float angle2)
 * \brief           Adds an oval arc to a path.
 * \param p         The pointer of the path.
 * \param xc        The abscissa of the center of the oval.
 * \param yc        The ordiante of the center of the oval.
 * \param radiusx   The horizontal radius of the circle.
 * \param radiusy   The vertical radius of the circle.
 * \param angle1    The first angle of the oval arc in radian.
 * \param angle2    The second angle of the oval arc in radian.
 */
void epath_arc_oval(t_epath *p, const float xc, const float yc,
                                const float radiusx, const float radiusy,
                                const float angle1, const float angle2);

/*!
 * \fn              void epath_apply_matrix(t_epath *p, t_matrix const* matrix)
 * \brief           Applies a matrix transformation on a path.
 * \param p         The pointer of the path.
 * \param matrix    The pointer of the matrix.
 */
void epath_apply_matrix(t_epath *p, t_matrix const* matrix);

/*!
 * \fn          void epath_close(t_epath *p)
 * \brief       Closes a path to the first point.
 * \param p     The pointer of the path.
 */
void epath_close(t_epath *p);


/** @} */

#endif
