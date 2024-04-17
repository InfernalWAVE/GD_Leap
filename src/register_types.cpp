#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "ultraleap.h"
#include "bone.h"
#include "digit.h"
#include "palm.h"
#include "hand.h"
#include "frame.h"
#include "types.h"
#include "device.h"
#include "device_node.h"
#include "device_list.h"

using namespace godot;

void initialize_gd_leap(ModuleInitializationLevel p_level){
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
    ClassDB::register_class<UltraleapHandTracking>();
    ClassDB::register_class<UltraleapBone>();
    ClassDB::register_class<UltraleapDigit>();
    ClassDB::register_class<UltraleapHand>();
    ClassDB::register_class<UltraleapPalm>();
    ClassDB::register_class<UltraleapFrame>();
    ClassDB::register_class<UltraleapTypes>();
    ClassDB::register_class<UltraleapDevice>();
    ClassDB::register_class<UltraleapDeviceNode>();
    ClassDB::register_class<UltraleapDeviceList>();
}

void uninitialize_gd_leap(ModuleInitializationLevel p_level){
    if(p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
    // potential cleanup
}

extern "C"{
    GDExtensionBool GDE_EXPORT gd_leap_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_intitialization){
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_intitialization);

        init_obj.register_initializer(initialize_gd_leap);
        init_obj.register_terminator(uninitialize_gd_leap);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}