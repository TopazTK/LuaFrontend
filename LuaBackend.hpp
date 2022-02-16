#ifndef LUABACKEND
#define LUABACKEND

#include <iostream>
#include <sol.hpp>

#include <CRC32.h>
#include <Console.hpp>

#include <MemoryLib.hpp>
#include <DCInstance.hpp>
#include <Operator32Lib.hpp>

#include <QDirIterator>

using namespace sol;
using namespace std;

using LuaError = sol::error;
using LuaState = sol::state;
using LuaObject = sol::object;
using LuaFunction = sol::safe_function;
using LuaResult = sol::protected_function_result;

class LuaBackend
{
	public:
		struct LuaScript
		{
			LuaState luaState;
            LuaResult parseResult;
			LuaFunction initFunction;
			LuaFunction frameFunction;

            QString scriptPath;
		};

        float frameLimit;
        string scrPath;

		std::vector<LuaScript*> loadedScripts;

        static int ExceptionHandle(lua_State* luaState, sol::optional<const std::exception&> thrownException, sol::string_view)
		{
            const std::exception _ex = *thrownException;
			return sol::stack::push(luaState, _ex.what());
		}

		void SetFunctions(LuaState*);
		void LoadScripts(const char*, uint64_t);

        LuaBackend();
        LuaBackend(const char*, uint64_t, Console*);

    private:
        Console* _outputConsole;
};

#endif
