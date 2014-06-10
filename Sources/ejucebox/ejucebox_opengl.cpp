/*
 // Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ejucebox.h"

#ifdef JUCE_WRAPPER

PdOpenGlComponent::PdOpenGlComponent()
{
    m_context = new OpenGLContext();
    m_context->setComponentPaintingEnabled(false);
    
	m_context->setRenderer(this); // need this to set m_scale
    m_context->attachTo(*this);
    m_context->setContinuousRepainting(false);
    setInterceptsMouseClicks(false, false);
    setOpaque(true);
	setVisible(false);
    
    //m_image = new Image(OpenGLImageType().create(Image::RGB, 1000, 1000, true));
    //m_buffer = OpenGLImageType::getFrameBufferFrom(*m_image);
}

Image PdOpenGlComponent::makeScreenshot(t_object* x, double width, double height)
{
    if(!m_context->isActive())
        return *m_image;
    
    m_buffer->makeCurrentAndClear();
    m_buffer->clear(Colours::red);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    glEnable(GL_DEPTH_TEST);
    glColor4d(1., 0., 0., 1.);
    glBegin(GL_LINE_STRIP);
    glVertex3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(1, 0, 0);
    
    glPointSize(1.0f);
    glEnd();
     
    //(object_getmethod(x, gensym("jucebox_paint")))(x, width * m_scale, height * m_scale);
	
    m_buffer->releaseAsRenderingTarget();
    return *m_image;
}

void PdOpenGlComponent::setBounds (const int x, const int y, int w, int h)
{
    Component::setBounds(x, y, w, h);
    //delete m_image;
    //m_image = new Image(OpenGLImageType().create(Image::RGB, roundToInt(getWidth()), roundToInt(getWidth()), true));
    //m_buffer = OpenGLImageType::getFrameBufferFrom(*m_image);
}

PdOpenGlComponent::~PdOpenGlComponent()
{
    m_context->detach();
    delete m_context;
}

void eopenglbox_initclass(t_eclass* c, long flags)
{
    eclass_init((t_eclass *)c, flags);
    eclass_addmethod(c, (method) eopenglbox_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) eopenglbox_notify,          "notify",           A_CANT, 0);
    c->c_juce = 1;
    
    initialiseMac();
    initialiseJuce_GUI();
}

void eopenglbox_new(t_openglbox* x, long flags)
{
    ebox_new((t_ebox *)x, flags);
    x->j_component = new PdOpenGlComponent();
    x->j_component->addToDesktop(ComponentPeer::windowIsTemporary);
}

void eopenglbox_free(t_openglbox* x)
{
    ebox_free((t_ebox *)x);
	x->j_component->removeFromDesktop();
    delete x->j_component;
}

static char OpenGLColorBuffer[10];

static char OpenGLHexDigits[] = "0123456789ABCDEF";

char* openglcolor_to_hex(Colour color)
{
    int r = color.getRed();
    int g = color.getGreen();
    int b = color.getBlue();
    OpenGLColorBuffer[0] = '#';
    OpenGLColorBuffer[1] = OpenGLHexDigits[(r >> 4) & 15];
    OpenGLColorBuffer[2] = OpenGLHexDigits[r & 15];
    OpenGLColorBuffer[3] = OpenGLHexDigits[(g >> 4) & 15];
    OpenGLColorBuffer[4] = OpenGLHexDigits[g & 15];
    OpenGLColorBuffer[5] = OpenGLHexDigits[(b >> 4) & 15];
    OpenGLColorBuffer[6] = OpenGLHexDigits[b & 15];
    OpenGLColorBuffer[7] = '\0';
    return &OpenGLColorBuffer[0];
}

t_pd_err eopenglbox_notify(t_jucebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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


void eopenglbox_paint(t_openglbox* x, t_object *patcherview)
{
    t_rect rect;
    int offset = x->j_box.b_boxparameters.d_borderthickness;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    
    x->j_component->setVisible(1);
    if(x->j_component->isOnDesktop())
    {
        x->j_component->setActive();
        
        if(x->j_component->isContextOk())
        {
            /*
            juce::Image openGLSnap = x->j_component->makeScreenshot((t_object *)x, rect.width, rect.height);
            
            sys_gui("set pixels { ");
            for(int i = 0; i < rect.height; i++)
            {
                sys_gui("{");
                for(int j = 0; j < rect.width; j++)
                {
                    sys_vgui("%s ", openglcolor_to_hex(openGLSnap.getPixelAt(j, i)));
                }
                sys_gui("} ");
            }
            sys_gui("}\n");
            
            sys_gui("set photo [image create photo]\n");
            sys_gui("$photo put $pixels\n");
            sys_vgui("%s delete %snopen\n", x->j_box.b_drawing_id->s_name, x->j_box.b_all_id->s_name);
            sys_vgui("%s create image %i %i -anchor nw -image $photo -tags %snopen\n", x->j_box.b_drawing_id->s_name, offset, offset, x->j_box.b_all_id->s_name);
             */
        }
    }
    
    x->j_component->setVisible(0);
}


#endif