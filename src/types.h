#ifndef ULTRALEAP_TYPES_H
#define ULTRALEAP_TYPES_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

using namespace godot;

class UltraleapTypes: public Resource {
    GDCLASS(UltraleapTypes, Resource);

public:
    enum Chirality {
        Left,
        Right
    };

    enum TrackingMode {
        Desktop,
        HMD, // not used
        Screentop
    };

    enum PolicyFlag {
        BackgroundFrames = eLeapPolicyFlag_BackgroundFrames,
        Images = eLeapPolicyFlag_Images,
        OptimizeHMD = eLeapPolicyFlag_OptimizeHMD,
        MapPoints = eLeapPolicyFlag_MapPoints,
        OptimizeScreenTop = eLeapPolicyFlag_OptimizeScreenTop,
        AllowPauseResume = eLeapPolicyFlag_AllowPauseResume
    };

    enum StatusFlag {
        Streaming = eLeapDeviceStatus_Streaming,
        Paused = eLeapDeviceStatus_Paused,
        Robust = eLeapDeviceStatus_Robust,
        Smudged = eLeapDeviceStatus_Smudged,
        LowResource = eLeapDeviceStatus_LowResource,
        UnknownFailure = eLeapDeviceStatus_UnknownFailure,
        BadCalibration = eLeapDeviceStatus_BadCalibration,
        BadFirmware = eLeapDeviceStatus_BadFirmware,
        BadTransport = eLeapDeviceStatus_BadTransport,
        BadControl = eLeapDeviceStatus_BadControl
    };

    static Vector3 ultraleap_vector3_to_godot_vector3(LEAP_VECTOR* vector);
    static Quaternion ultraleap_quaternion_to_godot_quaternion(LEAP_QUATERNION* quaternion);
    static String ultraleap_result_to_string(eLeapRS result);

    static bool read_flag(uint32_t flags, uint32_t flag);
    static uint32_t set_flag(uint32_t flags, uint32_t flag);
    static uint32_t clear_flag(uint32_t flags, uint32_t flag);

    static bool read_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag);
    static uint32_t set_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag);
    static uint32_t clear_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag);

    static bool read_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag);
    static uint32_t set_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag);
    static uint32_t clear_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag);

protected:
    static void _bind_methods();
};

VARIANT_ENUM_CAST(UltraleapTypes::Chirality);
VARIANT_ENUM_CAST(UltraleapTypes::TrackingMode);
VARIANT_ENUM_CAST(UltraleapTypes::PolicyFlag);
VARIANT_ENUM_CAST(UltraleapTypes::StatusFlag);

#endif