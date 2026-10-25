#include "PluginEditor.h"
#include "ui/Chassis.h"

namespace
{
constexpr int initialWidth = 1064;
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

    // Pantalla de presets
    audioProcessor.refreshUserPresets();
    lcd.setItems (audioProcessor.getPresetNames());
    lcd.setSelected (audioProcessor.getLcdSelectionIndex());
    lcd.onSelect = [this] (int index) { audioProcessor.selectPreset (index); };
    addAndMakeVisible (lcd);

    presetUpButton.onClick = [this] { lcd.setSelected (lcd.getSelected() - 1); audioProcessor.selectPreset (lcd.getSelected()); };
    presetDownButton.onClick = [this] { lcd.setSelected (lcd.getSelected() + 1); audioProcessor.selectPreset (lcd.getSelected()); };
    addAndMakeVisible (presetUpButton);
    addAndMakeVisible (presetDownButton);
    saveButton.setLabel ("SAVE");
    saveButton.onClick = [this]
    {
        juce::String initial;
        const int selected = lcd.getSelected();
        if (selected >= 51) initial = audioProcessor.getPresetNames()[selected];
        auto* alert = new juce::AlertWindow ("Save preset",
            "Name (max 18 characters: letters, numbers, space, - & ( ) )", juce::AlertWindow::NoIcon);
        alert->addTextEditor ("name", initial);
        alert->addButton ("Save", 1, juce::KeyPress (juce::KeyPress::returnKey));
        alert->addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));
        juce::Component::SafePointer<JunkyNoizeAudioProcessorEditor> safeEditor (this);
        juce::Component::SafePointer<juce::AlertWindow> safeAlert (alert);
        alert->enterModalState (true, juce::ModalCallbackFunction::create ([safeEditor, safeAlert] (int result)
        {
            if (safeEditor == nullptr || safeAlert == nullptr || result != 1) return;
            auto name = safeAlert->getTextEditorContents ("name");
            name.retainCharacters ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 -&()");
            name = name.trim().substring (0, 18);
            if (name.isEmpty()) { safeEditor->lcd.setHeader ("ENTER A NAME", 1500); return; }
            auto save = [safeEditor, name]
            {
                if (safeEditor == nullptr) return;
                if (! safeEditor->audioProcessor.saveUserPreset (name, true))
                { safeEditor->lcd.setHeader ("SAVE FAILED", 1500); return; }
                safeEditor->lcd.setItems (safeEditor->audioProcessor.getPresetNames());
                safeEditor->lcd.setSelected (safeEditor->audioProcessor.getLcdSelectionIndex());
                safeEditor->lcd.setHeader ("SAVED: " + name, 1200);
            };
            const auto names = safeEditor->audioProcessor.getPresetNames();
            bool duplicate = false;
            for (int i = 51; i < names.size(); ++i)
                if (names[i].equalsIgnoreCase (name)) { duplicate = true; break; }
            if (! duplicate) { save(); return; }
            auto* replace = new juce::AlertWindow ("Replace preset", "Replace preset " + name + "?",
                                                   juce::AlertWindow::NoIcon);
            replace->addButton ("Replace", 1, juce::KeyPress (juce::KeyPress::returnKey));
            replace->addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));
            replace->enterModalState (true, juce::ModalCallbackFunction::create ([save] (int replaceResult)
            { if (replaceResult == 1) save(); }), true);
        }), true);
    };
    addAndMakeVisible (saveButton);

    folderButton.onClick = [this]
    {
        auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
            .getChildFile ("Junky Noize").getChildFile ("Presets");
        dir.createDirectory();
        if (! dir.startAsProcess()) lcd.setHeader ("CANNOT OPEN FOLDER", 1500);
    };
    addAndMakeVisible (folderButton);

    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);

    setResizable (true, true);
    setResizeLimits (initialWidth / 2, initialHeight / 2, initialWidth * 2, initialHeight * 2);
    getConstrainer()->setFixedAspectRatio ((double) junky::theme::designWidth / (double) junky::theme::designHeight);
    const auto saved = audioProcessor.getEditorSize();
    const int minWidth = initialWidth / 2, maxWidth = initialWidth * 2;
    const int minHeight = initialHeight / 2, maxHeight = initialHeight * 2;
    const int width = saved.x > 0 ? juce::jlimit (minWidth, maxWidth, saved.x)
                                  : saved.y > 0 ? juce::roundToInt ((float) saved.y * junky::theme::designWidth / junky::theme::designHeight)
                                                : initialWidth;
    const int height = saved.y > 0 ? juce::jlimit (minHeight, maxHeight, saved.y)
                                   : juce::roundToInt ((float) width * junky::theme::designHeight / junky::theme::designWidth);
    setSize (width, height);

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
    junky::paintLabels (g);
}

void JunkyNoizeAudioProcessorEditor::resized()
{
    if (getWidth() >= initialWidth / 2 && getHeight() >= initialHeight / 2)
        audioProcessor.setEditorSize (getWidth(), getHeight());

    rebuildBackground();

    lcd.setBounds (fromDesign (340, 55, 420, 215));
    presetUpButton.setBounds (fromDesign (787, 70, 78, 85));
    presetDownButton.setBounds (fromDesign (787, 165, 78, 85));
    saveButton.setBounds (fromDesign (787, 258, 78, 34));
    folderButton.setBounds (fromDesign (960, 262, 64, 46));

    filterKnob.setBounds (fromDesign (192, 443, 170, 170));
    mixKnob.setBounds (fromDesign (390, 327, 300, 300));

    drunkKnob.setBounds (fromDesign (723, 443, 170, 170));

    monoButton.setBounds (fromDesign (90, 780, 110, 100));
    stereoButton.setBounds (fromDesign (250, 780, 110, 100));

    bassButton.setBounds (fromDesign (465, 778, 90, 95));
    bassFreqKnob.setBounds (fromDesign (620, 792, 120, 120));

    inputMeter.setBounds (fromDesign (843, 728, 90, 268));
    outputMeter.setBounds (fromDesign (947, 728, 90, 268));
    inputKnob.setBounds (fromDesign (825, 1002, 96, 96));
    outputKnob.setBounds (fromDesign (932, 1002, 96, 96));

    juce::Slider* colourKnobs[] = { &driveKnob, &bitsKnob, &rateKnob, &aliasKnob, &noiseKnob, &characterKnob };
    constexpr float lefts[] = { 104, 218, 332, 446, 560, 674 };
    for (int i = 0; i < 6; ++i)
        colourKnobs[i]->setBounds (fromDesign (lefts[i], 1032, 68, 68));
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

    // Actualiza presets nuevos o borrados sin bloquear el audio.
    if (++presetRefreshTicks >= 60)
    {
        presetRefreshTicks = 0;
        if (audioProcessor.refreshUserPresets())
        {
            lcd.setItems (audioProcessor.getPresetNames());
            lcd.setSelected (audioProcessor.getLcdSelectionIndex());
        }
    }
    lcd.setSelected (audioProcessor.getLcdSelectionIndex());

}
