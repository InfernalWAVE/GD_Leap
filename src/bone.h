#ifndef ULTRALEAP_BONE_H
#define ULTRALEAP_BONE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <LeapC.h>

using namespace godot;

class UltraleapBone : public Resource {
    GDCLASS(UltraleapBone, Resource);

public:
    enum BoneType {
        Metacarpal,
        Proximal,
        Intermediate,
        Distal,
        Arm,
        None
    };

    BoneType type = BoneType::None;

    Vector3 prev_joint;
    Vector3 next_joint;
    Vector3 center;
    Vector3 translation;

    float width;
    float length;

    Quaternion orientation;
    Quaternion rotation;

    Ref<UltraleapBone> previous_bone;
    Ref<UltraleapBone> next_bone;
    
    Transform3D transform;
    Transform3D global_transform;

    Transform3D get_global_transform() { return global_transform; }
    void set_global_transform(Transform3D new_transform);
    void set_global_transform(Transform3D new_transform, Transform3D parent);

    Transform3D get_transform() { return transform; }
    void set_transform(Transform3D new_transform);

    // Getters / Setters
    BoneType get_type() { return type; }
    void set_type(BoneType value) { type = value; }

    Vector3 get_prev_joint() { return prev_joint; }
    void set_prev_joint(Vector3 value) { prev_joint = value; }

    Vector3 get_next_joint() { return next_joint; }
    void set_next_joint(Vector3 value) { next_joint = value; }

    Vector3 get_center() { return center; }
    void set_center(Vector3 value) { center = value; }

    Vector3 get_translation() { return translation; }
    void set_translation(Vector3 value) { translation = value; }

    float get_width() { return width; }
    void set_width(float value) { width = value; }

    float get_length() { return length; }
    void set_length(float value) { length = value; }

    Quaternion get_orientation() { return orientation; }
    void set_orientation(Quaternion value) { orientation = value; }

    Quaternion get_rotation() { return rotation; }
    void set_rotation(Quaternion value) { rotation = value; }

    static void fill_bone_data(Ref<UltraleapBone> bone, LEAP_BONE* leap_bone, BoneType type);
    static void fill_bone_data(Ref<UltraleapBone> bone, Transform3D root, LEAP_BONE* leap_bone, BoneType type, Transform3D rigging_transform);
    static void fill_bone_data(Ref<UltraleapBone> bone, Ref<UltraleapBone> previous_bone, LEAP_BONE* leap_bone, BoneType type, Transform3D rigging_transform);
    static void compute_bone_data(Ref<UltraleapBone> bone);

protected:
    static void _bind_methods();
};

VARIANT_ENUM_CAST(UltraleapBone::BoneType);

#endif