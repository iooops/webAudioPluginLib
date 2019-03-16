## AudioWorklet/WASM with c++ code ported from JUCE
the directory "modules" contains JUCE modules needed to compile the ported c++ code from JUCE project.
the directory "wasm" contains the c++ sdk for Web Audio Modules and the necessary libs for compiling JUCE code
the directory "webOBXD" is cloned from https://github.com/jariseon/webOBXD and will be used as an example to demonstrate how to build JUCE/C++ project to AudioWorklet/WASM.
under this directory, there are 4 subdirectories:
1. "Source": it contains all the JUCE/C++ code, which implements DSP and GUI part of your plugins/synths.
2. "JuceLibraryCode": includes all the JUCE macro definitions and headers files for the JUCE Modules
3. "WAM": the subdirectory "cpp" contains the cpp files which wraps the JUCE/c++ code under the directory "Source" and all the interfaces here are exposed to javascript; the subdirectory "web" contains the html/css/javascript files, and these files can be embedded into a html page.
4. "Builds": it contains project files, and build scripts which are under subdirectory "WAM".

!!new JUCE/C++ projects need to be created following this example project "webOBXD".

## prerequisites
download and install [node.js](https://nodejs.org/en/download/) and WASM [toolchain](https://webassembly.org/getting-started/developers-guide/)

## building
open console, check if emmake command can be found, if not, give the following commands:

"export PATH=$PATH:/to/emsdk/where/emmake/resides"
"cd webOBXD/Builds/WAM"
"sh build.sh"

as for the demo project, the compilation step creates `obxd.wasm.js` and `obxd.emsc.js` files in `WAM/web/worklet` folder. The first file is the WASM binary and the second file contains its loader/support code.

## running
