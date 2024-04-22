/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#include "ultraleap.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <LeapC.h>

#include <thread>
#include <functional>

#include "frame.h"
#include "allocator.h"
#include "device.h"

UltraleapHandTracking::UltraleapHandTracking() {
    printf("%p : Initializing UltraleapHandTracking\n", this);

        devices.instantiate();
}

UltraleapHandTracking::~UltraleapHandTracking() {
    printf("%p : Destroying UltraleapHandTracking\n", this);

    devices = Ref<UltraleapDeviceList>(NULL);

    printf("%p : UltraleapHandTracking destroyed\n", this);
}

void UltraleapHandTracking::dispose_ultraleap() {
    devices->devices.clear();

    if (opened && connectionHandle) {
        printf("%p : Closing the connection\n", this);
        LeapCloseConnection(connectionHandle);
        opened = false;
    }

    if (connectionHandle) {
        printf("%p : Destroying the connection\n", this);
        LeapDestroyConnection(connectionHandle);
        connectionHandle = NULL;
    }

    printf("%p : Ultraleap was disposed of\n", this);
}

void UltraleapHandTracking::_bind_methods() {
    PropertyInfo tracking_mode_info = PropertyInfo(
        Variant::INT,
        "tracking_mode",
        PROPERTY_HINT_ENUM,
        "Desktop, HMD, Screentop"
    );

    ClassDB::bind_method(D_METHOD("start"), &UltraleapHandTracking::start);
    ClassDB::bind_method(D_METHOD("stop"), &UltraleapHandTracking::stop);

    ClassDB::bind_method(D_METHOD("is_connected"), &UltraleapHandTracking::is_connected);
    ClassDB::bind_method(D_METHOD("is_started"), &UltraleapHandTracking::is_started);

    ClassDB::bind_method(D_METHOD("get_last_frame"), &UltraleapHandTracking::get_last_frame);

    ClassDB::bind_method(D_METHOD("set_tracking_mode", "tracking_mode"), &UltraleapHandTracking::set_tracking_mode);
    ClassDB::bind_method(D_METHOD("get_tracking_mode"), &UltraleapHandTracking::get_tracking_mode);

    ClassDB::bind_method(D_METHOD("get_devices"), &UltraleapHandTracking::get_devices);

    ClassDB::bind_method(D_METHOD("set_service_ip", "service_ip"), &UltraleapHandTracking::set_service_ip);
    ClassDB::bind_method(D_METHOD("get_service_ip"), &UltraleapHandTracking::get_service_ip);

    ClassDB::bind_method(D_METHOD("set_service_port", "service_port"), &UltraleapHandTracking::set_service_port);
    ClassDB::bind_method(D_METHOD("get_service_port"), &UltraleapHandTracking::get_service_port);

    ClassDB::bind_method(D_METHOD("set_autostart", "enable"), &UltraleapHandTracking::set_autostart);
    ClassDB::bind_method(D_METHOD("get_autostart"), &UltraleapHandTracking::get_autostart);

    ClassDB::bind_method(D_METHOD("set_allow_pause_resume_policy", "value"), &UltraleapHandTracking::set_allow_pause_resume_policy);

    ClassDB::bind_method(D_METHOD("set_primary_device", "device"), &UltraleapHandTracking::set_primary_device);
    ClassDB::bind_method(D_METHOD("get_primary_device"), &UltraleapHandTracking::get_primary_device);

    ClassDB::bind_method(D_METHOD("set_pause", "value"), &UltraleapHandTracking::set_pause);
    
    // PROPERTIES

    ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::OBJECT,
            "last_frame",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapFrame",
            PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NO_INSTANCE_STATE
        ),
        "",
        "get_last_frame"
    );

    ClassDB::add_property(
        "UltraleapHandTracking",
        tracking_mode_info,
        "set_tracking_mode",
        "get_tracking_mode"
    );

    ClassDB::add_property_group(
        "UltraleapHandTracking",
        "Devices",
        "devices"
    );

    ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::ARRAY,
            "devices_list",
            PROPERTY_HINT_ARRAY_TYPE,
            "UltraleapDevice",
            PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NO_INSTANCE_STATE
        ),
        "",
        "get_devices"
    );

        ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::OBJECT,
            "devices_primary_device",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapDevice",
            PROPERTY_USAGE_READ_ONLY | PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_NO_INSTANCE_STATE
        ),
        "set_primary_device",
        "get_primary_device"
    );

    ClassDB::add_property_group(
        "UltraleapHandTracking",
        "Connection",
        "service"
    );

    ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::STRING,
            "service_ip"
        ),
        "set_service_ip",
        "get_service_ip"
    );

    ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::INT,
            "service_port"
        ),
        "set_service_port",
        "get_service_port"
    );

    ClassDB::add_property_group(
        "UltraleapHandTracking",
        "Startup",
        "startup"
    );

    ClassDB::add_property(
        "UltraleapHandTracking",
        PropertyInfo(
            Variant::BOOL,
            "startup_autostart"
        ),
        "set_autostart",
        "get_autostart"
    );

    // SIGNALS

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "connection_status_changed",
            PropertyInfo(
                Variant::BOOL,
                "connected"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "tracking_mode_changed",
            tracking_mode_info
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "device_added",
            PropertyInfo(
                Variant::OBJECT,
                "device",
                PROPERTY_HINT_RESOURCE_TYPE,
                "UltraleapDevice"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "device_removed",
            PropertyInfo(
                Variant::OBJECT,
                "device",
                PROPERTY_HINT_RESOURCE_TYPE,
                "UltraleapDevice"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "background_frames_policy_changed",
            PropertyInfo(
                Variant::BOOL,
                "state"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "map_points_policy_changed",
            PropertyInfo(
                Variant::BOOL,
                "state"
            )
        )
    );

    ClassDB::add_signal(
        "UltraleapHandTracking",
        MethodInfo(
            "allow_pause_resume_policy_changed",
            PropertyInfo(
                Variant::BOOL,
                "state"
            )
        )
    );
}

void UltraleapHandTracking::start() {
    if (started) {
        UtilityFunctions::print("The tracking is already started");
        return;
    }

    printf("%p : Starting the tracking\n", this);

    started = true;

    LEAP_CONNECTION_CONFIG config = {0};

    // Set connection to multi-device aware
    config.size = sizeof(config);
    config.flags = (uint32_t)eLeapConnectionConfig_MultiDeviceAware;

    CharString server_namespace = generate_connection_payload().utf8();
    config.server_namespace = server_namespace.get_data();

    if (LeapCreateConnection(&config, &connectionHandle) == eLeapRS_Success) {
        eLeapRS result = LeapOpenConnection(connectionHandle);
        if(result == eLeapRS_Success) {
            opened = true;

            {
                LEAP_ALLOCATOR allocator = { allocate, deallocate, NULL };
                LeapSetAllocator(connectionHandle, &allocator);
            }

            // Create the message poll thread
            messageLoop = std::thread(&UltraleapHandTracking::serviceMessageLoop, std::ref(*this));
        }
    }
}

void UltraleapHandTracking::stop() {
    if (!started) {
        printf("%p : Tracking already stopped\n", this);
        return;
    }

    if (is_running || messageLoop.joinable()) {
        printf("%p : Stopping the tracking thread\n", this);
        keep_running = false;
        messageLoop.join();
    }

    dispose_ultraleap();

    started = false;

    printf("%p : Tracking stopped!\n", this);
}

void UltraleapHandTracking::handle_connection_event(const LEAP_CONNECTION_EVENT *evt) {
    if (is_inside_tree()) {
        call_deferred_thread_group("emit_signal", "connection_status_changed", true);
    }
    connected = true;
}

void UltraleapHandTracking::handle_connection_lost_event(const LEAP_CONNECTION_LOST_EVENT *event) {
    if (is_inside_tree()) {
        call_deferred_thread_group("emit_signal", "connection_status_changed", true);
    }
    connected = false;

    // TODO: write something more pretty
    devices->devices.clear();
}

void UltraleapHandTracking::serviceMessageLoop() {
    LEAP_CONNECTION_MESSAGE msg;
    eLeapRS result;

    is_running = true;

    while (keep_running) {
        result = LeapPollConnection(connectionHandle, 100, &msg);

        if (result != eLeapRS_Success && result != eLeapRS_Timeout) {
            UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
            continue;
        }

        // Polling may have taken some time, re-check exit condition
        if (!keep_running) {
            break;
        }

        switch (msg.type){
            case eLeapEventType_Connection:
                handle_connection_event(msg.connection_event);
                break;
            case eLeapEventType_ConnectionLost:
                handle_connection_lost_event(msg.connection_lost_event);
                break;
            case eLeapEventType_Device:
                handle_device_event(msg.device_event);
                break;
            case eLeapEventType_DeviceLost:
                handle_device_lost_event(msg.device_event);
                break;
            case eLeapEventType_DeviceFailure:
                UtilityFunctions::print("A device failed!");
                handle_device_lost_event(msg.device_event);
                break;
            case eLeapEventType_Tracking:
                handle_tracking_event(msg.tracking_event, msg.device_id);
                break;
            case eLeapEventType_LogEvent:
                break;
            case eLeapEventType_Policy:
                handle_policy_change_event(msg.policy_event);
                break;
            case eLeapEventType_ConfigChange:
                break;
            case eLeapEventType_ConfigResponse:
                break;
            case eLeapEventType_PointMappingChange:
                break;
            case eLeapEventType_TrackingMode:
                // TODO: Check if having this and the policy event use the same path to notify a change in tracking mode
                handle_tracking_mode_event(msg.tracking_mode_event, msg.device_id);
                break;
            case eLeapEventType_LogEvents:
                break;
            case eLeapEventType_HeadPose:
                break;
            case eLeapEventType_IMU:
                break;
            case eLeapEventType_DeviceStatusChange:
                UtilityFunctions::print("Device status change!");
                handle_device_status_change_event(msg.device_status_change_event, msg.device_id);
                break;
            case eLeapEventType_DroppedFrame:
                break;
            case eLeapEventType_Eyes:
                break;
            case eLeapEventType_None:
                break;
            default:
                UtilityFunctions::print("Unhandled message type");
                break;
        }
    }

    is_running = false;
}

UltraleapTypes::TrackingMode UltraleapHandTracking::get_tracking_mode() const {
    if (primary_device == NULL) {
        return UltraleapTypes::TrackingMode::Desktop;
    }
    
    return primary_device->tracking_mode;
}

void UltraleapHandTracking::set_tracking_mode(UltraleapTypes::TrackingMode value) {
    if (primary_device == NULL) {
        return;
    }

    if (value != primary_device->tracking_mode) {
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
        LeapSetPolicyFlagsEx(connectionHandle, primary_device->device, set_flag, clear_flag);
    }
}

void UltraleapHandTracking::set_background_frames_policy(bool value) {
    set_policy(UltraleapTypes::PolicyFlag::BackgroundFrames, value);
}

void UltraleapHandTracking::set_map_points_policy(bool value) {
    set_policy(UltraleapTypes::PolicyFlag::MapPoints, value);
}

void UltraleapHandTracking::set_allow_pause_resume_policy(bool value) {
    set_policy(UltraleapTypes::PolicyFlag::AllowPauseResume, value);
    // Apparently, there's no policy changed event with this so we force the service to send an update
    // However it looks like it never changes anyway, maybe it's deprecated...
    LeapSetPolicyFlags(connectionHandle, 0, 0);
}

void UltraleapHandTracking::set_policy(UltraleapTypes::PolicyFlag flag, bool value) {
    uint64_t set_flags = value ? UltraleapTypes::set_policy_flag(0, flag) : 0;
    uint64_t clear_flags = !value ? UltraleapTypes::set_policy_flag(0, flag) : 0;
    LeapSetPolicyFlags(connectionHandle, set_flags, clear_flags);
}

bool UltraleapHandTracking::get_policy(UltraleapTypes::PolicyFlag flag) const {
    return UltraleapTypes::read_policy_flag(policy_flags, flag);
}

void UltraleapHandTracking::set_pause(bool value) {
    eLeapRS result = LeapSetPause(connectionHandle, value);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print(UltraleapTypes::ultraleap_result_to_string(result));
    }
}

