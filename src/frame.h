#ifndef ULTRALEAP_FRAME_H
#define ULTRALEAP_FRAME_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/ref.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

#include "hand.h"

using namespace godot;

class UltraleapFrame: public Resource {
    GDCLASS(UltraleapFrame, Resource);

public:

protected:
    static void _bind_methods();

private:
    Ref<UltraleapHand> left_hand;
    Ref<UltraleapHand> right_hand;

};

#endif