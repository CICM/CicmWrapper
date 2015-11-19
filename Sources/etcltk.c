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
#include "eclass.h"

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

static void eguicontext_view_create_border(t_eguicontext *ctxt, t_eview *view)
{
    const float offset = ceilf(view->v_params.d_borderthickness * 0.5f);
    sys_vgui(".x%lx.c.canvas%lx delete border%lx\n", (unsigned long)view->v_canvas, (unsigned long)view, (unsigned long)view);
    if(view->v_selected)
    {
        sys_vgui(".x%lx.c.canvas%lx create rectangle %f %f %f %f -outline #0000FF -width %f -tags border%lx\n",
                 (unsigned long)view->v_canvas, (unsigned long)view,
                 offset, offset, view->v_bounds.width - offset, view->v_bounds.height - offset,
                 view->v_params.d_borderthickness, (unsigned long)view);
    }
    else
    {
        sys_vgui(".x%lx.c.canvas%lx create rectangle %f %f %f %f -outline %s -width %f -tags border%lx\n",
                 (unsigned long)view->v_canvas, (unsigned long)view,
                 offset, offset, view->v_bounds.width - offset, view->v_bounds.height - offset,
                 rgba_to_hex(&view->v_params.d_bordercolor), view->v_params.d_borderthickness, (unsigned long)view);
    }
}

#include "eobj.h"
#include "m_imp.h"

inline static char view_isinsubcanvas(t_eview const* view)
{
    return view->v_canvas != eobj_getcanvas(view->v_owner);
}

static void eguicontext_view_create_iolet(t_eguicontext *ctxt, t_eview *view)
{
    int i, pos;
    const int ninlets = obj_ninlets((t_object *)view->v_owner);
    const int noutlet = obj_noutlets((t_object *)view->v_owner);
    sys_vgui(".x%lx.c.canvas%lx delete iolet%lx\n", (unsigned long)view->v_canvas, (unsigned long)view, (unsigned long)view);
    if(!view_isinsubcanvas(view))
    {
        for(i = 0; i < ninlets; i++)
        {
            pos = 0;
            if(ninlets != 1)
            {
                pos = (int)(i / (float)(ninlets - 1) * (view->v_bounds.width - 8));
            }
            sys_vgui(".x%lx.c.canvas%lx create rectangle %i 0 %i 2 -fill #000000 -width 0 -tags iolet%lx\n",
                     (unsigned long)view->v_canvas, (unsigned long)view,
                     pos, pos + 7, (unsigned long)view);
        }
        for(i = 0; i < noutlet; i++)
        {
            pos = 0;
            if(noutlet != 1)
            {
                pos = (int)(i / (float)(noutlet - 1) * (view->v_bounds.width - 8));
            }
            sys_vgui(".x%lx.c.canvas%lx create rectangle %i %f %i %f -fill #000000 -width 0 -tags iolet%lx\n",
                     (unsigned long)view->v_canvas, (unsigned long)view,
                     pos, view->v_bounds.width - 2.f, pos + 7, view->v_bounds.width, view);
        }
    }
}

t_pd_err eguicontext_view_add(t_eguicontext *ctxt, t_eview *view)
{
    sys_vgui("destroy .x%lx.c.canvas%lx\n", (unsigned long)view->v_canvas, (unsigned long)view);
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
    eguicontext_view_create_border(ctxt, view);
    eguicontext_view_create_iolet(ctxt, view);
    return 0;
}

t_pd_err eguicontext_view_remove(t_eguicontext *ctxt, t_eview *view)
{
    sys_vgui("destroy .x%lx.c.canvas%lx\n", (unsigned long)view->v_canvas, (unsigned long)view);
    sys_vgui("destroy .win%lx\n", (unsigned long)view);
    return 0;
}

t_pd_err eguicontext_view_setcursor(t_eguicontext *ctxt, t_eview *view, ecursor_types type)
{
    sys_vgui(".x%lx.c.canvas%lx configure -cursor %s\n", (unsigned long)view->v_canvas, (unsigned long)view, my_cursorlist[type]);
    return 0;
}

