/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JbangAudioProcessor::JbangAudioProcessor()
{
}

JbangAudioProcessor::~JbangAudioProcessor()
{
}

//==============================================================================
const String JbangAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int JbangAudioProcessor::getNumParameters()
{
    return 0;
}

float JbangAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void JbangAudioProcessor::setParameter (int index, float newValue)
{
}

const String JbangAudioProcessor::getParameterName (int index)
{
    return String::empty;
}

const String JbangAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String JbangAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String JbangAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool JbangAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool JbangAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool JbangAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JbangAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JbangAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double JbangAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JbangAudioProcessor::getNumPrograms()
{
    return 0;
}

int JbangAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JbangAudioProcessor::setCurrentProgram (int index)
{
}

const String JbangAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void JbangAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JbangAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JbangAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void JbangAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);

        // ..do something to the data...
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool JbangAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JbangAudioProcessor::createEditor()
{
    return new JbangAudioProcessorEditor (this);
}

//==============================================================================
void JbangAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JbangAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JbangAudioProcessor();
}
