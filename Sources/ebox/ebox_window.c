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
    
    sprintf(buffer,"ebox%s", x->e_canvas_id->s_name);
    pd_bind(&x->e_obj.ob_pd, gensym(buffer));
    
    sprintf(buffer,".x%lx", (long unsigned int) canvas);
    x->e_frame_id = gensym(buffer);
    
    sprintf(buffer,"%s.ecanvas%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_drawing_id = gensym(buffer);    
    
    sprintf(buffer,"%s.ewindow%lx", x->e_canvas_id->s_name, (long unsigned int)x);
    x->e_window_id = gensym(buffer);
    
    sprintf(buffer,"all%lx", (long unsigned int)x);
    x->e_all_id = gensym(buffer);
}

void ebox_bind_events(t_ebox* x)
{
	int right = EMOD_CMD;
#ifdef _WINDOWS
	right += 8;
#endif
    sys_vgui("bind %s <Enter> {pdsend {%s mouseenter}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
    sys_vgui("bind %s <Leave> {pdsend {%s mouseleave}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
   
	sys_vgui("bind %s <Button-3> {pdsend {%s mousedown %%x %%y %i}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name, right);
    sys_vgui("bind %s <Button-2> {pdsend {%s mousedown %%x %%y %i}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name, right);
    sys_vgui("bind %s <Button-1>        {pdsend {%s mousedown   %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
    sys_vgui("bind %s <ButtonRelease>   {pdsend {%s mouseup     %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
    sys_vgui("bind %s <Motion>          {pdsend {%s mousemove   %%x %%y %%s}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
    sys_vgui("bind %s <MouseWheel> {pdsend {%s mousewheel  %%x %%y %%D %%s}}\n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
  
    sys_vgui("bind %s <KeyPress>    {pdsend {%s keydown    %%k %%N}} \n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
    sys_vgui("bind %s <KeyRelease>  {pdsend {%s keyup      %%k %%N}} \n", x->e_drawing_id->s_name, x->e_name_rcv->s_name);
}

void ebox_create_widget(t_ebox* x)
{
    sys_vgui("namespace eval ebox%lx {} \n", x);
    sys_vgui("destroy %s \n", x->e_drawing_id->s_name);
    
    sys_vgui("canvas %s -width %d -height %d -bd 0 -highlightthickness 0 -insertborderwidth 0\n",
             x->e_drawing_id->s_name,
             (int)(x->e_rect.width + x->e_boxparameters.d_borderthickness),
             (int)(x->e_rect.height + x->e_boxparameters.d_borderthickness));
}

void ebox_create_window(t_ebox* x, t_glist* glist)
{
    ebox_tk_ids(x, glist);
    ebox_create_widget(x);
	ebox_bind_events(x);
    
    sys_vgui("%s create window %d %d -anchor nw -window %s -tags %s -width %d -height %d\n",
             x->e_canvas_id->s_name,
             (int)(x->e_obj.te_xpix - x->e_boxparameters.d_borderthickness),
             (int)(x->e_obj.te_ypix - x->e_boxparameters.d_borderthickness),
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



