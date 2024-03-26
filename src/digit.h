#ifndef ULTRALEAP_DIGIT_H
#define ULTRALEAP_DIGIT_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <LeapC.h>

#include "bone.h"
#include "types.h"

using namespace godot;

class UltraleapDigit : public Resource {
    GDCLASS(UltraleapDigit, Resource);

public:

protected:
    static void _bind_methods();

};
#endif