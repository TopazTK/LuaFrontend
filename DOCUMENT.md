## LuaEngine Cheatsheet v5.10

---

## Global Variables

- ``GAME_ID`` => Returns the current game's CRC32 checksum as an integer. This checksum is generated from the process name at this moment.
- ``LUA_NAME`` => The name of your Lua script, used by functions such as ConsolePrint. It is the filename of your Lua by default.
- ``SCRIPT_PATH`` => Returns the path which Lua scripts are loaded from as a string. Ex: ``"C:\Users\Topaz\Desktop\LuaBackend\scripts"``
- ``CHEATS_PATH`` => Always returns ``"NOT_AVAILABLE"``
- ``ENGINE_VERSION`` => Returns the LuaBackend's engine version as a float. Ex: ``5``
- ``ENGINE_TYPE`` => Always returns ``"BACKEND"``

## Memory Functions

### Read\[XXXX\](Address, Absolute = false) / ReadString(Address, Length, Absolute = false)

Reads a certain value from **Address**. Returns the read value. If reading a String, length must also be declared.  
The "**XXXX**" is a placeholder for the value type.  
  
If **Absolute** is true, the address is taken as written and base address is not added into it.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    local _tempInt = ReadInt(0xDEADB00) -- Read an integer from address BaseAddress+0xDEADB00
    local _tempByte = ReadByte(0xB007555) -- Read a byte from address BaseAddress+0xB007555
```

### Write\[XXXX\](Address, Value, Abolsute = false)

Writes **Value** to **Address** calculated relative to the base address. 
The "**\[XXXX\]**" is a placeholder for the value type.  
  
If **Absolute** is true, the address is taken as written and base address is not added into it.  
  
List of types available: Byte, Short, Int, Long, Float, String, Boolean

Example:
```lua
    WriteShort(0xDEADB00, 0xFFFF) -- Write 0xFFFF to BaseAddress+0xDEADB00
    WriteBoolean(0xB007555, true) -- Write true to BaseAddress+0xB007555
```


### ReadArray(Address, Length, Absolute = false)

Reads **Length** amount of bytes from memory, starting at **Address**. Returns the read bytes as an array.  
If **Absolute** is true, the address is taken as written and base address is not added into it.  

Example:
```lua
    local _tempArray = ReadArray(0xBEEFDED, 0x10) -- Reads 16 bytes starting at BaseAddress+0xBEEFDED
```

### WriteArray(Address, Array, Absolute = false)

Writes **Array** to the memory, starting at **Address**. The array must be made of bytes (8-bit integers).  
If **Absolute** is true, the address is taken as written and base address is not added into it.  

Example:
```lua
    local _tempArray = { 0xFE, 0xA5, 0x70 }
    WriteArray(0x6660420, _tempArray) -- Writes _tempArray to memory, starting at BaseAddress+0x6660420
```

### GetPointer(Address, Offset, Absolute = false)

Calculates a pointer with the given values.  
Basically, reads the value at **Address** and adds **Offset** to it. Returns the resulting value.

If **Absolute** is true, the address is taken as written and base address is not added into it.

Example:
```lua
    local _tempPointer = GetPointer(0xBEEFDAD, 0x15) -- Reads the value at BaseAddress+0xBEEFDAD and adds 0x15 to it.
```

## General I/O Functions

### GetHertz()

Gets the frequency of which LuaBackend scripts execute.

### SetHertz(Frequency)

Sets the Backend execution cycle to **Frequency**. The accepted values are **60**, **120**, and **240**.

### ULShift32(Base, Shift)

Shifts **Base** left by **Shift** abont of bytes. Only exists for 32-bit applications.

Example:
```lua
    ULShift32(4294967295, 8) -- Shifts 4294967295 left by 8 bytes without an overflow to 64-bits.
```

### ConsolePrint(Text, Type = NULL)

Prints **Text** to the console, in the format given below. 
Unless **Type** is declared, nothing will prefix the message.

**Type** can be the following:
- 0 = MESSAGE
- 1 = SUCCESS
- 2 = WARNING
- 3 = ERROR

Format:
```
[LUA_NAME] TYPE: Text
```

Example:
```lua
    LUA_NAME = "SomeDudeScript"
    ConsolePrint("GET UP ON THE HYDRA'S BACK!") -- Prints the following: [SomeDudeScript] GET UP ON THE HYDRA'S BACK!
    ConsolePrint("NICE VIEW FROM UP HERE!", 0) -- Prints the following: [SomeDudeScript] MESSAGE: NICE VIEW FROM UP HERE!
```

## Discord RPC Functions

### InitializeRPC(ApplicationID)

Initializes and hooks a Discord Application with **ApplicationID**, for Rich Presence control.

Example:
```lua
    InitializeRPC("833511404274974740") -- Initiaizes Discord Application "833511404274974740".
```

### UpdateDetails(Text)

Updates the details of the rich presence application initialized with InitializeRPC to say **Text**.

Example:
```lua
    UpdateDetails("Hello World!") -- Updates the details section of the RPC to say "Hello World!".
```

### UpdateState(Text)

Updates the state of the rich presence application initialized with InitializeRPC to say **Text**.

Example:
```lua
    UpdateState("Hello World!") -- Updates the state section of the RPC to say "Hello World!".
```

### UpdateLImage(Key, Text)

Updates the large image of the rich presence to be the image with **Key**. And it's text is updates to say **Text**.

Example:
```lua
    UpdateLImage("image", "This is Image!") -- Updates the large image to "image" and it's text to "This is Image!"
```

### UpdateSImage(Key, Text)

Updates the small image of the rich presence to be the image with **Key**. And it's text is updates to say **Text**.

Example:
```lua
    UpdateSImage("imagetwo", "This too, is Image!") -- Updates the small image to "imagetwo" and it's text to "This too, is Image!"
```

### WriteExec(Address, Value)

Writes the given byte array **Value** to **Address** relative to the current EXE's address, NOT the provided base address.
This function is non-existent on PCSX2-EX.

Example:
```lua
    WriteExec(0x00FBAAD, 0x15) -- Writes 0x15 to EXE_NAME.exe+0x00FBAAD.

```

#### * Any functions present in this document signify compatibility with the latest version. Any function absent in this document is no longer supported and should be considered obsolete.
