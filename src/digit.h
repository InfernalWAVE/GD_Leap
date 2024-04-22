/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ULTRALEAP_DIGIT_H
#define ULTRALEAP_DIGIT_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/object.hpp>

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
    enum FingerType {
        Thumb,
        Index,
        Middle,
        Ring,
        Pinky,
        None
    };

    FingerType type = None;

    bool is_extended;

    FingerType get_type() { return type; }
    void set_type(FingerType value) { type = value; }

    Ref<UltraleapBone> get_metacarpal() { return metacarpal; }
    void set_metacarpal(Ref<UltraleapBone> value) { metacarpal = value; }

    Ref<UltraleapBone>  get_proximal() { return proximal; }
    void set_proximal(Ref<UltraleapBone>  value) { proximal = value; }

    Ref<UltraleapBone>  get_intermediate() { return intermediate; }
    void set_intermediate(Ref<UltraleapBone>  value) { intermediate = value; }

    Ref<UltraleapBone>  get_distal() { return distal; }
    void set_distal(Ref<UltraleapBone>  value) { distal = value; }

    bool get_is_extended() { return is_extended; }
    void set_is_extended(bool value) { is_extended = value; }

    Array get_bones() {
        if (bones == Variant::NIL) {
            bones = Array();
        }
        if (bones.size() == 0) {
            bones.append(metacarpal);
            bones.append(proximal);
            bones.append(intermediate);
            bones.append(distal);
        }
        return bones;
    }

    static void fill_digit_data(Ref<UltraleapDigit> ul_digit, LEAP_DIGIT* digit, FingerType type, Transform3D root, Transform3D rigging_transform);

protected:
    static void _bind_methods();

private:
    Ref<UltraleapBone> metacarpal;
    Ref<UltraleapBone> proximal;
    Ref<UltraleapBone> intermediate;
    Ref<UltraleapBone> distal;
    Array bones;

};

VARIANT_ENUM_CAST(UltraleapDigit::FingerType);

#endif