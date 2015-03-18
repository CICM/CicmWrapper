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

//! @cond

//! The mouse enter method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_mouse_enter(t_ebox* x)
{
    t_eclass *c = eobj_getclass(x);

    if(!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        sys_vgui("focus %s\n", x->b_drawing_id->s_name);
        if(c->c_widget.w_mouseenter)
            c->c_widget.w_mouseenter(x);
    }
    else if(x->b_obj.o_canvas->gl_edit)   
        eobj_poll_mouse(x);
}


//! The mouse leave method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = eobj_getclass(x);

    if(!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        sys_vgui("focus %s\n", x->b_canvas_id->s_name);
        if(c->c_widget.w_mouseleave)
            c->c_widget.w_mouseleave(x);

        ebox_set_cursor(x, 0);
        eobj_nopoll_mouse(x);
    }
    else if(x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        ebox_set_cursor(x, 4);
        eobj_nopoll_mouse(x);
    }
}

long modifier_wrapper(long mod)
{
#ifdef __APPLE__

#elif _WINDOWS

	if(mod >= 131080)
	{
		mod -= 131080;
		mod += EMOD_ALT;
	}
	else
		mod -= 8;
#else
    mod -= 16;
#endif
    //post("MOD : %ld", mod);
    return mod;
}

//! The mouse move method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_move(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_atom av[2];
    t_eclass *c = eobj_getclass(x);
    eobj_get_mouse_global_position(x);

    x->b_modifiers = modifier_wrapper((long)atom_getfloat(argv+2));


    if(!x->b_obj.o_canvas->gl_edit)
    {
        if(!x->b_mouse_down)
        {
            if(x->b_flags & EBOX_IGNORELOCKCLICK)
                ebox_set_cursor(x, 0);
            else
                ebox_set_cursor(x, 1);

            x->b_mouse.x = atom_getfloat(argv);
            x->b_mouse.y = atom_getfloat(argv+1);

            if(c->c_widget.w_mousemove)
                c->c_widget.w_mousemove(x, x->b_obj.o_canvas, x->b_mouse, x->b_modifiers);
        }
        else
        {
            ebox_mouse_drag(x, s, argc, argv);
        }
    }
    else if(!x->b_isinsubcanvas)
    {
        if(!x->b_mouse_down)
        {
            ebox_mouse_move_editmode(x, atom_getfloat(argv), atom_getfloat(argv+1), x->b_modifiers);
        }
        else
        {
            x->b_mouse.x = atom_getfloat(argv);
            x->b_mouse.y = atom_getfloat(argv+1);
            if(x->b_selected_item == EITEM_NONE)
            {
                x->b_move_box = eobj_get_mouse_canvas_position(x);
                sys_vgui("pdtk_canvas_motion %s %i %i 0\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);
            }
            else
            {
                if(x->b_flags & EBOX_GROWNO)
                    return;
                else if(x->b_flags & EBOX_GROWLINK)
                {
                    if(x->b_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->b_rect_last.width + (x->b_mouse.y - x->b_rect_last.height));
                        atom_setfloat(av+1, x->b_mouse.y);
                    }
                    else if(x->b_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, x->b_mouse.x);
                        atom_setfloat(av+1, x->b_rect_last.height + (x->b_mouse.x - x->b_rect_last.width));
                    }
                    else if(x->b_selected_item == EITEM_CORNER)
                    {
                        if(x->b_mouse.y > x->b_mouse.x)
                        {
                            atom_setfloat(av, x->b_mouse.y);
                            atom_setfloat(av+1, x->b_mouse.y);
                        }
                        else
                        {
                            atom_setfloat(av, x->b_mouse.x);
                            atom_setfloat(av+1, x->b_mouse.x);
                        }
                    }
                }
                else if (x->b_flags & EBOX_GROWINDI)
                {
                    if(x->b_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->b_rect_last.width);
                        atom_setfloat(av+1, x->b_mouse.y);
                    }
                    else if(x->b_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, x->b_mouse.x);
                        atom_setfloat(av+1, x->b_rect_last.height);
                    }
                    else if(x->b_selected_item == EITEM_CORNER)
                    {
                        atom_setfloat(av, x->b_mouse.x);
                        atom_setfloat(av+1, x->b_mouse.y);
                    }
                }
                mess3((t_pd *)x, gensym("size"),  gensym("size"), (void *)2, (void *)av);
            }
        }
    }
    else
    {
        x->b_move_box = eobj_get_mouse_canvas_position(x);
        if(!x->b_mouse_down)
        {
            sys_vgui("pdtk_canvas_motion %s %i %i 0\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);
        }
        else
        {
            sys_vgui("pdtk_canvas_motion %s %i %i 1\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);
        }
    }
}

//! The mouse drag method called via mouse move method when mouse down (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_drag(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = eobj_getclass(x);
    x->b_modifiers -= EPD_MAX_SIGS;

    if(!x->b_obj.o_canvas->gl_edit)
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousedrag && x->b_mouse_down)
            c->c_widget.w_mousedrag(x, x->b_obj.o_canvas, x->b_mouse, x->b_modifiers);
    }
}

