/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ULTRALEAP_HAND_H
#define ULTRALEAP_HAND_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

#include "digit.h"
#include "palm.h"
#include "bone.h"
#include "types.h"


using namespace godot;

class UltraleapHand : public Resource {
    GDCLASS(UltraleapHand, Resource);
    
public:
    UltraleapTypes::Chirality type;
    float confidence;
    uint64_t visible_time;
    float pinch_distance;
    float grab_angle;
    float pinch_strength;
    float grab_strength;

    Transform3D wrist;

    // Getters / Setters
    float get_confidence() { return confidence; }
    void set_confidence(float value) { confidence = value; }

    float get_pinch_distance() { return pinch_distance; }
    void set_pinch_distance(float value) { pinch_distance = value; }

    float get_pinch_strength() { return pinch_strength; }
    void set_pinch_strength(float value) { pinch_strength = value; }

    float get_grab_angle() { return grab_angle; }
    void set_grab_angle(float value) { grab_angle = value; }

    float get_grab_strength() { return grab_strength; }
    void set_grab_strength(float value) { grab_strength = value; }

    uint64_t get_visible_time() { return visible_time; }
    void set_visible_time(uint64_t value) { visible_time = value; }

    UltraleapTypes::Chirality get_type() { return type; }
    void set_type(UltraleapTypes::Chirality value) { type = value; }

    Ref<UltraleapDigit> get_thumb() { return thumb; }
    void set_thumb(Ref<UltraleapDigit> value) { thumb = value; }

    Ref<UltraleapDigit> get_index() { return index; }
    void set_index(Ref<UltraleapDigit> value) { index = value; }

    Ref<UltraleapDigit> get_middle() { return middle; }
    void set_middle(Ref<UltraleapDigit> value) { middle = value; }

    Ref<UltraleapDigit> get_ring() { return ring; }
    void set_ring(Ref<UltraleapDigit> value) { ring = value; }

    Ref<UltraleapDigit> get_pinky() { return pinky; }
    void set_pinky(Ref<UltraleapDigit> value) { pinky = value; }

    Ref<UltraleapBone>  get_arm() { return arm; }
    void set_arm(Ref<UltraleapBone> value) { arm = value; }

    Ref<UltraleapPalm> get_palm() { return palm; }
    void set_palm(Ref<UltraleapPalm> value) { palm = value; }

    Array get_digits() {
        if (digits_array == Variant::NIL) {
            digits_array = Array();
        }
        if (digits_array.size() == 0) {
            digits_array.append(thumb);
            digits_array.append(index);
            digits_array.append(middle);
            digits_array.append(ring);
            digits_array.append(pinky);
        }
        return digits_array;
    }

    Transform3D get_wrist() { return wrist; }
    void set_wrist(Transform3D value) { wrist = value; }

    static void fill_hand_data(Ref<UltraleapHand> ul_hand, LEAP_HAND* hand, Transform3D rigging_transform);

protected:
    static void _bind_methods();

private:
    Ref<UltraleapBone> arm;
    Ref<UltraleapPalm> palm;
    Array digits_array;

    Ref<UltraleapDigit> thumb;
    Ref<UltraleapDigit> index;
    Ref<UltraleapDigit> middle;
    Ref<UltraleapDigit> ring;
    Ref<UltraleapDigit> pinky;
};

#endif