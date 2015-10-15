/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "egui.h"
#include "ecommon.h"
#include "elayer.h"
#include "eobj.h"
#include "eview.h"

struct _egui
{
    t_object        g_object;           /*!< The object. */
    t_object*       g_owner;            /*!< The gui owner. */
    long            g_flags;            /*!< The gui flags. */
    t_eview**       g_views;            /*!< The gui view. */
    size_t          g_nviews;           /*!< The gui number of views. */
    t_pt            g_size;             /*!< The gui size. */
    
    t_symbol*       g_receive;          /*!< The reveive symbol (attribute). */
    t_symbol*       g_send;             /*!< The send send (attribute). */
    char            g_pinned;           /*!< The pinned state (attribute). */
    char            g_ignore_click;     /*!< The igore click state (attribute). */
    char            g_visible;          /*!< The visible state (attribute). */
};

static void egui_free(t_egui *gui)
{
    size_t i;
    for(i = 0; i < gui->g_nviews; i++)
    {
        pd_free((t_pd *)gui->g_views[i]);
    }
    if(gui->g_nviews && gui->g_views)
    {
        free(gui->g_views);
    }
}



static t_eview* egui_getview(t_egui const* gui, t_glist const* glist)
{
    size_t i;
    for(i = 0; i < gui->g_nviews; i++)
    {
        if(eview_getcanvas(gui->g_views[i]) == glist)
        {
            return gui->g_views[i];
        }
    }
    return NULL;
}

void egui_view_getposition(t_egui const* gui, t_glist const* glist, t_pt* pos)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_getposition(view, pos);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_getposition(gui->g_views[0], pos);
    }
}

void egui_view_getsize(t_egui const* gui, t_glist const* glist, t_pt* size)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_getsize(view, size);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_getsize(gui->g_views[0], size);
    }
}

void egui_view_getbounds(t_egui const* gui, t_glist const* glist, t_rect* bounds)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_getbounds(view, bounds);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
       eview_getbounds(gui->g_views[0], bounds);
    }
}

void egui_view_setposition(t_egui *gui, t_glist *glist, t_pt const* pos)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_setposition(view, pos);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_setposition(gui->g_views[0], pos);
    }
}

void egui_view_setsize(t_egui* gui, t_glist const* glist, t_pt const* size)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_setsize(view, size);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_setsize(gui->g_views[0], size);
    }
}

void egui_view_setbounds(t_egui* gui, t_glist const* glist, t_rect const* bounds)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_setbounds(view, bounds);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_setbounds(gui->g_views[0], bounds);
    }
}

void egui_view_select(t_egui *gui, t_glist *glist)
{
    int todo;
}

void egui_view_deselect(t_egui *gui, t_glist *glist)
{
    int todo;
}

void egui_view_setcursor(t_egui *gui, t_glist *glist, ebox_cursors cursor)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            eview_setcursor(view, cursor);
        }
    }
    else if(gui->g_nviews && gui->g_views)
    {
        eview_setcursor(gui->g_views[0], cursor);
    }
}

t_pd_err egui_view_invalidate_layer(t_egui *gui, t_glist *glist, t_symbol *name)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            return eview_layer_invalidate(view, name);
        }
        return -1;
    }
    else if(gui->g_nviews && gui->g_views)
    {
        return eview_layer_invalidate(gui->g_views[0], name);
    }
    return 0;
}

t_elayer* egui_view_start_layer(t_egui *gui, t_glist *glist, t_symbol *name, const float width, const float height)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            return eview_layer_start(view, name, width, height);
        }
        return NULL;
    }
    else if(gui->g_nviews && gui->g_views)
    {
        return eview_layer_start(gui->g_views[0], name, width, height);
    }
    return NULL;
}

t_pd_err egui_view_end_layer(t_egui *gui, t_glist *glist, t_symbol *name)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            return eview_layer_end(view, name);
        }
        return -1;
    }
    else if(gui->g_nviews && gui->g_views)
    {
        return eview_layer_end(gui->g_views[0], name);
    }
    return 0;
}

t_pd_err egui_view_paint_layer(t_egui *gui, t_glist *glist, t_symbol *name,
                          const float xoffset, const float yoffset)
{
    t_eview* view = NULL;
    if(glist)
    {
        view = egui_getview(gui, glist);
        if(view)
        {
            return eview_layer_paint(view, name, xoffset, yoffset);
        }
        return -1;
    }
    else if(gui->g_nviews && gui->g_views)
    {
        return eview_layer_paint(gui->g_views[0], name, xoffset, yoffset);
    }
    return 0;
}


void egui_view_remove(t_egui *gui, t_glist *glist)
{
    size_t i;
    t_eview ** temp;
    for(i = 0; i < gui->g_nviews; i++)
    {
        if(eview_getcanvas(gui->g_views[i]) == glist)
        {
            pd_free((t_pd *)gui->g_views[i]);
            for(i = i+1; i < gui->g_nviews; i++)
            {
                gui->g_views[i] = gui->g_views[i-1];
            }
            gui->g_nviews--;
            if(!gui->g_nviews)
            {
                free(gui->g_views);
            }
            else
            {
                temp = (t_eview **)realloc(gui->g_views, gui->g_nviews * sizeof(t_eview *));
                if(temp)
                {
                    gui->g_views = temp;
                }
                else
                {
                    pd_error(gui, "a strange error occurs while freeing memory.");
                }
            }
        }
    }
}

