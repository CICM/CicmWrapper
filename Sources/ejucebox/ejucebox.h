/*
// Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_JUCEBOX_WRAPPER
#define DEF_JUCEBOX_WRAPPER

extern "C" {
#include "../cicm_wrapper.h"
}
#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;
extern void initialiseMac(void);
extern void componentCreateWindow (Component* comp);

class PdJuceComponent : public Component
{
private:
    t_ebox*  m_box;
    
    Colour ecolor_to_jcolor(t_rgba color)
    {
        return Colour::fromFloatRGBA(color.red, color.green, color.blue, color.alpha);
    }
    
public:
    
	PdJuceComponent(t_ebox* owner) : Component()
    {
        m_box = owner;
        setBounds(100, 100, 300, 300);
        addToDesktop(ComponentPeer::StyleFlags::windowIgnoresMouseClicks | ComponentPeer::StyleFlags::windowIsTemporary);
        setEnabled(true);
        setAlwaysOnTop(true);
        setVisible (true);
        toFront(false);
        setOpaque(false);
    }
    
    void paint (Graphics& g)
    {
        Path region;
        region.addRectangle(0, 0, 300, 300);
        g.reduceClipRegion(region, AffineTransform::identity);
        g.setColour(ecolor_to_jcolor(m_box->b_boxparameters.d_boxfillcolor));
        g.fillAll();

        
        float bordersize = m_box->b_boxparameters.d_borderthickness;
        g.setColour(ecolor_to_jcolor(m_box->b_boxparameters.d_bordercolor));
        g.drawRect(bordersize, bordersize, getWidth() - bordersize, getHeight() - bordersize, bordersize);
    }
    
    ~PdJuceComponent()
    {
        
    }
};


void jucebox_initclass(t_eclass* c, method paint, long flags);
void jucebox_new(t_jucebox* x);
void jucebox_free(t_jucebox* x);
void jucebox_paint(t_jucebox* x, t_object *patcherview);
void jucebox_setsize(t_jucebox* x);

#endif