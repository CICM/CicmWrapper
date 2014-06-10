/*
 // Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ejucebox.h"

#ifdef JUCE_WRAPPER

PdJuceComponent::PdJuceComponent(t_ebox* owner) : Component()
{
    m_box = owner;
    
    setEnabled(true);
    toFront(false);
    setOpaque(true);
    setVisible(0);
    setBounds(-100, -100, 100, 100);
}

void PdJuceComponent::paint(Graphics& g)
{
    t_rgba bgcolor = m_box->b_boxparameters.d_boxfillcolor;
    g.setColour(Colour::fromFloatRGBA(bgcolor.red, bgcolor.green, bgcolor.blue, 1));
    g.fillAll();
    
    if(eobj_getclass(m_box)->c_widget.w_paint_juce)
        eobj_getclass(m_box)->c_widget.w_paint_juce(m_box, &g);
}

PdJuceComponent::~PdJuceComponent()
{
    
}

void ejucebox_initclass(t_eclass* c, long flags)
{
    eclass_init((t_eclass *)c, flags);
    eclass_addmethod(c, (method) ejucebox_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) ejucebox_notify,          "notify",           A_CANT, 0);
    c->c_juce = 1;
    
    initialiseMac();
    initialiseJuce_GUI();
}

void ejucebox_new(t_jucebox* x, long flags)
{
    ebox_new((t_ebox *)x, flags);
    x->j_component = new PdJuceComponent((t_ebox *)x);
    x->j_component->addToDesktop(ComponentPeer::StyleFlags::windowIsTemporary);
}

void ejucebox_free(t_jucebox* x)
{
    ebox_free((t_ebox *)x);
	x->j_component->removeFromDesktop();
    delete x->j_component;
}

static char JuceColorBuffer[10];

static char JuceHexDigits[] = "0123456789ABCDEF";

char* jucecolor_to_hex(Colour color)
{
    int r = color.getRed();
    int g = color.getGreen();
    int b = color.getBlue();
    JuceColorBuffer[0] = '#';
    JuceColorBuffer[1] = JuceHexDigits[(r >> 4) & 15];
    JuceColorBuffer[2] = JuceHexDigits[r & 15];
    JuceColorBuffer[3] = JuceHexDigits[(g >> 4) & 15];
    JuceColorBuffer[4] = JuceHexDigits[g & 15];
    JuceColorBuffer[5] = JuceHexDigits[(b >> 4) & 15];
    JuceColorBuffer[6] = JuceHexDigits[b & 15];
    JuceColorBuffer[7] = '\0';
    return &JuceColorBuffer[0];
}

t_pd_err ejucebox_notify(t_jucebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("size") && x->j_component)
		{
            x->j_component->setBounds(-x->j_box.b_rect.width, -x->j_box.b_rect.height, x->j_box.b_rect.width, x->j_box.b_rect.height);
		}
	}
    
    if(eobj_getclass(x)->c_widget.w_notify_juce)
        eobj_getclass(x)->c_widget.w_notify_juce(x, s, msg, sender, data);

	return 0;
}

void ejucebox_paint(t_jucebox* x, t_object *patcherview)
{
    t_rect rect;
    int offset = x->j_box.b_boxparameters.d_borderthickness;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    x->j_component->setVisible(1);
    if(x->j_component->isOnDesktop())
    {
        x->j_component->repaint();
        
        juce::Image openGLSnap = x->j_component->createComponentSnapshot(Rectangle<int>(rect.width, rect.height));
        sys_gui("set pixels { ");
        for(int i = 0; i < rect.height; i++)
        {
            sys_gui("{");
            for(int j = 0; j < rect.width; j++)
            {
                sys_vgui("%s ", jucecolor_to_hex(openGLSnap.getPixelAt(j, i)));
            }
            sys_gui("} ");
        }
        sys_gui("}\n");
        
        sys_gui("set photo [image create photo]\n");
        sys_gui("$photo put $pixels\n");
        sys_vgui("%s delete %snopen\n", x->j_box.b_drawing_id->s_name, x->j_box.b_all_id->s_name);
        sys_vgui("%s create image %i %i -anchor nw -image $photo -tags %snopen\n", x->j_box.b_drawing_id->s_name, offset, offset, x->j_box.b_all_id->s_name);
     
    }
    x->j_component->setVisible(0);
    
}

#endif