//! The mouse down method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_down(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = eobj_getclass(x);

    x->b_modifiers = modifier_wrapper((long)atom_getfloat(argv+2));

    if(!x->b_obj.o_canvas->gl_edit)
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousedown)
            c->c_widget.w_mousedown(x, x->b_obj.o_canvas, x->b_mouse, x->b_modifiers);
    }
    else
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(x->b_selected_item == EITEM_NONE)
        {
            x->b_move_box = eobj_get_mouse_canvas_position(x);
            if(x->b_modifiers == EMOD_CMD)
                sys_vgui("pdtk_canvas_rightclick %s %i %i 0\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);
            else
                sys_vgui("pdtk_canvas_mouse %s %i %i 0 0\n", x->b_canvas_id->s_name, (int)(x->b_move_box.x), (int)(x->b_move_box.y));
        }
        else
        {
            x->b_rect_last = x->b_rect;
        }
    }
    x->b_mouse_down = 1;
}

//! The mouse up method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_up(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = eobj_getclass(x);

    x->b_modifiers = modifier_wrapper((long)atom_getfloat(argv+2));

    if(!x->b_obj.o_canvas->gl_edit)
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mouseup)
            c->c_widget.w_mouseup(x, x->b_obj.o_canvas, x->b_mouse, x->b_modifiers);
    }
    else
    {
        x->b_move_box = eobj_get_mouse_canvas_position(x);
        sys_vgui("pdtk_canvas_mouseup %s %i %i 0\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);
        if(x->b_selected_inlet != -1 || x->b_selected_outlet != -1)
        {
            x->b_selected_inlet = -1;
            x->b_selected_outlet = -1;
            ebox_redraw(x);
        }
    }

    x->b_mouse_down = 0;
}

//! The mouse double click method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = eobj_getclass(x);

    x->b_modifiers = modifier_wrapper((long)atom_getfloat(argv+2));

    if(!x->b_obj.o_canvas->gl_edit)
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_dblclick)
            c->c_widget.w_dblclick(x, x->b_obj.o_canvas, x->b_mouse);
    }
}

//! The mouse wheel method called by tcl/tk (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_wheel(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    float delta;
    t_eclass *c = eobj_getclass(x);

    x->b_modifiers = modifier_wrapper((long)atom_getfloat(argv+3));
    delta = atom_getfloat(argv+2);

    if(!x->b_obj.o_canvas->gl_edit)
    {
        x->b_mouse.x = atom_getfloat(argv);
        x->b_mouse.y = atom_getfloat(argv+1);
        if(c->c_widget.w_mousewheel)
            c->c_widget.w_mousewheel(x, x->b_obj.o_canvas, x->b_mouse, (long)x->b_modifiers, delta, delta);
    }
}

