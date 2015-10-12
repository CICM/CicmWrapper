/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eobj.h"
#include "ecommon.h"
#include "egraphics.h"
#include "epopup.h"
#include "edsp.h"
#include "egui.h"
#include "eproxy.h"
#include "ecommon.h"
#include "eclass.h"

#include "m_imp.h"

extern void edsp_initclass(t_eclass* c);
extern void eobj_initclass(t_eclass* c);
extern void ebox_initclass(t_eclass* c);

void *eobj_new(t_eclass *c)
{
    t_eobj* x = (t_eobj *)pd_new(c);
    char buffer[MAXPDSTRING];
    if(x)
    {
        x->o_nproxy = 0;
        x->o_proxy = NULL;
        x->o_canvas = canvas_getcurrent();
        sprintf(buffer,"#%s%lx", class_getname(c), (long unsigned int)x);
        x->o_id = gensym(buffer);
        pd_bind(&x->o_obj.ob_pd, x->o_id);
        sprintf(buffer, "%ldcamo", (long unsigned int)x);
        x->o_listeners = gensym(buffer);
    }
    return (x);
}

void eobj_free(void *x)
{
    size_t i;
    t_eobj* z = (t_eobj *)x;
    ewindowprop_destroy(z);
    if(z->o_id)
    {
        pd_unbind((t_pd *)x, z->o_id);
        z->o_id = NULL;
    }
    if(z->o_proxy && z->o_nproxy)
    {
        for(i = 0; i < z->o_nproxy;i++)
        {
            pd_free((t_pd *)z->o_proxy[i]);
        }
        z->o_proxy = NULL;
        z->o_nproxy= 0;
    }
}

char eobj_iscicm(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_iscicm);
}

char eobj_isgui(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_isgui);
}

char eobj_isdsp(void const* x)
{
    return (char)zgetfn((t_pd *)x, s_cream_isdsp);
}

void eobj_proxynew(void* x, ...)
{
    va_list ap;
    t_symbol* type;
    t_eproxy** temp = NULL;
    t_eproxy* proxy = NULL;
    t_eobj* z       = (t_eobj *)x;
    va_start(ap, x);
    type = va_arg(ap, t_symbol*);
    va_end(ap);
    
    proxy = eproxy_new(x, type, z->o_nproxy);
    if(proxy)
    {
        if(z->o_proxy && z->o_nproxy)
        {
            temp = (t_eproxy **)realloc(z->o_proxy, (size_t)(z->o_nproxy + 1) * sizeof(t_eproxy *));
        }
        else
        {
            temp = (t_eproxy **)malloc(1 * sizeof(t_eproxy *));
        }
        if(temp)
        {
            z->o_proxy = (t_object **)temp;
            z->o_proxy[z->o_nproxy] = (t_object *)proxy;
            z->o_nproxy++;
        }
        else
        {
            pd_error(z, "can't allocate memory for a new proxy inlet.");
        }
    }
}

void eobj_proxyfree(void* x)
{
    t_eobj *z   = (t_eobj *)x;
    t_eproxy ** temp;
    if(z->o_proxy && z->o_nproxy)
    {
        pd_free((t_pd *)z->o_proxy[z->o_nproxy-1]);
        z->o_nproxy--;
        if(z->o_nproxy)
        {
            temp = (t_eproxy **)realloc(z->o_proxy, (size_t)(z->o_nproxy) * sizeof(t_eproxy *));
            if(temp)
            {
                z->o_proxy = (t_object **)temp;
            }
            else
            {
                pd_error(z, "a proxy hasn't been freed.");
            }
        }
        else
        {
            free(z->o_proxy);
            z->o_proxy = NULL;
        }
    }
    else
    {
        pd_error(x, "can't free a proxy if there isn't one dude.");
    }
}

size_t eobj_getproxy(void const* x)
{
    return ((t_eobj const*)x)->o_cproxy;
}

t_eclass* eobj_getclass(void const* x)
{
    return (t_eclass *)((t_gobj const*)x)->g_pd;
}

t_symbol* eobj_getclassname(void const* x)
{
    return gensym(class_getname(eobj_getclass(x)));
}

