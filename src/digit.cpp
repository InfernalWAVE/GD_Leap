#include "digit.h"
#include "types.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>

#include <LeapC.h>

using namespace godot;


void UltraleapDigit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_type"), &UltraleapDigit::get_type);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &UltraleapDigit::set_type);

    ClassDB::bind_method(D_METHOD("get_metacarpal"), &UltraleapDigit::get_metacarpal);
    ClassDB::bind_method(D_METHOD("set_metacarpal", "metacarpal"), &UltraleapDigit::set_metacarpal);

    ClassDB::bind_method(D_METHOD("get_proximal"), &UltraleapDigit::get_proximal);
    ClassDB::bind_method(D_METHOD("set_proximal", "proximal"), &UltraleapDigit::set_proximal);

    ClassDB::bind_method(D_METHOD("get_intermediate"), &UltraleapDigit::get_intermediate);
    ClassDB::bind_method(D_METHOD("set_intermediate", "intermediate"), &UltraleapDigit::set_intermediate);

    ClassDB::bind_method(D_METHOD("get_distal"), &UltraleapDigit::get_distal);
    ClassDB::bind_method(D_METHOD("set_distal", "distal"), &UltraleapDigit::set_distal);

    ClassDB::bind_method(D_METHOD("get_is_extended"), &UltraleapDigit::get_is_extended);
    ClassDB::bind_method(D_METHOD("set_is_extended", "is_extended"), &UltraleapDigit::set_is_extended);

    ClassDB::bind_method(D_METHOD("get_bones"), &UltraleapDigit::get_bones);

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::INT,
            "type",
            PROPERTY_HINT_ENUM,
            "Thumb,Index,Middle,Ring,Pinky,None"
        ),
        "set_type",
        "get_type"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::OBJECT,
            "metacarpal",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapBone"
        ),
        "set_metacarpal",
        "get_metacarpal"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::OBJECT,
            "proximal",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapBone"
        ),
        "set_proximal",
        "get_proximal"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::OBJECT,
            "intermediate",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapBone"
        ),
        "set_intermediate",
        "get_intermediate"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::OBJECT,
            "distal",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapBone"
        ),
        "set_distal",
        "get_distal"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::BOOL,
            "is_extended"
        ),
        "set_is_extended",
        "get_is_extended"
    );

    ClassDB::add_property(
        "UltraleapDigit",
        PropertyInfo(
            Variant::ARRAY,
            "bones",
            PROPERTY_HINT_ARRAY_TYPE,
            "UltraleapBone",
            PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NO_INSTANCE_STATE
        ),
        "",
        "get_bones"
    );

    BIND_ENUM_CONSTANT(Thumb);
    BIND_ENUM_CONSTANT(Index);
    BIND_ENUM_CONSTANT(Middle);
    BIND_ENUM_CONSTANT(Ring);
    BIND_ENUM_CONSTANT(Pinky);
    BIND_ENUM_CONSTANT(None);
}

void UltraleapDigit::fill_digit_data(Ref<UltraleapDigit> ul_digit, LEAP_DIGIT* digit, FingerType type, Transform3D root, Transform3D rigging_transform) {
    if (ul_digit->metacarpal == NULL) {
        ul_digit->metacarpal.instantiate();
        ul_digit->proximal.instantiate();
        ul_digit->intermediate.instantiate();
        ul_digit->distal.instantiate();
    }

    ul_digit->is_extended = digit->is_extended == 1;
    ul_digit->type = type;

    // We need some extra logic here because the metacarpal doesn't exist in Ultraleap's hand data
    if (type == UltraleapDigit::FingerType::Thumb) {
        UltraleapBone::fill_bone_data(ul_digit->proximal, root, &digit->proximal, UltraleapBone::BoneType::Proximal, rigging_transform);

        // Still filling it out but shouldn't be used
        UltraleapBone::fill_bone_data(ul_digit->metacarpal, root, &digit->metacarpal, UltraleapBone::BoneType::Metacarpal, rigging_transform);
    }
    else {
        UltraleapBone::fill_bone_data(ul_digit->metacarpal, root, &digit->metacarpal, UltraleapBone::BoneType::Metacarpal, rigging_transform);
        UltraleapBone::fill_bone_data(ul_digit->proximal, ul_digit->metacarpal, &digit->proximal, UltraleapBone::BoneType::Proximal, rigging_transform);
    }

    UltraleapBone::fill_bone_data(ul_digit->intermediate, ul_digit->proximal, &digit->intermediate, UltraleapBone::BoneType::Intermediate, rigging_transform);
    UltraleapBone::fill_bone_data(ul_digit->distal, ul_digit->intermediate, &digit->distal, UltraleapBone::BoneType::Distal, rigging_transform);
}