//! The mouse move editmode method called via mouse move method when mouse down (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_mouse_move_editmode(t_ebox* x, float x_p, float y_p, float key)
{
    int i;
    int right, bottom;

    x->b_selected_outlet    = -1;
    x->b_selected_inlet     = -1;
    x->b_selected_item      = EITEM_NONE;
    x->b_move_box = eobj_get_mouse_canvas_position(x);
    sys_vgui("pdtk_canvas_motion %s %i %i 0\n", x->b_canvas_id->s_name, (int)x->b_move_box.x, (int)x->b_move_box.y);

    right   = x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.;
    bottom  = x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.;

    // TOP //
    if(y_p >= 0 && y_p < 3)
    {
        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->b_rect.width - 8));

            if(x_p >= pos_x_inlet && x_p <= pos_x_inlet +7)
            {
                x->b_selected_inlet = i;
                ebox_set_cursor(x, 4);
                break;
            }
        }
        ebox_invalidate_layer(x, gensym("eboxio"));
        ebox_redraw(x);
        return;
    }
    // BOTTOM & RIGHT //
    else if(y_p > bottom - 3 && y_p <= bottom && x_p > right - 3 && x_p <= right)
    {
        x->b_selected_item = EITEM_CORNER;
        ebox_set_cursor(x, 8);
        return;
    }
    // BOTTOM //
    else if(y_p > bottom - 3 && y_p < bottom)
    {
        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->b_rect.width - 8));

            if(x_p >= pos_x_outlet && x_p <= pos_x_outlet +7)
            {
                x->b_selected_outlet = i;
                ebox_set_cursor(x, 5);
                break;
            }
        }
        if(x->b_selected_outlet == -1)
        {
            x->b_selected_item = EITEM_BOTTOM;
            ebox_set_cursor(x, 7);
        }
        ebox_invalidate_layer(x, gensym("eboxio"));
        ebox_redraw(x);
        return;
    }
    // RIGHT //
    else if(x_p > right - 3 && x_p <= right)
    {
        x->b_selected_item = EITEM_RIGHT;
        ebox_set_cursor(x, 9);
        return;
    }

    // BOX //
    ebox_set_cursor(x, 4);

    ebox_invalidate_layer(x, gensym("eboxio"));
    ebox_redraw(x);
}

//! The key down method called by tcl/tk (PRIVATE AND NOT READY)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
 */
void ebox_key(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    t_eclass *c = eobj_getclass(x);

    if(argc >= 2 && argv && atom_gettype(argv+1) == A_FLOAT)
    {
        if(!x->b_obj.o_canvas->gl_edit)
        {
            if(atom_getfloat(argv+1) == 65288)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_DEL, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_DEL, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65289)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_TAB, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_TAB, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65293)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ENTER, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_ENTER, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65307)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ESC, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_ESC, 0);
                }
            }
            else
            {
                if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, (char)atom_getfloat(argv+1), 0);
                }
            }
        }
        else
        {
            ;
        }
    }
}

//! The key up method called by tcl/tk (PRIVATE AND NOT READY)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param s         The message selector
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Nothing
*/
void ebox_deserted(t_ebox *x)
{
    t_eclass *c = eobj_getclass(x);
    if(c->c_widget.w_deserted && x->b_ready_to_draw)
        c->c_widget.w_deserted(x);
}

//! The default save method for UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param b         The binbuf
 \ @return          Nothing
*/
void ebox_dosave(t_ebox* x, t_binbuf *b)
{
    int         i;
    char        attr_name[MAXPDSTRING];
    long        argc    = 0;
    t_atom*     argv    = NULL;
    t_eclass *c = eobj_getclass(x);

    binbuf_addv(b, "ssiis", gensym("#X"), gensym("obj"), (int)x->b_obj.o_obj.te_xpix, (int)x->b_obj.o_obj.te_ypix, eobj_getclassname(x));

    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->save)
        {
            sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
            object_attr_getvalueof((t_object *)x, c->c_attr[i]->name, &argc, &argv);
            if(argc && argv)
            {
                binbuf_append_attribute(b, gensym(attr_name), argc, argv);
                argc = 0;
                free(argv);
                argv = NULL;
            }
        }
    }

    if(c->c_widget.w_save != NULL)
        c->c_widget.w_save(x, b);

    binbuf_addv(b, ";");
}