t_canvas* eobj_getcanvas(void const* x)
{
    return ((t_eobj const*)x)->o_canvas;
}

t_symbol* eobj_getid(void const* x)
{
    return ((t_eobj const*)x)->o_id;
}

void eobj_bind(void const* b, void* const l)
{
    if(getfn((t_pd *)l, s_cream_notify))
    {
        pd_bind((t_pd *)l, ((t_eobj const*)b)->o_listeners);
    }
}

void eobj_unbind(void const* b, void* const l)
{
    if(getfn((t_pd *)l, s_cream_notify))
    {
        pd_unbind((t_pd *)l, ((t_eobj const*)b)->o_listeners);
    }
}

static void eobj_setproxyindex(t_eobj *x, size_t index)
{
    x->o_cproxy = index;
}

static void eobj_save(t_gobj* x, t_binbuf *b)
{
    t_binbuf_method m;
    binbuf_addv(b, (char *)"ssii", &s__X, s_cream_obj, (t_int)((t_text *)x)->te_xpix, (t_int)((t_text *)x)->te_ypix);
    if(eobj_isgui(x))
    {
        int important_todo;
        /*
        binbuf_addv(b, (char *)"s", eobj_getclassname(x));
        for(i = 0; i < c->c_nattr; i++)
        {
            if(c->c_attr[i] && c->c_attr[i]->save && c->c_attr[i]->name)
            {
                eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
                if(argc && argv)
                {
                    if(!(argc == 1 && atom_gettype(argv) == A_SYMBOL && !is_valid_symbol(atom_getsymbol(argv))))
                    {
                        snprintf(buffer, MAXPDSTRING, "@%s", c->c_attr[i]->name->s_name);
                        binbuf_append_attribute(b, gensym(buffer), argc, argv);
                    }
                    argc = 0;
                    free(argv);
                    argv = NULL;
                }
            }
        }
        int parameter_save_todo_later;
        
        argv = (t_atom *)malloc(3 * sizeof(t_atom));
        if(argv)
        {
            for(i = 0; i < y->b_nparams; i++)
            {
                if(y->b_params[i])
                {
                    snprintf(buffer, MAXPDSTRING, "@param%i", i);
                    atom_setsym(argv, y->b_params[i]->p_name);
                    atom_setsym(argv+1, y->b_params[i]->p_label);
                    atom_setfloat(argv+2, y->b_params[i]->p_index);
                    binbuf_append_attribute(b, gensym(buffer), 3, argv);
                }
            }
        }
        */
    }
    else
    {
        binbuf_addbinbuf(b, ((t_text *)x)->te_binbuf);
    }
    
    m = (t_binbuf_method)zgetfn((t_pd *)x, gensym("save"));
    if(m)
    {
        (m)(x, b);
    }
    binbuf_addv(b, (char *)";");
}

extern void eobj_popup(t_eobj* x, t_symbol* s, float itemid)
{
    t_epopup* popup;
    t_eclass* c = eobj_getclass(x);
    int todo;
    /*
     if(s && c->c_widget.w_popup)
     {
     popup = epopupmenu_getfromsymbol(s);
     if(popup)
     {
     c->c_widget.w_popup(x, popup, (long)itemid);
     }
     }
     */
}

extern void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);
    int todo;
    /*
    // The file name is defined
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
        // The folder seems defined
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsymbol(argv));
            if(c->c_widget.w_write)
            {
                c->c_widget.w_write(x, s, 1, av);
            }
        }
        // The folder isn't defined so write it in the canvas folder
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_write)
            {
                c->c_widget.w_write(x, s, 1, av);
            }
        }
    }
    // The file name is not defined so we popup a window
    else
    {
        sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
    }
     */
}