t_pd_err eguicontext_view_boundschanged(t_eguicontext *ctxt, t_eview* view)
{
    const float xpos = text_xpix(((t_text *)view->v_owner), view->v_canvas);
    const float ypos = text_ypix(((t_text *)view->v_owner), view->v_canvas);
    sys_vgui(".x%lx.c coords win%lx %d %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)xpos, (int)ypos);
    sys_vgui(".x%lx.c itemconfigure win%lx -width %d -height %d\n", (unsigned long)view->v_canvas, (unsigned long)view,
             (int)view->v_bounds.width, (int)view->v_bounds.height);
    eguicontext_view_create_border(ctxt, view);
    eguicontext_view_create_iolet(ctxt, view);
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
    size_t i; t_pt ref;
    t_gpath_impl const* tp;
    if(a->a_type == EPD_GATOM_PATH && a->a_word.w_path.i_path.p_size)
    {
        tp = &a->a_word.w_path;
        if(tp->i_path.p_points[0].x == E_PATH_MOVE)
        {
            if(tp->i_filled)
            {
                ref = tp->i_path.p_points[1];
                sys_vgui(".x%lx.c.canvas%lx create polygon %f %f\n",
                         (unsigned long)view->v_canvas, (unsigned long)view,
                         tp->i_path.p_points[1].x + xoffset, tp->i_path.p_points[1].y + yoffset);
                i = 2;
                while(i < tp->i_path.p_size)
                {
                    if(tp->i_path.p_points[i].x == E_PATH_MOVE)
                    {
                        sys_vgui(" -fill %s -width 0 -tags layer%lx\n", rgba_to_hex(&tp->i_color), (unsigned long)l);
                        sys_vgui(".x%lx.c.canvas%lx create polygon %f %f\n",
                                 (unsigned long)view->v_canvas, (unsigned long)view,
                                 tp->i_path.p_points[i+1].x + xoffset, tp->i_path.p_points[i+1].y + yoffset);
                        i += 2;
                    }
                    else if(tp->i_path.p_points[i].x == E_PATH_LINE)
                    {
                        sys_vgui(" %f %f\n", tp->i_path.p_points[i+1].x + xoffset, tp->i_path.p_points[i+1].y + yoffset);
                        i += 2;
                    }
                    else if(tp->i_path.p_points[i].x == E_PATH_CURVE)
                    {
                        sys_vgui(" %f %f\n", tp->i_path.p_points[i+1].x + xoffset, tp->i_path.p_points[i+1].y + yoffset);
                        i += 2;
                    }
                }
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







typedef struct t_ewindowprop
{
    t_object    c_obj;
    t_object*   c_owner;
    size_t      c_nitems;
}t_ewindowprop;

static t_class* ewindowprop_setup()
{
    t_class* eclass = NULL;
    t_pd* obj = gensym("ewindowprop1572")->s_thing;
    if(!obj)
    {
        eclass = class_new(gensym("ewindowprop"), NULL, (t_method)NULL, sizeof(t_ewindowprop), CLASS_PD, A_GIMME, 0);
        obj = pd_new(eclass);
        pd_bind(obj, gensym("ewindowprop1572"));
        return eclass;
    }
    else
    {
        return *obj;
    }
}

t_object* ewindowprop_getfromobject(t_eobj* x)
{
    char text[MAXPDSTRING];
    t_class* c = ewindowprop_setup();
    if(c)
    {
        sprintf(text, "windowprop%lx", (unsigned long)x);
        return (t_object *)pd_findbyclass(gensym(text), c);
    }
    return NULL;
}

void ewindowprop_destroy(t_eobj* x)
{
    char text[MAXPDSTRING];
    t_ewindowprop* wm = (t_ewindowprop *)ewindowprop_getfromobject(x);
    if(wm)
    {
        sys_vgui("destroy .epw%lx\n", (unsigned long)x);
        sprintf(text, "windowprop%lx", (unsigned long)x);
        pd_unbind((t_pd *)wm, gensym(text));
        free(wm);
    }
}

void ewindowprop_update(t_eobj* x)
{
    /*
    int i, j, argc;
    t_atom* argv;
    t_rgba color;
    char text[MAXPDSTRING];
    t_ewindowprop* wm = (t_ewindowprop *)ewindowprop_getfromobject(x);
    const unsigned long ref = (unsigned long)x;
    t_eclass const* c = eobj_getclass(x);
    if(wm)
    {
        for(i = 0; i < c->c_nattr; i++)
        {
            if(!c->c_attr[i]->invisible)
            {
                eobj_attr_getvalue(x,  c->c_attr[i]->name, &argc, &argv);
                if(c->c_attr[i]->style == s_cream_checkbutton)
                {
                    sys_vgui("set var%lxattr_value%i %lx\n", ref, i+1, atom_getintarg(0, argc, argv));
                }
                else if(c->c_attr[i]->style == s_cream_color)
                {
                    atom_getcolorarg(0, argc, argv, &color);
                    sys_vgui("set var%lxattr_value%i %s\n", ref, i+1, rgba_to_hex(&color));
                    sys_vgui("if {[winfo exists .epw%lx]} {\n \
                             .epw%lx.attr_values%i.label configure -readonlybackground $var%lxattr_value%i\n}\n",
                             ref, ref, i+1, ref, i+1);
                }
                else if(c->c_attr[i]->style == s_cream_number)
                {
                    sys_vgui("set var%lxattr_value%i %g\n", ref, i+1, atom_getfloatarg(0, argc, argv));
                }
                else if(c->c_attr[i]->style == s_cream_menu)
                {
                    sys_vgui("set var%lxattr_value%i \"%s\"\n", ref, i+1, atom_getsymbolarg(0, argc, argv));
                }
                else
                {
                    if(argc && argv)
                    {
                        sys_vgui("set var%lxattr_value%i [concat ", ref, i+1);
                        for(j = 0; j < argc; j++)
                        {
                            if(atom_gettype(argv+j) == A_FLOAT)
                            {
                                sys_vgui("%g ", atom_getfloat(argv+j));
                            }
                            else if(atom_gettype(argv+j) == A_SYMBOL)
                            {
                                atom_string(argv+j, text, MAXPDSTRING);
                                sys_vgui("'%s' ", text);
                            }
                        }
                        sys_gui("]\n");
                    }
                    else
                    {
                        sys_vgui("set var%lxattr_value%i \"\"\n", ref, i+1);
                    }
                }
            }
        }
    }
     */
}

static void ewindowprop_addattr(t_ewindowprop* x, t_eattr const* attr, size_t i)
{
    size_t j, nitems; int argc;
    t_atom* argv;
    t_rgba color;
    char text[MAXPDSTRING];
    const unsigned long ref = (unsigned long)x->c_owner;
    t_symbol* name  = eattr_getname(attr);
    t_symbol* style = eattr_getstyle(attr);
    t_symbol* label = eattr_getlabel(attr);
    t_symbol** items;
    
    sys_vgui("frame .epw%lx.attr_label%i\n", ref, i+1);
    sys_vgui("frame .epw%lx.attr_name%i\n",  ref, i+1);
    sys_vgui("frame .epw%lx.attr_values%i\n",ref, i+1);
    
    sys_vgui("label .epw%lx.attr_label%i.label -justify left -font {Helvetica 12} -text \"%s\"\n",
             ref, i+1, label->s_name);
    sys_vgui("label .epw%lx.attr_name%i.label -justify left -font {Helvetica 12 italic} -text \"%s\"\n",
             ref, i+1, name->s_name);
    
    eobj_attr_getvalue(x->c_owner, name, &argc, &argv);
    if(style == s_cream_checkbutton)
    {
        sys_vgui("set var%lxattr_value%i %f\n", ref, i+1, (int)atom_getfloatarg(0, argc, argv));
        
        sys_vgui("checkbutton .epw%lx.attr_values%i.label ", ref, i+1);
        sys_vgui("-variable var%lxattr_value%i -command {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
    }
    else if(style == s_cream_color)
    {
        atom_getcolorarg(0, argc, argv, &color);
        sys_vgui("set var%lxattr_value%i %s\n", ref, i+1, rgba_to_hex(&color));
        
        sys_vgui("entry .epw%lx.attr_values%i.label -font {Helvetica 12} -width 20 ", ref, i+1);
        sys_vgui("-readonlybackground $var%lxattr_value%i -state readonly\n", ref, i+1);
        
        sys_vgui("bind .epw%lx.attr_values%i.label <Button> ", ref, i+1);
        sys_vgui("[concat epicker_apply %s %s $var%lxattr_value%i .epw%lx.attr_values%i.label]\n",
                 eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1, ref, i+1);
    }
    else if(style == s_cream_number)
    {
        sys_vgui("set var%lxattr_value%i %g\n", ref, i+1, atom_getfloatarg(0, argc, argv));
        
        sys_vgui("spinbox .epw%lx.attr_values%i.label -font {Helvetica 12} -width 18 ", ref, i+1);
        sys_vgui("-textvariable var%lxattr_value%i -command {pdsend \"%s %s $var%lxattr_value%i\"} ",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("-increment %f -from %f -to %f\n", eattr_getstep(attr), eattr_getminimum(attr), eattr_getmaximum(attr));
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getfloatarg(0, argc, argv));
    }
    else if(style == s_cream_menu)
    {
        eattr_getitems(attr, &nitems, &items);
        sys_vgui("set var%lxattr_value%i \"%s\"\n", ref, i+1, atom_getsymbolarg(0, argc, argv));
        
        sys_vgui("spinbox .epw%lx.attr_values%i.label -font {Helvetica 12} -width 18 -state readonly ", ref, i+1);
        sys_vgui("-textvariable [string trim var%lxattr_value%i] -command {pdsend \"%s %s $var%lxattr_value%i\"}",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("-values {");
        for(j = 0; j < nitems; j++)
        {
            sys_vgui("%s ", items[nitems- 1 - j]->s_name);
        }
        sys_vgui("}\n");
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getsymbolarg(0, argc, argv));
        if(nitems && items)
        {
            free(items);
        }
    }
    else
    {
        if(argc && argv)
        {
            sys_vgui("set var%lxattr_value%i [concat ", ref, i+1);
            for(j = 0; j < (size_t)argc; j++)
            {
                if(atom_gettype(argv+j) == A_FLOAT)
                {
                    sys_vgui("%g ", atom_getfloat(argv+j));
                }
                else if(atom_gettype(argv+j) == A_SYMBOL)
                {
                    atom_string(argv+j, text, MAXPDSTRING);
                    sys_vgui("'%s' ", text);
                }
            }
            sys_gui("]\n");
        }
        else
        {
            sys_vgui("set var%lxattr_value%i \"\"\n", ref, i+1);
        }
        
        sys_vgui("entry .epw%lx.attr_values%i.label -font {Helvetica 12} -width 20 ", ref, i+1);
        sys_vgui("-textvariable var%lxattr_value%i\n", ref, i+1);
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getsymbolarg(0, argc, argv));
    }
    if(argc && argv)
    {
        free(argv);
    }
    argv = NULL;
    argc = 0;
    
    
    sys_vgui("pack .epw%lx.attr_label%i.label    -side left -fill both -expand 1\n",    ref, i+1);
    sys_vgui("pack .epw%lx.attr_name%i.label     -side left -fill both -expand 1\n",    ref, i+1);
    sys_vgui("pack .epw%lx.attr_values%i.label   -side left -fill both -expand 1\n",    ref, i+1);
    
    sys_vgui("grid config .epw%lx.attr_label%i   -column 0 -columnspan 2 -row %i -sticky w\n", ref, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.attr_name%i    -column 2 -row %i -sticky w\n",               ref, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.attr_values%i  -column 3 -columnspan 2 -row %i -sticky w\n", ref, i+1, x->c_nitems++);
}

/*
static void ewindowprop_addparam(t_ewindowprop* x, t_eparam const* param, int i)
{
    t_ebox const* z        = (t_ebox const*)x->c_owner;
    const unsigned long tx = (unsigned long)x->c_owner;
    
    sys_vgui("frame .epw%lx.params_menu_index%i\n",   tx, i+1);
    sys_vgui("frame .epw%lx.params_menu_name%i\n",    tx, i+1);
    sys_vgui("frame .epw%lx.params_menu_label%i\n",   tx, i+1);
    
    sys_vgui("set var%lxparam_menu_index%i %i\n", tx, i+1, z->b_params[i]->p_index);
    sys_vgui("entry .epw%lx.params_menu_index%i.entry -font {Helvetica 12} -width 5 \
             -textvariable var%lxparam_menu_index%i\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_INDEX) ? "disable" : "normal");
    sys_vgui("set var%lxparam_menu_name%i \"%s\"\n", tx, i+1, param->p_name->s_name);
    sys_vgui("entry .epw%lx.params_menu_name%i.entry -font {Helvetica 12} -width 13 \
             -textvariable var%lxparam_menu_name%i -state %s\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_NAME) ? "disable" : "normal");
    sys_vgui("set var%lxparam_menu_label%i \"%s\"\n", tx, i+1, z->b_params[i]->p_label->s_name);
    sys_vgui("entry .epw%lx.params_menu_label%i.entry -font {Helvetica 12} -width 15 \
             -textvariable var%lxparam_menu_label%i -state %s\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_LABEL) ? "disable" : "normal");
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Return> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Return> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Return> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Tab> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Tab> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Tab> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <FocusOut> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <FocusOut> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <FocusOut> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Escape> {set var%lxparam_menu_index%i %i}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_index);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Escape> {set var%lxparam_menu_name%i %s}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_name->s_name);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Escape> {set var%lxparam_menu_label%i %s}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_label->s_name);
    
    sys_vgui("pack  .epw%lx.params_menu_index%i.entry -side left -fill both -expand 1\n",  tx);
    sys_vgui("pack  .epw%lx.params_menu_name%i.entry -side left -fill both -expand 1\n",  tx);
    sys_vgui("pack  .epw%lx.params_menu_label%i.entry -side left -fill both -expand 1\n",  tx);
    
    sys_vgui("grid config .epw%lx.params_menu_index%i -column 0 -row %i -sticky w\n",   tx, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.params_menu_name%i -column 1 -row %i -sticky w\n",    tx, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.params_menu_label%i -column 2 -row %i -sticky w\n",   tx, i+1, x->c_nitems++);
}
 */

t_object* eguicontext_propw_new(t_eguicontext *ctxt, t_object* owner)
{
    size_t i;
    char text[MAXPDSTRING];
    t_ewindowprop* wm = NULL;
    const unsigned long ref = (unsigned long)owner;
    size_t          nattrs;
    t_eattr const** attrs;
    t_class* c2 = ewindowprop_setup();
    if(eobj_iscicm(owner))
    {
        if(c2)
        {
            wm = (t_ewindowprop *)pd_new(c2);
            if(wm)
            {
                sprintf(text, "windowprop%lx", ref);
                wm->c_owner     = owner;
                wm->c_nitems    = 0;
                pd_bind((t_pd *)wm, gensym(text));
            }
        }
    }
    if(wm)
    {
        wm->c_nitems = 0;
        // Window
        sys_vgui("toplevel .epw%lx\n", ref);
        sys_vgui("wm title .epw%lx {%s properties}\n", ref, class_getname((t_class *)eobj_getclass(owner)));
        sys_vgui("wm resizable .epw%lx 0 0\n", ref);
        
        eclass_get_attrs(eobj_getclass(owner), &nattrs, &attrs);
        if(nattrs && attrs)
        {
            sys_vgui("frame .epw%lx.attrs\n", ref);
            sys_vgui("label .epw%lx.attrs.label -justify center -font {Helvetica 13 bold} -text Attributes -height 2\n", ref);
            sys_vgui("pack  .epw%lx.attrs.label -side left -fill both -expand 1\n", ref);
            sys_vgui("grid config .epw%lx.attrs -columnspan 5 -row %i -sticky w\n", ref, wm->c_nitems++);
            
            sys_vgui("frame .epw%lx.attrs_menu_label\n", ref);
            sys_vgui("frame .epw%lx.attrs_menu_name\n",  ref);
            sys_vgui("frame .epw%lx.attrs_menu_values\n",ref);
            
            sys_vgui("label .epw%lx.attrs_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",    ref);
            sys_vgui("label .epw%lx.attrs_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",      ref);
            sys_vgui("label .epw%lx.attrs_menu_values.label -justify left -font {Helvetica 12 bold} -text Values\n",  ref);
            
            sys_vgui("pack  .epw%lx.attrs_menu_label.label -side left -fill both -expand 1\n",   ref);
            sys_vgui("pack  .epw%lx.attrs_menu_name.label -side left -fill both -expand 1\n",    ref);
            sys_vgui("pack  .epw%lx.attrs_menu_values.label -side left -fill both -expand 1\n",  ref);
            
            sys_vgui("grid config .epw%lx.attrs_menu_label -column 0 -columnspan 2 -row %i -sticky w\n",   ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.attrs_menu_name -column 2 -row %i -sticky w\n",                  ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.attrs_menu_values -column 3 -columnspan 2 -row %i -sticky w\n",  ref, wm->c_nitems++);
            
            
            for(i = 0; i < nattrs; i++)
            {
                if(eattr_isvisible(attrs[i]))
                {
                    ewindowprop_addattr(wm, attrs[i], i);
                }
            }
            free(attrs);
        }
        
        // Params
        /*
        if(eobj_isbox(x) && z->b_nparams)
        {
            sys_vgui("frame .epw%lx.params\n", ref);
            sys_vgui("label .epw%lx.params.label -justify center -font {Helvetica 13  bold} -text Parameters -height 2\n", ref);
            sys_vgui("pack  .epw%lx.params.label -side left -fill both -expand 1\n",  ref);
            sys_vgui("grid config .epw%lx.params -columnspan 3 -row %i -sticky w\n", ref, wm->c_nitems++);
            
            sys_vgui("frame .epw%lx.params_menu_index\n",ref);
            sys_vgui("frame .epw%lx.params_menu_name\n", ref);
            sys_vgui("frame .epw%lx.params_menu_label\n",ref);
            
            sys_vgui("label .epw%lx.params_menu_index.label -justify left -font {Helvetica 12 bold} -text Index\n",   ref);
            sys_vgui("label .epw%lx.params_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",     ref);
            sys_vgui("label .epw%lx.params_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",   ref);
            
            sys_vgui("pack  .epw%lx.params_menu_index.label -side left -fill both -expand 1\n",  ref);
            sys_vgui("pack  .epw%lx.params_menu_name.label -side left -fill both -expand 1\n",   ref);
            sys_vgui("pack  .epw%lx.params_menu_label.label -side left -fill both -expand 1\n",  ref);
            
            sys_vgui("grid config .epw%lx.params_menu_index -column 0 -row %i -sticky w\n",    ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.params_menu_name -column 1 -row %i -sticky w\n",     ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.params_menu_label -column 2 -row %i -sticky w\n",    ref, wm->c_nitems++);
            
            for(i = 0; i < z->b_nparams; i++)
            {
                if(z->b_params[i])
                {
                    ewindowprop_addparam(wm, z->b_params[i], i);
                }
            }
        }*/
    }
    return (t_object *)wm;
}

#endif




