# LuaFrontend

LuaFrontend is a standalone script interpreter for PC games. It attaches itself to the PC application and executes Lua scripts. 
It is fully compatible with LuaEngine.

## How do I use LuaEngine mod that I have downloaded with LuaFrontend?

To use a LuaEngine script, all you have to do is to right click the scripts section of the game you want, and click "**Open Script Folder**",  
then drop the script in question to the opened folder. That's it!

However, LuaEngine also has I/O capability. So if a mod you have downloaded also has "**io_load**" or "**io_packages**" directories included, 
you would need to drop those folders to that folder as well.

## How do I use libraries made for Lua with LuaEngine?

The libraries you would need to use must be 5.4.3 compliant. In addition, if the libraries need to be compiled, they must be compiled
for x86_64 and with Lua 5.4.3 with the MSVC 2019 compiler.

Put the libraries on the "**io_packages**" folder in the script folder of your game. To reference them, you can then use the ``require`` command.
Exampe being, if I want to use ``socket.http`` from LuaSocket, which I put in ``scripts/io_packages/LuaSocket/``, I would need to do:

```lua
local http = require("LuaSocket/socket.http")
```

That's it!

## How do I use LuaFrontend to create a LuaEngine mod myself?

Right clock the scripts section of the game you want to make a script for and click "**New Script**".  
The script will be created and opened automatically. You should see something like this:

```lua
LUAGUI_NAME = ""
LUAGUI_AUTH = ""
LUAGUI_DESC = ""

function _OnInit()
   -- Code here executes on script load.
end

function _OnFrame()
   -- Code here executes on every frame.
end
```

The "LUAGUI" globals are for LuaFrontend. They signify the Title, Author and Description sections respectively.  
They are optional globals and not required for operation.

The script in question should have at least one of these functions to work. The "**\_OnInit**" function is executed when the script is first loaded, 
and the "**\_OnFrame**" function is executed every frame. So put the things you want to happen every frame into the "**\_OnFrame**" function.

That is the minimum you would need to have a functioning script. The rest is on you. The scripts in LuaEngine function just as if they were regular Lua scripts.
Because... they kind of are!  

In **DOCUMENT.md**, you can find the cheatsheet to all LuaEngine specific functions.  

## How do I add games to LuaFrontend

Simply open the "**configs/gameConfig.toml**" file and edit it accordingly. You can copy-paste an already-existing game as a template.  
The ability to add games from the GUI will come soon-ish.

## A script errors out. How can I see what's wrong

Hover over the script with your mouse to see warnings.  
If prompted by the tooltip, double click the script to show a more detailed error.

## Important Notes to using LuaEngine

- All values are unsigned.
- There is no limit for the amount of scripts loaded at this moment.

## Third Party Libraries

https://qt.io/ (Qt 5.15.2)  
http://www.lua.org/ (Lua 5.4.3)  
https://github.com/ThePhD/sol2  
https://github.com/d-bahr/CRCpp  
https://github.com/ToruNiina/toml11
https://github.com/discord/discord-rpc  
