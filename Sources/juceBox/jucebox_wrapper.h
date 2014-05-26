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
//#include "../JuceModules/juce_audio_plugin_client/utility/juce_FakeMouseMoveGenerator.h"

using namespace juce;
extern void initialiseMac(void);
extern void componentCreateWindow (Component* comp);

class PdJuceComponent : public Component, public Timer
{
private:
    t_eobj*  m_box;
    t_clock* m_clock;
    t_method m_callback;
public:
	PdJuceComponent() : Component()
    {
        setBounds(100, 100, 300, 300);
        setOpaque(false);
        addToDesktop(ComponentPeer::StyleFlags::windowIsTemporary);
        setEnabled(true);
        setAlwaysOnTop(true);
        setVisible (true);
        toFront(false);
        componentCreateWindow(this);
        startTimer(100);
        m_clock = clock_new((void *)m_box, m_callback);
    }
    
    void paint (Graphics& g)
    {
        g.setColour(juce::Colours::green);
        g.drawEllipse(2, 2, getWidth() - 4, getWidth() - 4, 5);
    }
    
    void mouseEnter(const MouseEvent &event)
    {
        post("%f %f", event.x, event.y);
    }
    
    void mouseMove(const MouseEvent &event)
    {
        post("%f %f", event.x, event.y);
    }
    
    void mouseDown(const MouseEvent &event)
    {
        post("%f %f", event.x, event.y);
    }
    
    void timerCallback()
    {
        post("zaza");
    }
    
    void mouseDrag(const MouseEvent &event){};
    void mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel){};
    
    ~PdJuceComponent()
    {
        
    }
};

typedef struct _jucebox
{
	t_ebox               j_box;
	PdJuceComponent*     j_component;
} t_jucebox;

void jucebox_initclass(t_eclass* c, method paint, long flags);
void jucebox_new(t_jucebox* x);
void jucebox_free(t_jucebox* x);
void jucebox_paint(t_jucebox* x, t_object *patcherview);
void jucebox_setsize(t_jucebox* x);

#endif