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

#ifndef DEF_EWIDGET
#define DEF_EWIDGET

#include "../estruct.h"
#include "../ebox/ebox.h"
#include "../epopup/epopup.h"


void ewidget_init(t_eclass* c);
void ewidget_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2);
void ewidget_vis(t_gobj *z, t_glist *glist, int vis);
void ewidget_displace(t_gobj *z, t_glist *glist, int dx, int dy);
void ewidget_select(t_gobj *z, t_glist *glist, int selected);
void ewidget_delete(t_gobj *z, t_glist *glist);
void ewidget_paint(t_ebox *x, t_glist *glist, int mode);
int  ewidget_mousemove(t_gobj *x, struct _glist *glist, int posx, int posy, int shift, int alt, int ctrl, int mousedown);
void ewidget_mousedown(t_ebox *x, t_glist *glist, t_pt pt, long modifiers);
void ewidget_mousedrag(t_ebox *x, t_floatarg posx, t_floatarg posy);
void ewidget_save(t_gobj *z, t_binbuf *b);

void ewidget_paint_default(t_ebox *x, t_object *view);
void ewidget_mousemove_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers);
void ewidget_mousedown_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers);
void ewidget_mousedrag_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers);
void ewidget_mouseup_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers);
void ewidget_getdrawparams_default(t_ebox *x, t_object *patcherview, t_edrawparams *params);
t_pd_err ewidget_notify_default(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void ewidget_save_default(t_gobj *z, t_binbuf *b);
void ewidget_popup_default(t_ebox *x, t_symbol *s, long itemid, t_pt pt);
void ewidget_dsp_default(t_ebox *x, t_object *dsp, short *count, double samplerate, long vectorsize, long flags);
void ewidget_perform_default(t_ebox *x, t_object *dsp, float **ins, long ni, float **outs, long no, long vectorsize, long flags,void *userparam);




#endif 