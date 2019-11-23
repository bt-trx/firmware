# Building the firmware

## Prerequisites

Make sure these files exist, otherwise the program will not compile.

### git_revision.h

In a pre-build step, the current git revision
(`git describe --tags`)
is written to `src/git_revision.h` using `scripts/preBuild.py`, e.g.

``` CPP
const char* GIT_REVISION = "0.1.0";
```

### website.h

To be able to have a usable website (`index.html`) in the repository,
the include file for the firmware is generated just before compilation.
It is also generated within a pre-build step, using the script

```
scripts/generateWebsite.py
```

## Compile

``` BASH
platformio run
```

The binary is generated at `.pio/build/esp32/firmware.bin`

## Run Tests

No prerequisites required, script will download and build gtest and arduino-mock
automatically.

```bash
cd test
./build.sh
```

## Lint

Coding style is fixed by clang-format

``` BASH
./tools/code-style.sh src/*.cpp src/*.h test/*.cpp test/*.h
```