//! The method to move an UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param newx      The new abscissa
 \ @param newy      The new ordinate
 \ @return          Nothing
 */
void ebox_pos(t_ebox* x, float newx, float newy)
{
    x->b_rect.x = newx;
    x->b_rect.y = newy;
    x->b_obj.o_obj.te_xpix = newx;
    x->b_obj.o_obj.te_ypix = newy;
    
    ebox_move(x);
}

//! The method to show or hide an UI ebox (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param vis       The visible state
 \ @return          Nothing
 */
void ebox_vis(t_ebox* x, int vis)
{
    vis = pd_clip_minmax(vis, 0, 1);
    if(x->b_visible != vis)
    {
        x->b_visible = vis;
        if(x->b_visible && x->b_ready_to_draw && x->b_obj.o_canvas)
        {
            canvas_redraw(x->b_obj.o_canvas);
        }
        else
            ebox_erase(x);
    }
}

//! The default user id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
*/
t_pd_err ebox_set_receiveid(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    t_symbol* sname;
    if(argc && argv && atom_gettype(argv) == A_SYM && atom_getsym(argv) != gensym("(null)"))
    {
		if(x->b_receive_id != gensym("(null)"))
        {
            sname = x->b_receive_id;
            canvas_realizedollar(eobj_getcanvas(x), sname);
			pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_receive_id);
        }
        sname = atom_getsym(argv);
        x->b_receive_id = sname;
        canvas_realizedollar(eobj_getcanvas(x), sname);
        pd_bind(&x->b_obj.o_obj.ob_pd, sname);
    }
    else
    {
        if(x->b_receive_id != gensym("(null)"))
        {
            sname = x->b_receive_id;
            canvas_realizedollar(eobj_getcanvas(x), sname);
			pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_receive_id);
        }
        x->b_receive_id = gensym("(null)");
    }
    return 0;
}

//! The default user send id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_sendid(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM && atom_getsym(argv) != gensym("(null)"))
    {
        x->b_send_id = atom_getsym(argv);
    }
    else
    {
        x->b_send_id = gensym("(null)");
    }

    return 0;
}

//! The default user preset id method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_presetid(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM && atom_getsym(argv) != gensym("(null)"))
    {
		if(x->b_objpreset_id != gensym("(null)"))
        {
			pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_objpreset_id);
        }
        x->b_objpreset_id = atom_getsym(argv);
        pd_bind(&x->b_obj.o_obj.ob_pd, x->b_objpreset_id);
    }
    else
    {
        if(x->b_objpreset_id != gensym("(null)"))
        {
			pd_unbind(&x->b_obj.o_obj.ob_pd, x->b_objpreset_id);
        }
        x->b_objpreset_id = gensym("(null)");
    }
    return 0;
}

//! The default user font method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_font(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
		if(atom_getsym(argv) == gensym("(null)"))
            x->b_font.c_family = gensym("Helvetica");
        else
            x->b_font.c_family = atom_getsym(argv);
    }
    else
        x->b_font.c_family = gensym("Helvetica");

    x->b_font.c_family = gensym(strtok(x->b_font.c_family->s_name," ',.-"));
    x->b_font.c_family->s_name[0] = toupper(x->b_font.c_family->s_name[0]);
    return 0;
}

//! The default user fontweight method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontweight(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
		if(atom_getsym(argv) == gensym("bold") || atom_getsym(argv) == gensym("Bold"))
            x->b_font.c_weight = gensym("bold");
        else
            x->b_font.c_weight = gensym("normal");
    }
    else
        x->b_font.c_weight = gensym("normal");

    return 0;
}

//! The default user fontslant method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontslant(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYM)
    {
		if(atom_getsym(argv) == gensym("italic") || atom_getsym(argv) == gensym("Italic"))
            x->b_font.c_slant = gensym("italic");
        else
            x->b_font.c_slant = gensym("roman");
    }
    else
        x->b_font.c_slant = gensym("roman");

    return 0;
}

