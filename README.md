<div align="center">
    <h1>Yugioh Image Guesser</h1>
    <em>A zoom-out image guesser software for any image-based subject</em>
</div>


## Plans (As of May 14, 2026)
I need to add an autoupdater to be able to download the files easily so I can host them here for future sets. (Using Libcurl) (Planned)\
Add other TCGS implentations / Subjects (Planned)\
Add easy to USER Interface for customization (Any subjects / File) (?Planned?)

I am currently working on a game so I might not spend much more time on this so feel free to rip and tear!


## Binaries

Pre-built binaries for Windows and Linux can be found on the [GitHub Releases](https://github.com/Retrorage/ImageGuesser/releases) page.

The pre-built binaries contain all dependencies and assets within the archive, so it can be ran out of the box.


## Dependencies

- [SFML](https://github.com/sfml/sfml)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- C++17 (or later) compatible standard library

<br/>

Additionally, the `assets` folder must be in the same directory as the executable at runtime.


## Building from Source

> The msvc compiler is not supported when building through cmake, as the project uses clang/gcc-style compiler flags.
> If building on windows, use `clang-cl` or a [mingw](https://www.mingw-w64.org/) environment instead

Building from source is done through cmake. For example:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja
cmake --build . -j $(nproc)
```

If building for Windows on Unix, you can use the provided toolchain file:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja --toolchain ../cmake/x86_64-w64-mingw32.cmake
cmake --build . -j $(nproc)
```


## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
