#ifndef ULTRALEAP_DEVICE_LIST_H
#define ULTRALEAP_DEVICE_LIST_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "device.h"

using namespace godot;

class UltraleapDeviceList : public Resource {
    GDCLASS(UltraleapDeviceList, Resource);

public:

protected:
    static void _bind_methods();
};

#endif