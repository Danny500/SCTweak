# SCTweak
SimCity 2013 tweaks is written in Microsoft Visual Studio 2010 C++ (free version).

This memory patcher and active debugger enables the user to build bridges in SimCity 2013, wich was initially not supported by the game.
It also provides the user with a simple speed mod.

I have stopped maintaning this mod just a couple of months after the initial release of SimCity 2013.
2 Years later I decide to make the source of this tool publicly available, because examples like these used to help me a lot about understanding reverse engineering and writing my own game mods.

Even though this trainer has the capability of searching trough the memory looking for the magic offsets, it will probably still require you to update some of the hardcoded search patterns to make it work with the latest SimCity 2013 updates. You could look trough the info.txt file for some hints on how to find these offsets.

Some key features that you can find in this source code:
 - Memory pattern matching for dynamic offsets (check bridge mod)
 - Debugging support, using software breakpoints (check speedhack mod)
 - Basic process io handling
 - PatchEngine class that wraps these keyfeatures in to a reusable class for other projects
