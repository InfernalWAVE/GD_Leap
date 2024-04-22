/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ULTRALEAP_H
#define ULTRALEAP_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/string.hpp>

#include <LeapC.h>
#include <thread>
#include <queue>
#include <atomic>

#include "types.h"
#include "frame.h"
#include "device.h"
#include "device_list.h"

using namespace godot;

class UltraleapHandTracking : public Node3D {
    GDCLASS(UltraleapHandTracking, Node3D);

public:
    UltraleapHandTracking();
    ~UltraleapHandTracking();

    void start();
    void stop();

    UltraleapTypes::TrackingMode default_tracking_mode = UltraleapTypes::TrackingMode::Desktop;

    Ref<UltraleapDeviceList> devices;

    String service_ip = String("127.0.0.1");
    uint16_t service_port = 12345;

    bool autostart = true;

    UltraleapTypes::TrackingMode get_tracking_mode() const;
    void set_tracking_mode(UltraleapTypes::TrackingMode value);

    Ref<UltraleapFrame> get_last_frame() const;

    Array get_devices() const { return devices->devices; }
    void set_devices(Array value) { devices->devices = value; }

    String get_service_ip() const { return service_ip; }
    void set_service_ip(String value) { service_ip = value; }

    int get_service_port() const { return service_port; }
    void set_service_port(int value) { service_port = value; }

    bool get_autostart() const { return autostart; }
    void set_autostart(bool value) { autostart = value; }

    bool is_connected() const { return connected; }
    bool is_started() const { return started; }

    void set_background_frames_policy(bool value);
    void set_map_points_policy(bool value);
    void set_allow_pause_resume_policy(bool value);

    // Set and get primary device
    void set_primary_device(Ref<UltraleapDevice> device);
    Ref<UltraleapDevice> get_primary_device() const;

    void set_pause(bool value);

    void _notification(int p_what);

protected:
    static void _bind_methods();

private:
    std::atomic_bool is_running = ATOMIC_VAR_INIT(false);
    std::atomic_bool keep_running = ATOMIC_VAR_INIT(true);

    LEAP_CONNECTION connectionHandle = NULL;

    std::thread messageLoop;

    bool started = false;           /* The whole thing is started */
    bool opened = false;            /* Indicate an opened connection */
    bool connected = false;         /* We have confirmed connection */

    Ref<UltraleapDevice> primary_device;

    void serviceMessageLoop();
    void dispose_ultraleap();

    void handle_tracking_event(const LEAP_TRACKING_EVENT* event, uint32_t device_id);

    void handle_connection_event(const LEAP_CONNECTION_EVENT *evt);
    void handle_connection_lost_event(const LEAP_CONNECTION_LOST_EVENT* event);
    void handle_device_event(const LEAP_DEVICE_EVENT* event);
    void handle_device_lost_event(const LEAP_DEVICE_EVENT* event);
    void handle_tracking_mode_event(const LEAP_TRACKING_MODE_EVENT* event, uint32_t device_id);
    void handle_policy_change_event(const LEAP_POLICY_EVENT* event);
    void handle_device_status_change_event(const LEAP_DEVICE_STATUS_CHANGE_EVENT* event, uint32_t device_id);

    void set_policy(UltraleapTypes::PolicyFlag flag, bool value);
    bool get_policy(UltraleapTypes::PolicyFlag flag) const;

    String generate_connection_payload();

    void refresh_device_list();

    Ref<UltraleapDevice> create_device(LEAP_DEVICE_REF ref);

    uint32_t policy_flags = UINT32_MAX;

};

#endif