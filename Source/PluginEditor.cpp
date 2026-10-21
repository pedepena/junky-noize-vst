#include "PluginEditor.h"
#include "ui/Chassis.h"

namespace
{
constexpr int initialWidth = 760;
constexpr int initialHeight = (int) (initialWidth * junky::theme::designHeight / junky::theme::designWidth);
} // namespace

JunkyNoizeAudioProcessorEditor::JunkyNoizeAudioProcessorEditor (JunkyNoizeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    using namespace junky::params;

    setupKnob (filterKnob);
    setupKnob (mixKnob);
    setupKnob (bassFreqKnob);
    setupKnob (inputKnob);
    setupKnob (outputKnob);

    filterAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, filter, filterKnob);
    mixAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, mix, mixKnob);
    bassFreqAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, bassFreq, bassFreqKnob);
    inputAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, input, inputKnob);
    outputAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, output, outputKnob);

    // Doble clic en un knob = volver al valor por defecto
    for (auto* knob : { &filterKnob, &mixKnob, &bassFreqKnob, &inputKnob, &outputKnob })
        knob->setDoubleClickReturnValue (true, knob->getValue());

    filterKnob.setDoubleClickReturnValue (true, 0.0);
    mixKnob.setDoubleClickReturnValue (true, 1.0);
    bassFreqKnob.setDoubleClickReturnValue (true, 100.0);
    inputKnob.setDoubleClickReturnValue (true, 0.0);
    outputKnob.setDoubleClickReturnValue (true, 0.0);

    // Bass boost: interruptor conectado al parametro
    bassButton.setClickingTogglesState (true);
    bassAttachment = std::make_unique<juce::ButtonParameterAttachment> (*audioProcessor.apvts.getParameter (bassBoost), bassButton);
    addAndMakeVisible (bassButton);

    // Mono / Stereo: dos botones que se excluyen entre si
    monoButton.onClick = [this] { audioProcessor.setParameterPlain (junky::params::mono, 1.0f); };
    stereoButton.onClick = [this] { audioProcessor.setParameterPlain (junky::params::mono, 0.0f); };
    addAndMakeVisible (monoButton);
    addAndMakeVisible (stereoButton);

    // A/B
    abButton.onClick = [this]
    {
        audioProcessor.toggleAB();
        repaint();
    };
    addAndMakeVisible (abButton);

    // Pantalla de presets
    juce::StringArray names;
    for (const auto& preset : junky::factoryPresets)
        names.add (preset.name);

    lcd.setItems (names);
    lcd.setSelected (audioProcessor.getCurrentPreset());
    lcd.onSelect = [this] (int index) { audioProcessor.selectPreset (index); };
    addAndMakeVisible (lcd);

    presetUpButton.onClick = [this] { lcd.setSelected (lcd.getSelected() - 1); audioProcessor.selectPreset (lcd.getSelected()); };
    presetDownButton.onClick = [this] { lcd.setSelected (lcd.getSelected() + 1); audioProcessor.selectPreset (lcd.getSelected()); };
    addAndMakeVisible (presetUpButton);
    addAndMakeVisible (presetDownButton);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);

    setResizable (true, true);
    setResizeLimits (initialWidth / 2, initialHeight / 2, initialWidth * 2, initialHeight * 2);
    getConstrainer()->setFixedAspectRatio ((double) junky::theme::designWidth / (double) junky::theme::designHeight);
    setSize (initialWidth, initialHeight);

    startTimerHz (30);
}

JunkyNoizeAudioProcessorEditor::~JunkyNoizeAudioProcessorEditor()
{
    for (auto* knob : { &filterKnob, &mixKnob, &bassFreqKnob, &inputKnob, &outputKnob })
        knob->setLookAndFeel (nullptr);
}

void JunkyNoizeAudioProcessorEditor::setupKnob (juce::Slider& knob)
{
    knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    knob.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f, juce::MathConstants<float>::pi * 2.75f, true);
    knob.setPopupDisplayEnabled (true, true, this);
    knob.setLookAndFeel (&knobLook);
    addAndMakeVisible (knob);
}

