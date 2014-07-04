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

#include "ebox.h"

//! Initialize all the ebox ids (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param canvas    The ebox's canvas
 \ @return          Nothing
*/
void ebox_tk_ids(t_ebox *x, t_canvas *canvas)
{
    char buffer[MAXPDSTRING];

    x->b_obj.o_canvas = canvas;

    sprintf(buffer,".x%lx", (long unsigned int) canvas);
    x->b_editor_id = gensym(buffer);

    sprintf(buffer,".x%lx.c", (long unsigned int) canvas);
    x->b_canvas_id = gensym(buffer);

    sprintf(buffer,"%s.ecanvas%lx", x->b_canvas_id->s_name, (long unsigned int)x);
    x->b_drawing_id = gensym(buffer);

    sprintf(buffer,"%s.ewindow%lx", x->b_canvas_id->s_name, (long unsigned int)x);
    x->b_window_id = gensym(buffer);

    sprintf(buffer,"all%lx", (long unsigned int)x);
    x->b_all_id = gensym(buffer);
}

//! Bind an ebox to several events (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          Nothing
*/
void ebox_bind_events(t_ebox* x)
{
    t_eclass* c = (t_eclass *)x->b_obj.o_obj.te_g.g_pd;
	int right = EMOD_CMD;
#ifdef __APPLE__
    
#elif _WINDOWS
	right += 8;
#else
    right += 16;
#endif

    sys_vgui("bind %s <Button-3> {pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, right);
    sys_vgui("bind %s <Button-2> {pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, right);
    sys_vgui("bind %s <Button-1> {pdsend {%s mousedown %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <ButtonRelease> {pdsend {%s mouseup %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Motion> {pdsend {%s mousemove %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    sys_vgui("bind %s <Enter> {pdsend {%s mouseenter}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Leave> {pdsend {%s mouseleave}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    if(c->c_widget.w_dblclick)
       sys_vgui("bind %s <Double-Button-1> {pdsend {%s dblclick   %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    if(c->c_widget.w_mousewheel)
        sys_vgui("bind %s <MouseWheel> {pdsend {%s mousewheel  %%x %%y %%D %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    if(c->c_widget.w_key || c->c_widget.w_keyfilter)
        sys_vgui("bind %s <Key>  {pdsend {%s key  %%k %%N}} \n",  x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    // Canvas & Editor Binding //
    sys_vgui("bind %s <<EditMode>>  {+pdsend {erouter1572 %s editmode}} \n", x->b_editor_id->s_name, x->b_obj.o_id->s_name);
    if(c->c_widget.w_deserted)
    {
        sys_vgui("bind %s <FocusIn>     {+pdsend {erouter1572 %s focus 1}} \n", x->b_editor_id->s_name, x->b_obj.o_id->s_name);
        sys_vgui("bind %s <FocusOut>    {+pdsend {erouter1572 %s focus 0}} \n", x->b_editor_id->s_name, x->b_obj.o_id->s_name);
    }
}

//! Create the canvas widget (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj object
 \ @return          Nothing
*/
void ebox_create_widget(t_ebox* x)
{
    sys_vgui("namespace eval ebox%lx {} \n", x);
    sys_vgui("destroy %s \n", x->b_drawing_id->s_name);

    sys_vgui("canvas %s -width %d -height %d -bd 0 -highlightthickness 0 -insertborderwidth 0 -state normal -takefocus 1 -insertwidth 0 -confine 0\n",
             x->b_drawing_id->s_name,
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness));
}

//! Create the window widget (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param glist     The ebox's glist
 \ @return          Nothing
*/
void ebox_create_window(t_ebox* x, t_glist* glist)
{
    eobj_attach_torouter((t_object *)x);
    ebox_tk_ids(x, glist_getcanvas(glist));
    ebox_create_widget(x);
	ebox_bind_events(x);

    if(!glist->gl_havewindow)
    {
        x->b_isinsubcanvas = 1;
        x->b_rect.x = glist->gl_obj.te_xpix + x->b_obj.o_obj.te_xpix - glist->gl_xmargin;
        x->b_rect.y = glist->gl_obj.te_ypix + x->b_obj.o_obj.te_ypix - glist->gl_ymargin;
    }
    else
    {
        x->b_isinsubcanvas = 0;
        x->b_rect.x = x->b_obj.o_obj.te_xpix;
        x->b_rect.y = x->b_obj.o_obj.te_ypix;
    }

    sys_vgui("%s create window %d %d -anchor nw -window %s -tags %s -width %d -height %d\n",
             x->b_canvas_id->s_name,
             (int)(x->b_rect.x  - x->b_boxparameters.d_borderthickness),
             (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness),
             x->b_drawing_id->s_name,
             x->b_window_id->s_name,
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));

    x->b_modifiers = 0;
#ifdef _WINDOWS
	sys_vgui("focus -force %s\n", x->b_canvas_id->s_name);
#else
    sys_vgui("focus -force %s\n", x->b_canvas_id->s_name);
#endif
    x->b_have_window = 1;
}