extern void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_namelist* var;
    t_eclass* c = eobj_getclass(x);
    int todo;
    /*
    // Name
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        // Valid path
        if((access(atom_getsymbol(argv)->s_name, O_RDONLY) != -1))
        {
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, argv);
        }
        // Invalid path or no path
        else
        {
            // Wrong path but we don't care
            pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
            if(pch != NULL)
            {
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, argv);
            }
            else
            {
                // Look in the canvas folder
                sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
                if((access(buf, O_RDONLY) != -1))
                {
                    atom_setsym(av, gensym(buf));
                    if(c->c_widget.w_read)
                        c->c_widget.w_read(x, s, 1, av);
                    return;
                }
                // Look in the search path
                var = sys_searchpath;
                while (var)
                {
                    sprintf(buf, "%s/%s", var->nl_string, atom_getsymbol(argv)->s_name);
                    if((access(buf, O_RDONLY) != -1))
                    {
                        atom_setsym(av, gensym(buf));
                        if(c->c_widget.w_read)
                            c->c_widget.w_read(x, s, 1, av);
                        return;
                    }
                    var = var->nl_next;
                }
                // Nothing work but we don't care
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, av);
                return;
            }
        }
    }
    // No name so we popup a window
    else
    {
        sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
    }
     */
}

static t_pd_err eobj_attr_dosetvalue(t_eobj *x, t_eattr* attr, int argc, t_atom* argv)
{
    int use_static_symbols;
    mess3((t_pd *)attr, gensym("setvalue"), x, (void *)argc, argv);
    int todo_important;
    //ebox_notify(z, s, s_cream_attr_modified, NULL, NULL);
    /*
     if(c->c_widget.w_notify != NULL)
     {
     c->c_widget.w_notify(x, s, s_cream_attr_modified, NULL, NULL);
     }
     */
    if(eattr_repaint(attr))
    {
        int todo_important;
        /*
         if(c->c_widget.w_oksize != NULL)
         {
         c->c_widget.w_oksize(x, &z->b_rect);
         }
         if(c->c_widget.w_getdrawparameters != NULL)
         {
         c->c_widget.w_getdrawparameters(x, NULL, &params);
         if(!rgba_is_equal(&(params.d_bordercolor), &(z->b_boxparameters.d_bordercolor)))
         {
         memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
         ebox_invalidate_layer((t_ebox *)x, s_cream_eboxbd);
         }
         else if(params.d_borderthickness != z->b_boxparameters.d_borderthickness)
         {
         memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
         ebox_notify(z, s_cream_size, s_cream_size, NULL, NULL);
         }
         else
         {
         memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
         }
         }
         ebox_redraw(z);
         */
    }
    if(eattr_issaved(attr) && eobj_isgui(x) && !eobj_getcanvas(x)->gl_loading)
    {
        canvas_dirty(eobj_getcanvas(x), 1);
    }
    if(eattr_isvisible(attr))
    {
        ewindowprop_update((t_eobj *)x);
    }
}

static t_pd_err eobj_attr_setvalue(t_eobj *x, t_symbol* s, int argc, t_atom* argv)
{
    t_eattr* attr = eclass_getattr(eobj_getclass(x), s);
    if(attr)
    {
        eobj_attr_dosetvalue(x, attr, argc, argv);
    }
    return 0;
}

void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    size_t  i;
    int       defc        = 0;
    t_atom*   defv        = NULL;
    size_t    nattrs;
    t_eattr** attrs;
    eclass_getattrs(eobj_getclass(x), &nattrs, &attrs);
    if(attrs && nattrs)
    {
        for(i = 0; i < nattrs; i++)
        {
            atoms_get_attribute(argc, argv, eattr_getname(attrs[i]), &defc, &defv);
            if(defc && defv)
            {
                eobj_attr_dosetvalue(x, attrs[i], defc, defv);
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
        free(attrs);
    }
    int later_manage_the_parameters;
}

void eobj_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    eobj_attrprocess_viatoms(x, binbuf_getnatom(d), binbuf_getvec(d));
}

static void eobj_propertieswindow(t_eobj* x, t_glist *glist)
{
    ewindowprop_create(x);
}

void eobj_attr_setvalueof(void *x, t_symbol* s, int argc, t_atom *argv)
{
    t_typ_method setvalue = (t_typ_method)getfn((t_pd *)x, s);
    if(setvalue)
    {
        setvalue(x, s, argc, argv);
    }
}