//! The default user fontsize method for all ebox called by PD (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The gobj
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_set_fontsize(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_FLOAT)
    {
		x->b_font.c_sizereal = pd_clip_min(atom_getfloat(argv), 4);
    }
    else
        x->b_font.c_sizereal = 11;
#ifdef __APPLE__
    x->b_font.c_size = x->b_font.c_sizereal;
#elif  _WINDOWS
    x->b_font.c_size = x->b_font.c_sizereal - 1;
#else
    x->b_font.c_size = x->b_font.c_sizereal - 3;
#endif
    return 0;
}

//! The default size attribute method of ebox called when an size attribute has changed. This function restrains the width and the height depending of the ebox flags EBOX_GROWNO, EBOX_GROWLINK and EBOX_GROWINDI // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param attr      Nothing (for Max 6 compatibility)
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms that contains the new width and the new height
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_size_set(t_ebox *x, t_object *attr, long argc, t_atom *argv)
{
    float width, height;
    if(argc && argv)
    {
        if(x->b_flags & EBOX_GROWNO)
            return 0;
        else if(x->b_flags & EBOX_GROWLINK)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                width  = pd_clip_min(atom_getfloat(argv), 4);
                height = x->b_rect.height;
                x->b_rect.height += width - x->b_rect.width;
                if(x->b_rect.height < 4)
                {
                    x->b_rect.width += 4 - height;
                    x->b_rect.height = 4;
                }
                else
                {
                    x->b_rect.width  =  width;
                }
            }
            
        }
        else if (x->b_flags & EBOX_GROWINDI)
        {
            if(atom_gettype(argv) == A_FLOAT)
                x->b_rect.width = pd_clip_min(atom_getfloat(argv), 4);
            if(atom_gettype(argv+1) == A_FLOAT)
                x->b_rect.height = pd_clip_min(atom_getfloat(argv+1), 4);
        }
    }

	return 0;
}

//! The default notify method of ebox called when an attribute has changed // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param s         The name of the attribute
 \ @param msg       Nothing (for Max 6 compatibility)
 \ @param sender    Nothing (for Max 6 compatibility)
 \ @param data      Nothing (for Max 6 compatibility)
 \ @return          Always 0 (for the moment)
 */
t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_eclass* c = eobj_getclass(x);
    if(s == gensym("size"))
    {
        if(c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->b_rect);
        ebox_invalidate_all(x);
        if(ebox_isdrawable(x))
        {
            sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.), (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));
            canvas_fixlinesfor(x->b_obj.o_canvas, (t_text *)x);
        }
        ebox_redraw(x);
    }

    return 0;
}

//! The attribute print method that post all the attributes characteristics in the PD console // PRIVATE
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @return          Nothing
 */
void ebox_attrprint(t_ebox* x)
{
    int i;
    t_eclass* c = eobj_getclass(x);
    post("%s attributes :", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {

        post("Label : \"%s\" Name : \"%s\" Type : \"%s\" Size : \"%i\"", c->c_attr[i]->label->s_name, c->c_attr[i]->name->s_name, c->c_attr[i]->type->s_name, c->c_attr[i]->size);
    }
}

//! Open the properties window (PRIVATE)
/*
 \ @memberof        ebox
 \ @param z         The gobj object
 \ @param glist     The canvas
 \ @return          Nothing
 */
void ebox_properties(t_ebox *x, t_glist *glist)
{
    int i, j;
    t_atom *argv = NULL;
    long    argc = 0;
    t_eclass* c = eobj_getclass(x);
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];

    sprintf(buffer, "pdtk_%s_dialog %%s", c->c_class.c_name->s_name);

    for(i = 0; i < c->c_nattr; i++)
    {
        if(!c->c_attr[i]->invisible)
        {
            object_attr_getvalueof((t_object *)x, c->c_attr[i]->name, &argc, &argv);
            strcat(buffer, " ");
            strcat(buffer, "\"");
            if(argc && argv)
            {
#ifndef _WINDOWS
                for(j = 0; j < argc-1; j++)
#else
                    for(j = 0; j < argc; j++)
#endif
                    {
                        atom_string(argv+j, temp, MAXPDSTRING);
                        if(c->c_attr[i]->type == gensym("symbol") && strchr(temp, ' '))
                        {
                            
                            strcat(buffer, "'");
                            strcat(buffer, temp);
                            strcat(buffer, "'");
                        }
                        else
                        {
                            strcat(buffer, temp);
                        }
                        strcat(buffer, " ");
                    }
#ifndef _WINDOWS
                atom_string(argv+j, temp, MAXPDSTRING);
                if(c->c_attr[i]->type == gensym("symbol") && strchr(temp, ' '))
                {
                    
                    strcat(buffer, "'");
                    strcat(buffer, temp);
                    strcat(buffer, "'");
                }
                else
                {
                    strcat(buffer, temp);
                }
                free(argv);
                argc = 0;
#endif
            }
            strcat(buffer, "\"");
        }
    }
    strcat(buffer, "\n");
    //post(buffer);

    gfxstub_new(&x->b_obj.o_obj.ob_pd, x, buffer);
}

