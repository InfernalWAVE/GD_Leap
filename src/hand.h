#ifndef ULTRALEAP_HAND_H
#define ULTRALEAP_HAND_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

using namespace godot;

class UltraleapHand : public Resource {
    GDCLASS(UltraleapHand, Resource);

public:

protected:
    static void _bind_methods();

};

#endif