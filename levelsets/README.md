# Levelsets

>Jingcong Zhang		2017-03-02

This program is to render levelsets generated from polygon obj file or implicit functions. 
It will convert polygon mesh to levelset and stamp implicit functions to openVDB sparse grid.
It will do lights using deep shadow map.
It can render levelsets and the combination of levelsets and implicit functions.
In this program, key light, rim light and back light are used, and levelsets have no emission color.

### Overview Demo
[![ScreenShot](https://cloud.githubusercontent.com/assets/16331066/24186444/aeba0226-0eae-11e7-89a4-0663cd07fd29.png)](https://vimeo.com/209519029)

### Compilation
```sh
$ make
```

### Usage
```sh
$ ./levelsets frameStart frameNum [model_tag]
```
[type_tag]

  -b: bunny; 
  -t: teapot; 
  -h: hybrid of bunny, teapot and humanoid made from implicit functions

