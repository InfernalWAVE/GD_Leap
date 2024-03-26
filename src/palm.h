#ifndef ULTRALEAP_PALM_H
#define ULTRALEAP_PALM_H

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>

# include <LeapC.h>

#include "bone.h"

using namespace godot;

class UltraleapPalm : public Resource {
    GDCLASS(UltraleapPalm, Resource);

public:

protected:
    static void _bind_methods();
};

#endif