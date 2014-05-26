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
    
public:
	PdJuceComponent()
    {
        setBounds(100, 100, 300, 300);
        setAlwaysOnTop(1);
        setFocusContainer(1);
    }
    
    void paint (Graphics& g)
    {
        g.setColour(juce::Colours::green);
        g.drawEllipse(2, 2, getWidth() - 4, getWidth() - 4, 5);
    }
    
    void mouseMove(const MouseEvent &event)
    {
        post("%f %f", event.x, event.y);
    }
    
    void mouseDown(const MouseEvent &event)
    {
        post("%f %f", event.x, event.y);
    }
    
    void mouseDrag(const MouseEvent &event){};
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel){};
    
    ~PdJuceComponent()
    {
        
    }
};

typedef struct _jucebox
{
	t_eobj               j_box;
	PdJuceComponent*     j_component;
} t_jucebox;

void jucebox_initclass(t_eclass* c, method paint, long flags);
void jucebox_new(t_jucebox* x);
void jucebox_free(t_jucebox* x);
void jucebox_paint(t_jucebox* x, t_object *patcherview);
void jucebox_setsize(t_jucebox* x);

#endif