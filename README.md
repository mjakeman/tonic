# tonic
A game engine

> In music, the tonic is the first scale degree of the diatonic
scale. Scales are named after their tonics: for instance, the tonic of the
C major scale is the note C.

That's definitely what it means ;)

## Building
In order to build, you will need:
 * meson (build system)
 * a c++ compiler

Run the following commands:
```sh
# Run meson to configure the project. It will
# use '_build' as the build directory.
$ meson _build

# Run ninja to build
$ ninja -C _build

# Run the built program
$ _build/tonic
```

Have fun!
