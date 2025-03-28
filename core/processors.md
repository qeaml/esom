# Processors

## `fadeIn` and `fadeOut`

Apply a fade-in or fade-out respectively.

Parameters:

* `duration`: duration of the fade in seconds. If the provided buffer is shorter
  than this time, then the fade will be for the entire duration of the buffer.

## `gain`

Change the volume of all audio samples equally.

Parameters:

* `gain`: gain scale, where 0 is silence, 1 is unchanged volume.

## `glitchStretch`

Stretch the provided buffer by a factor by duplicating samples in between.

Parameters:

* `factor`: how much longer should the output be. 1 is the same exact length, 2
  is double the length, 0.5 is half the length, etc.

## `interpStretch`

Stretch the provided buffer by a factor by using an interpolation function.

Parameters:

* `factor`: same as for `glitchStretch`.
* `interp`: what interpolation function to use.

### Interpolation functions

* `0`: linear interpolation.
* `1`: pow2 interpolation, so linear interpolation but squared afterwards.
* `2`: inverse linear interpolation, so linear interpolation but with the values
  inverted.
* `3`: bit-fuck interpolation. The samples are interpolated based on the bits of
  a value calculated from the samples and time. Creates a semi-random noise.

## `hardClip`

Harshly clips any samples that go above the provided threshold.

Parameters:

* `threshold`: max sample value before clipping. Same unit as samples.
* `synched`: whether to synchronize clipping. If this is not `0` and one of the
  channels gets clipped, then the other channel is clipped as well. If this is
  left at `0`, channels are clipped independently.
