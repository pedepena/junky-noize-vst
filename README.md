# Junky Noize

Efecto de audio VST3 para dar sabor lo-fi a cualquier sonido. Se inspira en las máquinas de sampleo clásicas de los años 80 y 90.

![Vista del panel de Junky Noize](Source/ui/assets/background_empty.png)

## Qué hace

Junky Noize combina reducción de bits y de frecuencia de muestreo, saturación, ruido, filtros y aliasing. Sus presets de máquinas son interpretaciones sonoras ajustadas para que cada carácter se note; no son réplicas exactas de circuitos.

**Los nombres de las máquinas son homenajes; no hay relación con las marcas originales.** MBC, AKS, EMV, RLD, PS, ENS, KRG y CSO son nombres propios del proyecto.

## Instalación

Descarga **JunkyNoize-Setup.exe** desde [Releases](https://github.com/pedepena/junky-noize-vst/releases) y ejecuta el instalador. Elige la carpeta VST3 si quieres cambiar la ubicación sugerida. Después, vuelve a escanear los plugins en tu DAW.

En FL Studio, abre **Mixer → un hueco de efectos → Junky Noize**. Si no aparece, ve a **Options → Manage plugins** y pulsa **Find installed plugins** para refrescar la lista y reescanear.

## Controles

| Control | Función |
| --- | --- |
| FILTER | Oscurece el sonido con valores negativos y lo aclara con valores positivos. |
| MIX | Mezcla el sonido original con el procesado. |
| DRUNK | Añade variaciones de tiempo y afinación, como una cinta inestable. |
| BITS | Elige la resolución: 16, 12, 8 o 4 bits. |
| RATE | Reduce la frecuencia de muestreo; en 0 queda desactivado. |
| DRIVE | Añade saturación. |
| ALIAS | Ajusta el suavizado y el aliasing: bajo suaviza, alto deja más grano. |
| NOISE | Añade ruido de fondo. |
| CHARACTER | Ajusta cuánto colorea el sonido el perfil de máquina seleccionado. |
| BASS BOOST | Activa un realce de graves. |
| FREQ | Elige la frecuencia del realce de graves, de 20 a 500 Hz. |
| MONO / STEREO | Mono suma los canales; estéreo mantiene los canales separados. |
| INPUT | Ajusta el nivel de entrada. |
| OUTPUT | Ajusta el nivel de salida. |
| SAVE | Guarda los ajustes actuales como preset con nombre. |
| Botón de carpeta | Abre la carpeta de presets guardados por el usuario. |

Usa **MIX** para equilibrar el efecto con el sonido original. Vigila los medidores **IN** y **OUT** para controlar los niveles.

## Créditos y licencia

Hecho con cariño por pedepena. Gratis y de código abierto.

Repositorio: [github.com/pedepena/junky-noize-vst](https://github.com/pedepena/junky-noize-vst)

El código de Junky Noize se distribuye bajo la licencia **GNU Affero General Public License, versión 3 (AGPL-3.0)**. Consulta [LICENSE](LICENSE). JUCE mantiene sus propios términos de licencia.