void UltraleapHandTracking::handle_tracking_event(const LEAP_TRACKING_EVENT* event, uint32_t device_id) {
    // Get a device from our list and notify it of a new tracking event
    // Stores it in this node if this is the primary device

    Ref<UltraleapDevice> dev = devices->find_by_id(device_id);

    // If we don't find the device, it might not be registered and that
    // could be a problem. Maybe we should notify the user?
    if (dev == NULL) {
        return;
    }

    dev->on_frame_received(event);
}

void UltraleapHandTracking::handle_policy_change_event(const LEAP_POLICY_EVENT* event) {
    uint32_t flags = event->current_policy;

    // If current policy flags are set to UINT32_MAX, that means this is the
    // first time we get the policies. In this case we just store it and don't
    // broadcast the signals
    // Edit: we actually want to notify our application of all the policy flags
    // so we will just put in cache the inverse bits of the policy flags,
    // and we will comment the return
    if (policy_flags == UINT32_MAX) {
        UtilityFunctions::print("Received policies for the first time");
        policy_flags = ~flags;
        //return;
    }

    // If no changes in policies, we can ignore it
    if (policy_flags == flags) {
        return;
    }

    bool previous_background_frames_policy = UltraleapTypes::read_policy_flag(policy_flags, UltraleapTypes::PolicyFlag::BackgroundFrames);
    bool previous_map_points_policy = UltraleapTypes::read_policy_flag(policy_flags, UltraleapTypes::PolicyFlag::MapPoints);
    bool previous_allow_pause_resume_policy = UltraleapTypes::read_policy_flag(policy_flags, UltraleapTypes::PolicyFlag::AllowPauseResume);

    bool current_background_frames_policy = UltraleapTypes::read_policy_flag(flags, UltraleapTypes::PolicyFlag::BackgroundFrames);
    bool current_map_points_policy = UltraleapTypes::read_policy_flag(flags, UltraleapTypes::PolicyFlag::MapPoints);
    bool current_allow_pause_resume_policy = UltraleapTypes::read_policy_flag(flags, UltraleapTypes::PolicyFlag::AllowPauseResume);

    if (is_inside_tree()) {
        if (current_background_frames_policy != previous_background_frames_policy) {
            call_deferred_thread_group("emit_signal", "background_frames_policy_changed", current_background_frames_policy);
        }

        if (current_map_points_policy != previous_map_points_policy) {
            call_deferred_thread_group("emit_signal", "map_points_policy_changed", current_map_points_policy);
        }

        if (current_allow_pause_resume_policy != previous_allow_pause_resume_policy) {
            call_deferred_thread_group("emit_signal", "allow_pause_resume_policy_changed", current_allow_pause_resume_policy);
        }
    }

    policy_flags = flags;
}

