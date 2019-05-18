# Building the firmware

## git_revision.h

In a pre-build step, the current git revision
(`git describe --tags | sed 's/-/.post/' | cut -f1 -d'-'`)
is written to `src/git_revision.h` using `scripts/preBuild.py`, e.g.

``` CPP
const char* GIT_REVISION = "0.1.0";
```

Make sure this file exists, otherwise the program will not compile.

## Compile

``` BASH
platformio run
```

The binary is generated at `.pioenvs/teensy32/firmware.hex`

# Unit Testing

### Run Tests

No prerequisites required, script will download and build gtest and arduino-mock
automatically.

```bash
cd test
./build.sh
```