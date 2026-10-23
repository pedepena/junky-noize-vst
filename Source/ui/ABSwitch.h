#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
class ABSwitch : public juce::Component
{
public:
    std::function<void (int)> onSelect;
    void setActiveSlotB (bool isB) { activeB = isB; repaint(); }
    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent& e) override;

private:
    bool activeB = false;
};
} // namespace junky