void UltraleapHandTracking::handle_tracking_mode_event(const LEAP_TRACKING_MODE_EVENT* event, uint32_t device_id) {
    eLeapTrackingMode current_tracking_mode = event->current_tracking_mode;
    UltraleapTypes::TrackingMode tm = default_tracking_mode;

    // Parse into a defined type
    if (current_tracking_mode == eLeapTrackingMode_HMD) {
        tm = UltraleapTypes::TrackingMode::HMD;
    }
    else if (current_tracking_mode == eLeapTrackingMode_ScreenTop) {
        tm = UltraleapTypes::TrackingMode::Screentop;
    }
    else if (current_tracking_mode == eLeapTrackingMode_Desktop) {
        tm = UltraleapTypes::TrackingMode::Desktop;
    }

    if (primary_device != NULL && device_id == primary_device->id) {
        if (primary_device->tracking_mode != tm && is_inside_tree()) {
            call_deferred_thread_group("emit_signal", "tracking_mode_changed", tm);
        }
    }

    Ref<UltraleapDevice> dev = devices->find_by_id(device_id);
    if (dev != NULL && dev->tracking_mode != tm) {
        dev->tracking_mode_changed(tm);
    }
}

void UltraleapHandTracking::handle_device_event(const LEAP_DEVICE_EVENT* event) {
    Ref<UltraleapDevice> dev = create_device(event->device);
    if (!devices->has_device(dev)) {
        devices->add_device(dev);
    }
    else {
        UtilityFunctions::print("Device was already in the list, updating");
        devices->remove_device(dev);
        devices->add_device(dev);
    }

    if (is_inside_tree()) {
        call_deferred_thread_group("emit_signal", "device_added", dev);
    }
}

