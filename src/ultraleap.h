#ifndef ULTRALEAP_H
#define ULTRALEAP_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/string.hpp>

#include <LeapC.h>
#include <thread>
#include <queue>
#include <atomic>

#include "types.h"
#include "frame.h"
#include "device.h"
#include "device_list.h"

using namespace godot;

class UltraleapHandTracking : public Node3D {
    GDCLASS(UltraleapHandTracking, Node3D);

public:

protected:
    static void _bind_methods();


};

#endif