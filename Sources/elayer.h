/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#ifndef DEF_ELAYER
#define DEF_ELAYER

#include "ecommon.h"
#include "epath.h"
#include "etextlayout.h"
#include "ecolor.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

/*!< Open state of a layer. */
#define EPD_LAYER_OPEN      0
/*!< Close state of a layer. */
#define EPD_LAYER_CLOSE     1
/*!< Invalid state of a layer. */
#define EPD_LAYER_INVALID   2
 /*!< REady state of a layer. */
#define EPD_LAYER_READY     3

/*!
 * \fn              t_elayer* elayer_new(t_object *owner, t_symbol *name, const float width, const float height)
 * \brief           Allocates and initializes a new layer.
 * \param name      The name of the layer.
 * \param width     The width of the layer.
 * \param height    The height of the layer.
 * \return          This function returns a pointer to a new layer.
 */
t_elayer* elayer_new(t_symbol *name, const float width, const float height);


/*!
 * \fn              void elayer_free(t_elayer *g)
 * \brief           Frees a layer t_elayer.
 * \param g     The layer pointer.
 */
void elayer_free(t_elayer *g);

/*!
 * \fn              t_elayer* elayer_findbyname(t_symbol* name)
 * \brief           Retrieves the layer that matchs to the name.
 * \param name      The name of the layer.
 * \return          The layer that matchs to the name if it exists, othersise NULL.
 */
t_elayer* elayer_findbyname(t_symbol* name);

/*!
* \fn          long elayer_get_state(t_elayer const* g)
* \brief       Gets the state of a layer.
* \param g     The layer pointer.
* \return     The state of the layer.
*/
long elayer_getstate(t_elayer const* g);

/*!
 * \fn          t_symbol* elayer_get_name(t_elayer const* g)
 * \brief       Gets the name of a layer.
 * \param g     The layer pointer.
 * \return      The name of the layer.
 */
t_symbol* elayer_getname(t_elayer const* g);

/*!
 * \fn          void elayer_invalidate(t_elayer* g)
 * \brief       Invalidates a layer.
 * \param g     The layer pointer.
* \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err elayer_invalidate(t_elayer* g);

/*!
 * \fn          t_pd_err elayer_end(t_elayer* g)
 * \brief       Ends a layer.
 * \param g     The layer pointer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err elayer_end(t_elayer* g);

/*!
 * \fn          t_pd_err elayer_paint(t_elayer* g)
 * \brief       Paints a layer.
 * \param g     The layer pointer.
 * \return A null value if nothing goes wrong, otherwise a non-null value.
 */
t_pd_err elayer_paint(t_elayer* g);

/*!
 * \fn          void elayer_set_line_width(t_elayer *g, const float width)
 * \brief       Sets the line width that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param width The line width in pixel.
 */
void elayer_set_line_width(t_elayer *g, const float width);

/*!
 * \fn          void elayer_set_color_rgba(t_elayer *g, const t_rgba *rgba)
 * \brief       Sets the color in rgba that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param rgba  The t_rgba color.
 */
void elayer_set_color_rgba(t_elayer *g, const t_rgba *rgba);

/*!
 * \fn          void elayer_set_color_rgb(t_elayer *g, const t_rgb *rgb)
 * \brief       Sets the color in rgb that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param rgb   The t_rgb color.
 */
void elayer_set_color_rgb(t_elayer *g, const t_rgb *rgb);

/*!
 * \fn          void elayer_set_color_hex(t_elayer *g, const char *hex)
 * \brief       Sets the color in hexadecimal that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hex   The hexadecimal color.
 */
void elayer_set_color_hex(t_elayer *g, const char *hex);

/*!
 * \fn          void elayer_set_color_hsla(t_elayer *g, const t_hsla *hsla)
 * \brief       Sets the color in hsla that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hsla  The hsla color.
 */
void elayer_set_color_hsla(t_elayer *g, const t_hsla *hsla);

/*!
 * \fn          void elayer_set_color_hsl(t_elayer *g, const t_hsl *hsl)
 * \brief       Sets the color in hsl that will be used by the t_elayer.
 * \param g     The t_elayer pointer.
 * \param hsl   The hsla color.
 */
void elayer_set_color_hsl(t_elayer *g, const t_hsl *hsl);

/*!
 * \fn          void elayer_matrix_init(t_matrix *x, const float xx, const float yx, const float xy, const float yy, const float x0, const float y0)
 * \brief       Initializes a matrix.
 * \param x     The t_matrix pointer.
 * \param xx    The xx coefficient.
 * \param yx    The yx coefficient.
 * \param xy    The xy coefficient.
 * \param yy    The yy coefficient.
 * \param x0    The abscissa offset.
 * \param y0    The ordiante offset.
 */
void elayer_matrix_init(t_matrix *x, const float xx, const float yx, const float xy, const float yy, const float x0, const float y0);

