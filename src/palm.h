/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

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
    Vector3 position;
    Vector3 stabilized_position;
    Vector3 velocity;
    Vector3 normal;
    float width;
    Vector3 direction;
    Quaternion orientation;

    // Getters / Setters
    Vector3 get_position() { return position; }
    void set_position(Vector3 value) { position = value; }

    Vector3 get_stabilized_position() { return stabilized_position; }
    void set_stabilized_position(Vector3 value) { stabilized_position = value; }

    Vector3 get_velocity() { return velocity; }
    void set_velocity(Vector3 value) { velocity = value; }

    Vector3 get_normal() { return normal; }
    void set_normal(Vector3 value) { normal = value; }

    Vector3 get_direction() { return direction; }
    void set_direction(Vector3 value) { direction = value; }

    float get_width() { return width; }
    void set_width(float value) { width = value; }

    Quaternion get_orientation() { return orientation; }
    void set_orientation(Quaternion value) { orientation = value; }

    static void fill_palm_data(Ref<UltraleapPalm> ul_palm, LEAP_PALM* palm);
    
protected:
    static void _bind_methods();
};

#endif