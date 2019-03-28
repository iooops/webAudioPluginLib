# JUCE 5 (WebAssembly/AudioWorklet version)

This repo ports the DSP subsection of JUCE library to WebAssembly (WASM). The port enables JUCE plugins to run in web browsers as Web Audio API AudioWorklets. Precompiled static WASM bitcode library (juce-audioworklet.bc) and its build configuration (Makefile and juce\_lib.cpp) are [here](https://github.com/jariseon/JUCE/tree/master/wasm/lib)

The port is bundled into two commits. The [first one](https://github.com/jariseon/JUCE/commit/e4b5ed006e3a3bf0a2518d48a8fae164be883e5d) contains modifications to existing JUCE files (mostly just #ifdefs), while the [second one](https://github.com/jariseon/JUCE/commit/5156a5d30fa4aed0eb77fd2a08679282c49920d6) contains build configuration, Web Audio Module (WAM) wrapper code and some dummy overrides for functionality that is unsupported in WASM and AudioWorklet environments (threads and high resolution timer). Porting was surprisingly straight forward.

## usage

please see this [example](https://github.com/jariseon/webOBXD)

## license

This WASM version of JUCE uses juce\_audio\_basics, juce\_core and juce\_events modules, which are permissively licensed under [ISC](http://www.isc.org/downloads/software-support-policy/isc-license) license. It also uses limited functionality from juce\_audio\_processors and juce\_audio\_plugin\_client modules, which are under JUCE license. Please see the original JUCE [repo] (https://github.com/WeAreROLI/JUCE) for more licensing info.

