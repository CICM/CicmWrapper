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
#include "eproxy.h"
#include "ecommon.h"

#include "m_imp.h"

extern void edsp_initclass(t_eclass* c);
extern void eobj_initclass(t_eclass* c);

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

void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    int     defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);
    /*
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
     */
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







static t_edsp* eobj_getdsp(void *x)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxdsp", (unsigned long)x);
    return edsp_findbyname(gensym(text));
}

void eobj_dspsetup(void *x, size_t nins, size_t nouts)
{
    size_t i;
    char text[MAXPDSTRING];
    t_edsp* dsp = edsp_new(x, nins, nouts);
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


extern void eobj_initclass(t_eclass* c)
{
    char help[MAXPDSTRING];
    t_widgetbehavior widget;
    
    class_addmethod(c, (t_method)truemethod,         s_cream_iscicm,            A_CANT, 0);
    class_addmethod(c, (t_method)eobj_setproxyindex, gensym("setproxyindex"),   A_CANT, 0);
    class_addmethod(c, (t_method)eobj_popup,         gensym("dopopup"),         A_SYMBOL, A_FLOAT, 0);
    class_addmethod(c, (t_method)eobj_read,          gensym("doread"),          A_GIMME, 0);
    class_addmethod(c, (t_method)eobj_write,         gensym("dowrite"),         A_GIMME, 0);
    
    sprintf(help, "helps/%s", class_getname(c));
    class_sethelpsymbol((t_class *)c, gensym(help));
    class_setpropertiesfn(c, (t_propertiesfn)eobj_propertieswindow);
    class_setsavefn((t_class *)c, (t_savefn)eobj_save);
    class_setwidget((t_class *)c, (t_widgetbehavior *)&widget);
}

extern void edsp_initclass(t_eclass* c)
{
    class_addmethod(c, (t_method)truemethod,    s_cream_isdsp,          A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dsp,      gensym("dsp"),          A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dspadd,   gensym("dsp_add"),      A_CANT, 0);
    class_addmethod(c, (t_method)eobj_dspadd,   gensym("dsp_add64"),    A_CANT, 0);
}





