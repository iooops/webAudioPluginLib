// OBXD WAM Processor
// Jari Kleimola 2017-2018 (jari@webaudiomodules.org)

class OBXDAWP extends AudioWorkletGlobalScope.WAMProcessor
{
  constructor(options) {
    options = options || {}
    options.mod = AudioWorkletGlobalScope.WAM.OBXD;
    super(options);
    this.numOutChannels = [2];

    var wam = options.mod;
    var wam_getParameter = wam.cwrap("wam_getParameter", "number", []);
 	console.log(wam_getParameter(0));
  }
}

registerProcessor("OBXD", OBXDAWP);
