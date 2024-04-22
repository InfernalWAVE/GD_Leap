/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ULTRALEAP_DEVICE_NODE_H
#define ULTRALEAP_DEVICE_NODE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "device.h"
#include "ultraleap.h"
#include "frame.h"

using namespace godot;

class UltraleapDeviceNode : public Node3D {
    GDCLASS(UltraleapDeviceNode, Node3D);

public:
    NodePath tracker;

    NodePath get_tracker() { return tracker; }
    void set_tracker(NodePath value);

    UltraleapTypes::TrackingMode tracking_mode = UltraleapTypes::TrackingMode::Desktop;

    Ref<UltraleapFrame> last_frame;

    UltraleapTypes::TrackingMode get_tracking_mode() { return tracking_mode; }
    void set_tracking_mode(UltraleapTypes::TrackingMode value);

    void device_added(Ref<UltraleapDevice> device);
    void device_removed(Ref<UltraleapDevice> device);

    void _notification(int p_what);
    void _get_property_list(List<PropertyInfo> *p_list) const;
    bool _set(const StringName &p_name, const Variant &p_value);
    bool _get(const StringName &p_name, Variant &r_ret) const;

    Ref<UltraleapFrame> get_last_frame();
    Ref<UltraleapFrame> get_interpolated_frame(int64_t time);

    void set_run_in_editor(bool value);
    bool get_run_in_editor() { return run_in_editor; }

    Transform3D get_rigging_transform();
    void set_rigging_transform(Transform3D t);


protected:
    static void _bind_methods();

private:
    Array devices = Array();

    int chosen_serial = 0;
    bool run_in_editor = false;

    UltraleapHandTracking* get_tracking();
    
    Ref<UltraleapDevice> current_device;

    void open_and_subscribe();
    void unsubscribe_and_close();
    void setup_device(Ref<UltraleapDevice> device);
    
    bool should_run();

    int rigging_transform_type = 0;

    // Used as a cache for when/if no device is connected yet (in editor for example)
    Transform3D rigging_transform;

};
#endif