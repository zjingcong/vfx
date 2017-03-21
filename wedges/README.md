# Wedges
This program is to generate noise/pyrosphere/wisp wedges by modifying parms:
- noise: octaves, freq, fjump, fade
- pyrosphere: octaves, freq, fjump, gamma
- wisp: octaves, freq, fjump, clump

All wedges are with the key light and the rim light and no emission color.

### Overview Demo
[![ScreenShot](https://cloud.githubusercontent.com/assets/16331066/24129538/aedc0506-0db9-11e7-8aa3-0d45c2f3ba62.png)](https://vimeo.com/209309886)
### Compilation
```sh
$ cd cmake-build-default
$ make
```
### Usage
```sh
$ ./wedges type_tag frame_id [output_dir]
```
- type_tag

	-p: pyro
	-n: noise
	-w: wisp