void UltraleapHandTracking::handle_device_lost_event(const LEAP_DEVICE_EVENT* event) {
    UtilityFunctions::print("Device lost with ID ", event->device.id);

    Ref<UltraleapDevice> dev = devices->find_by_id(event->device.id);
    if (dev != NULL) {
        if (dev == primary_device) {
            primary_device = Variant::NIL;
        }
        devices->remove_device(dev);
        if (is_inside_tree()) {
            call_deferred_thread_group("emit_signal", "device_removed", dev);
        }
    }
    else {
        UtilityFunctions::print("Device was not in the list");
    }
}

void UltraleapHandTracking::handle_device_status_change_event(const LEAP_DEVICE_STATUS_CHANGE_EVENT* event, uint32_t device_id) {
    // Not implemented yet, I don't know what is deprecated or not ???
}

void UltraleapHandTracking::refresh_device_list() {
    UtilityFunctions::print("Refresh device list");

    uint32_t device_count = 0;
    eLeapRS result = LeapGetDeviceList(connectionHandle, NULL, &device_count);
    if (result != eLeapRS_Success) {
        UtilityFunctions::print("Error while counting the number of devices");
    }

    LEAP_DEVICE_REF* devices_ref = (LEAP_DEVICE_REF*)memalloc(device_count * sizeof(LEAP_DEVICE_REF));
    result = LeapGetDeviceList(connectionHandle, devices_ref, &device_count);

    if (result != eLeapRS_Success) {
        memfree(devices_ref);
        UtilityFunctions::print("Error while enumerating the devices");
    }

    devices->devices.clear();

    for (int index = 0; index < device_count; index++) {
        Ref<UltraleapDevice> dev = create_device(devices_ref[index]);
        devices->add_device(dev);
    }

    memfree(devices_ref);

    notify_property_list_changed();
}

