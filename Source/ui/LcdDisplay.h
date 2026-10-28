#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Layout.h"

namespace junky
{
/** Pantalla verde estilo LCD con la lista de presets. */
class LcdDisplay : public juce::Component, private juce::Timer
{
public:
    std::function<void (int)> onSelect;

    void setItems (const juce::StringArray& newItems);
    void setSelected (int index);
    void selectRelative (int delta);
    int getSelected() const { return selected; }
    void setHeader (const juce::String& text, int durationMs = 0);

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseWheelMove (const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

private:
    static constexpr int visibleRows = layout::presetLcdVisibleRows;

    void ensureVisible();
    void choose (int index);

    juce::StringArray items;
    int selected = 0;
    int offset = 0;
    juce::String header;
    void timerCallback() override;
};
} // namespace junky
