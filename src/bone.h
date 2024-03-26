#ifndef ULTRALEAP_BONE_H
#define ULTRALEAP_BONE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <LeapC.h>

using namespace godot;

class UltraleapBone : public Resource {
    GDCLASS(UltraleapBone, Resource);

public:

protected:
    static void _bind_methods();
};

#endif