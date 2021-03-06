#include <script_system.h>
#include <script_system.inl>

#include <luadbg.h>
#include "imgui/cximgui.h"
#include "logger.h"
#include "window.h"
#include "file_loading.h"

#include <file_system.h>
#include "cxlua.h"
#include "net_thread_queue.h"
#include <time/timer_manager.h>


int main(int argc, char* argv[])
{
	handle_command_args(argc, argv);
	FileSystem::InitWorkPath();

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_luadbg(L);
	luaopen_filesystem(L);
	luaopen_cximgui(L);
	luaopen_logger(L);
	luaopen_window(L);
	luaopen_cxlua(L);
	luaopen_timer_manager(L);

	int res = luaL_loadbuffer(L, script_system_lua_code, strlen(script_system_lua_code), "__script_system_lua_code__");
	check_lua_error(L, res);
	res = lua_pcall(L, 0, LUA_MULTRET, 0);
	check_lua_error(L, res);

	res = luaL_dofile(L, command_arg_opt_str("luapath", FileSystem::GetLuaPath("main.lua").c_str()));
	check_lua_error(L, res);
	lua_close(L);

	return 0;
}
