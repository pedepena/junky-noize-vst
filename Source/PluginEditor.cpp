#include "PluginEditor.h"
#include "ui/Chassis.h"
#include "ui/Theme.h"
#include "Layout.h"
#include "BinaryData.h"

namespace
{
constexpr int minimumWidth = 800;
constexpr int initialWidth = 1400;
constexpr int maximumWidth = 2000;
constexpr int initialHeight = (int) (initialWidth * junky::theme::designHeight / junky::theme::designWidth + 0.5f);
constexpr int minimumHeight = (int) (minimumWidth * junky::theme::designHeight / junky::theme::designWidth + 0.5f);
constexpr int maximumHeight = (int) (maximumWidth * junky::theme::designHeight / junky::theme::designWidth + 0.5f);
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

    // Los controles de las fases posteriores permanecen ocultos.
    // BASS BOOST usa su parametro existente y empieza apagado.
    bassButton.setClickingTogglesState (true);
    bassAttachment = std::make_unique<juce::ButtonParameterAttachment> (*audioProcessor.apvts.getParameter (bassBoost), bassButton);
    const bool bassBoostOn = audioProcessor.apvts.getRawParameterValue (junky::params::bassBoost)->load() > 0.5f;
    const bool isMono = audioProcessor.apvts.getRawParameterValue (junky::params::mono)->load() > 0.5f;
    bassLed.setOn (bassBoostOn);
    monoLed.setOn (isMono);
    stereoLed.setOn (! isMono);
    monoStereoLever.setMono (isMono);
    addAndMakeVisible (bassButton);
    addAndMakeVisible (bassLed);

    monoStereoLever.onToggle = [this] (bool isMono)
    {
        audioProcessor.setParameterPlain (junky::params::mono, isMono ? 1.0f : 0.0f);
    };
    addAndMakeVisible (monoStereoLever);
    addAndMakeVisible (monoLed);
    addAndMakeVisible (stereoLed);

    // Pantalla de presets
    audioProcessor.refreshUserPresets();
    lcd.setItems (audioProcessor.getPresetNames());
    lcd.setSelected (audioProcessor.getLcdSelectionIndex());
    lcd.onSelect = [this] (int index) { audioProcessor.selectPreset (index); };
    addAndMakeVisible (lcd);

    presetUpButton.setImageData (BinaryData::btn_up_png, BinaryData::btn_up_pngSize);
    presetDownButton.setImageData (BinaryData::btn_down_png, BinaryData::btn_down_pngSize);
    saveButton.setImageData (BinaryData::btn_save_png, BinaryData::btn_save_pngSize);
    folderButton.setImageData (BinaryData::btn_folder_png, BinaryData::btn_folder_pngSize);
    presetUpButton.onClick = [this] { lcd.selectRelative (-1); };
    presetDownButton.onClick = [this] { lcd.selectRelative (1); };
    addAndMakeVisible (presetUpButton);
    addAndMakeVisible (presetDownButton);
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

    inputMeter.setComponentID ("input-meter");
    outputMeter.setComponentID ("output-meter");
    addAndMakeVisible (inputMeter);
    addAndMakeVisible (outputMeter);

    // En FASE 2 solo se muestran los 12 knobs sobre el fondo vacío.


    setResizable (true, true);
    setResizeLimits (minimumWidth, minimumHeight, maximumWidth, maximumHeight);
    getConstrainer()->setFixedAspectRatio ((double) junky::theme::designWidth / (double) junky::theme::designHeight);
    const auto saved = audioProcessor.getEditorSize();
    const int width = saved.x > 0 ? juce::jlimit (minimumWidth, maximumWidth, saved.x)
                                  : saved.y > 0 ? juce::jlimit (minimumWidth, maximumWidth,
                                        juce::roundToInt ((float) saved.y * junky::theme::designWidth / junky::theme::designHeight))
                                                : initialWidth;
    const int height = juce::roundToInt ((float) width * junky::theme::designHeight / junky::theme::designWidth);
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
    knob.getProperties().set ("wearSeed", nextKnobWearSeed++);
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
    background = juce::ImageCache::getFromMemory (BinaryData::background_empty_png,
                                                  BinaryData::background_empty_pngSize);
}

void JunkyNoizeAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (background.isValid())
        g.drawImage (background, getLocalBounds().toFloat());


}

void JunkyNoizeAudioProcessorEditor::resized()
{
    if (getWidth() >= minimumWidth && getWidth() <= maximumWidth)
        audioProcessor.setEditorSize (getWidth(), getHeight());

    rebuildBackground();

    filterKnob.setBounds (fromDesign (junky::layout::filterKnob.x(), junky::layout::filterKnob.y(),
                                      junky::layout::filterKnob.size, junky::layout::filterKnob.size));
    mixKnob.setBounds (fromDesign (junky::layout::mixKnob.x(), junky::layout::mixKnob.y(),
                                   junky::layout::mixKnob.size, junky::layout::mixKnob.size));
    drunkKnob.setBounds (fromDesign (junky::layout::drunkKnob.x(), junky::layout::drunkKnob.y(),
                                     junky::layout::drunkKnob.size, junky::layout::drunkKnob.size));
    bassFreqKnob.setBounds (fromDesign (junky::layout::bassFreqKnob.x(), junky::layout::bassFreqKnob.y(),
                                        junky::layout::bassFreqKnob.size, junky::layout::bassFreqKnob.size));
    bitsKnob.setBounds (fromDesign (junky::layout::bitsKnob.x(), junky::layout::bitsKnob.y(),
                                    junky::layout::bitsKnob.size, junky::layout::bitsKnob.size));
    rateKnob.setBounds (fromDesign (junky::layout::rateKnob.x(), junky::layout::rateKnob.y(),
                                    junky::layout::rateKnob.size, junky::layout::rateKnob.size));
    driveKnob.setBounds (fromDesign (junky::layout::driveKnob.x(), junky::layout::driveKnob.y(),
                                     junky::layout::driveKnob.size, junky::layout::driveKnob.size));
    aliasKnob.setBounds (fromDesign (junky::layout::aliasKnob.x(), junky::layout::aliasKnob.y(),
                                     junky::layout::aliasKnob.size, junky::layout::aliasKnob.size));
    noiseKnob.setBounds (fromDesign (junky::layout::noiseKnob.x(), junky::layout::noiseKnob.y(),
                                     junky::layout::noiseKnob.size, junky::layout::noiseKnob.size));
    characterKnob.setBounds (fromDesign (junky::layout::characterKnob.x(), junky::layout::characterKnob.y(),
                                         junky::layout::characterKnob.size, junky::layout::characterKnob.size));
    inputKnob.setBounds (fromDesign (junky::layout::inputKnob.x(), junky::layout::inputKnob.y(),
                                     junky::layout::inputKnob.size, junky::layout::inputKnob.size));
    outputKnob.setBounds (fromDesign (junky::layout::outputKnob.x(), junky::layout::outputKnob.y(),
                                      junky::layout::outputKnob.size, junky::layout::outputKnob.size));

    bassButton.setBounds (fromDesign (junky::layout::bassButton.x(), junky::layout::bassButton.y(),
                                      junky::layout::bassButton.width, junky::layout::bassButton.height));
    bassLed.setBounds (fromDesign (junky::layout::bassLed.x(), junky::layout::bassLed.y(),
                                   junky::layout::bassLed.width, junky::layout::bassLed.height));
    monoLed.setBounds (fromDesign (junky::layout::monoLed.x(), junky::layout::monoLed.y(),
                                   junky::layout::monoLed.width, junky::layout::monoLed.height));
    stereoLed.setBounds (fromDesign (junky::layout::stereoLed.x(), junky::layout::stereoLed.y(),
                                     junky::layout::stereoLed.width, junky::layout::stereoLed.height));
    monoStereoLever.setBounds (fromDesign (junky::layout::switchTrackStartX,
                                           junky::layout::switchTrackCentreY - junky::layout::switchTrackHitHeight * 0.5f,
                                           junky::layout::switchTrackEndX - junky::layout::switchTrackStartX,
                                           junky::layout::switchTrackHitHeight));
    lcd.setBounds (fromDesign (junky::layout::presetLcd.x(), junky::layout::presetLcd.y(),
                               junky::layout::presetLcd.width, junky::layout::presetLcd.height));
    presetUpButton.setBounds (fromDesign (junky::layout::presetUpButton.x(), junky::layout::presetUpButton.y(),
                                          junky::layout::presetUpButton.width, junky::layout::presetUpButton.height));
    presetDownButton.setBounds (fromDesign (junky::layout::presetDownButton.x(), junky::layout::presetDownButton.y(),
                                            junky::layout::presetDownButton.width, junky::layout::presetDownButton.height));
    saveButton.setBounds (fromDesign (junky::layout::presetSaveButton.x(), junky::layout::presetSaveButton.y(),
                                      junky::layout::presetSaveButton.width, junky::layout::presetSaveButton.height));
    folderButton.setBounds (fromDesign (junky::layout::presetFolderButton.x(), junky::layout::presetFolderButton.y(),
                                        junky::layout::presetFolderButton.width, junky::layout::presetFolderButton.height));
    inputMeter.setBounds (fromDesign (junky::layout::inputMeter.x, junky::layout::inputMeter.y,
                                      junky::layout::inputMeter.width, junky::layout::inputMeter.height));
    outputMeter.setBounds (fromDesign (junky::layout::outputMeter.x, junky::layout::outputMeter.y,
                                       junky::layout::outputMeter.width, junky::layout::outputMeter.height));
}