/*!
 * \fn          void elayer_set_matrix(t_elayer *g, const t_matrix *matrix)
 * \brief       Initializes the t_matrix of a t_elayer.
 * \param g     The t_elayer pointer.
 * \param matrix The t_matrix pointer.
 */
void elayer_set_matrix(t_elayer *g, const t_matrix *matrix);

/*!
 * \fn          void elayer_rotate(t_elayer *g, const float angle)
 * \brief       Adds a rotation to the t_matrix of a t_elayer.
 * \param g     The t_elayer pointer.
 * \param angle  The offset of rotation in radian.
 */
void elayer_rotate(t_elayer *g, const float angle);

/*!
 * \fn          void elayer_move_to(t_elayer *g, const float x, const float y)
 * \brief       Starts a new path in a t_elayer.
 * \details     All paths should start will the function.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the new path.
 * \param y     The ordiante of the new path.
 */
void elayer_move_to(t_elayer *g, const float x, const float y);

/*!
 * \fn          void elayer_line_to(t_elayer *g, const float x, const float y)
 * \brief       Adds a line in the current path of a t_elayer.
 * \details     The start point is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the end point.
 * \param y     The ordiante of the end point.
 */
void elayer_line_to(t_elayer *g, const float x, const float y);

/*!
 * \fn          void elayer_curve_to(t_elayer *g, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
 * \brief       Adds a bezier curve in the current path of a t_elayer. 
 * \details     The first point of the curve is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param ctrl1x     The abscissa of the first control point.
 * \param ctrl1y     The ordiante of the first control point.
 * \param ctrl2x     The abscissa of the second control point.
 * \param ctrl2y     The ordiante of the second control point.
 * \param endx     The abscissa of the end point.
 * \param endy     The ordiante of the end point.
 */
void elayer_curve_to(t_elayer *g, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy);

/*!
 * \fn          void elayer_arc_to(t_elayer *g, const float cx, const float cy, const float extend)
 * \brief       Adds an arc to the current path of a t_elayer.
 * \details     The start point of the arc is the previous point in the path.
 * \param g     The t_elayer pointer.
 * \param cx     The abscissa of the center of the arc.
 * \param cy     The ordiante of the center of the arc.
 * \param extend The extend angle in radian.
 */
void elayer_arc_to(t_elayer *g, const float cx, const float cy, const float extend);

/*!
 * \fn          void elayer_arc_oval_to(t_elayer *g, const float cx, const float cy, const float radius, const float extend)
 * \brief       Adds an oval arc to the current path of a t_elayer.
 * \details     The start point of the oval arc is the previous point in the path and the first radius of the oval is compute with the center of the oval arc.
 * \param g     The t_elayer pointer.
 * \param cx     The abscissa of the center of the oval arc.
 * \param cy     The ordiante of the center of the oval arc.
 * \param radius The second radius of the oval arc.
 * \param extend The extend angle in radian.
 */
void elayer_arc_oval_to(t_elayer *g, const float cx, const float cy, const float radius, const float extend);

/*!
 * \fn          void elayer_line(t_elayer *g, const float x0, const float y0,  float x1, const float y1)
 * \brief       Adds a new path at specific point and link it linearly to another.
 * \details     This function just call elayer_move_to and elayer_line_to.
 * \param g     The t_elayer pointer.
 * \param x0     The abscissa of the first point.
 * \param y0     The ordiante of the first point.
 * \param x1     The abscissa of the second point.
 * \param y1     The ordiante of the second point.
 */
void elayer_line(t_elayer *g, const float x0, const float y0,  float x1, const float y1);

/*!
 * \fn          void elayer_curve(t_elayer *g, const float startx, const float starty, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy)
 * \brief       Adds a new path at specific point and continue the path with a curve.
 * \details     This function just call elayer_move_to and elayer_curve_to.
 * \param g     The t_elayer pointer.
 * \param startx     The abscissa of the first point.
 * \param starty     The ordiante of the first point.
 * \param ctrl1x     The abscissa of the first control point.
 * \param ctrl1y     The ordiante of the first control point.
 * \param ctrl2x     The abscissa of the second control point.
 * \param ctrl2y     The ordiante of the second control point.
 * \param endx     The abscissa of the end point.
 * \param endy     The ordiante of the end point.
 */
void elayer_curve(t_elayer *g, const float startx, const float starty, const float ctrl1x, const float ctrl1y, const float ctrl2x, const float ctrl2y, const float endx, const float endy);

/*!
 * \fn          void elayer_line_fast(t_elayer *g, const float x0, const float y0, const float x1, const float y1)
 * \brief       Adds a new path at specific point and link it linearly to another then draw directly the line.
 * \details     This function just call elayer_move_to, elayer_line_to and elayer_stroke.
 * \param g     The t_elayer pointer.
 * \param x0     The abscissa of the first point.
 * \param y0     The ordiante of the first point.
 * \param x1     The abscissa of the second point.
 * \param y1     The ordiante of the second point.
 */
