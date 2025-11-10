# bbx

A minimalistic musical livecoding environment designed to run on resource-constrained platforms such as the PocketCHIP.

Inspired on Bytebeat, it uses a simple audio synthesis approach based on a single audio generator function.

The generator function outputs a value in the range of [-1, 1] for each sample, which is then converted to a sound wave and played back. It's written in C.

The environment is designed to be simple and easy to use, with a focus on minimalism and accessibility.

## API

bbx provides a simple API for audio synthesis.

### spl

The `spl(path, pos)` function returns a sample value in the range of [-1, 1] from an audio file.

```c
spl("/path/to/audio/file.wav", t) -- returns a sample value in the range of [-1, 1] from the audio file at position t
```

### sin

The `sin(pos, freq, phase)` function returns a sine wave value in the range of [-1, 1].

```c
sin(pos, freq, phase) -- returns a sine wave value in the range of [-1, 1]
```

The `phase` parameter is optional and defaults to 0.

### saw

The `saw(pos, freq, phase)` function returns a sawtooth wave value in the range of [-1, 1].

```c
saw(pos, freq, phase) -- returns a sawtooth wave value in the range of [-1, 1]
```

The `phase` parameter is optional and defaults to 0.

### sq

The `sq(pos, freq, phase)` function returns a square wave value in the range of [-1, 1].

```c
sq(pos, freq, phase) -- returns a square wave value in the range of [-1, 1]
```

The `phase` parameter is optional and defaults to 0.

### tri

The `tri(pos, freq, phase)` function returns a triangle wave value in the range of [-1, 1].

```c
tri(pos, freq, phase) -- returns a triangle wave value in the range of [-1, 1]
```

The `phase` parameter is optional and defaults to 0.

### rnd

The `rnd()` function returns a random float value in the range of [0, 1].

```c
rnd() -- returns a random float value in the range of [0, 1]
```

### rndf

The `rndf(max)` function returns a random float value in the range of [0, max].

```c
rndf(max) -- returns a random float value in the range of [0, max]
```

A max value of 1 is used by default.

The alternative signature `rndf(min, max)` returns a random float value in the range of [min, max].

```c
rndf(min, max) -- returns a random float value in the range of [min, max]
```

### rndi

The `rndi(max)` function returns a random integer value in the range of [0, max].

```c
rndi(max) -- returns a random integer value in the range of [0, max]
```

The alternative signature `rndi(min, max)` returns a random integer value in the range of [min, max].
