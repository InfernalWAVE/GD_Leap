#include "register_types.h"
#include "HelloWorld.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_hello_world(ModuleInitializationLevel p_level){
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }

    ClassDB::register_class<HelloWorld>();
}

void uninitialize_hello_world(ModuleInitializationLevel p_level){
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
    // potential cleanup
}

extern "C"{
    GDExtensionBool GDE_EXPORT hello_world_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_intitialization){
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_intitialization);

        init_obj.register_initializer(initialize_hello_world);
        init_obj.register_terminator(uninitialize_hello_world);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}