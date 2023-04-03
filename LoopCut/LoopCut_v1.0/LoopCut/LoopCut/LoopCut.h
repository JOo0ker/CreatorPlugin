#pragma once

#include <mgapiall.h>

#include "ToolHead.h"
#include "ControlCallbackFunc.h"
#include "LoopCutFunc.h"
#include "resource.h"

/**
 * \brief loop set tool start function, especially get the plugin tool struct
 * \param plugin_tool the plug-in tool
 * \param user_data PLUGINTOOLSTRUCT
 * \param call_data mgeditorcallbackrec
 * \return always return <m MSTAT_OK>
 */
static mgstatus loop_cut_start_func(mgplugintool plugin_tool,
                                    void* user_data,
                                    void* call_data);

/**
 * \brief create dialog and set callback function
 * \param plugin_tool the plugin-in tool
 * \param tool_data PLUGINTOOLSTRUCT
 * \return tool gui dialog / MG_NULL
 */
static mggui create_dialog_func(mgplugintool plugin_tool,
                                void* tool_data);

/**
 * \brief initialize dialog control callbacks and  dialog data
 * \param dialog the dialog
 * \param dialog_id the identifier of the dialog
 * \param callback_reason the dialog event that triggered the callback
 * \param user_data PLUGINTOOLSTRUCT
 * \param call_data mgguicallbackrec
 * \return MSTAT_OK
 */
static mgstatus dialog_proc(mggui dialog,
                       mgdialogid dialog_id,
                       mgguicallbackreason callback_reason,
                       void* user_data,
                       void* call_data);

/**
 * \brief no in use; only for test
 * \param editor_context the editor context that identifies the editor instance receiving the pick input
 * \param vertex_input_data mgvertexinputdata, the pick vertex input data
 * \param tool_data PLUGINTOOLSTRUCT
 * \return Return 1 if the 3D vertex coordinates passed in should be copied to Reference Coordinate in Track Coordinate Window, 0 otherwise.
 */
static int vertex_func(mgeditorcontext editor_context,
                       mgvertexinputdata* vertex_input_data,
                       void* tool_data);

/**
 * \brief no in use; only for test
 * \param editor_context the editor context that identifies the editor instance receiving the pick input
 * \param point_input_data mgpointinputdata, the pick point input data
 * \param tool_data PLUGINTOOLSTRUCT
 */
static void point_func(mgeditorcontext editor_context,
                       mgpointinputdata* point_input_data,
                       void* tool_data);

/**
 * \brief Check if the selected content is an edge and call the loop cut function
 * \param editor_context the editor context that identifies the editor instance receiving the pick input
 * \param pick_input_data mgpickinputdata, the pick input data
 * \param tool_data PLUGINTOOLSTRUCT
 */
static void pick_func(mgeditorcontext editor_context,
                      mgpickinputdata* pick_input_data,
                      void* tool_data);

/**
 * \brief register editor and set editor's functions
 * \param plugin loop cut plugin handle
 * \param resource tool resource handle
 * \param argc ...
 * \param argv ...
 * \return MG_TRUE/MG_FALSE
 */
mgbool init(mgplugin* plugin, mgresource* resource, int* argc, char* argv[]);

/**
 * \brief reserved function
 * \param plugin loop cut plugin handle
 */
void exit_loop_cut(mgplugin plugin);

/**
 * \brief invoke the loop cut execute function
 * \param pt_s PLUGINTOOLSTRUCT
 */
void generate(plugintool_struct* pt_s);

/**
 * \brief no in use; only for test
 * \param gui tool gui
 * \param control_id gui control id
 * \param callback_reason the dialog control event that triggered the callback
 * \param user_data PLUGINTOOLSTRUCT
 * \param call_data ...?
 * \return MSTAT_OK
 */
static mgstatus generate_callback(mggui gui,
                                  mgcontrolid control_id,
                                  mgguicallbackreason callback_reason,
                                  void* user_data,
                                  void* call_data);

/**
 * \brief initialize gui control callbacks
 * \param pt_s PLUGINTOOLSTRUCT
 */
static void initialize_control_callbacks(plugintool_struct* pt_s);

/**
 * \brief initialize dialog
 * \param pt_s PLUGINTOOLSTRUCT
 * \return MG_FALSE/MSTAT_OK
 */
static mgstatus initialize_dialog(plugintool_struct* pt_s);

/**
 * \brief editor termination callback functions; invoke when gui is terminated
 * \param editor_context the editor context that identifies the editor instance being terminated
 * \param reason the reason editor is being terminated : MTRM_DONE/MTRM_CANCEL/MTRM_SELF/MTRM_SYSTEM
 * \param tool_data PLUGINTOOLSTRUCT
 */
static void terminate_func(mgeditorcontext editor_context,
                           mgtoolterminationreason reason,
                           void* tool_data);

/**
 * \brief cut face loop and free face loop
 * \param pt_s PLUGINTOOLSTRUCT
 */
static void loop_cut_execute(plugintool_struct* pt_s);