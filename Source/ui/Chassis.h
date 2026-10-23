#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
/** Dibuja el fondo de la maquina en las coordenadas del diseno (1085 x 1182). */
void paintChassis (juce::Graphics& g);

/** Dibuja los textos fijos: logo, etiquetas y la nota adhesiva. */
void paintLabels (juce::Graphics& g, bool isSlotB);
} // namespace junky
