##MoonFreeType: Lua bindings for FreeType

MoonFreeType is a Lua binding library for the 
[FreeType library](https://www.freetype.org/).

It runs on GNU/Linux and on Windows (MSYS2/MinGW) and requires 
[Lua](http://www.lua.org/) (>=5.3) and 
[FreeType](https://www.freetype.org/download.html) (>=2.6.1).

_Authored by:_ _[Stefano Trettel](https://www.linkedin.com/in/stetre)_

[![Lua logo](./doc/powered-by-lua.gif)](http://www.lua.org/)

#### License

MIT/X11 license (same as Lua). See [LICENSE](./LICENSE).

#### Documentation

See the [Reference Manual](https://stetre.github.io/moonfreetype/doc/index.html).

#### Getting and installing

Setup the build environment as described [here](./SETUP.md), then:

```sh
$ git clone https://github.com/stetre/moonfreetype
$ cd moonfreetype
moonfreetype$ make
moonfreetype$ make install # or 'sudo make install' (Ubuntu)
```

#### Example

The script below loads a font face from a file and lists the contained glyphs:

```lua
-- MoonFreeType example: listglyphs.lua

ft = require("moonfreetype")

fontfile = arg[1]
if not fontfile then print("Usage: " .. arg[0] .. " <fontfile>") return end

-- Create a library:
lib = ft.init_freetype()

-- Create a face, loading from fontfile:
face = ft.new_face(lib, fontfile)

-- List all the glyphs (character code - glyph index - glyph name):
charcode, index = face:get_first_char()
while index do
   print(charcode, index, face:get_glyph_name(index))
   charcode, index = face:get_next_char(charcode)
end

lib:done()

```

The script can be executed at the shell prompt with the standard Lua interpreter,
passing it a font file name, e.g.:

```shell
$ lua listglyphs.lua myfavouritefont.ttf
```

Other examples can be found in the **examples/** directory.

#### See also

* [MoonLibs - Graphics and Audio Lua Libraries](https://github.com/stetre/moonlibs).

