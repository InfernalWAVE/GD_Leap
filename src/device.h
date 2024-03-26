#ifndef ULTRALEAP_DEVICE_H
#define ULTRALEAP_DEVICE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <LeapC.h>

#include <mutex>

#include "types.h"
#include "frame.h"

using namespace godot;

class UltraleapDevice: public Resource{
    GDCLASS(UltraleapDevice, Resource);

public:

protected:
    static void _bind_methods();



};

#endif