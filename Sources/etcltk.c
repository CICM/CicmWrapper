/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eguicontext.h"
#include "eguiimp.h"

#ifdef CICM_WRAPPER_TCL

struct _eguicontext
{
    t_object    g_object;
    t_symbol**  g_fonts;
    size_t      g_nfonts;
};

static char *my_cursorlist[] =
{
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
};

static t_eguicontext* context = NULL;

static void eguicontext_fonts(t_eguicontext* ctxt, t_symbol* s, int argc, t_atom* argv)
{
    int i = 0;
    const char* name;
    t_symbol** temp;
    if(argc == 1 && atom_gettype(argv) == A_SYMBOL)
    {
        name = atom_getsymbol(argv)->s_name;
        while(name[i])
        {
            if(!isalpha(name[i]))
            {
                return;
            }
            i++;
        }
        if(ctxt->g_fonts && ctxt->g_nfonts)
        {
            temp = (t_symbol **)realloc(ctxt->g_fonts, (ctxt->g_nfonts + 1) * sizeof(t_symbol *));
            if(temp)
            {
                ctxt->g_fonts = temp;
                ctxt->g_fonts[ctxt->g_nfonts] = atom_getsymbol(argv);
                ctxt->g_nfonts++;
            }
            else
            {
                pd_error(ctxt, "can't allocate memory for font %s", atom_getsymbol(argv)->s_name);
            }
        }
        else
        {
            ctxt->g_fonts = (t_symbol **)malloc(sizeof(t_symbol *));
            if(ctxt->g_fonts)
            {
                ctxt->g_fonts[0] = atom_getsymbol(argv);
                ctxt->g_nfonts = 1;
            }
            else
            {
                pd_error(ctxt, "can't allocate memory for font %s", atom_getsymbol(argv)->s_name);
            }
        }
    }
}

static void eguicontext_free(t_eguicontext* ctxt)
{
    if(ctxt->g_nfonts && ctxt->g_fonts)
    {
        free(ctxt->g_fonts);
        ctxt->g_fonts = NULL;
        ctxt->g_nfonts= 0;
    }
    pd_unbind((t_pd *)ctxt, gensym("eguicontext1572"));
}

void eguicontext_init(void)
{
    if(!context)
    {
        t_class* eclass = NULL;
        t_eguicontext* obj = (t_eguicontext *)gensym("eguicontext1572")->s_thing;
        if(!obj)
        {
            eclass = class_new(gensym("eguicontext"), NULL, (t_method)eguicontext_free, sizeof(t_eview), CLASS_PD, A_GIMME, 0);
            
            class_addmethod(eclass, (t_method)eguicontext_fonts, gensym("fonts"), A_GIMME);
            obj = (t_eguicontext *)pd_new(eclass);
            if(obj)
            {
                obj->g_nfonts = 0;
                obj->g_fonts  = NULL;
                
                pd_bind((t_pd *)obj, gensym("eguicontext1572"));
                context = (t_eguicontext *)obj;
                
                sys_gui("set fontlist [font families]\n");
                sys_gui("foreach cf $fontlist {\n");
                sys_gui("pdsend \"eguicontext1572 fonts $cf\"}\n");
            }
        }
        else
        {
            context = (t_eguicontext *)obj;
        }
    }
    
}


t_eguicontext* eguicontext_get(void)
{
    return context;
}

t_pd_err eguicontext_fontfamilies(t_eguicontext *ctxt, size_t* nfonts, t_symbol*** fonts)
{
    if(ctxt->g_nfonts && ctxt->g_fonts)
    {
        *fonts = (t_symbol **)malloc(ctxt->g_nfonts * sizeof(t_symbol*));
        if(*fonts)
        {
            memcpy(*fonts, ctxt->g_fonts, ctxt->g_nfonts * sizeof(t_symbol*));
            *nfonts = ctxt->g_nfonts;
            return 0;
        }
        else
        {
            *fonts = NULL;
            *nfonts = 0;
            return 1;
        }
    }
    *fonts = NULL;
    *nfonts = 0;
    return 0;
}

