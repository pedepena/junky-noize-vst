# Junky Noize

Plugin de audio (efecto) que imitara el sonido, la calidez y las texturas de varias maquinas de sampleo
clasicas (MPC, S900/S950, SP-1200, SP-404, Emulator, Ensoniq...). Funciona en cualquier DAW,
en Windows, Mac y Linux, y con cualquier controlador MIDI.

Estado actual: **fase 1** (esqueleto + interfaz). Ya hay interfaz completa y controles funcionando
(IN/OUT, MIX, FILTER, BASS BOOST 20-500 Hz, MONO/STEREO, A/B, presets), pero **todavia no estan los
modelos de las maquinas**: llegan en la fase 2.

## Como obtener el plugin compilado (sin instalar nada)

El plugin se compila automaticamente en la nube con GitHub, para Windows, Mac y Linux a la vez.

1. Crea una cuenta gratuita en https://github.com
2. Crea un repositorio nuevo (boton **New**), ponle de nombre `JunkyNoize`, **privado** si quieres.
3. En la pagina del repositorio pulsa **uploading an existing file** y arrastra **todo el contenido**
   de esta carpeta (incluida la carpeta oculta `.github`). Pulsa **Commit changes**.
4. Ve a la pestana **Actions**. Veras tres tareas ("Windows", "macOS", "Linux") trabajando.
   Tardan unos 10-20 minutos la primera vez.
5. Cuando terminen (circulo verde), entra en la ejecucion y abajo, en **Artifacts**, descarga el de tu sistema.
6. Dentro encontraras la carpeta `VST3` (el plugin) y `Standalone` (un programa para probarlo sin DAW).

## Como instalar el VST3

- **Windows**: copia la carpeta `Junky Noize.vst3` a `C:\Program Files\Common Files\VST3`
- **Mac**: copia `Junky Noize.vst3` a `/Library/Audio/Plug-Ins/VST3` (y `Junky Noize.component` a `/Library/Audio/Plug-Ins/Components` si usas AU)
- **Linux**: copia `Junky Noize.vst3` a `~/.vst3`

Despues abre tu DAW y vuelve a escanear los plugins (en FL Studio: *Options > Manage plugins > Find installed plugins*).

> En Mac, como el plugin no esta firmado, el sistema puede bloquearlo la primera vez:
> *Ajustes del Sistema > Privacidad y seguridad > Permitir de todos modos*.

## Estructura del proyecto

```
JunkyNoize/
├── CMakeLists.txt            Receta de compilacion (descarga JUCE automaticamente)
├── .github/workflows/        Compilacion automatica para Windows, Mac y Linux
└── Source/
    ├── PluginProcessor.h/.cpp   El "cerebro": procesa el audio
    ├── PluginEditor.h/.cpp      La ventana del plugin
    ├── Parameters.h             Lista de controles (IN, OUT, MIX, FILTER...)
    ├── Presets.h                Lista de presets
    ├── dsp/                     Todo lo relacionado con el sonido
    │   └── Biquad.h             Filtro basico
    └── ui/                      Todo lo relacionado con el aspecto
        ├── Theme.h              Colores y tipos de letra
        ├── Chassis.h/.cpp       Fondo metalico, tornillos, logo y etiquetas
        ├── KnobLookAndFeel.*    Aspecto de los knobs
        ├── PanelButton.h        Botones y flechas
        ├── LcdDisplay.*         Pantalla verde de presets
        └── LevelMeter.h         Medidores de nivel
```

## Siguientes fases

2. Motor de sonido compartido (bits, frecuencia de muestreo, filtros, saturacion, ruido) y primeras maquinas de referencia.
3. Resto de maquinas, agrupadas por familias.
4. Pulido: presets finales, instaladores, MIDI Learn en la version Standalone.
