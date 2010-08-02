#include "common.h"
#include "util.h"

#define MOD_NAME "loader"
#define AUTHOR "Zinx Verituse <zinx@zenthought.org>"
#define FULL_NAME MOD_NAME " by "AUTHOR" [built @ "__DATE__" "__TIME__"]"

#define INI_NAME MOD_NAME ".ini"

static int modules;
static HMODULE module[4096];

static void loader_ini_load(void)
{
	ini_t *ini = ini_load(INI_NAME);

	modules = 0;

	if (!ini) return;

	while (ini_seek_section(ini, "loader")) {
		while (ini_seek_setting(ini, NULL)) {
			char *value = ini_value(ini);
			module[modules] = LoadLibrary(value);
			if (module[modules] != NULL) {
				LOG("Loaded %s at %p", value, modules[module]);
				++modules;
			} else {
				LOG("Failed to loaded %s", value);
			}
		}
	}

	ini_free(ini);
}

static void loader_exit()
{
	/* Reverse unload order is intentional. */
	while (modules--) {
		FreeLibrary(module[modules]);
	}
	modules = 0;
}

WINAPI BOOL DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		loader_ini_load();
		LOG(FULL_NAME " loaded %d modules", modules);
		return modules != 0;
	case DLL_PROCESS_DETACH:
		loader_exit();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