Ref<UltraleapDevice> UltraleapHandTracking::create_device(LEAP_DEVICE_REF ref) {
    LEAP_DEVICE device;
    LeapOpenDevice(ref, &device);

    LEAP_DEVICE_INFO deviceInfo;
    memset(&deviceInfo, 0, sizeof(deviceInfo));

    // Use stack memory to allocate for the serial number field.
    char serial[1000];
    memset(serial, 0, sizeof(serial));
    deviceInfo.serial = serial;
    deviceInfo.serial_length = sizeof(serial) - 1;
    deviceInfo.size = sizeof(deviceInfo);

    LeapGetDeviceInfo(device, &deviceInfo);

    Ref<UltraleapDevice> dev;
    dev.instantiate();
    dev->serial = String(deviceInfo.serial);
    dev->baseline = deviceInfo.baseline;
    dev->id = ref.id;

    dev->device_ref = ref;
    dev->connection = &connectionHandle;

    UtilityFunctions::print("Created device with ID: ", ref.id, ", type: ", deviceInfo.pid, ", serial number: ", deviceInfo.serial);

    LeapCloseDevice(device);

    return dev;
}

void UltraleapHandTracking::_notification(int p_what) {
    if (p_what == Node::NOTIFICATION_ENTER_TREE) {
        printf("%p : Entering tree\n", this);
        // For now we will also start automatically if we're in the editor, so that we can have data for tool scripts
        //if (autostart || Engine::get_singleton()->is_editor_hint()) {
        // Okay very temporarily, we'll just obey to the flag
        if (autostart) {
            printf("%p : Autostart is ON\n", this);
            start();
        }
    }
    else if (p_what == Node::NOTIFICATION_EXIT_TREE) {
        printf("%p : Exiting tree\n", this);
        stop();
    }
}

String UltraleapHandTracking::generate_connection_payload() {
    Array values = Array();
    values.append(service_ip);
    values.append(service_port);

    String payload = String("{\"tracking_server_ip\": \"%0\", \"tracking_server_port\": %1}");
    return payload.format(values, "%_");
}

void UltraleapHandTracking::set_primary_device(Ref<UltraleapDevice> device) {
    if (primary_device != NULL) {
        if (!primary_device->is_opened()) {
            primary_device->close();
        }
    }

    device->open();
    primary_device = device;
    LeapSetPrimaryDevice(connectionHandle, device->device, false);
}

Ref<UltraleapDevice> UltraleapHandTracking::get_primary_device() const {
    return primary_device;
}

Ref<UltraleapFrame> UltraleapHandTracking::get_last_frame() const {
    if (primary_device == NULL) {
        return NULL;
    }

    return primary_device->get_last_frame();
}