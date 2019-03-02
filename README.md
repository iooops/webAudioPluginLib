# Web Audio Plugin Library (First Draft Version)

### Mission: Creating a web audio plugin library that can help people make web audio plugins easier for web DAW hosts.

## Before we start
There're currently several projects going on for making web audio plugins. 

[Web Audio Modules](https://github.com/webaudiomodules)

[Web Audio Plugins](https://github.com/micbuffa/WebAudioPlugins)

Basically, the concept of web audio plugins is to make a dynamic web page/API for the hosts to use. Typically, they can run standalone, or they can also be hosted in a web DAW.


## Intro: Why do we need a web audio plugin library

Typically, to build a native plugin(VST/AU/AAX,etc.), we need to not only code for audio DSP algorithms, but also the GUI part. One of the most general approaches for developing cross-DAW plugins is using the JUCE library, which has a lot of built-in dsp apis and built-in GUIs and is also easily extensible (cabbage/faust are 2 of the choices, too). Similarly, to make things easier, it's good to have a web audio plugin library. 

## Targetted Features

1. **Parameters and Events:** Setters/Getters/onMIDI/onAudio/etc. should be explicit to developers.
2. **Graphical User Interface:** For the GUI part, there should be some generic GUI components, ideally, sliders, knobs, switch buttons, select menus, etc. Also, there should be some ways for making custom GUI components.
3. **Audio Libraries:** Besides built-in web audio nodes, there're some emscripten-compiled audio libraries, like faust/csound/JUCE, porting in audioworklet. It's good to put them directly into the library and get everything set up so that audio developers can directly use them. 
4. **Presets:** Developers should be able to making presets, e.g. storing the predefined parameters and their corresponding names into the preset library.


## Some Questions
Since this project is still on its extremely early stage, there are some questions that are brought out:

1. How to load, instantiate, and connect audio plugins? (from [Web Audio Modules](https://github.com/webaudiomodules))
2. How to make the GUI work correspondingly with parameteres? How to customize them?
3. How to resolve the conflicts of using different libraries, if there's any?
4. How to preserve the presets? JSON? javascript? How to handle preset change => params change?
5. ......

