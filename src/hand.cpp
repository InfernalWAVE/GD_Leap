/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#include "hand.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void UltraleapHand::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_type", "type"), &UltraleapHand::set_type, DEFVAL(UltraleapTypes::Chirality::Left));
    ClassDB::bind_method(D_METHOD("get_type"), &UltraleapHand::get_type);

    ClassDB::bind_method(D_METHOD("set_confidence", "confidence"), &UltraleapHand::set_confidence);
    ClassDB::bind_method(D_METHOD("get_confidence"), &UltraleapHand::get_confidence);

    ClassDB::bind_method(D_METHOD("set_visible_time", "visible_time"), &UltraleapHand::set_visible_time);
    ClassDB::bind_method(D_METHOD("get_visible_time"), &UltraleapHand::get_visible_time);

    ClassDB::bind_method(D_METHOD("set_pinch_distance", "pinch_distance"), &UltraleapHand::set_pinch_distance);
    ClassDB::bind_method(D_METHOD("get_pinch_distance"), &UltraleapHand::get_pinch_distance);

    ClassDB::bind_method(D_METHOD("set_pinch_strength", "pinch_strength"), &UltraleapHand::set_pinch_strength);
    ClassDB::bind_method(D_METHOD("get_pinch_strength"), &UltraleapHand::get_pinch_strength);

    ClassDB::bind_method(D_METHOD("set_grab_strength", "grab_strength"), &UltraleapHand::set_grab_strength);
    ClassDB::bind_method(D_METHOD("get_grab_strength"), &UltraleapHand::get_grab_strength);

    ClassDB::bind_method(D_METHOD("set_grab_angle", "grab_angle"), &UltraleapHand::set_grab_angle);
    ClassDB::bind_method(D_METHOD("get_grab_angle"), &UltraleapHand::get_grab_angle);

    ClassDB::bind_method(D_METHOD("set_thumb", "thumb"), &UltraleapHand::set_thumb);
    ClassDB::bind_method(D_METHOD("get_thumb"), &UltraleapHand::get_thumb);

    ClassDB::bind_method(D_METHOD("set_index", "index"), &UltraleapHand::set_index);
    ClassDB::bind_method(D_METHOD("get_index"), &UltraleapHand::get_index);

    ClassDB::bind_method(D_METHOD("set_middle", "middle"), &UltraleapHand::set_middle);
    ClassDB::bind_method(D_METHOD("get_middle"), &UltraleapHand::get_middle);

    ClassDB::bind_method(D_METHOD("set_ring", "ring"), &UltraleapHand::set_ring);
    ClassDB::bind_method(D_METHOD("get_ring"), &UltraleapHand::get_ring);

    ClassDB::bind_method(D_METHOD("set_pinky", "pinky"), &UltraleapHand::set_pinky);
    ClassDB::bind_method(D_METHOD("get_pinky"), &UltraleapHand::get_pinky);

    ClassDB::bind_method(D_METHOD("set_arm", "arm"), &UltraleapHand::set_arm);
    ClassDB::bind_method(D_METHOD("get_arm"), &UltraleapHand::get_arm);

    ClassDB::bind_method(D_METHOD("set_palm", "palm"), &UltraleapHand::set_palm);
    ClassDB::bind_method(D_METHOD("get_palm"), &UltraleapHand::get_palm);

    ClassDB::bind_method(D_METHOD("get_digits"), &UltraleapHand::get_digits);

    ClassDB::bind_method(D_METHOD("set_wrist", "wrist"), &UltraleapHand::set_wrist);
    ClassDB::bind_method(D_METHOD("get_wrist"), &UltraleapHand::get_wrist);

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::INT,
            "type"
        ),
        "set_type",
        "get_type"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "confidence"
        ),
        "set_confidence",
        "get_confidence"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "visible_time"
        ),
        "set_visible_time",
        "get_visible_time"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "pinch_distance"
        ),
        "set_pinch_distance",
        "get_pinch_distance"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "pinch_strength"
        ),
        "set_pinch_strength",
        "get_pinch_strength"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "grab_strength"
        ),
        "set_grab_strength",
        "get_grab_strength"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::FLOAT,
            "grab_angle"
        ),
        "set_grab_angle",
        "get_grab_angle"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "thumb",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDigit"
        ),
        "set_thumb",
        "get_thumb"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "index",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDigit"
        ),
        "set_index",
        "get_index"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "middle",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDigit"
        ),
        "set_middle",
        "get_middle"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "ring",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDigit"
        ),
        "set_ring",
        "get_ring"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "pinky",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDigit",
            PROPERTY_USAGE_DEFAULT,
            "UltraleapDigit"
        ),
        "set_pinky",
        "get_pinky"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "arm",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapBone"
        ),
        "set_arm",
        "get_arm"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::OBJECT,
            "palm",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapPalm"
        ),
        "set_palm",
        "get_palm"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::ARRAY,
            "digits",
            PROPERTY_HINT_ARRAY_TYPE,
            "UltraleapDigit",
            PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NO_INSTANCE_STATE
        ),
        "",
        "get_digits"
    );

    ClassDB::add_property(
        "UltraleapHand",
        PropertyInfo(
            Variant::TRANSFORM3D,
            "wrist"
        ),
        "set_wrist",
        "get_wrist"
    );
};

