#include "register_types.h"

#include "droplet_body3d.h"
#include "droplet_server.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_droplet_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
		return;
	}

	ClassDB::register_class<DropletBody3D>();
    ClassDB::register_class<DropletServer>();
}

void uninitialize_droplet_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
		return;
	}
}

extern "C"
{
    // Initialization
    GDExtensionBool GDE_EXPORT droplet_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_droplet_module);
        init_obj.register_terminator(uninitialize_droplet_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}