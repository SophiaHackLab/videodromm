Cinder-Hap
==========

CinderHap enables the [Hap](http://vdmx.vidvox.net/blog/hap) codec on [Cinder](http://github.com/cinder/Cinder).

Hap is an [open source](https://github.com/Vidvox) video codec for fast decompression on modern graphics hardware.
Usually when a movie is being played, the CPU has to decompress every frame before passing them to the graphics card (GPU).
Hap passes the compressed frames to the GPU, who does all the decompression, saving your precious CPU from this work.

Hap was developed by [Tom Butterworth](https://twitter.com/bang_noise) and commissioned by [Vidvox](http://vidvox.net/).

For general information about Hap, read the [the Hap announcement](http://vdmx.vidvox.net/blog/hap).

For technical information about Hap, see [the Hap project](http://github.com/vidvox/hap).

The Hap codec is developed for Mac OSX only, but a Windows version is in the works.


Open-Source
===========

This code is released under the Modified BSD License, same as [Cinder](http://libcinder.org/).

This project was originally written by [Roger Sodr�](http://www.studioavante.com/), 2013.

Rewrite for latest cinder version, and windows port by Eric Renaud-Houde, MPC NY, 2014.