void eobj_attr_getvalueof(void *x, t_symbol *s, int *argc, t_atom **argv)
{
    char realname[MAXPDSTRING];
    t_typ_method getvalue = NULL;
    sprintf(realname, "get%s", s->s_name);
    argc[0] = 0;
    argv[0] = NULL;
    getvalue = (t_typ_method)getfn((t_pd *)x, gensym(realname));
    if(getvalue)
    {
        getvalue(x, s, argc, argv);
    }
}







static t_edsp* eobj_getdsp(void const* x)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxdsp", (unsigned long)x);
    return edsp_findbyname(gensym(text));
}

void eobj_dspsetup(void *x, size_t nins, size_t nouts)
{
    size_t i;
    char text[MAXPDSTRING];
    t_edsp* dsp = edsp_new(x);
    if(dsp)
    {
        sprintf(text, "%lxdsp", (unsigned long)x);
        pd_bind((t_pd *)dsp, gensym(text));
    }
    for(i = (size_t)obj_nsigoutlets((t_object *)x); i < nouts; i++)
    {
        outlet_new((t_object *)x, &s_signal);
    }
    for(i = (size_t)obj_nsiginlets((t_object *)x); i < nins; i++)
    {
        eproxy_new((t_object *)x, &s_signal, i);
    }
}

void eobj_dspflags(void *x, long flags)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        mess1((t_pd *)dsp, gensym("flags"), (void *)flags);
    }
}

void eobj_dspfree(void *x)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        pd_free((t_pd *)dsp);
    }
    eobj_free(x);
}

static void eobj_dsp(void *x, t_signal **sp)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        mess1((t_pd *)dsp, gensym("prepare"), sp);
    }
}

static void eobj_dspadd(void *x, t_symbol* s, t_object* za, t_typ_method m, long flags, void *userparam)
{
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        mess3((t_pd *)dsp, gensym("add"), m, (void *)(flags), (userparam));
    }
}

t_sample* eobj_dspgetinsamples(void *x, size_t index)
{
    t_ret_method m;
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        m = (t_ret_method)zgetfn((t_pd *)dsp, gensym("insamples"));
        if(m)
        {
            return (t_sample *)((m)((t_pd *)dsp, index));
        }
    }
    return NULL;
}

t_sample* eobj_dspgetoutsamples(void *x, size_t index)
{
    t_ret_method m;
    t_edsp* dsp = eobj_getdsp(x);
    if(dsp)
    {
        m = (t_ret_method)zgetfn((t_pd *)dsp, gensym("outsamples"));
        if(m)
        {
            return (t_sample *)((m)((t_pd *)dsp, index));
        }
    }
    return NULL;
}









static t_egui* eobj_getgui(void const* x)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxgui", (unsigned long)x);
    return egui_findbyname(gensym(text));
}

void ebox_new(t_ebox *x, long flags)
{
    size_t    i, nattrs;
    t_eattr** attrs;
    char text[MAXPDSTRING];
    t_egui* gui = egui_new((t_object *)x, flags);
    if(gui)
    {
        sprintf(text, "%lxdsp", (unsigned long)x);
        pd_bind((t_pd *)gui, gensym(text));
    }
    eclass_getattrs(eobj_getclass(x), &nattrs, &attrs);
    if(attrs && nattrs)
    {
        for(i = 0; i < nattrs; i++)
        {
            mess1((t_pd *)attrs[0], gensym("setvaluedefault"), x);
        }
        free(attrs);
    }
    int later_manage_parameter_construction;
}

void ebox_ready(t_ebox *x)
{
    
}

void ebox_free(t_ebox* x)
{
    t_egui* gui = eobj_getgui(x);
    if(gui)
    {
        pd_free((t_pd *)gui);
    }
    eobj_isdsp(x) ? eobj_dspfree(x) : eobj_free(x);
    
    int later_manage_parameter_destruction;
}

t_pd* ebox_getsender(t_ebox* x)
{
    t_egui const* gui = eobj_getgui(x);
    if(gui && is_valid_symbol(egui_getreceive_symbol(gui)))
    {
        return canvas_realizedollar(eobj_getcanvas(x), egui_getreceive_symbol(gui))->s_thing;
    }
    return NULL;
}

