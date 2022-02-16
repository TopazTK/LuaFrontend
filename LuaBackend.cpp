#include <LuaBackend.hpp>

LuaBackend::LuaBackend() { }

LuaBackend::LuaBackend(const char* ScrPath, uint64_t BaseInput, Console* TargetConsole)
{
    frameLimit = 1000 / 60;
	loadedScripts = vector<LuaScript*>();
    scrPath = ScrPath;

    _outputConsole = TargetConsole;

	LoadScripts(ScrPath, BaseInput);
}

void LuaBackend::LoadScripts(const char* ScrPath, uint64_t BaseInput)
{
	loadedScripts.clear();
    auto _iterator = QDirIterator(ScrPath, QDirIterator::Subdirectories);

    while (_iterator.hasNext())
	{
        auto _path = _iterator.next();

        if (_path.contains(".lua") && !_path.contains("io_packages") && !_path.contains("io_load"))
        {
            LuaScript* _script = new LuaScript();

            _script->scriptPath = _path;

            _script->luaState.open_libraries
            (
                lib::base,
                lib::package,
                lib::coroutine,
                lib::string,
                lib::os,
                lib::math,
                lib::table,
                lib::io,
                lib::bit32,
                lib::utf8
            );

            _script->luaState.set_exception_handler(&ExceptionHandle);

            SetFunctions(&_script->luaState);

            string _luaPath = ScrPath;
            _luaPath.append("\\io_packages\\?.lua");

            string _dllPath = ScrPath;
            _dllPath.append("\\io_packages\\?.dll");

            _script->luaState["package"]["path"] = _luaPath;
            _script->luaState["package"]["cpath"] = _dllPath;

            string _loadPath = ScrPath;
            _loadPath.append("\\io_load");

            _script->luaState["LOAD_PATH"] = _loadPath;
            _script->luaState["SCRIPT_PATH"] = ScrPath;
            _script->luaState["CHEATS_PATH"] = "NOT_AVAILABLE";

            string _pathFull = MemoryLib::PName;
            auto _pathExe = _pathFull.substr(_pathFull.find_last_of("\\") + 1);

            _script->luaState["ENGINE_VERSION"] = 5;
            _script->luaState["ENGINE_TYPE"] = "BACKEND";
            _script->luaState["GAME_ID"] = CRC::Calculate(_pathExe.c_str(), _pathExe.length(), CRC::CRC_32());
            _script->luaState["BASE_ADDR"] = BaseInput;

            _script->parseResult = _script->luaState.script_file(_path.toStdString(), &sol::script_pass_on_error);

            _script->initFunction = _script->luaState["_OnInit"];
            _script->frameFunction = _script->luaState["_OnFrame"];

            string _luaName = _path.split('/').last().toStdString();
            _script->luaState["LUA_NAME"] = _luaName.substr(0, _luaName.size() - 4);

            loadedScripts.push_back(_script);
        }
	}
}

