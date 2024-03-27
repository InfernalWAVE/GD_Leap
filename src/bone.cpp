#include "bone.h"
#include "types.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void UltraleapBone::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_type"), &UltraleapBone::get_type);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &UltraleapBone::set_type);

    ClassDB::bind_method(D_METHOD("get_prev_joint"), &UltraleapBone::get_prev_joint);
    ClassDB::bind_method(D_METHOD("set_prev_joint", "prev_joint"), &UltraleapBone::set_prev_joint);

    ClassDB::bind_method(D_METHOD("get_next_joint"), &UltraleapBone::get_next_joint);
    ClassDB::bind_method(D_METHOD("set_next_joint", "next_joint"), &UltraleapBone::set_next_joint);

    ClassDB::bind_method(D_METHOD("get_center"), &UltraleapBone::get_center);
    ClassDB::bind_method(D_METHOD("set_center", "center"), &UltraleapBone::set_center);

    ClassDB::bind_method(D_METHOD("get_translation"), &UltraleapBone::get_translation);
    ClassDB::bind_method(D_METHOD("set_translation", "translation"), &UltraleapBone::set_translation);

    ClassDB::bind_method(D_METHOD("get_width"), &UltraleapBone::get_width);
    ClassDB::bind_method(D_METHOD("set_width", "width"), &UltraleapBone::set_width);

    ClassDB::bind_method(D_METHOD("get_length"), &UltraleapBone::get_length);
    ClassDB::bind_method(D_METHOD("set_length", "length"), &UltraleapBone::set_length);

    ClassDB::bind_method(D_METHOD("get_orientation"), &UltraleapBone::get_orientation);
    ClassDB::bind_method(D_METHOD("set_orientation", "orientation"), &UltraleapBone::set_orientation);

    ClassDB::bind_method(D_METHOD("get_rotation"), &UltraleapBone::get_rotation);
    ClassDB::bind_method(D_METHOD("set_rotation", "rotation"), &UltraleapBone::set_rotation);

    ClassDB::bind_method(D_METHOD("get_global_transform"), &UltraleapBone::get_global_transform);
    ClassDB::bind_method(D_METHOD("get_transform"), &UltraleapBone::get_transform);

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::INT,
            "type",
            PROPERTY_HINT_ENUM,
            "Metacarpal,Proximal,Intermediate,Distal,Arm,None"
        ),
        "set_type",
        "get_type"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::VECTOR3,
            "prev_joint"
        ),
        "set_prev_joint",
        "get_prev_joint"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::VECTOR3,
            "next_joint"
        ),
        "set_next_joint",
        "get_next_joint"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::VECTOR3,
            "center"
        ),
        "set_center",
        "get_center"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::VECTOR3,
            "translation"
        ),
        "set_translation",
        "get_translation"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::FLOAT,
            "width"
        ),
        "set_width",
        "get_width"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::FLOAT,
            "length"
        ),
        "set_length",
        "get_length"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::QUATERNION,
            "orientation"
        ),
        "set_orientation",
        "get_orientation"
    );

    ClassDB::add_property(
        "UltraleapBone",
        PropertyInfo(
            Variant::QUATERNION,
            "rotation"
        ),
        "set_rotation",
        "get_rotation"
    );

    BIND_ENUM_CONSTANT(Metacarpal);
    BIND_ENUM_CONSTANT(Proximal);
    BIND_ENUM_CONSTANT(Intermediate);
    BIND_ENUM_CONSTANT(Distal);
    BIND_ENUM_CONSTANT(Arm);
    BIND_ENUM_CONSTANT(None);
}

void UltraleapBone::fill_bone_data(Ref<UltraleapBone> bone, LEAP_BONE* leap_bone, BoneType type) {
    bone->type = type;
    bone->prev_joint = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&leap_bone->prev_joint);
    bone->next_joint = UltraleapTypes::ultraleap_vector3_to_godot_vector3(&leap_bone->next_joint);
    bone->width = leap_bone->width / 1000;
    bone->orientation = UltraleapTypes::ultraleap_quaternion_to_godot_quaternion(&leap_bone->rotation);
}

void UltraleapBone::fill_bone_data(Ref<UltraleapBone> bone, Transform3D root, LEAP_BONE* leap_bone, BoneType type, Transform3D rigging_transform) {
    // This function is for metacarpals or arms, this way root can be changed to be anything

    fill_bone_data(bone, leap_bone, type);

    // Set rotation and translation
    Transform3D current_bone_transform = Transform3D(
        Basis(bone->orientation),
        type == UltraleapBone::BoneType::Arm ? bone->next_joint : bone->prev_joint
    );

    Transform3D relative_bone_transform = root.inverse() * current_bone_transform;
    bone->rotation = relative_bone_transform.basis.get_quaternion();
    bone->translation = relative_bone_transform.origin;

    compute_bone_data(bone);

    // Set transform for rigging
    bone->set_global_transform(current_bone_transform * rigging_transform, root * rigging_transform);
}
 
void UltraleapBone::fill_bone_data(Ref<UltraleapBone> bone, Ref<UltraleapBone> previous_bone, LEAP_BONE* leap_bone, BoneType type, Transform3D rigging_transform) {
    fill_bone_data(bone, leap_bone, type);

    // Set previous bone (and set its next bone to current bone)
    previous_bone->next_bone = bone;
    bone->previous_bone = previous_bone;

    // Set rotation and translation
    Transform3D previous_bone_transform = Transform3D(
        Basis(bone->previous_bone->orientation),
        bone->previous_bone->prev_joint
    );

    Transform3D current_bone_transform = Transform3D(
        Basis(bone->orientation),
        bone->prev_joint
    );

    Transform3D relative_bone_transform = previous_bone_transform.inverse() * current_bone_transform;
    bone->rotation = relative_bone_transform.basis.get_quaternion();
    bone->translation = relative_bone_transform.origin;

    compute_bone_data(bone);

    // Set transform for rigging
    bone->set_global_transform(current_bone_transform * rigging_transform);
}

void UltraleapBone::compute_bone_data(Ref<UltraleapBone> bone) {
    bone->center = (bone->prev_joint + bone->next_joint) / 2.0f;
    bone->length = (bone->prev_joint - bone->next_joint).length();
}

void UltraleapBone::set_transform(Transform3D new_transform) {
    if (previous_bone != NULL) {
        global_transform = previous_bone->global_transform * new_transform;
    }

    // Ideally, we want to add a flag to allow propagating the transform
    // to next bones
}

void UltraleapBone::set_global_transform(Transform3D new_transform) {
    global_transform = new_transform;

    // Set the local transform based on the global transform with the previous bone
    if (previous_bone != NULL) {
        transform = previous_bone->global_transform.inverse() * new_transform;
    }

    // Ideally, we want to add a flag to allow propagating the transform
    // to next bones
}

void UltraleapBone::set_global_transform(Transform3D new_transform, Transform3D parent) {
    global_transform = new_transform;

    // Set the local transform based on the global transform with the previous transform
    transform = parent.inverse() * new_transform;

    // Ideally, we want to add a flag to allow propagating the transform
    // to next bones
}