void JunkyNoizeAudioProcessorEditor::timerCallback()
{
    auto toDb = [] (float peak) { return juce::Decibels::gainToDecibels (peak, -100.0f); };
    constexpr float fallDbPerTick = 20.0f / 30.0f;
    const float inputPeakDb = toDb (audioProcessor.takeInputPeak());
    const float outputPeakDb = toDb (audioProcessor.takeOutputPeak());
    inputDisplayDb = juce::jmax (inputPeakDb, inputDisplayDb - fallDbPerTick);
    outputDisplayDb = juce::jmax (outputPeakDb, outputDisplayDb - fallDbPerTick);
    if (inputDisplayDb > inputPeakHoldDb + 0.01f) { inputPeakHoldDb = inputDisplayDb; inputPeakHoldTicks = 18; }
    else if (inputPeakHoldTicks > 0) --inputPeakHoldTicks;
    else inputPeakHoldDb = juce::jmax (inputDisplayDb, inputPeakHoldDb - fallDbPerTick);
    if (outputDisplayDb > outputPeakHoldDb + 0.01f) { outputPeakHoldDb = outputDisplayDb; outputPeakHoldTicks = 18; }
    else if (outputPeakHoldTicks > 0) --outputPeakHoldTicks;
    else outputPeakHoldDb = juce::jmax (outputDisplayDb, outputPeakHoldDb - fallDbPerTick);
    inputMeter.setLevels (inputDisplayDb, inputPeakHoldDb);
    outputMeter.setLevels (outputDisplayDb, outputPeakHoldDb);

    const bool bassBoostOn = audioProcessor.apvts.getRawParameterValue (junky::params::bassBoost)->load() > 0.5f;
    const bool isMono = audioProcessor.apvts.getRawParameterValue (junky::params::mono)->load() > 0.5f;
    bassLed.setOn (bassBoostOn);
    monoLed.setOn (isMono);
    stereoLed.setOn (! isMono);
    monoStereoLever.setMono (isMono);

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
