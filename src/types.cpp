#include "types.h"

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void UltraleapTypes::_bind_methods() {
    BIND_ENUM_CONSTANT(Right);
    BIND_ENUM_CONSTANT(Left);

    BIND_ENUM_CONSTANT(Desktop);
    BIND_ENUM_CONSTANT(HMD);
    BIND_ENUM_CONSTANT(Screentop);

    BIND_ENUM_CONSTANT(BackgroundFrames);
    BIND_ENUM_CONSTANT(Images);
    BIND_ENUM_CONSTANT(OptimizeHMD);
    BIND_ENUM_CONSTANT(MapPoints);
    BIND_ENUM_CONSTANT(OptimizeScreenTop);
    BIND_ENUM_CONSTANT(AllowPauseResume);
}

Vector3 UltraleapTypes::ultraleap_vector3_to_godot_vector3(LEAP_VECTOR* vector) {
    return Vector3(
        vector->x / 1000,
        vector->y / 1000,
        vector->z / 1000
    );
}

Quaternion UltraleapTypes::ultraleap_quaternion_to_godot_quaternion(LEAP_QUATERNION* quaternion) {
    return Quaternion(
        quaternion->x,
        quaternion->y,
        quaternion->z,
        quaternion->w
    );
}

String UltraleapTypes::ultraleap_result_to_string(eLeapRS result) {
    switch (result)
    {
    case eLeapRS_Success:
        return "Success";
        break;
    case eLeapRS_TimestampTooEarly:
        return "Timestap too early";
        break;
    case eLeapRS_RoutineIsNotSeer:
        return "Routine is not Seer";
        break;
    case eLeapRS_NotConnected:
        return "Not connected";
        break;
    case eLeapRS_UnknownError:
        return "Unknown error";
        break;
    case eLeapRS_InvalidArgument:
        return "Invalid argument";
        break;
    case eLeapRS_InsufficientResources:
        return "Insufficient resources";
        break;
    case eLeapRS_InsufficientBuffer:
        return "Insufficient buffers";
        break;
    case eLeapRS_Timeout:
        return "Timeout";
        break;
    case eLeapRS_HandshakeIncomplete:
        return "Handshake incomplete";
        break;
    case eLeapRS_BufferSizeOverflow:
        return "Buffer size overflow";
        break;
    case eLeapRS_ProtocolError:
        return "Protocol error";
        break;
    case eLeapRS_InvalidClientID:
        return "Invalid client ID";
        break;
    case eLeapRS_UnexpectedClosed:
        return "Unexpected close";
        break;
    case eLeapRS_UnknownTrackingFrameID:
        return "Unknown tracking frame ID";
        break;
    case eLeapRS_ConcurrentPoll:
        return "Concurrent poll";
        break;
    case eLeapRS_NotAvailable:
        return "Not available";
        break;
    case eLeapRS_NotStreaming:
        return "Not streaming";
        break;
    case eLeapRS_CannotOpenDevice:
        return "Cannot open device";
        break;
    case eLeapRS_Unsupported:
        return "Unsupported";
        break;
    default:
        return "Result message conversion to string not implemented";
        break;
    }
}

bool UltraleapTypes::read_flag(uint32_t flags, uint32_t flag) {
    return (flags & flag) == flag;
}

uint32_t UltraleapTypes::set_flag(uint32_t flags, uint32_t flag) {
    return flags | flag;
}

uint32_t UltraleapTypes::clear_flag(uint32_t flags, uint32_t flag) {
    return flags & ~flag;
}

bool UltraleapTypes::read_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag) {
    return read_flag(flags, (uint32_t)flag);
    //return (flags & (uint32_t)flag) == (uint32_t)flag;
}

uint32_t UltraleapTypes::set_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag) {
    return set_flag(flags, (uint32_t)flag);
    //return flags | (uint32_t)flag;
}

uint32_t UltraleapTypes::clear_policy_flag(uint32_t flags, UltraleapTypes::PolicyFlag flag) {
    return clear_flag(flags, (uint32_t)flag);
    //return flags & ~(uint32_t)flag;
}

bool UltraleapTypes::read_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag) {
    return read_flag(flags, (uint32_t)flag);
}

uint32_t UltraleapTypes::set_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag) {
    return set_flag(flags, (uint32_t)flag);
}

uint32_t UltraleapTypes::clear_status_flag(uint32_t flags, UltraleapTypes::StatusFlag flag) {
    return clear_flag(flags, (uint32_t)flag);
}