void UltraleapHand::fill_hand_data(Ref<UltraleapHand> ul_hand, LEAP_HAND* hand, Transform3D rigging_transform) {
    // For now we just check if the arm ref is not set
    if (ul_hand->arm == NULL) {
        ul_hand->arm.instantiate();
        ul_hand->palm.instantiate();
        ul_hand->thumb.instantiate();
        ul_hand->index.instantiate();
        ul_hand->middle.instantiate();
        ul_hand->ring.instantiate();
        ul_hand->pinky.instantiate();
    }

    ul_hand->type = hand->type == eLeapHandType_Left ? UltraleapTypes::Chirality::Left : UltraleapTypes::Chirality::Right;
    ul_hand->confidence = hand->confidence;
    ul_hand->visible_time = hand->visible_time;
    ul_hand->pinch_distance = hand->pinch_distance;
    ul_hand->pinch_strength = hand->pinch_strength;
    ul_hand->grab_angle = hand->grab_angle;
    ul_hand->grab_strength = hand->grab_strength;

    UltraleapPalm::fill_palm_data(ul_hand->palm, &hand->palm);

    ul_hand->wrist = Transform3D(
        Basis(UltraleapTypes::ultraleap_quaternion_to_godot_quaternion(&hand->palm.orientation)),
        UltraleapTypes::ultraleap_vector3_to_godot_vector3(&hand->arm.next_joint)
    );

    UltraleapDigit::fill_digit_data(ul_hand->get_thumb(), &hand->thumb, UltraleapDigit::FingerType::Thumb, ul_hand->wrist, rigging_transform);
    UltraleapDigit::fill_digit_data(ul_hand->get_index(), &hand->index, UltraleapDigit::FingerType::Index, ul_hand->wrist, rigging_transform);
    UltraleapDigit::fill_digit_data(ul_hand->get_middle(), &hand->middle, UltraleapDigit::FingerType::Middle, ul_hand->wrist, rigging_transform);
    UltraleapDigit::fill_digit_data(ul_hand->get_ring(), &hand->ring, UltraleapDigit::FingerType::Ring, ul_hand->wrist, rigging_transform);
    UltraleapDigit::fill_digit_data(ul_hand->get_pinky(), &hand->pinky, UltraleapDigit::FingerType::Pinky, ul_hand->wrist, rigging_transform);

    UltraleapBone::fill_bone_data(ul_hand->arm, ul_hand->wrist, &hand->arm, UltraleapBone::BoneType::Arm, rigging_transform);
}