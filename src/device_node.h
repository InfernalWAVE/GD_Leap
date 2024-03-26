#ifndef ULTRALEAP_DEVICE_NODE_H
#define ULTRALEAP_DEVICE_NODE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "device.h"
#include "ultraleap.h"
#include "frame.h"

using namespace godot;

class UltraleapDeviceNode : public Node3D {
    GDCLASS(UltraleapDeviceNode, Node3D);

public:

protected:
    static void _bind_methods();

};
#endif