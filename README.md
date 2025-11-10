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

### sine

The `sine(pos, freq, phase)` function returns a sine wave value in the range of [-1, 1].

```c
sine(t, 440)
```

The `phase` parameter is optional and defaults to 0.

Additionally, you can drive a sine by explicit phase using `sinep(phase)`:

```c
sinep(phase) -- returns sin(phase) when you compute phase yourself
```

## Utilities

Short helper functions for timing and envelopes.

- **sec(pos)**

```c
sec(t) -- seconds since start (t / SR)
```

- **step_t(pos, bpm, div, phase)**

```c
step_t(t, 120, 1, 0) -- seconds since start of current step (step = 60/(bpm*div))
```

- **step_p(pos, bpm, div, phase)**

```c
step_p(t, 120, 1, 0) -- normalized phase in current step [0, 1)
```

- **gate(pos, bpm, div, duty, phase)**

```c
gate(t, 120, 1, 0.25, 0) -- 0/1 gate per step, with duty cycle
```

- **env_exp(tau, t_rel)**

```c
env_exp(0.2, step_t(t, 120, 1, 0)) -- exponential decay envelope
```

- **lerp(a, b, x)**

```c
lerp(150.0, 50.0, x) -- linear interpolation from a to b
```

### Higher-level helpers

Convenience functions that encapsulate common livecoding patterns.

- **phase_exp_tb(tb, f0, f1, k)**

```c
// Per-beat phase for exponential frequency sweep f0->f1 with rate k
phase_exp_tb(tb, 150.0, 50.0, 50.0)
```

- **env_trig_exp(pos, bpm, div, tau, phase)**

```c
// Exponential decay envelope retriggered every step
env_trig_exp(t, 120, 1, 0.01, 0)
```

- **tb(pos, bpm, div, phase)** (alias)

```c
// Alias of step_t for brevity
tb(t, 120, 1, 0)
```

### saw

The `saw(pos, freq, phase)` function returns a sawtooth wave value in the range of [-1, 1].

```c
saw(t, 220)
```

The `phase` parameter is optional and defaults to 0.

### sq

The `sq(pos, freq, phase)` function returns a square wave value in the range of [-1, 1].

```c
sq(t, 330)
```

The `phase` parameter is optional and defaults to 0.

### tri

The `tri(pos, freq, phase)` function returns a triangle wave value in the range of [-1, 1].

```c
tri(t, 110)
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
