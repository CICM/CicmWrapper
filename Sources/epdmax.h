/*
 * CicmWrapper
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

#ifndef DEF_EPD_MAX
#define DEF_EPD_MAX

#define object_error       pd_error
#define sys_getdspstate()  canvas_dspstate
#define A_LONG             A_FLOAT
#define A_SYM              A_SYMBOL
#define ASSIST_INLET       1
#define ASSIST_OUTLET      2
#define atom_getsym(atom)  atom_getsymbol(atom)

#define JBOX_DRAWFIRSTIN        0
#define JBOX_NODRAWBOX          0
#define JBOX_DRAWINLAST         0
#define JBOX_TRANSPARENT        0

#define JBOX_NOGROW             EBOX_GROWNO
#define JBOX_GROWY              EBOX_GROWLINK
#define JBOX_GROWBOTH           EBOX_GROWINDI

#define JBOX_IGNORELOCKCLICK    EBOX_IGNORELOCKCLICK
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
#define t_pxobject          t_ebox

#define Z_NO_INPLACE 1
#define Z_PUT_LAST 2
#define Z_PUT_FIRST 4
#define Z_IGNORE_DISABLE 8

#define t_jboxdrawparams    t_edrawparams
#define t_max_err           t_pd_err

#define class_new                                   eclass_new
#define class_addmethod                             eclass_addmethod
 
#define jbox_initclass(class, flags)                eclass_guiinit(class, flags)
#define class_dspinit(class)                        eclassbox_dspinit(class)
#define class_dspinitjbox(class)                    eclass_dspinit(class)
#define class_register(name_space, class)           eclass_register(name_space, class)
#define object_alloc(class)                         eobj_new(class)
#define jbox_new(t_jbox, flags, argc, argv)         ebox_new(t_jbox, flags, argc, argv)
#define dsp_setupjbox(t_jbox, nins, nouts)          ebox_dspsetup(t_jbox, nins, nouts)
#define jbox_ready(t_jbox)                          ebox_ready(t_jbox)
#define jbox_free(t_jbox)                           ebox_free(t_jbox)
#define dsp_freejbox(t_jbox)                        ebox_dspfree(t_jbox)
#define jbox_redraw(t_jbox)                         ebox_redraw(t_jbox)
#define jbox_resize_inputs(t_jbox, nins)            ebox_resize_inputs(t_jbox, nins)
#define jbox_resize_outputs(t_jbox, nouts)          ebox_resize_outputs(t_jbox, nouts)
#define jbox_get_rect_for_view(t_object, view, rect) ebox_get_rect_for_view(t_object, view, rect)
#define jmouse_setcursor(patcherview, t_jbox, mode) ebox_set_cursor(t_jbox, mode)
#define jbox_start_layer(box, view, name, x, y)     ebox_start_layer((t_ebox *)box, name, x, y)
#define jbox_end_layer(box, view, name)             ebox_end_layer((t_ebox *)box, name)
#define jbox_invalidate_layer(box, view, name)      ebox_invalidate_layer((t_ebox *)box, name)
#define jbox_paint_layer(box, view, name, x, y)     ebox_paint_layer((t_ebox *)box, name, x, y)
#define jbox_notify(object, s, msg, sender, data)   ebox_notify(object, s, msg, sender, data)

#define jbox_get_fontname(object)                   ebox_get_fontname(object)
#define jbox_get_font_slant(object)                 ebox_font_slant(object)
#define jbox_get_font_weight(object)                ebox_font_weight(object)
#define jbox_get_fontsize(object)                   ebox_font_size(object)

#define t_dictionary                                t_binbuf
#define dictionary_new                              binbuf_new
#define dictionary_free                             binbuf_free
#define dictionary_write(dict, filename, dir)       binbuf_write(dict, filename, dir, 0)
#define attr_dictionary_process(object, dict)       ebox_attrprocess_viabinbuf(object, dict)
#define object_dictionaryarg(ac, av)                binbuf_via_atoms(ac, av)
#define dictionary_copyatoms(d, key, argc, argv)    binbuf_get_attribute(d, key, argc, argv)

#define jgraphics_set_line_width    egraphics_set_line_width
#define jgraphics_set_source_jrgba  egraphics_set_color_rgba
#define jgraphics_fill              egraphics_fill
#define jgraphics_fill_preserve     egraphics_fill_preserve
#define jgraphics_stroke            egraphics_stroke
#define jgraphics_stroke_preserve   egraphics_stroke_preserve

#define jgraphics_rotate            egraphics_rotate

// POPUP MENU //
#define jpopupmenu_create(jbox, name)               epopupmenu_create(jbox, name)
#define jpopupmenu_setfont(menu, font)              epopupmenu_setfont(menu, font)
#define jpopupmenu_popup(menu, screen, defitemid)   epopupmenu_popup(menu, screen, defitemid)
#define jpopupmenu_destroy(menu)                    (menu = menu) // Useless...
#define jpopupmenu_addseperator(menu)               epopupmenu_addseperator(menu)
#define jpopupmenu_additem(menu, itemid, text, textColor, checked, disabled, icon) epopupmenu_additem(menu, itemid, text, checked, disabled)


#define JGRAPHICS_2PI       CICM_2PI
#define JGRAPHICS_PI        CICM_PI

#define t_jmatrix           t_matrix
#define t_jgraphics         t_elayer
#define t_jrgba             t_rgba
#define t_jtextlayout       t_etext
#define t_jfont             t_efont
#define t_jpopupmenu        t_epopup

#define jgraphics_set_line_width    egraphics_set_line_width
#define jgraphics_fill              egraphics_fill
#define jgraphics_fill_preserve     egraphics_fill_preserve
#define jgraphics_stroke            egraphics_stroke
#define jgraphics_stroke_preserve   egraphics_stroke_preserve

#define jgraphics_rotate            egraphics_rotate
#define jgraphics_set_matrix        egraphics_set_matrix

#define jgraphics_rectangle(graphics, x, y, width, height)      egraphics_rectangle(graphics, x, y, width, height)
#define jgraphics_arc(graphics, xc, yc, radius, angle1, angle2) egraphics_arc(graphics, xc, yc, radius, angle1, angle2)

#define jgraphics_line_to(graphics, x, y)                       egraphics_line_to(graphics, x, y)
#define jgraphics_move_to(graphics, x, y)                       egraphics_move_to(graphics, x, y)

#define jgraphics_close_path(graphics)                          egraphics_close_path(graphics)

#define jgraphics_matrix_init(matrix, xx, yx, xy, yy, x0, y0)   egraphics_matrix_init(matrix, xx, yx, xy, yy, x0, y0)

#define jrgba_addContrast(color, contrast)                      rgba_addContrast(color, contrast)

#define jtextlayout_create()                                    etext_layout_create()
#define jtextlayout_destroy(textlayout)                         etext_layout_destroy(textlayout)
#define jtextlayout_set(textlayout, text, jfont, x, y, width, height, justification, wrap) etext_layout_set(textlayout, text, jfont, x, y, width, height, justification, wrap)
#define jtextlayout_settextcolor(textlayout, color)             etext_layout_settextcolor(textlayout, color)
#define jtextlayout_draw(textlayout, g)                    etext_layout_draw(textlayout, g)
#define jfont_create(family, slant, weight, size)               efont_create(family, slant, weight, size)
#define jfont_destroy(font)                                     efont_destroy(font)

#define cicm_rgba_addContrast(color, contrast)       rgba_addContrast(color, contrast)    


#endif

