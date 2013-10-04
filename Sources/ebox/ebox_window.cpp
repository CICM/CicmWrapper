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

void ebox_tk_ids(t_ebox *x, t_canvas *canvas)
{
    char buffer[MAXPDSTRING];
    x->e_canvas = canvas;
    
    sprintf(buffer,".x%lx.c", (long unsigned int) canvas);
    x->e_canvas_id = gensym(buffer);
    
    sprintf(buffer,"%s.frame%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_frame_id = gensym(buffer);
    
    sprintf(buffer,"%s.window%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_window_id = gensym(buffer);
    
    sprintf(buffer,"%s.handle%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_handle_id = gensym(buffer);
    
    sprintf(buffer,"all%lx", (long unsigned int)x);
    x->e_all_id = gensym(buffer);
}

void ebox_bind_events(t_ebox* x)
{
    sys_vgui("bind %s <Button> {pdtk_canvas_mouse %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b 0}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("bind %s <ButtonRelease> {pdtk_canvas_mouseup %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("bind %s <Shift-Button> {pdtk_canvas_mouse %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b 1}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("bind %s <Button-2> {pdtk_canvas_rightclick %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("bind %s <Button-3> {pdtk_canvas_rightclick %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    sys_vgui("bind %s <$::modifier-Button> {pdtk_canvas_rightclick %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] %%b}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);

    sys_vgui("bind %s <Motion> {pdtk_canvas_motion %s \
             [expr %%X - [winfo rootx %s]] [expr %%Y - [winfo rooty %s]] 0}\n",
             x->e_frame_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name, x->e_canvas_id->s_name);
    
    sys_vgui("bind %s <KeyRelease> {+pdsend {%s keyup %%N}} \n",
            x->e_frame_id->s_name, x->e_name_rcv->s_name);
}

void ebox_create_widget(t_ebox* x)
{
    sys_vgui("namespace eval ebox%lx {} \n", x);
    
    sys_vgui("destroy %s \n", x->e_frame_id);
    //sys_vgui("frame %s \n", x->e_frame_id);
    sys_vgui("canvas %s -width %d -height %d -bg #ddd -bd 0 \
             -highlightthickness 3 -highlightcolor {#f00}\n",
             x->e_frame_id, x->e_rect.width, x->e_rect.height);

   // sys_vgui("pack %s -side bottom -fill both -expand 1 \n", x->e_frame_id);
    
    ebox_bind_events(x);
}

void ebox_create_window(t_ebox* x, t_glist* glist)
{

    ebox_create_widget(x);
    sys_vgui("%s create window %d %d -anchor nw -window %s -tags {%s %s} -width %d -height %d \n",
             x->e_canvas_id->s_name,
             (int)x->e_rect.x,
             (int)x->e_rect.y,
             x->e_frame_id->s_name,
             x->e_window_id->s_name,
             x->e_all_id->s_name,
             (int)x->e_rect.width,
             (int)x->e_rect.height);
}



