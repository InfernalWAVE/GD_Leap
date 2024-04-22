/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#include "device.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/time.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

using namespace godot;

UltraleapDevice::~UltraleapDevice() {
    UtilityFunctions::print("Destroying device");
    if (connection == NULL || *connection == NULL) {
        printf("Connection is invalid or destroyed\n");
        return;
    }
    unsubscribe();
    close();
}

void UltraleapDevice::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_baseline", "baseline"), &UltraleapDevice::set_baseline);
    ClassDB::bind_method(D_METHOD("get_baseline"), &UltraleapDevice::get_baseline);

    ClassDB::bind_method(D_METHOD("set_serial", "serial"), &UltraleapDevice::set_serial);
    ClassDB::bind_method(D_METHOD("get_serial"), &UltraleapDevice::get_serial);

    ClassDB::bind_method(D_METHOD("set_id", "id"), &UltraleapDevice::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &UltraleapDevice::get_id);

    ClassDB::bind_method(D_METHOD("get_last_frame"), &UltraleapDevice::get_last_frame);
    ClassDB::bind_method(D_METHOD("get_interpolated_frame", "time"), &UltraleapDevice::get_interpolated_frame);

    ClassDB::bind_method(D_METHOD("set_tracking_mode", "tracking_mode"), &UltraleapDevice::set_tracking_mode, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_tracking_mode"), &UltraleapDevice::get_tracking_mode);

    ClassDB::bind_method(D_METHOD("get_rigging_transform"), &UltraleapDevice::get_rigging_transform);
    ClassDB::bind_method(D_METHOD("set_rigging_transform"), &UltraleapDevice::set_rigging_transform);

    ClassDB::bind_method(D_METHOD("open"), &UltraleapDevice::open);
    ClassDB::bind_method(D_METHOD("close"), &UltraleapDevice::close);
    ClassDB::bind_method(D_METHOD("subscribe"), &UltraleapDevice::subscribe);
    ClassDB::bind_method(D_METHOD("unsubscribe"), &UltraleapDevice::unsubscribe);

    ClassDB::add_property(
        "UltraleapDevice",
        PropertyInfo(
            Variant::INT,
            "baseline"
        ),
        "set_baseline",
        "get_baseline"
    );

    ClassDB::add_property(
        "UltraleapDevice",
        PropertyInfo(
            Variant::STRING,
            "serial"
        ),
        "set_serial",
        "get_serial"
    );

    ClassDB::add_property(
        "UltraleapDevice",
        PropertyInfo(
            Variant::INT,
            "id"
        ),
        "set_id",
        "get_id"
    );

    ClassDB::add_property(
        "UltraleapDevice",
        PropertyInfo(
            Variant::TRANSFORM3D,
            "rigging_transform"
        ),
        "set_rigging_transform",
        "get_rigging_transform"
    );

    // Signals

    ClassDB::add_signal(
        "UltraleapDevice",
        MethodInfo(
            "tracking_mode_changed",
            PropertyInfo(
                Variant::INT,
                "tracking_mode",
                PROPERTY_HINT_ENUM,
                "Desktop, HMD, Screentop"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapDevice",
        MethodInfo(
            "frame_received",
            PropertyInfo(
                Variant::OBJECT,
                "frame",
                PROPERTY_HINT_RESOURCE_TYPE,
                "UltraleapFrame"
            )
        )
    );
}

void UltraleapDevice::set_tracking_mode(UltraleapTypes::TrackingMode value) {
    if (value != tracking_mode) {
        uint64_t set_flag = 0;
        uint64_t clear_flag = 0;
        if (value == UltraleapTypes::TrackingMode::Desktop) {
            clear_flag = eLeapPolicyFlag_OptimizeHMD | eLeapPolicyFlag_OptimizeScreenTop;
        }
        else if (value == UltraleapTypes::TrackingMode::HMD) {
            set_flag = eLeapPolicyFlag_OptimizeHMD;
            clear_flag = eLeapPolicyFlag_OptimizeScreenTop;
        }
        else if (value == UltraleapTypes::TrackingMode::Screentop) {
            clear_flag = eLeapPolicyFlag_OptimizeHMD;
            set_flag = eLeapPolicyFlag_OptimizeScreenTop;
        }
        else {
            UtilityFunctions::print("Invalid value for tracking mode");
        }
        LeapSetPolicyFlagsEx(*connection, device, set_flag, clear_flag);
    }
}

void UltraleapDevice::tracking_mode_changed(UltraleapTypes::TrackingMode value) {
    tracking_mode = value;
}

void UltraleapDevice::subscribe() {
    if (subscribed) {
        UtilityFunctions::print("Device already subscribed to");
        return;
    }
    eLeapRS result = LeapSubscribeEvents(*connection, device);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
        return;
    }
    subscribed = true;
}

void UltraleapDevice::unsubscribe() {
    if (!subscribed) {
        UtilityFunctions::print("Device not subscribed to");
        return;
    }
    eLeapRS result = LeapUnsubscribeEvents(*connection, device);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
        return;
    }
    subscribed = false;
}

void UltraleapDevice::open() {
    if (opened) {
        UtilityFunctions::print("Device already opened");
        return;
    }
    eLeapRS result = LeapOpenDevice(device_ref, &device);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
        return;
    }
    opened = true;
    result = LeapCreateClockRebaser(&rebaser);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
        return;
    }
    interpolation_available = true;
}

void UltraleapDevice::close() {
    if (!opened) {
        UtilityFunctions::print("Device already closed");
        return;
    }
    LeapCloseDevice(device);
    opened = false;
    if (rebaser != NULL) {
        LeapDestroyClockRebaser(rebaser);
        rebaser = NULL;
        interpolation_available = false;
    }
}

void UltraleapDevice::on_frame_received(const LEAP_TRACKING_EVENT* frame) {
    Ref<UltraleapFrame> new_frame;
    new_frame.instantiate();
    UltraleapFrame::fill_frame_data(new_frame, frame, rigging_transform);
    frame_mutex.lock();
    last_frame_ref = new_frame;
    frame_mutex.unlock();
}

Ref<UltraleapFrame> UltraleapDevice::get_last_frame() {
    std::lock_guard<std::mutex> guard(frame_mutex);
    return last_frame_ref;
}

Ref<UltraleapFrame> UltraleapDevice::get_interpolated_frame(int64_t time) {
    if (!interpolation_available) {
        return NULL;
    }

    int64_t cpu_time = Time::get_singleton()->get_ticks_usec();
    LeapUpdateRebase(rebaser, cpu_time, LeapGetNow());
    LeapRebaseClock(rebaser, cpu_time, &time);

    uint64_t targetFrameSize = 0;

    //Get the buffer size needed to hold the tracking data
    eLeapRS result = LeapGetFrameSizeEx(*connection, device, time, &targetFrameSize);
    if (result == eLeapRS_Success) {
        //Allocate enough memory
        LEAP_TRACKING_EVENT* interpolatedFrame = (LEAP_TRACKING_EVENT*)malloc((size_t)targetFrameSize);
        //Get the frame
        result = LeapInterpolateFrameEx(*connection, device, time, interpolatedFrame, targetFrameSize);
        if (result == eLeapRS_Success) {
            Ref<UltraleapFrame> new_frame;
            new_frame.instantiate();
            UltraleapFrame::fill_frame_data(new_frame, interpolatedFrame, rigging_transform);
            return new_frame;
        }
    }

    return NULL;
}

void UltraleapDevice::on_lost() {
    unsubscribe();
    close();
}