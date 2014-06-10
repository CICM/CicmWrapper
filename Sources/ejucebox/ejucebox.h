/*
// Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_JUCEBOX_WRAPPER
#define DEF_JUCEBOX_WRAPPER

#ifdef JUCE_WRAPPER

extern "C" {
#include "../epd.h"
#include "../ecommon/ecommon.h"
#include "../epd_max.h"
}

#include <JuceHeader.h>

using namespace juce;
extern void initialiseMac(void);

#include "../JuceLibraryCode/JuceHeader.h"
#include <OpenGL/gl.h>

class PdJuceComponent : public Component
{
private:
    t_ebox*  m_box;
public:
    
	PdJuceComponent(t_ebox* owner);
    void paint (Graphics& g);
    ~PdJuceComponent();
};

typedef struct _jucebox
{
	t_ebox              j_box;
	PdJuceComponent*    j_component;
} t_jucebox;

typedef struct _dspjucebox
{
	t_edspbox           j_box;
	PdJuceComponent*    j_component;
} t_dspjucebox;

void ejucebox_new(t_jucebox* x, long flags);
void ejucebox_initclass(t_eclass* c, long flags);
void ejucebox_free(t_jucebox* x);
void ejucebox_paint(t_jucebox* x, t_object *patcherview);
t_pd_err ejucebox_notify(t_jucebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

class PdOpenGlComponent : public juce::Component, public OpenGLRenderer
{
private:
    OpenGLContext*      m_context;
    Image*              m_image;
    OpenGLFrameBuffer*  m_buffer;
	float m_scale;
public:
	PdOpenGlComponent();
	Image makeScreenshot(t_object* x, double width, double height);
	
    void setActive(){ m_context->makeActive();}
    inline char isActive() const { return (char)m_context->isActive(); }
    inline char isContextOk() const { return (char)(m_context == m_context->getCurrentContext()); }
	
	void newOpenGLContextCreated(){};
    void renderOpenGL(){m_scale = m_context->getRenderingScale();};
	void openGLContextClosing(){};
    void setBounds (const int x, const int y, int w, int h);
    ~PdOpenGlComponent();
};

typedef struct _openglbox
{
	t_ebox              j_box;
	PdOpenGlComponent*  j_component;
} t_openglbox;

void eopenglbox_initclass(t_eclass* c, long flags);
void eopenglbox_new(t_openglbox* x, long flags);
void eopenglbox_free(t_openglbox* x);
void eopenglbox_paint(t_openglbox* x, t_object *patcherview);
t_pd_err eopenglbox_notify(t_jucebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data);


#endif


#endif