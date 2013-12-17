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
    
    x->e_canvas = canvas;
    
    sprintf(buffer,".x%lx", (long unsigned int) canvas);
    x->e_editor_id = gensym(buffer);
    
    sprintf(buffer,".x%lx.c", (long unsigned int) canvas);
    x->e_canvas_id = gensym(buffer);
    
    sprintf(buffer,"%s.ecanvas%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_drawing_id = gensym(buffer);    
    
    sprintf(buffer,"%s.ewindow%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_window_id = gensym(buffer);
    
    sprintf(buffer,"all%lx", (long unsigned int)x);
    x->e_all_id = gensym(buffer);
}

//! Bind an ebox to several events (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          Nothing
*/
void ebox_bind_events(t_ebox* x)
{
    // TODO : FAIRE LES BINDS EN FONCTION DES METHODS !!! (eviter les appels inutiles)
	int right = EMOD_CMD;
#ifdef _WINDOWS
	right += 8;
#endif
    sys_vgui("bind %s <Enter> {+pdsend {%s mouseenter}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <Leave> {+pdsend {%s mouseleave}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
   
    sys_vgui("bind %s <Double-Button-1> {+pdsend {%s dblclick   %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
	sys_vgui("bind %s <Button-3> {+pdsend {%s mousedown %%x %%y %i}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name, right);
    sys_vgui("bind %s <Button-2> {+pdsend {%s mousedown %%x %%y %i}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name, right);
    sys_vgui("bind %s <Button-1>        {+pdsend {%s mousedown   %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <ButtonRelease>   {+pdsend {%s mouseup     %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <Motion>          {+pdsend {%s mousemove   %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <MouseWheel> {+pdsend {%s mousewheel  %%x %%y %%D %%s}}\n", x->e_drawing_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <Key>  {pdsend {%s key  %%k %%N}} \n",  x->e_drawing_id->s_name, x->e_object_id->s_name);
    
    // Canvas & Editor Binding //
    
    sys_vgui("bind %s <<EditMode>>  {+pdsend {#erouter %s editmode}} \n", x->e_editor_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <FocusIn>     {+pdsend {#erouter %s focus 1}} \n", x->e_editor_id->s_name, x->e_object_id->s_name);
    sys_vgui("bind %s <FocusOut>    {+pdsend {#erouter %s focus 0}} \n", x->e_editor_id->s_name, x->e_object_id->s_name);
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
    sys_vgui("destroy %s \n", x->e_drawing_id->s_name);
    
    sys_vgui("canvas %s -width %d -height %d -bd 0 -highlightthickness 0 -insertborderwidth 0 -state normal -takefocus 1 -insertwidth 0 -confine 0\n",
             x->e_drawing_id->s_name,
             (int)(x->e_rect.width + x->e_boxparameters.d_borderthickness),
             (int)(x->e_rect.height + x->e_boxparameters.d_borderthickness));
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
    erouter_setup(glist_getcanvas(glist)->gl_list->g_pd);
    erouter_attach((t_object *)x);
    
    ebox_tk_ids(x, glist_getcanvas(glist));
    ebox_create_widget(x);
	ebox_bind_events(x);
    
    if(!glist->gl_havewindow)
    {
        x->e_rect.x = glist->gl_obj.te_xpix + x->e_obj.te_xpix - glist->gl_xmargin;
        x->e_rect.y = glist->gl_obj.te_ypix + x->e_obj.te_ypix - glist->gl_ymargin;
    }
    else
    {
        x->e_rect.x = x->e_obj.te_xpix;
        x->e_rect.y = x->e_obj.te_ypix;
    }
    
    sys_vgui("%s create window %d %d -anchor nw -window %s -tags %s -width %d -height %d\n",
             x->e_canvas_id->s_name,
             (int)(x->e_rect.x  - x->e_boxparameters.d_borderthickness),
             (int)(x->e_rect.y - x->e_boxparameters.d_borderthickness),
             x->e_drawing_id->s_name,
             x->e_window_id->s_name,
             (int)(x->e_rect.width + x->e_boxparameters.d_borderthickness * 2.),
             (int)(x->e_rect.height + x->e_boxparameters.d_borderthickness * 2.));

    x->e_modifiers = 0;
#ifdef _WINDOWS
	
#else
    sys_vgui("focus -force %s\n", x->e_canvas_id->s_name);
#endif

}



