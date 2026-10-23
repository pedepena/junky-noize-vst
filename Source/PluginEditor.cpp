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
    setupKnob (drunkKnob);
    setupKnob (driveKnob);
    setupKnob (bitsKnob);
    setupKnob (rateKnob);
    setupKnob (aliasKnob);
    setupKnob (noiseKnob);
    setupKnob (characterKnob);

    bitsKnob.setRange (0.0, 3.0, 1.0);
    bitsKnob.getProperties().set ("steps", 4);

    filterAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, filter, filterKnob);
    mixAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, mix, mixKnob);
    bassFreqAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, bassFreq, bassFreqKnob);
    inputAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, input, inputKnob);
    outputAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, output, outputKnob);
    drunkAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, drunk, drunkKnob);
    driveAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, drive, driveKnob);
    bitsAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, bits, bitsKnob);
    rateAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, rate, rateKnob);
    aliasAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, alias, aliasKnob);
    noiseAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, noise, noiseKnob);
    characterAttachment = std::make_unique<SliderAttachment> (audioProcessor.apvts, character, characterKnob);

    // Doble clic en un knob = volver al valor por defecto
    for (auto* knob : { &filterKnob, &mixKnob, &bassFreqKnob, &inputKnob, &outputKnob,
                        &drunkKnob, &driveKnob, &bitsKnob, &rateKnob, &aliasKnob, &noiseKnob, &characterKnob })
        knob->setDoubleClickReturnValue (true, knob->getValue());

    filterKnob.setDoubleClickReturnValue (true, 0.0);
    mixKnob.setDoubleClickReturnValue (true, 1.0);
    bassFreqKnob.setDoubleClickReturnValue (true, 100.0);
    inputKnob.setDoubleClickReturnValue (true, 0.0);
    outputKnob.setDoubleClickReturnValue (true, 0.0);
    drunkKnob.setDoubleClickReturnValue (true, 0.0);
    driveKnob.setDoubleClickReturnValue (true, 0.0);
    bitsKnob.setDoubleClickReturnValue (true, 0.0);
    rateKnob.setDoubleClickReturnValue (true, 0.0);
    aliasKnob.setDoubleClickReturnValue (true, 0.5);
    noiseKnob.setDoubleClickReturnValue (true, 0.0);
    characterKnob.setDoubleClickReturnValue (true, 1.0);

    // Bass boost: interruptor conectado al parametro
    bassButton.setClickingTogglesState (true);
    bassAttachment = std::make_unique<juce::ButtonParameterAttachment> (*audioProcessor.apvts.getParameter (bassBoost), bassButton);
    addAndMakeVisible (bassButton);

    // Mono / Stereo: dos botones que se excluyen entre si
    monoButton.onClick = [this] { audioProcessor.setParameterPlain (junky::params::mono, 1.0f); };
    stereoButton.onClick = [this] { audioProcessor.setParameterPlain (junky::params::mono, 0.0f); };
    monoButton.setToggleState (false, juce::dontSendNotification);
    stereoButton.setToggleState (true, juce::dontSendNotification);
    addAndMakeVisible (monoButton);
    addAndMakeVisible (stereoButton);

    // A/B
    abSwitch.onSelect = [this] (int)
    {
        audioProcessor.toggleAB();
    };
    abSwitch.setActiveSlotB (audioProcessor.isSlotB());
    addAndMakeVisible (abSwitch);

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
    saveButton.setLabel ("SAVE");
    saveButton.onClick = [this]
    {
        const int slot = audioProcessor.saveCustomPreset();
        lcd.setHeader (slot >= 0 ? "SAVED TO CUSTOM " + juce::String (slot + 1) : "SELECT A CUSTOM SLOT", slot >= 0 ? 1200 : 1500);
    };
    addAndMakeVisible (saveButton);

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
    for (auto* knob : { &filterKnob, &mixKnob, &bassFreqKnob, &inputKnob, &outputKnob,
                        &drunkKnob, &driveKnob, &bitsKnob, &rateKnob, &aliasKnob, &noiseKnob, &characterKnob })
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
    saveButton.setBounds (fromDesign (787, 258, 78, 34));

    filterKnob.setBounds (fromDesign (192, 443, 170, 170));
    mixKnob.setBounds (fromDesign (390, 327, 300, 300));

    drunkKnob.setBounds (fromDesign (723, 443, 170, 170));
    abSwitch.setBounds (fromDesign (918, 264, 144, 50));

    monoButton.setBounds (fromDesign (90, 780, 110, 100));
    stereoButton.setBounds (fromDesign (250, 780, 110, 100));

    bassButton.setBounds (fromDesign (465, 778, 90, 95));
    bassFreqKnob.setBounds (fromDesign (590, 750, 180, 180));

    inputMeter.setBounds (fromDesign (843, 728, 90, 268));
    outputMeter.setBounds (fromDesign (947, 728, 90, 268));
    inputKnob.setBounds (fromDesign (825, 1002, 96, 96));
    outputKnob.setBounds (fromDesign (932, 1002, 96, 96));

    juce::Slider* colourKnobs[] = { &driveKnob, &bitsKnob, &rateKnob, &aliasKnob, &noiseKnob, &characterKnob };
    constexpr float centres[] = { 118, 236, 354, 472, 590, 708 };
    for (int i = 0; i < 6; ++i)
        colourKnobs[i]->setBounds (fromDesign (centres[i] - 44.0f, 1008, 88, 88));
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
        abSwitch.setActiveSlotB (displayedSlotB);
        repaint();
    }
}
