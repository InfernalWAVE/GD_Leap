/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#include "palm.h"
#include "types.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include <LeapC.h>

using namespace godot;

void UltraleapPalm::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_position"), &UltraleapPalm::get_position);
    ClassDB::bind_method(D_METHOD("set_position", "position"), &UltraleapPalm::set_position);

    ClassDB::bind_method(D_METHOD("get_stabilized_position"), &UltraleapPalm::get_stabilized_position);
    ClassDB::bind_method(D_METHOD("set_stabilized_position", "stabilized_position"), &UltraleapPalm::set_stabilized_position);

    ClassDB::bind_method(D_METHOD("get_velocity"), &UltraleapPalm::get_velocity);
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &UltraleapPalm::set_velocity);

    ClassDB::bind_method(D_METHOD("get_normal"), &UltraleapPalm::get_normal);
    ClassDB::bind_method(D_METHOD("set_normal", "normal"), &UltraleapPalm::set_normal);

    ClassDB::bind_method(D_METHOD("get_direction"), &UltraleapPalm::get_direction);
    ClassDB::bind_method(D_METHOD("set_direction", "direction"), &UltraleapPalm::set_direction);

    ClassDB::bind_method(D_METHOD("get_width"), &UltraleapPalm::get_width);
    ClassDB::bind_method(D_METHOD("set_width", "width"), &UltraleapPalm::set_width);

    ClassDB::bind_method(D_METHOD("get_orientation"), &UltraleapPalm::get_orientation);
    ClassDB::bind_method(D_METHOD("set_orientation", "orientation"), &UltraleapPalm::set_orientation);

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::VECTOR3,
            "position"
        ),
        "set_position",
        "get_position"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::VECTOR3,
            "stabilized_position"
        ),
        "set_stabilized_position",
        "get_stabilized_position"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::VECTOR3,
            "velocity"
        ),
        "set_velocity",
        "get_velocity"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::VECTOR3,
            "normal"
        ),
        "set_normal",
        "get_normal"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::VECTOR3,
            "direction"
        ),
        "set_direction",
        "get_direction"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::FLOAT,
            "width"
        ),
        "set_width",
        "get_width"
    );

    ClassDB::add_property(
        "UltraleapPalm",
        PropertyInfo(
            Variant::QUATERNION,
            "orientation"
        ),
        "set_orientation",
        "get_orientation"
    );
}

void UltraleapPalm::fill_palm_data(Ref<UltraleapPalm> ul_palm, LEAP_PALM* palm) {
    ul_palm->position = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&palm->position);
    ul_palm->stabilized_position = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&palm->stabilized_position);
    ul_palm->velocity = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&palm->velocity);
    ul_palm->normal = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&palm->normal);
    ul_palm->direction = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&palm->direction);
    ul_palm->width = palm->width / 1000;
    ul_palm->orientation = UltraleapTypes::ultraleap_quaternion_to_godot_quaternion(&palm->orientation);
}