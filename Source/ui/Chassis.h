#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
/** Dibuja el fondo de la maquina: chapa metalica, paneles, tornillos, rejilla, oxido y aranazos.
    Se dibuja en las coordenadas del diseno (1085 x 992). */
void paintChassis (juce::Graphics& g);

/** Dibuja los textos fijos: logo, etiquetas y la nota adhesiva. */
void paintLabels (juce::Graphics& g, bool isSlotB);
} // namespace junky