long ebox_getflags(t_ebox* x)
{
    t_egui const* gui = eobj_getgui(x);
    if(gui && is_valid_symbol(egui_getreceive_symbol(gui)))
    {
        return egui_getflags(gui);
    }
    return 0;
}

static t_pd_err ebox_defaultattibutes_set(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    t_egui* gui = eobj_getgui(x);
    if(gui)
    {
        pd_typedmess((t_pd *)gui, eattr_getname((t_eattr *)attr), argc, argv);
        return 0;
    }
    return -1;
}

static t_pd_err ebox_defaultattibutes_get(t_ebox *x, t_object *attr, int* argc, t_atom **argv)
{
    t_egui* gui = eobj_getgui(x);
    if(gui)
    {
        int todo;
    }
    return -1;
}

static void ebox_wgetrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_egui const* gui = eobj_getgui(z);
    *xp1 = text_xpix((t_text *)z, glist);
    *yp1 = text_ypix((t_text *)z, glist);
    if(gui)
    {
        int todo;
    }
    else
    {
        *xp2 = *xp1 + (int)((t_text *)z)->te_width;
        *yp2 = *yp1 + (int)10;
    }
}

void ebox_wvis(t_gobj *z, t_glist *glist, int vis)
{
    long i;
    t_ebox* x   = (t_ebox *)z;
    t_object *view = NULL, **temp;
    /*
    if(vis)
    {
        view = eview_create(x, glist);
        if(view)
        {
            for(i = 0; i < x->b_nviews; i++)
            {
                if(x->b_views[i] == view)
                {
                    return;
                }
            }
            
            if(x->b_nviews)
            {
                temp = (t_object **)realloc(x->b_views, sizeof(t_object *) * (size_t)(x->b_nviews + 1));
                if(temp)
                {
                    x->b_views[x->b_nviews] = view;
                    x->b_nviews++;
                }
                else
                {
                    eview_destroy(view);
                    pd_error(x, "can't register view for %s.", eobj_getclassname(x)->s_name);
                }
            }
            else
            {
                x->b_views = (t_object **)malloc(sizeof(t_object *));
                if(x->b_views)
                {
                    x->b_views[0] = view;
                    x->b_nviews   = 1;
                }
                else
                {
                    eview_destroy(view);
                    pd_error(x, "can't register view for %s.", eobj_getclassname(x)->s_name);
                }
            }
        }
        else
        {
            pd_error(x, "can't create view for %s.", eobj_getclassname(x)->s_name);
        }
    }
    else
    {
        ebox_erase(x);
        canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
    }
     */
}

//! Widget
void ebox_wdisplace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    /*
    t_object* view = eview_create((t_ebox *)z, glist);
    if(view)
    {
        pd_symbol((t_pd *)view, s_cream_changes);
    }
     */
}

//! Widget
void ebox_wselect(t_gobj *z, t_glist *glist, int selected)
{
    /*
    t_ebox *x = (t_ebox *)z;
    x->b_selected_box = selected ? 1 : 0;
    ebox_select(x);
     */
}

//! Widget
void ebox_wdelete(t_gobj *z, t_glist *glist)
{
    /*
    t_ebox *x = (t_ebox *)z;
    ebox_erase(x);
    canvas_deletelinesfor(glist, (t_text *)z);
     */
}





extern void eobj_initclass(t_eclass* c)
{
    char help[MAXPDSTRING];
    class_addmethod(c, (t_method)truemethod,         s_cream_iscicm,            A_CANT, 0);
    class_addmethod(c, (t_method)eobj_setproxyindex, gensym("setproxyindex"),   A_CANT, 0);
    class_addmethod(c, (t_method)eobj_popup,         gensym("dopopup"),         A_SYMBOL, A_FLOAT, 0);
    class_addmethod(c, (t_method)eobj_read,          gensym("doread"),          A_GIMME, 0);
    class_addmethod(c, (t_method)eobj_write,         gensym("dowrite"),         A_GIMME, 0);
    
    sprintf(help, "helps/%s", class_getname(c));
    class_sethelpsymbol((t_class *)c, gensym(help));
    class_setpropertiesfn(c, (t_propertiesfn)eobj_propertieswindow);
    class_setsavefn((t_class *)c, (t_savefn)eobj_save);
}

