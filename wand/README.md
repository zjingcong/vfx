# Wand
## RESEARCH IMAGES
**wisps**
  1. https://www.shutterstock.com/video/clip-8216416-stock-footage-light-fx-fractal-light-waveforms-ripple-and-shine-loop.html?src=rel/99511:2/3p
  2. https://www.shutterstock.com/video/clip-2000180-stock-footage-music-waveform.html?src=recommended/99511:1/3p
  3. https://www.youtube.com/watch?v=OansHmHngjQ
  
## TO DO LIST
### wand
  - modify wisp class
     - ~~add more parms: offset (random location offset), exp (displacement exponent)~~
     - ~~normalize FSPN1 and FSPN2 value~~
     - ~~clean wisp class~~
  - modify wisps parms
  - build particle class
     - ~~Particle struct: id, lifetime, age, pos, tangent, isActive~~
     - ~~Cloud: std::map\<int, Particle\>: a collection of particle mapping by particle id~~
  - build a wisp cloud for our wand!
     - modify wisp parms
     - add path: create a path class or struct, and locate guide particle along the path