//! Receive the properties window messages and change the attributes values (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param s         Nothing (for Max 6 compatibility)
 \ @return          Nothing
 */
void ebox_dialog(t_ebox *x, t_symbol *s, long argc, t_atom* argv)
{
    int i;
    int attrindex;
    t_eclass* c = eobj_getclass(x);
    t_atom *av;
    long ac;
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];
    t_rgb color;
    
    if(argc > 2 && argv)
    {
        ebox_attrprocess_viatoms(x, argc, argv);
        if(atom_gettype(argv) == A_SYM && atom_gettype(argv+1) ==A_FLOAT)
        {
            attrindex = (int)atom_getfloat(argv+1) - 1;
            if(attrindex >= 0 && attrindex < c->c_nattr)
            {
                object_attr_getvalueof((t_object *)x, c->c_attr[attrindex]->name, &ac, &av);
                if(ac && av)
                {
                    if(c->c_attr[attrindex]->style == gensym("checkbutton"))
                    {
                        if(atom_getfloat(av) == 0)
                            sys_vgui("%s.sele%i.selec deselect \n", atom_getsym(argv)->s_name, attrindex+1);
                        else
                            sys_vgui("%s.sele%i.selec select \n", atom_getsym(argv)->s_name, attrindex+1);
                    }
                    else if(c->c_attr[attrindex]->style == gensym("color"))
                    {
                        color.red = atom_getfloat(av);
                        color.green = atom_getfloat(av+1);
                        color.blue = atom_getfloat(av+2);
                        sys_vgui("%s.sele%i.selec configure -readonlybackground %s \n", atom_getsym(argv)->s_name, attrindex+1, rgb_to_hex(color));
                    }
                    else if(c->c_attr[attrindex]->style == gensym("menu"))
                    {
                        atom_string(av, buffer, MAXPDSTRING);
                        for(i = 1; i < ac; i++)
                        {
                            atom_string(av+i, temp, MAXPDSTRING);
                            strcat(buffer, " ");
                            strcat(buffer, temp);
                        }
                        sys_vgui("%s.sele%i.selec delete 0 end \n", atom_getsym(argv)->s_name, attrindex+1);
                        sys_vgui("%s.sele%i.selec insert 0 \"%s\" \n", atom_getsym(argv)->s_name, attrindex+1, buffer);
                    }
                    else
                    {
                        atom_string(av, buffer, MAXPDSTRING);
                        for(i = 1; i < ac; i++)
                        {
                            atom_string(av+i, temp, MAXPDSTRING);
                            strcat(buffer, " ");
                            strcat(buffer, temp);
                        }
                        sys_vgui("%s.sele%i.selec delete 0 end \n", atom_getsym(argv)->s_name, attrindex+1);
                        sys_vgui("%s.sele%i.selec insert 0 \"%s\" \n", atom_getsym(argv)->s_name, attrindex+1, buffer);
                        
                    }
                }
                
            }
        }
    }
}


//! @endcond





