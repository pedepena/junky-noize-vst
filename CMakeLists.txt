cmake_minimum_required(VERSION 3.22)

project(JunkyNoize VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE se descarga automaticamente de GitHub la primera vez que se configura el proyecto.
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_binary_data(JunkyNoizeAssets SOURCES Source/ui/assets/JunkyNoizeReference.jpg)

# Formatos de plugin: VST3 en todos los sistemas, AU solo en Mac,
# y una version Standalone (programa independiente) para probarlo sin DAW.
set(JUNKY_FORMATS VST3 Standalone)
if(APPLE)
    list(APPEND JUNKY_FORMATS AU)
endif()

juce_add_plugin(JunkyNoize
    COMPANY_NAME "Junky Noize"
    PLUGIN_MANUFACTURER_CODE Jknz
    PLUGIN_CODE Jnzn
    FORMATS ${JUNKY_FORMATS}
    PRODUCT_NAME "Junky Noize"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES Fx Distortion
    AU_MAIN_TYPE kAudioUnitType_Effect)

file(GLOB_RECURSE JUNKY_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h)

target_sources(JunkyNoize PRIVATE ${JUNKY_SOURCES})
target_include_directories(JunkyNoize PRIVATE Source)

target_compile_definitions(JunkyNoize PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(JunkyNoize
    PRIVATE
        JunkyNoizeAssets
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