void LuaBackend::SetFunctions(LuaState* _state)
{
    // Reader Functions

    _state->set_function("ReadByte",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadByte(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadByte(_addr, _absolute); }
    ));

    _state->set_function("ReadShort",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadShort(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadShort(_addr, _absolute); }
    ));

    _state->set_function("ReadInt",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadInt(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadInt(_addr, _absolute); }
    ));

    _state->set_function("ReadLong",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadLong(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadLong(_addr, _absolute); }
    ));

    _state->set_function("ReadFloat",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadFloat(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadFloat(_addr, _absolute); }
    ));

    _state->set_function("ReadBoolean",
    sol::overload
    (
        [](uint64_t _addr) { return MemoryLib::ReadBool(_addr); },
        [](uint64_t _addr, bool _absolute) { return MemoryLib::ReadBool(_addr, _absolute); }
    ));

    _state->set_function("ReadArray",
    sol::overload
    (
        [](uint64_t _addr, int _len) { return MemoryLib::ReadBytes(_addr, _len); },
        [](uint64_t _addr, int _len, bool _absolute) { return MemoryLib::ReadBytes(_addr, _len, _absolute); }
    ));

    _state->set_function("ReadString",
    sol::overload
    (
        [](uint64_t _addr, int _len) { return MemoryLib::ReadString(_addr, _len); },
        [](uint64_t _addr, int _len, bool _absolute) { return MemoryLib::ReadString(_addr, _len, _absolute); }
    ));

    // Writer Functions

    _state->set_function("WriteByte",
    sol::overload
    (
        [](uint64_t _addr, uint8_t _val) { MemoryLib::WriteByte(_addr, _val); },
        [](uint64_t _addr, uint8_t _val, bool _absolute) { MemoryLib::WriteByte(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteShort",
    sol::overload
    (
        [](uint64_t _addr, uint16_t _val) { MemoryLib::WriteShort(_addr, _val); },
        [](uint64_t _addr, uint16_t _val, bool _absolute) { MemoryLib::WriteShort(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteInt",
    sol::overload
    (
        [](uint64_t _addr, uint32_t _val) { MemoryLib::WriteInt(_addr, _val); },
        [](uint64_t _addr, uint32_t _val, bool _absolute) { MemoryLib::WriteInt(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteLong",
    sol::overload
    (
        [](uint64_t _addr, uint64_t _val) { MemoryLib::WriteLong(_addr, _val); },
        [](uint64_t _addr, uint64_t _val, bool _absolute) { MemoryLib::WriteLong(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteFloat",
    sol::overload
    (
        [](uint64_t _addr, float _val) { MemoryLib::WriteFloat(_addr, _val); },
        [](uint64_t _addr, float _val, bool _absolute) { MemoryLib::WriteFloat(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteBoolean",
    sol::overload
    (
        [](uint64_t _addr, bool _val) { MemoryLib::WriteBool(_addr, _val); },
        [](uint64_t _addr, bool _val, bool _absolute) { MemoryLib::WriteBool(_addr, _val, _absolute); }
    ));

    _state->set_function("WriteArray",
    sol::overload
    (
        [](uint64_t _addr, vector<uint8_t> _val) { MemoryLib::WriteBytes(_addr, _val); },
        [](uint64_t _addr, vector<uint8_t> _val, bool _absolute) { MemoryLib::WriteBytes(_addr, _val, _absolute); }

    ));

    _state->set_function("WriteString",
    sol::overload
    (
        [](uint64_t _addr, string _val) { MemoryLib::WriteString(_addr, _val); },
        [](uint64_t _addr, string _val, bool _absolute) { MemoryLib::WriteString(_addr, _val, _absolute); }
    ));

	_state->set_function("WriteExec", MemoryLib::WriteExec);

    // Misc. Functions

	_state->set_function("ConsolePrint", 
		sol::overload(
            [this](LuaObject Text)
			{
                 auto _text = QString::fromStdString(Text.as<string>());
                 _outputConsole->printMessage(_text, 0);
			}, 

            [this](LuaObject Text, int MessageType)
			{
                auto _text = QString::fromStdString(Text.as<string>());
                _outputConsole->printMessage(_text, MessageType);
			}
		)
	);

    _state->set_function("GetHertz", [this]() { return floor(1000 / frameLimit); });
    _state->set_function("SetHertz", [this](int _input) { frameLimit = floor(1000 / _input); });

    _state->set_function("ULShift32", Operator32Lib::UnsignedShift32);

    // Discord Functions

    _state->set_function("InitializeRPC", DCInstance::InitializeRPC);
    _state->set_function("UpdateDetails", DCInstance::UpdateDetails);
    _state->set_function("UpdateState", DCInstance::UpdateState);
    _state->set_function("UpdateLImage", DCInstance::UpdateLImage);
    _state->set_function("UpdateSImage", DCInstance::UpdateSImage);
}
