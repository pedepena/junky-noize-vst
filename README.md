# Junky Noize

Efecto de audio VST3 para dar sabor lo-fi a cualquier sonido. También genera AU en macOS. Se inspira en las máquinas de sampleo clásicas de los años 80 y 90.

## Qué incluye

Hay **51 presets de fábrica**: DEFAULT, 37 presets inspirados en máquinas y 13 creativos. Cada preset combina bits, frecuencia de muestreo, filtro, saturación, ruido, aliasing y carácter para sugerir un sonido. Son interpretaciones y homenajes, no réplicas exactas de circuitos.

**Los nombres de las máquinas son homenajes; no hay relación con las marcas originales.** MBC, AKS, EMV, RLD, PS, ENS, KRG y CSO son nombres propios del proyecto.

## Instalación

### Windows

Descarga **JunkyNoize-Setup.exe** desde [Releases](https://github.com/pedepena/junky-noize-vst/releases) y ejecútalo. El instalador propone la carpeta VST3 del sistema y permite elegir otra. Después, vuelve a escanear los plugins en tu DAW.

En FL Studio: **Mixer → hueco de efectos → Junky Noize**. Si no aparece, abre **Options → Manage plugins** y pulsa **Find installed plugins** para refrescar la lista y reescanear.

### macOS

Descarga el ZIP de macOS desde Releases y descomprímelo. Copia `VST3/Junky Noize.vst3` a `~/Library/Audio/Plug-Ins/VST3/`. El paquete también incluye `AU/Junky Noize.component`, que puedes copiar a `~/Library/Audio/Plug-Ins/Components/` para instalar el formato AU.

### Linux

Descarga el TAR.GZ de Linux desde Releases y descomprímelo. Copia `VST3/Junky Noize.vst3` a `~/.vst3/`. Crea la carpeta si no existe y vuelve a escanear los plugins en tu DAW.

Los paquetes de macOS y Linux se generan cuando se ejecuta el workflow con la opción correspondiente. Consulta [Compilación](#compilación).

## Controles

| Control | Función |
| --- | --- |
| FILTER | Gira hacia DARK para oscurecer el sonido y hacia BRIGHT para aclararlo; el centro es neutro. |
| MIX | Mezcla la señal original con la procesada. |
| DRUNK | Añade variaciones de tiempo y afinación, como una cinta inestable. |
| BITS | Elige la resolución digital: 16, 12, 8 o 4 bits. |
| RATE | Reduce la frecuencia de muestreo, de 48k (limpio, sin reducción) hasta 4k (muy sucio). |
| DRIVE | Añade saturación. |
| ALIAS | Ajusta el suavizado y el aliasing: bajo suaviza y alto deja pasar más grano. |
| NOISE | Añade ruido de fondo. |
| CHARACTER | Ajusta cuánto colorea el sonido el perfil de máquina seleccionado. |
| BASS BOOST | Activa el realce de graves. |
| FREQ | Elige la frecuencia del realce de graves, de 20 a 500 Hz. |
| MONO / STEREO | Mono suma los canales; estéreo conserva los canales separados. |
| INPUT | Ajusta el nivel de entrada. |
| OUTPUT | Ajusta el nivel de salida. |
| SAVE | Guarda los ajustes actuales como preset con nombre; permite reemplazar uno existente. |
| Botón de carpeta | Abre la carpeta de presets guardados por el usuario. |

Usa **MIX** para equilibrar el efecto con el sonido original. Vigila los medidores **IN** y **OUT** para controlar los niveles.

Los presets de usuario se guardan como archivos XML en la carpeta de datos del usuario `Junky Noize/Presets`. El botón de carpeta abre esa ubicación.

## Compilación

Se necesita CMake 3.22 o superior, Git y una cadena de herramientas C++ compatible. CMake descarga JUCE 8.0.8 automáticamente.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

En macOS, para compilar como binario universal:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
cmake --build build --config Release --parallel
```

El formato VST3 se genera en Windows, macOS y Linux; AU se genera en macOS. En Linux, instala las bibliotecas de desarrollo que aparecen en el paso **Instalar librerías de Linux** del workflow antes de configurar CMake.

### GitHub Actions

El workflow se ejecuta manualmente desde **Actions → Compilar Junky Noize → Run workflow**, o al publicar un tag `v*` (por ejemplo, `v1.0.0`). Por defecto compila Windows. Para incluir macOS y Linux en una ejecución manual, marca **todas_las_plataformas**. Al crear un tag se publica una Release con el instalador de Windows; para incluir paquetes de macOS y Linux en una Release, ejecuta el workflow sobre ese tag con la casilla marcada.

## Estructura del proyecto

```text
.github/workflows/build.yml       Workflow de compilación y Releases
installer/                        Script Inno Setup, información y banner
packaging/package-unix.sh         Empaquetado de macOS y Linux
packaging/windows/JunkyNoize.iss  Script antiguo, no usado por el workflow
Source/                           Código del plugin
  dsp/                            Procesamiento de audio
  ui/                             Interfaz y recursos gráficos
CMakeLists.txt                    Configuración de CMake y JUCE
LICENSE                           Licencia AGPL-3.0
README.md                         Esta documentación
```

No hay una carpeta `tools/` ni una captura de pantalla del plugin en el repositorio.

## Licencia

El código de Junky Noize se distribuye bajo la **GNU Affero General Public License, versión 3 (AGPL-3.0)**. Consulta el archivo [LICENSE](LICENSE). JUCE mantiene sus propios términos de licencia.

## Créditos

Hecho con cariño por **pedepena**. Gratis y de código abierto.

Repositorio: [github.com/pedepena/junky-noize-vst](https://github.com/pedepena/junky-noize-vst)
