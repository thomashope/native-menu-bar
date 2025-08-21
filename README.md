# Native Menu Bar

[![Mac Build](https://github.com/thomashope/native-menu-bar/actions/workflows/mac-build.yml/badge.svg)](https://github.com/thomashope/native-menu-bar/actions/workflows/mac-build.yml) [![Windows Build](https://github.com/thomashope/native-menu-bar/actions/workflows/windows-build.yaml/badge.svg)](https://github.com/thomashope/native-menu-bar/actions/workflows/windows-build.yaml)

## Building

In general you can use this library by simply adding `native_menu_bar.h` and `native_menu_bar.c` to your project.

On mac `native_menu_bar.c` should be compiled as Objective-C. You can do this by either changing the extension from `.c` to `.m`, or explicitly set it to compile as Objective-C within XCode.

## Building Examples

On Mac:

```sh
brew install sdl2
cd examples
./premake xcode4
open ./build/xcode4/examples.xcworkspace
```

Then select and run the example projects from within Xcode.

On Windows:

Run the following commands in Git Bash (or some other bash for windows prompt).

```sh
cd examples
./premake vs2022
start ./build/vs2022/examples.sln
```

## Contributing

Contributions are welcome. See [TODO](TODO.md) for things to work on and notes about current progress.