extern void edsp_initclass(t_eclass* c)
{
    class_addmethod(c, (t_method)truemethod,    s_cream_isdsp,          A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dsp,      gensym("dsp"),          A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dspadd,   gensym("dsp_add"),      A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dspadd,   gensym("dsp_add64"),    A_CANT, 0);
}

extern void ebox_initclass(t_eclass* c)
{
    t_widgetbehavior widget;
    widget.w_visfn      = ebox_wvis;
    widget.w_getrectfn  = ebox_wgetrect;
    widget.w_selectfn   = ebox_wselect;
    widget.w_deletefn   = ebox_wdelete;
    widget.w_displacefn = ebox_wdisplace;
    
    CLASS_ATTR_FLOAT_ARRAY  (c, "size", 0, t_ebox, o_dummy, 2);
    CLASS_ATTR_DEFAULT      (c, "size", 0, "100. 100.");
    CLASS_ATTR_FILTER_MIN   (c, "size", 4);
    CLASS_ATTR_SAVE         (c, "size", 0);
    CLASS_ATTR_PAINT        (c, "size", 0);
    CLASS_ATTR_CATEGORY		(c, "size", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "size", 0, "Patching Size");
    CLASS_ATTR_ACCESSORS    (c, "size", ebox_defaultattibutes_get, ebox_defaultattibutes_set);
    
    CLASS_ATTR_CHAR         (c, "pinned", 0, t_ebox, o_dummy);
    CLASS_ATTR_DEFAULT      (c, "pinned", 0, "0");
    CLASS_ATTR_FILTER_CLIP  (c, "pinned", 0, 1);
    CLASS_ATTR_SAVE         (c, "pinned", 0);
    CLASS_ATTR_CATEGORY		(c, "pinned", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "pinned", 0, "Pinned");
    CLASS_ATTR_STYLE        (c, "pinned", 0, "onoff");
    CLASS_ATTR_ACCESSORS    (c, "pinned", ebox_defaultattibutes_get, ebox_defaultattibutes_set);
    
    if(!(eclass_getflags(c) & EBOX_IGNORELOCKCLICK))
    {
        CLASS_ATTR_CHAR         (c, "ignoreclick", 0, t_ebox, o_dummy);
        CLASS_ATTR_DEFAULT      (c, "ignoreclick", 0, "0");
        CLASS_ATTR_FILTER_CLIP  (c, "ignoreclick", 0, 1);
        CLASS_ATTR_SAVE         (c, "ignoreclick", 0);
        CLASS_ATTR_CATEGORY		(c, "ignoreclick", 0, "Basic");
        CLASS_ATTR_LABEL		(c, "ignoreclick", 0, "Ignore Click");
        CLASS_ATTR_STYLE        (c, "ignoreclick", 0, "onoff");
        CLASS_ATTR_ACCESSORS    (c, "ignoreclick", ebox_defaultattibutes_get, ebox_defaultattibutes_set);
    }
    
    CLASS_ATTR_SYMBOL       (c, "receive", 0, t_ebox, o_dummy);
    CLASS_ATTR_DEFAULT      (c, "receive", 0, "");
    CLASS_ATTR_SAVE         (c, "receive", 0);
    CLASS_ATTR_CATEGORY		(c, "receive", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "receive", 0, "Receive Symbol");
    CLASS_ATTR_ACCESSORS    (c, "receive", ebox_defaultattibutes_get, ebox_defaultattibutes_set);
    
    CLASS_ATTR_SYMBOL       (c, "send", 0, t_ebox, o_dummy);
    CLASS_ATTR_DEFAULT      (c, "send", 0, "");
    CLASS_ATTR_SAVE         (c, "send", 0);
    CLASS_ATTR_CATEGORY		(c, "send", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "send", 0, "Send Symbol");
    CLASS_ATTR_ACCESSORS    (c, "send", ebox_defaultattibutes_get, ebox_defaultattibutes_set);
    
    class_setwidget((t_class *)c, (t_widgetbehavior *)&widget);
}





