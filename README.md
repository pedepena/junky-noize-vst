# Junky Noize

Efecto de audio VST3 (y AU en macOS) con controles de entrada/salida, mezcla, filtro, realce de graves, mono/estéreo, A/B y presets. Los modelos de máquinas de sampleo están previstos para una fase posterior; todavía no forman parte del motor de audio.

## Descargar e instalar

Los archivos instalables se publican en **Releases** de GitHub cuando se crea una versión con una etiqueta `v*` (por ejemplo, `v0.1.0`). Descarga el archivo correspondiente a tu sistema operativo:

- **Windows x64**: ejecuta `JunkyNoize-Setup-<version>-Windows-x64.exe` y sigue el asistente. El instalador copia el plugin a `C:\Program Files\Common Files\VST3\Junky Noize.vst3` y permite desinstalarlo desde Aplicaciones instaladas.
- **macOS universal**: descomprime el ZIP. Copia `VST3/Junky Noize.vst3` a `~/Library/Audio/Plug-Ins/VST3/`. Opcionalmente, copia `AU/Junky Noize.component` a `~/Library/Audio/Plug-Ins/Components/` para instalar el formato AU.
- **Linux x64**: descomprime el TAR.GZ y copia `VST3/Junky Noize.vst3` a `~/.vst3/`.

Crea la carpeta de destino si no existe y vuelve a escanear plugins en tu DAW. En FL Studio: **Options > Manage plugins > Find installed plugins**. En macOS, si Gatekeeper bloquea el plugin, permite su apertura desde **Ajustes del Sistema > Privacidad y seguridad**.

### Probar una compilación antes de una Release

Cada ejecución correcta del flujo **Compilar Junky Noize** publica instaladores y paquetes temporales en la sección **Actions > ejecución > Artifacts**. Los archivos de **Releases** aparecen al publicar una etiqueta `v*`.

## Compilar desde el código fuente

Se requiere CMake 3.22 o superior y una cadena de herramientas C++ compatible. JUCE 8.0.8 se descarga automáticamente al configurar el proyecto. GitHub Actions compila Windows, macOS universal y Linux al subir cambios o solicitar una ejecución manual desde **Actions**.

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

Para publicar instaladores: crea y sube una etiqueta versionada (por ejemplo, `v0.1.0`). Al terminar la compilación de las tres plataformas, GitHub crea una Release con los archivos listos para descargar.

## Estructura

```
JunkyNoize/
├── CMakeLists.txt
├── .github/workflows/build.yml
├── packaging/
│   ├── package-unix.sh
│   └── windows/JunkyNoize.iss
└── Source/
    ├── PluginProcessor.h/.cpp
    ├── PluginEditor.h/.cpp
    ├── Parameters.h
    ├── Presets.h
    ├── dsp/
    └── ui/
```