void elayer_line_fast(t_elayer *g, const float x0, const float y0, const float x1, const float y1);

/*!
 * \fn          void elayer_close_path(t_elayer *g)
 * \brief       Marks the current path as closed.
 * \param g     The t_elayer pointer.
 */
void elayer_close_path(t_elayer *g);

/*!
 * \fn          void elayer_rectangle(t_elayer *g, const float x, const float y, const float width, const float height)
 * \brief       Adds a rectangle in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the rectangle.
 * \param y     The ordiante of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 */
void elayer_rectangle(t_elayer *g, const float x, const float y, const float width, const float height);

/*!
 * \fn          void elayer_rectangle_rounded(t_elayer *g, const float x, const float y, const float width, const float height, const float roundness)
 * \brief       Adds a rounded rectangle in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param x     The abscissa of the rectangle.
 * \param y     The ordiante of the rectangle.
 * \param width     The width of the rectangle.
 * \param height    The height of the rectangle.
 * \param roundness The roundness of the angles in pixel.
 */
void elayer_rectangle_rounded(t_elayer *g, const float x, const float y, const float width, const float height, const float roundness);

/*!
 * \fn          void elayer_circle(t_elayer *g, const float xc, const float yc, const float radius)
 * \brief       Adds a circle in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the circle.
 * \param yc     The ordiante of the center of the circle.
 * \param radius The radius of the circle.
 */
void elayer_circle(t_elayer *g, const float xc, const float yc, const float radius);

/*!
 * \fn          void elayer_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy)
 * \brief       Adds a oval in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the oval.
 * \param yc     The ordiante of the center of the oval.
 * \param radiusx The horizontal radius of the circle.
 * \param radiusy The vertical radius of the circle.
 */
void elayer_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy);

/*!
 * \fn          void elayer_arc(t_elayer *g, const float xc, const float yc, const float radius, const float angle1, const float angle2)
 * \brief       Adds an arc in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the arc.
 * \param yc     The ordiante of the center of the arc.
 * \param radius The radius of the arc.
 * \param angle1 The first angle of the arc in radian.
 * \param angle2 The second angle of the arc in radian.
 */
void elayer_arc(t_elayer *g, const float xc, const float yc, const float radius, const float angle1, const float angle2);

/*!
 * \fn          void elayer_arc_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy, const float angle1, const float angle2)
 * \brief       Adds an oval arc in the t_elayer.
 * \param g     The t_elayer pointer.
 * \param xc     The abscissa of the center of the oval arc.
 * \param yc     The ordiante of the center of the oval arc.
 * \param radiusx The horizontal radius of the oval arc.
 * \param radiusy The vertical radius of the oval arc.
 * \param angle1 The first angle of the oval arc in radian.
 * \param angle2 The second angle of the oval arc in radian.
 */
void elayer_arc_oval(t_elayer *g, const float xc, const float yc, const float radiusx, const float radiusy, const float angle1, const float angle2);


/*!
 * \fn          void efont_init(t_efont* font, t_symbol* family, char bold, char italic, const float size)
 * \brief       Initializes a t_efont.
 * \param font The t_efont pointer.
 * \param family The font family.
 * \param bold  If the font is bold.
 * \param italic  If the font is italic.
 * \param size The font size.
 */
void efont_init(t_efont* font, t_symbol* family, char bold, char italic, const float size);


/*!
 * \fn          void elayer_fill(t_elayer *g)
 * \brief       Fills the t_elayer with the current path.
 * \param g     The t_elayer pointer.
 */
void elayer_fill(t_elayer *g);

/*!
 * \fn          void elayer_fill_preserve(t_elayer *g)
 * \brief       Fills the t_elayer with the current path and preserves the current path.
 * \param g     The t_elayer pointer.
 */
void elayer_fill_preserve(t_elayer *g);

/*!
 * \fn          void elayer_stroke(t_elayer *g)
 * \brief       Strokes the t_elayer with the current path.
 * \param g     The t_elayer pointer.
 */
void elayer_stroke(t_elayer *g);

/*!
 * \fn          void elayer_stroke_preserve(t_elayer *g)
 * \brief       Strokes the t_elayer with the current path and preserves the current path.
 * \param g     The t_elayer pointer.
 */
void elayer_stroke_preserve(t_elayer *g);

/*!
 * \fn          void etextlayout_draw(t_etextlayout* textlayout, t_elayer *g)
 * \brief       Draws the t_etextlayout in a t_elayer.
 * \param textlayout The t_etextlayout pointer.
 * \param g     The t_elayer pointer.
 */
void etextlayout_draw(t_etextlayout* textlayout, t_elayer *g);

/** @} */

#endif