juce::Rectangle<int> JunkyNoizeAudioProcessorEditor::fromDesign (float x, float y, float w, float h) const
{
    const float sx = (float) getWidth() / junky::theme::designWidth;
    const float sy = (float) getHeight() / junky::theme::designHeight;

    return { juce::roundToInt (x * sx), juce::roundToInt (y * sy), juce::roundToInt (w * sx), juce::roundToInt (h * sy) };
}

void JunkyNoizeAudioProcessorEditor::rebuildBackground()
{
    // Se dibuja al doble de tamano para que se vea nitido en pantallas de alta resolucion
    const int w = juce::jmax (1, getWidth() * 2);
    const int h = juce::jmax (1, getHeight() * 2);

    background = juce::Image (juce::Image::ARGB, w, h, true);
    juce::Graphics g (background);
    g.addTransform (juce::AffineTransform::scale ((float) w / junky::theme::designWidth,
                                                  (float) h / junky::theme::designHeight));
    junky::paintChassis (g);
}

void JunkyNoizeAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (background.isValid())
        g.drawImage (background, getLocalBounds().toFloat());

    g.addTransform (juce::AffineTransform::scale ((float) getWidth() / junky::theme::designWidth,
                                                  (float) getHeight() / junky::theme::designHeight));
    junky::paintLabels (g, audioProcessor.isSlotB());
}

void JunkyNoizeAudioProcessorEditor::resized()
{
    rebuildBackground();

    lcd.setBounds (fromDesign (340, 55, 420, 215));
    presetUpButton.setBounds (fromDesign (787, 70, 78, 85));
    presetDownButton.setBounds (fromDesign (787, 165, 78, 85));

    filterKnob.setBounds (fromDesign (192, 443, 170, 170));
    mixKnob.setBounds (fromDesign (390, 327, 300, 300));

    abButton.setBounds (fromDesign (860, 458, 90, 102));

    monoButton.setBounds (fromDesign (90, 780, 110, 100));
    stereoButton.setBounds (fromDesign (250, 780, 110, 100));

    bassButton.setBounds (fromDesign (465, 778, 90, 95));
    bassFreqKnob.setBounds (fromDesign (590, 750, 180, 180));

    inputMeter.setBounds (fromDesign (843, 730, 78, 115));
    outputMeter.setBounds (fromDesign (948, 730, 78, 115));
    inputKnob.setBounds (fromDesign (825, 840, 96, 96));
    outputKnob.setBounds (fromDesign (930, 840, 96, 96));
}

void JunkyNoizeAudioProcessorEditor::timerCallback()
{
    // Medidores: suben al instante y bajan poco a poco
    auto toDb = [] (float peak) { return juce::Decibels::gainToDecibels (peak, -100.0f); };

    inputDisplayDb = juce::jmax (toDb (audioProcessor.takeInputPeak()), inputDisplayDb - 1.5f);
    outputDisplayDb = juce::jmax (toDb (audioProcessor.takeOutputPeak()), outputDisplayDb - 1.5f);
    inputMeter.setLevelDb (inputDisplayDb);
    outputMeter.setLevelDb (outputDisplayDb);

    // Botones Mono / Stereo
    const bool isMono = audioProcessor.apvts.getRawParameterValue (junky::params::mono)->load() > 0.5f;
    monoButton.setToggleState (isMono, juce::dontSendNotification);
    stereoButton.setToggleState (! isMono, juce::dontSendNotification);

    // Preset elegido (por ejemplo, al cargar un proyecto)
    lcd.setSelected (audioProcessor.getCurrentPreset());

    // Pilotos A/B
    if (displayedSlotB != audioProcessor.isSlotB())
    {
        displayedSlotB = audioProcessor.isSlotB();
        repaint();
    }
}
