#include <new>

#include <mgapiall.h>
#include "resource.h"
#include "LoopCut.h"
#include "ToolHead.h"


// Plugin-in declaration
mgDeclarePlugin(
	/* vender name  */ "Sim-support Raymond",
	/* plug-in name */ "Loop cut",
	/* uuid string  */ "a4f3e3fe-2049-4cbd-81b8-c5c27b4a65e1"
);

static mgmodulehandle module_handle;
static mgresource resource;

/**
 * \brief initialize all sub-tools
 * \param plugin main plugin
 * \param resource resource
 * \param argc argc
 * \param argv argv
 * \return MG_TRUE/MG_FALSE
 */
static mgbool init_all_tools(mgplugin* plugin, mgresource* resource, int* argc, char* argv[])
{
	 //plugin tools ptr

	return init(plugin, resource, argc, argv);
}

/// <summary>
/// initialize main plugin and device functions
/// </summary>
/// <param name="plugin">main plugin</param>
/// <param name="argc">argc</param>
/// <param name="argv">argv</param>
/// <returns>MG_TRUE/MG_FALSE</returns>
MGPIDECLARE(mgbool) mgpInit(mgplugin plugin, int* argc, char* argv[])
{
	module_handle = mgGetModuleHandle(plugin);
	resource = mgLoadResource(module_handle);

	mgbool ini_ok = MG_FALSE;

	ini_ok = init_all_tools(&plugin, &resource, argc, argv);

	return ini_ok;
}

/// <summary>
/// free/delete/unregister/unload functions
/// </summary>
/// <param name="plugin">main plugin</param>
/// <returns>void</returns>
MGPIDECLARE(void) mgpExit(mgplugin plugin)
{
	// Terminate the tool declared
	exit_loop_cut(plugin);

	mgUnregisterAllTools(plugin);
	mgUnloadResource(resource);
}