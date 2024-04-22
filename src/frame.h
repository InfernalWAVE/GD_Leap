/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

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
    uint32_t id;

    bool is_left_hand_visible = false;
    bool is_right_hand_visible = false;

    float framerate;

    Ref<UltraleapHand> get_left_hand() { return left_hand; }
    void set_left_hand(Ref<UltraleapHand> value) { left_hand = value; }

    Ref<UltraleapHand> get_right_hand() { return right_hand; }
    void set_right_hand(Ref<UltraleapHand> value) { right_hand = value; }

    bool get_is_left_hand_visible() { return is_left_hand_visible; }
    void set_is_left_hand_visible(bool value) { is_left_hand_visible = value; }

    bool get_is_right_hand_visible() { return is_right_hand_visible; }
    void set_is_right_hand_visible(bool value) { is_right_hand_visible = value; }

    float get_framerate() { return framerate; }
    void set_framerate(float value) { return; }

    static void fill_frame_data(Ref<UltraleapFrame> ul_frame, const LEAP_TRACKING_EVENT* frame, Transform3D rigging_transform);
    
protected:
    static void _bind_methods();

private:
    Ref<UltraleapHand> left_hand;
    Ref<UltraleapHand> right_hand;

};

#endif