/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
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

#ifndef DEF_EPD_MAX
#define DEF_EPD_MAX

#define JBOX_DRAWFIRSTIN        0
#define JBOX_NODRAWBOX          0
#define JBOX_DRAWINLAST         0
#define JBOX_TRANSPARENT        0
#define JBOX_NOGROW             0
#define JBOX_GROWY              0
#define JBOX_GROWBOTH           0
#define JBOX_IGNORELOCKCLICK    0
#define JBOX_HILITE             0
#define JBOX_BACKGROUND         0
#define JBOX_NOFLOATINSPECTOR   0
#define JBOX_TEXTFIELD          0
#define JBOX_FIXWIDTH           0
#define JBOe_fontATTR           0
#define JBOX_TEXTJUSTIFICATIONATTR  0
#define JBOX_BINBUF             0
#define JBOe_mouseDRAGDELTA     0
#define JBOX_COLOR              0
#define JBOX_DRAWIOLOCKED       0
#define JBOX_DRAWBACKGROUND     0
#define JBOX_NOINSPECTFIRSTIN   0

#define t_jbox              t_ebox
#define t_jboxdrawparams    t_edrawparams
#define t_max_err           t_pd_err

#define class_new                                   eclass_new
#define class_addmethod                             eclass_addmethod
 
#define jbox_initclass(class, flags)                eclass_init(class, flags)
#define class_dspinitjbox(class)                    eclass_dspinit(class)

#define object_alloc(class)                         ebox_alloc(class)
#define jbox_new(t_jbox, flags, argc, argv)         ebox_new(t_jbox, flags, argc, argv)
#define dsp_setupjbox(t_jbox, nins, nouts)          ebox_dspsetup(t_jbox, nins, nouts)
#define jbox_ready(t_jbox)                          ebox_ready(t_jbox)
#define jbox_free(t_jbox)                           ebox_free(t_jbox)
#define dsp_freejbox(t_jbox)                        ebox_dspfree(t_jbox)
#define jbox_redraw(t_jbox)                         ebox_redraw(t_jbox)
#define jbox_resize_inputs(t_jbox, nins)            ebox_resize_inputs(t_jbox, nins)
#define jbox_get_rect_for_view(t_object, view, rect) ebox_get_rect_for_view(t_object, view, rect)

#define jbox_start_layer                            ebox_start_layer
#define jbox_end_layer                              ebox_end_layer
#define jbox_invalidate_layer                       ebox_invalidate_layer
#define jbox_paint_layer                            ebox_paint_layer
#define jbox_notify(object, s, msg, sender, data)   ebox_notify(object, s, msg, sender, data)

#define jbox_get_fontname(object)                   cicm_obj_get_fontname(object)
#define jbox_get_font_slant(object)                 cicm_obj_font_slant(object)
#define jbox_get_font_weight(object)                cicm_obj_font_weight(object)
#define jbox_get_fontsize(object)                   cicm_obj_fontsize(object)

#define attr_dictionary_process(object, dict) attr_cicm_dictionary_process(object, dict)
#define object_dictionaryarg(ac, av) object_cicm_dictionaryarg(ac, av)

#define jgraphics_set_line_width    cicm_graphics_set_line_width
#define jgraphics_set_source_jrgba  cicm_graphics_set_source_jrgba
#define jgraphics_fill              cicm_graphics_fill
#define jgraphics_fill_preserve     cicm_graphics_fill_preserve
#define jgraphics_stroke            cicm_graphics_stroke
#define jgraphics_stroke_preserve   cicm_graphics_stroke_preserve

#define jgraphics_rotate            cicm_graphics_rotate

#define jpopupmenu_create               epopupmenu_create
#define jpopupmenu_setfont              epopupmenu_setfont
#define jpopupmenu_additem(menu, itemid, text, textColor, checked, disabled, icon) epopupmenu_additem(menu, itemid, text, textColor, checked, disabled, icon)
#define jpopupmenu_addseperator(menu)   epopupmenu_addseperator(menu)
#define jpopupmenu_popup(menu, screen, defitemid) epopupmenu_popup(menu, screen, defitemid)


#define JGRAPHICS_2PI       CICM_2PI
#define JGRAPHICS_PI        CICM_PI

#define t_jmatrix           t_cicm_matrix
#define t_jgraphics         t_egraphics
#define t_jrgba             t_ergba
#define t_jtextlayout       t_etextlayout
#define t_jfont             t_efont
#define t_jpopupmenu        t_epopupmenu

#define jgraphics_set_line_width    cicm_graphics_set_line_width
#define jgraphics_set_source_jrgba  cicm_graphics_set_source_jrgba
#define jgraphics_fill              cicm_graphics_fill
#define jgraphics_fill_preserve     cicm_graphics_fill_preserve
#define jgraphics_stroke            cicm_graphics_stroke
#define jgraphics_stroke_preserve   cicm_graphics_stroke_preserve

#define jgraphics_rotate            cicm_graphics_rotate
#define jgraphics_set_matrix        cicm_graphics_set_matrix

#define jgraphics_rectangle(graphics, x, y, width, height)      cicm_graphics_rectangle(graphics, x, y, width, height)
#define jgraphics_arc(graphics, xc, yc, radius, angle1, angle2) cicm_graphics_arc(graphics, xc, yc, radius, angle1, angle2)
#define jgraphics_arc_negative(graphics, xc, yc, radius, angle1, angle2) cicm_graphics_arc_negative(graphics, xc, yc, radius, angle1, angle2)
#define jgraphics_line_to(graphics, x, y)                       cicm_graphics_line_to(graphics, x, y)
#define jgraphics_move_to(graphics, x, y)                       cicm_graphics_move_to(graphics, x, y)

#define jgraphics_close_path(graphics)                          cicm_graphics_close_path(graphics)
#define jgraphics_smooth_line(graphics)                         cicm_graphics_smooth_line(graphics)

#define jgraphics_matrix_init(matrix, xx, yx, xy, yy, x0, y0)   cicm_graphics_matrix_init(matrix, xx, yx, xy, yy, x0, y0)

#define jrgba_addContrast(color, contrast)  cicm_rgba_addContrast(color, contrast)

#define jtextlayout_create()                                    cicm_text_layout_create()
#define jtextlayout_destroy(textlayout)                         cicm_text_layout_destroy(textlayout)
#define jtextlayout_set(textlayout, text, jfont, x, y, width, height, justification, wrap) cicm_text_layout_set(textlayout, text, jfont, x, y, width, height, justification, wrap)
#define jtextlayout_settextcolor(textlayout, color)             cicm_text_layout_settextcolor(textlayout, color)
#define jtextlayout_draw(textlayout, g)                    cicm_text_layout_draw(textlayout, g)
#define jfont_create(family, slant, weight, size)               cicm_font_create(family, slant, weight, size)
#define jfont_destroy(font)                                     cicm_font_destroy(font)



#endif