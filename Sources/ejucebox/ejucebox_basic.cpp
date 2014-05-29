/*
 // Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ejucebox.h"

#ifdef JUCE_WRAPPER

#define WRAPPER_X_INC 22
#define WRAPPER_Y_INC 85

Colour PdJuceComponent::ecolor_to_jcolor(t_rgba color)
{
    return Colour::fromFloatRGBA(color.red, color.green, color.blue, color.alpha);
}

PdJuceComponent::PdJuceComponent(t_ebox* owner) : Component()
{
    m_box = owner;
    addToDesktop(ComponentPeer::StyleFlags::windowIgnoresMouseClicks | ComponentPeer::StyleFlags::windowIsTemporary);
    setEnabled(true);
    toFront(false);
    setOpaque(false);
}

void PdJuceComponent::paint(Graphics& g)
{
    Path region;
    region.addRectangle(0, 0, 300, 300);
    g.reduceClipRegion(region, AffineTransform::identity);
    g.setColour(ecolor_to_jcolor(m_box->b_boxparameters.d_boxfillcolor));
    g.fillAll();
    
    
    float bordersize = m_box->b_boxparameters.d_borderthickness;
    if(m_box->b_selected_box)
        g.setColour(Colours::blue);
    else
        g.setColour(ecolor_to_jcolor(m_box->b_boxparameters.d_bordercolor));
    g.drawRect(bordersize, bordersize, getWidth() - bordersize, getHeight() - bordersize, bordersize);
    post("paint");
}

PdJuceComponent::~PdJuceComponent()
{
    
}

void ejucebox_initclass(t_eclass* c, method paint, long flags)
{
    eclass_init((t_eclass *)c, flags);
    c->c_juce  = 1;
    
    // Widget redefinition
    c->c_widget.w_visfn             = ejucewidget_vis;
    c->c_widget.w_displacefn        = ejucewidget_displace;
    c->c_widget.w_selectfn          = ejucewidget_select;
    c->c_widget.w_activatefn        = NULL;
    c->c_widget.w_deletefn          = ejucewidget_delete;
    c->c_widget.w_clickfn           = NULL;
    c->c_widget.w_paint             = (method)ejucebox_paint;
    eclass_addmethod((t_eclass *)c, (method)paint, "jucebox_paint", A_CANT, 0);
    
    initialiseMac();
}

void ejucebox_new(t_jucebox* x)
{
    JUCE_AUTORELEASEPOOL
    {
        x->j_component = new PdJuceComponent((t_ebox *)x);
        x->j_component->setBounds(x->j_box.b_rect.x + eobj_getcanvas(x)->gl_screenx1, x->j_box.b_rect.y + eobj_getcanvas(x)->gl_screeny1, x->j_box.b_rect.width, x->j_box.b_rect.height);
    
        initialiseJuce_GUI();
    }
}

void ejucebox_free(t_jucebox* x)
{
    ebox_free((t_ebox *)x);
	x->j_component->removeFromDesktop();
    delete x->j_component;
}

void ejucebox_change(t_jucebox* x, void* wm)
{
    x->j_component->removeFromDesktop();
    x->j_component->setEnabled(false);
    x->j_component->toFront(false);
    x->j_component->setOpaque(false);
    x->j_component->addToDesktop(0, wm);
}

void ejucebox_paint(t_jucebox* x, t_object *patcherview)
{
    x->j_component->repaint();
}

void ejucewidget_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_jucebox* x   = (t_jucebox *)z;
    if(vis)
    {
        x->j_component->setBounds(x->j_box.b_obj.o_obj.te_xpix + eobj_getcanvas(x)->gl_screenx1 + WRAPPER_X_INC, x->j_box.b_obj.o_obj.te_ypix + eobj_getcanvas(x)->gl_screeny1 + WRAPPER_Y_INC, x->j_box.b_rect.width, x->j_box.b_rect.height);
        x->j_component->setVisible(true);
        x->j_component->setAlwaysOnTop(true);
        sys_vgui("pdsend \"%s window [winfo id .x%lx]\"\n", x->j_box.b_obj.o_id->s_name, (long unsigned int)glist);
    }
    else
    {
        x->j_component->setVisible(false);
        x->j_component->toFront(false);
    }
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ejucewidget_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
	t_jucebox *x;
	x = (t_jucebox *)z;
    
    x->j_box.b_rect.x += dx;
    x->j_box.b_rect.y += dy;
    x->j_box.b_obj.o_obj.te_xpix += dx;
    x->j_box.b_obj.o_obj.te_ypix += dy;
    
    x->j_component->setBounds(x->j_box.b_obj.o_obj.te_xpix + eobj_getcanvas(x)->gl_screenx1 + WRAPPER_X_INC, x->j_box.b_obj.o_obj.te_ypix + eobj_getcanvas(x)->gl_screeny1 + WRAPPER_Y_INC, x->j_box.b_rect.width, x->j_box.b_rect.height);
    
    sys_vgui("pdsend \"%s window [winfo id .x%lx]\"\n", x->j_box.b_obj.o_id->s_name, (long unsigned int)glist);
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ejucewidget_select(t_gobj *z, t_glist *glist, int selected)
{
    t_jucebox *x = (t_jucebox *)z;
    x->j_box.b_selected_box = selected;
    x->j_component->repaint();
}

void ejucewidget_delete(t_gobj *z, t_glist *glist)
{
    t_jucebox *x = (t_jucebox *)z;
    delete [] x->j_component;
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}



#endif