void egui_view_add(t_egui *gui, t_glist* glist)
{
    t_rect bounds;
    t_eview **temp = NULL;
    t_eview *view =  egui_getview(gui, glist);
    if(!view)
    {
        bounds.x = text_xpix((t_text *)gui->g_owner, glist);
        bounds.y = text_ypix((t_text *)gui->g_owner, glist);
        bounds.width = gui->g_size.x;
        bounds.height = gui->g_size.y;
        view = eview_new(gui->g_owner, glist, &bounds);
        if(view)
        {
            if(gui->g_nviews)
            {
                temp = (t_eview **)realloc(gui->g_views, sizeof(t_object *) * (size_t)(gui->g_nviews + 1));
                if(temp)
                {
                    gui->g_views[gui->g_nviews] = view;
                    gui->g_nviews++;
                    eview_redraw(view);
                }
                else
                {
                    pd_free((t_pd *)view);
                    pd_error(gui->g_owner, "can't register view for %s.", eobj_getclassname(gui->g_owner)->s_name);
                }
            }
            else
            {
                gui->g_views = (t_eview **)malloc(sizeof(t_object *));
                if(gui->g_views)
                {
                    gui->g_views[0] = view;
                    gui->g_nviews   = 1;
                    eview_redraw(view);
                }
                else
                {
                    pd_free((t_pd *)view);
                    pd_error(gui->g_owner, "can't register view for %s.", eobj_getclassname(gui->g_owner)->s_name);
                }
            }
        }
        else
        {
            pd_error(gui->g_owner, "can't create view for %s.", eobj_getclassname(gui->g_owner)->s_name);
        }
    }
    else
    {
        eview_redraw(view);
    }
}

void egui_redraw(t_egui* gui)
{
    size_t i = 0;
    for(i = 0; i < gui->g_nviews; i++)
    {
        eview_redraw(gui->g_views[i]);
    }
}






float egui_getwidth(t_egui const* gui)
{
    return gui->g_size.x;
}

float egui_getheight(t_egui const* gui)
{
    return gui->g_size.y;
}

char egui_ispinned(t_egui const* gui)
{
    return gui->g_pinned;
}

char egui_ignoreclick(t_egui const* gui)
{
    return gui->g_ignore_click;
}

t_symbol* egui_getreceive(t_egui const* gui)
{
    return gui->g_receive;
}

t_symbol* egui_getsend(t_egui const* gui)
{
    return gui->g_send;
}

long egui_getflags(t_egui const* gui)
{
    return gui->g_flags;
}

void egui_setreceive(t_egui *gui, t_symbol* s)
{
    if(is_valid_symbol(gui->g_receive))
    {
        pd_unbind((t_pd *)gui->g_owner, canvas_realizedollar(eobj_getcanvas(gui->g_owner), gui->g_receive));
    }
    gui->g_receive = s;
    if(is_valid_symbol(gui->g_receive))
    {
        pd_bind((t_pd *)gui->g_owner, canvas_realizedollar(eobj_getcanvas(gui->g_owner), gui->g_receive));
    }
}

void egui_setsend(t_egui *gui, t_symbol* s)
{
    gui->g_send = s;
}

void egui_setsize(t_egui *gui, float width, float height)
{
    if(gui->g_flags & EBOX_GROWLINK)
    {
        width  = (width > 4.f) ? width : 4.f;
        height = gui->g_size.y;
        gui->g_size.y += (width - gui->g_size.x);
        if(gui->g_size.y < 4)
        {
            gui->g_size.x += 4 - height;
            gui->g_size.y = 4;
        }
        else
        {
            gui->g_size.x =  width;
        }
        egui_view_setsize(gui, NULL, &gui->g_size);
    }
    else if(gui->g_flags & EBOX_GROWINDI)
    {
        gui->g_size.x = (width > 4.f) ? width : 4.f;
        gui->g_size.y = (height > 4.f) ? height : 4.f;
        egui_view_setsize(gui, NULL, &gui->g_size);
    }
}

void egui_setignoreclick(t_egui *gui, char state)
{
    gui->g_ignore_click = (state == 0) ? 0 : 1;
    int notify_views;
}

void egui_setpinned(t_egui *gui, char state)
{
    gui->g_pinned = (state == 0) ? 0 : 1;
    int notify_views;
}

static t_class* egui_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("egui1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("egui"), (t_newmethod)NULL, (t_method)egui_free, sizeof(t_egui), CLASS_PD, A_NULL, 0);
        if(c)
        {
            obj = pd_new(c);
            pd_bind(obj, gensym("egui1572"));
        }
        else
        {
            error("can't initialize flags class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

t_egui* egui_new(t_object* owner, long flags)
{
    t_egui*  x = NULL;
    t_class* c = egui_setup();
    if(c)
    {
        x = (t_egui *)pd_new(c);
        if(x)
        {
            x->g_owner              = owner;
            x->g_flags              = flags;
            x->g_receive            = s_cream_empty;
            x->g_send               = s_cream_empty;
            x->g_visible            = 1;
            x->g_ignore_click       = 0;
            x->g_views              = NULL;
            x->g_nviews             = 0;
        }
        else
        {
            pd_error(owner, "can't allocate dsp manager.");
        }
    }
    return x;
}

t_egui* egui_findbyname(t_symbol* name)
{
    t_class* c = egui_setup();
    if(c)
    {
        return (t_egui *)pd_findbyclass(name, c);
    }
    return NULL;
}







