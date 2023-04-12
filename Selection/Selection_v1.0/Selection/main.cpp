#include "selection.h"
#include "ToolHead.h"


// Plugin-in declaration
mgDeclarePlugin(
	/* vender name  */ "Sim-support Raymond",
	/* plug-in name */ "Selection",
	/* uuid string  */ "f81132b1-1deb-42ab-8376-c45c566b3b62"
);

static mgmodulehandle module_handle;
static mgresource resource;

static mgbool init_all_tools(mgplugin* plugin, mgresource* resource, int* argc, char* argv[])
{
	//plugin tools ptr

	return init(plugin, resource, argc, argv);
}


// Plug-in initialization
MGPIDECLARE(mgbool) mgpInit(mgplugin plugin, int* argc, char* argv[])
{
	 module_handle = mgGetModuleHandle(plugin);

	// Load the resource file
	 resource = mgLoadResource(module_handle);

	// Register the tools
	mgbool ini_ok = init_all_tools(&plugin, &resource,  argc, argv);

	return ini_ok;
}


MGPIDECLARE(void) mgpExit(mgplugin plugin)
{
	exit_selection();
	mgUnregisterAllTools(plugin);
	mgUnloadResource(resource);
}