t_pd_err eguicontext_view_add(t_eguicontext *ctxt, t_eview *view)
{
    sys_vgui("canvas .x%lx.c.canvas%lx -width %i -height %i -takefocus 1 -bg grey \
             -highlightthickness 0 -insertborderwidth 0 -state normal -insertwidth 0 -bd 0\n",
             (unsigned long)view->v_canvas, (unsigned long)view, (int)view->v_bounds.width, (int)view->v_bounds.height);
    
    sys_vgui("bind .x%lx.c.canvas%lx <Enter> {+pdsend {%s mouseenter %%x %%y %%s}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    sys_vgui("bind .x%lx.c.canvas%lx <Leave> {+pdsend {%s mouseleave %%x %%y %%s}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    sys_vgui("bind .x%lx.c.canvas%lx <Button-3> {+pdsend {%s mousedown %%x %%y %i}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name, EMOD_RIGHT);
    sys_vgui("bind .x%lx.c.canvas%lx <Button-2> {+pdsend {%s mousedown %%x %%y %i}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name, EMOD_RIGHT);
    sys_vgui("bind .x%lx.c.canvas%lx <Button-1> {+pdsend {%s mousedown %%x %%y %%s}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    sys_vgui("bind .x%lx.c.canvas%lx <ButtonRelease> {+pdsend {%s mouseup %%x %%y %%s}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    sys_vgui("bind .x%lx.c.canvas%lx <Motion> {+pdsend {%s mousemotion %%x %%y %%s}}\n",
             (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    
    if(view->v_mouse_dblclick)
    {
        sys_vgui("bind .x%lx.c.canvas%lx <Double-Button-1> {+pdsend {%s dblclick %%x %%y %%s}}\n",
                 (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    }
    if(view->v_mouse_wheel)
    {
        sys_vgui("bind .x%lx.c.canvas%lx <MouseWheel> {+pdsend {%s mousewheel  %%x %%y %%D %%s}}\n",
                 (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    }
    if(view->v_key_press || view->v_key_filter)
    {
        sys_vgui("bind .x%lx.c.canvas%lx <KeyPress>  {+pdsend {%s key %%N %%s}} \n",
                 (unsigned long)view->v_canvas, (unsigned long)view, view->v_tag->s_name);
    }
    
    sys_vgui(".x%lx.c create window %i %i -anchor nw -window .x%lx.c.canvas%lx -tags win%lx -width %i -height %i\n",
             (unsigned long)view->v_canvas,
             (int)view->v_bounds.x, (int)view->v_bounds.y,
             (unsigned long)view->v_canvas, (unsigned long)view, (unsigned long)view,
             (int)view->v_bounds.width, (int)view->v_bounds.height);
    
    sys_vgui("focus -force .x%lx.c\n", (unsigned long)view->v_canvas);
    return 0;
}

t_pd_err eguicontext_view_remove(t_eguicontext *ctxt, t_eview *view)
{
    sys_vgui("destroy .x%lx.c.canvas%lx\n", (unsigned long)view->v_canvas, (unsigned long)view);
    return 0;
}

t_pd_err eguicontext_view_setcursor(t_eguicontext *ctxt, t_eview *view, ecursor_types type)
{
    sys_vgui(".x%lx.c.canvas%lx configure -cursor %s\n", (unsigned long)view->v_canvas, (unsigned long)view, my_cursorlist[type]);
    return 0;
}

t_pd_err eguicontext_view_boundschanged(t_eguicontext *ctxt, t_eview* view)
{
    sys_vgui(".x%lx.c coords win%lx %d %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.x, (int)view->v_bounds.y);
    sys_vgui(".x%lx.c itemconfigure win%lx -width %d -height %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.width, (int)view->v_bounds.height);
    canvas_fixlinesfor(view->v_canvas, (t_text*)view->v_owner);
    return 0;
}

t_pd_err eguicontext_canvas_mousedown(t_eguicontext *ctxt, t_canvas* cnv, long mod)
{
    if(mod == EMOD_SHIFT)
    {
        sys_vgui("pdtk_canvas_mouse .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0 1\n", (unsigned long)cnv, (unsigned long)cnv, (unsigned long)cnv);
    }
    else if(mod == EMOD_RIGHT)
    {
        sys_vgui("pdtk_canvas_rightclick .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0\n", (unsigned long)cnv, (unsigned long)cnv, (unsigned long)cnv);
    }
    else
    {
        sys_vgui("pdtk_canvas_mouse .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 0 0\n", (unsigned long)cnv, (unsigned long)cnv, (unsigned long)cnv);
    }
    return 0;
}

t_pd_err eguicontext_canvas_mouseup(t_eguicontext *ctxt, t_canvas* cnv, long mod)
{
    sys_vgui("pdtk_canvas_mouseup .x%lx.c [expr [winfo pointerx .] - [winfo rootx .x%lx.c]] [expr [winfo pointery .] - [winfo rooty .x%lx.c]] 1\n", (unsigned long)cnv, (unsigned long)cnv, (unsigned long)cnv);
    return 0;
}


static void eguicontext_view_paint_gatom(t_eview *view, t_elayer const* l, t_gatom *a, const float xoffset, const float yoffset)
{
    t_gpath_impl const* tp;
    if(a->a_type == EPD_GATOM_PATH && a->a_word.w_path.i_path.p_size)
    {
        tp = &a->a_word.w_path;
        if(tp->i_path.p_points[0].x == E_PATH_MOVE)
        {
            if(tp->i_filled)
            {
                sys_vgui(".x%lx.c.canvas%lx create polygon %f %f %f %f -fill %s -width 0 -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), (unsigned long)l);
            }
            else
            {
                sys_vgui(".x%lx.c.canvas%lx create line %f %f %f %f -fill %s -width 0 -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), (unsigned long)l);
            }
        }
        else if(tp->i_path.p_points[0].x == E_PATH_RECT)
        {
            if(tp->i_filled)
            {
                sys_vgui(".x%lx.c.canvas%lx create rectangle %f %f %f %f -fill %s -width 0 -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), (unsigned long)l);
            }
            else
            {
                sys_vgui(".x%lx.c.canvas%lx create rectangle %f %f %f %f -outline %s -width %f -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), tp->i_width, (unsigned long)l);
            }
        }
        else if(tp->i_path.p_points[0].x == E_PATH_OVAL)
        {
            if(tp->i_filled)
            {
                sys_vgui(".x%lx.c.canvas%lx create oval %f %f %f %f -fill %s -width 0 -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), (unsigned long)l);
            }
            else
            {
                sys_vgui(".x%lx.c.canvas%lx create oval %f %f %f %f -outline %s -width %f -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         rgba_to_hex(&tp->i_color), tp->i_width, (unsigned long)l);
            }
        }
        else if(tp->i_path.p_points[0].x == E_PATH_ARC)
        {
            if(tp->i_filled)
            {
                sys_vgui(".x%lx.c.canvas%lx create arc %f %f %f %f -start %f -extent %f -style pieslice -fill %s -width 0 -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         tp->i_path.p_points[3].x + xoffset, tp->i_path.p_points[3].y + yoffset,
                         rgba_to_hex(&tp->i_color), (unsigned long)l);
            }
            else
            {
                sys_vgui(".x%lx.c.canvas%lx create arc %f %f %f %f -start %f -extent %f -style arc -outline %s -width %f -tags layer%lx\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset,
                         tp->i_path.p_points[2].x + xoffset, tp->i_path.p_points[2].y + yoffset,
                         tp->i_path.p_points[3].x + xoffset, tp->i_path.p_points[3].y + yoffset,
                         rgba_to_hex(&tp->i_color), tp->i_width, (unsigned long)l);
            }
            
        }
    }
    else if(a->a_type == EPD_GATOM_TEXT)
    {
        
    }
}

t_pd_err eguicontext_view_paint_layer(t_eguicontext *ctxt, t_eview *view, t_elayer *l, const float xoffset, const float yoffset)
{
    size_t i;
    if(l->l_state == EPD_LAYER_CLOSE)
    {
        sys_vgui(".x%lx.c.canvas%lx raise layer%lx\n", (unsigned long)view->v_canvas, (unsigned long)view, (unsigned long)l);
        return 0;
    }
    else if(l->l_state == EPD_LAYER_READY)
    {
        sys_vgui(".x%lx.c.canvas%lx delete layer%lx\n", (unsigned long)view->v_canvas, (unsigned long)view, (unsigned long)l);
        for (i = 0; i < l->l_natoms; i++)
        {
            eguicontext_view_paint_gatom(view, l, l->l_atoms+i, xoffset, yoffset);
        }
        return 0;
    }
    return -1;
}

/*
t_pd_err ebox_paint_layer(t_ebox *x, t_object* view, t_symbol *name, float x_p, float y_p)
{
    if(1)//!x->b_drawing_id)
    {
        return 0;
    }
    int i, j;
    float start, extent, radius;
    t_elayer* g = NULL;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            g = &x->b_layers[i];
            if(g->e_state == elayer_CLOSE)
            {
                sys_vgui("%s raise %s\n", x->b_drawing_id->s_name, g->e_id->s_name);
                return -1;
            }
            else if(g->e_state != elayer_TODRAW)
            {
                return -1;
            }
        }
    }
    if(g)
    {
        
        for(i = 0; i < g->e_number_objects; i++)
        {
            t_egobj* gobj = g->e_objects+i;
            t_pt * pt;
            ////////////// PATH & LINE ///////////////////////////
            if(gobj->e_type == E_GOBJ_PATH && gobj->e_npoints > 3)
            {
                char header[256];
                char bottom[256];
                int mode = E_PATH_MOVE;
                if(gobj->e_filled)
                {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                }
                else
                {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                }
                
                for(j = 0; j < gobj->e_npoints; )
                {
                    pt = gobj->e_points+j++;
                    if(pt->x == E_PATH_MOVE)
                    {
                        if(mode == E_PATH_CURVE)
                        {
                            sys_vgui("%s", bottom);
                        }
                        sys_vgui("%s", header);
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p), (int)(pt->y + y_p));
                        mode = E_PATH_MOVE;
                    }
                    else if(pt->x == E_PATH_CURVE)
                    {
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p),
                                 (int)((pt+2)->x + x_p), (int)((pt+2)->y + y_p),
                                 (int)((pt+3)->x + x_p), (int)((pt+3)->y + y_p));
                        j += 3;
                        mode = E_PATH_CURVE;
                    }
                    else if(pt->x == E_PATH_LINE)
                    {
                        pt = gobj->e_points+j-1;
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt-1)->x + x_p), (int)((pt-1)->y + y_p),
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p),
                                 (int)((pt+1)->x + x_p), (int)((pt+1)->y + y_p));
                        ++j;
                        mode = E_PATH_CURVE;
                    }
                    else
                    {
                        j++;
                    }
                }
                sys_vgui("%s", bottom);
            }
            ////////////// RECT ////////////////
            ////////////// OVAL ////////////////
            ////////////// ARC /////////////////
            ////////////// TEXT ////////////////
            else if(gobj->e_type == E_GOBJ_TEXT)
            {
                char text[256];
                stringifyJustification(gobj->e_justify, text);
                const t_pt np = recomputeRectangle(gobj->e_justify, gobj->e_points, gobj->e_points+1);
                sys_vgui("%s create text %d %d -text {%s} %s -font {{%s} %d %s %s} -fill %s -width %d -tags { %s %s }\n",
                         x->b_drawing_id->s_name,
                         (int)(np.x + x_p),
                         (int)(np.y + y_p),
                         gobj->e_text,
                         text,
                         gobj->e_font.family->s_name, (int)gobj->e_font.size, gobj->e_font.weight->s_name, gobj->e_font.slant->s_name,
                         rgba_to_hex(&gobj->e_color),
                         (int)(gobj->e_points[1].x) * (int)gobj->e_wrap,
                         g->e_id->s_name,
                         x->b_all_id->s_name);
            }
            else
            {
                return -1;
            }
        }
        g->e_state = elayer_CLOSE;
    }
    else
    {
        return -1;
    }
    
    return 0;
}
*/

#endif




