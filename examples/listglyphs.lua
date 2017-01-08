#!/usr/bin/env lua
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
