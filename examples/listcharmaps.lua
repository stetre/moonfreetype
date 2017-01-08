#!/usr/bin/env lua
-- MoonFreeType example: listcharmaps.lua

ft = require("moonfreetype")

fontfile = arg[1]
if not fontfile then print("Usage: " .. arg[0] .. " <fontfile>") return end

-- Create a library:
lib = ft.init_freetype()
-- Create a face, loading from fontfile:
face = ft.new_face(lib, fontfile)

-- List all the available charmaps:
charmaps = face:charmaps()
for i,c in ipairs(charmaps) do
   print("charmap", i)
   print("  encoding", c.encoding)
   print("  platform_id", c.platform_id, "("..c.platform_name..")")
   print("  encoding_id", c.encoding_id, "("..c.encoding_name..")")
end

face:select_charmap(1) -- select by charmap index

lib:done()
