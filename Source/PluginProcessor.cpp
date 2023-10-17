/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IntrotutorialAudioProcessor::IntrotutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

IntrotutorialAudioProcessor::~IntrotutorialAudioProcessor()
{
}

//==============================================================================
const juce::String IntrotutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IntrotutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IntrotutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IntrotutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IntrotutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IntrotutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IntrotutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IntrotutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IntrotutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void IntrotutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void IntrotutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void IntrotutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IntrotutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void IntrotutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // The following code is one way to process audio, using the buffer.

    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    // Read pointer is read only, write pointer is read and write
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // Note: when you write to the buffer, you are actually replacing the
    //    // contents of the buffer at that given time.

    //    // If the buffer size is 256, then this is going to run 256 times. The sample
    //    // is a point in time.
    //    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    //    {
    //        // This makes your audio louder.
    //        channelData[sample] *= 2.0;
    //    }
    //}

    // The following code is another way to process audio, using an audio block.

    juce::dsp::AudioBlock<float> block (buffer);

    for (int channel = 0; channel < block.getNumChannels(); ++channel) {
        auto* channelData = block.getChannelPointer(channel);

        for (int sample = 0; sample < block.getNumSamples(); ++sample) {
            channelData[sample] *= 0.5;
        }
    }

    // It is somewhat better to use the audio block because the built in Juce DSP
    // modules use the block to create their "process context". So this is a common
    // interface for those APIs.
}

//==============================================================================
bool IntrotutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* IntrotutorialAudioProcessor::createEditor()
{
    return new IntrotutorialAudioProcessorEditor (*this);
}

//==============================================================================
void IntrotutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void IntrotutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IntrotutorialAudioProcessor();
}
