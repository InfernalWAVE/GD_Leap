#include "device_node.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <LeapC.h>

#include "ultraleap.h"

using namespace godot;

void UltraleapDeviceNode::device_added(Ref<UltraleapDevice> device) {
    if (!devices.has(device->serial)) {
        devices.append(device->serial);
        notify_property_list_changed();
    }

    // If user has chosen "Any", the strategy here is to use the newly connected device
    if (chosen_serial == 0) {
        setup_device(device);
    }
    // Otherwise we check for the serial and subscribe to the device
    else {
        if (get_tracking() != NULL) {
            if ((chosen_serial - 1) < get_tracking()->devices->size()) {
                if (devices[chosen_serial - 1] == device->serial) {
                    setup_device(device);
                }
            }
        }
    }
}

void UltraleapDeviceNode::device_removed(Ref<UltraleapDevice> device) {
    if (!devices.has(device->serial)) {
        return;
    }

    // If the device being removed is the same as the one being used by this
    // node, unsubscribe and close it
    if (current_device != NULL) {
        if (device->serial == current_device->serial) {
            unsubscribe_and_close();
            current_device = Ref<UltraleapDevice>(NULL);
        }
    }

    devices.erase(device->serial);

    notify_property_list_changed();
}

void UltraleapDeviceNode::open_and_subscribe() {
    if (current_device != NULL) {
        if (!current_device->is_opened()) {
            current_device->open();
        }
        if (!current_device->is_subscribed()) {
            current_device->subscribe();
        }
    }
}

void UltraleapDeviceNode::unsubscribe_and_close() {
    if (current_device != NULL) {
        if (current_device->is_subscribed()) {
            current_device->unsubscribe();
        }
        if (current_device->is_opened()) {
            current_device->close();
        }
    }
}

void UltraleapDeviceNode::setup_device(Ref<UltraleapDevice> device) {
    if (current_device != NULL && should_run()) {
        unsubscribe_and_close();
    }
    current_device = device;
    current_device->set_rigging_transform(rigging_transform);
    if (should_run()) {
        open_and_subscribe();
    }
}

void UltraleapDeviceNode::_bind_methods() {
    PropertyInfo tracking_mode_info = PropertyInfo(
        Variant::INT,
        "tracking_mode",
        PROPERTY_HINT_ENUM,
        "Desktop, HMD, Screentop"
    );

    ClassDB::bind_method(D_METHOD("set_tracker", "tracker"), &UltraleapDeviceNode::set_tracker);
    ClassDB::bind_method(D_METHOD("get_tracker"), &UltraleapDeviceNode::get_tracker);

    ClassDB::bind_method(D_METHOD("get_last_frame"), &UltraleapDeviceNode::get_last_frame);
    ClassDB::bind_method(D_METHOD("get_interpolated_frame", "time"), &UltraleapDeviceNode::get_interpolated_frame);
    
    ClassDB::bind_method(D_METHOD("device_added", "device"), &UltraleapDeviceNode::device_added);
    ClassDB::bind_method(D_METHOD("device_removed", "device"), &UltraleapDeviceNode::device_removed);

    ClassDB::bind_method(D_METHOD("set_tracking_mode", "tracking_mode"), &UltraleapDeviceNode::set_tracking_mode, DEFVAL(0));
    ClassDB::bind_method(D_METHOD("get_tracking_mode"), &UltraleapDeviceNode::get_tracking_mode);

    ClassDB::bind_method(D_METHOD("set_run_in_editor", "run_in_editor"), &UltraleapDeviceNode::set_run_in_editor, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("get_run_in_editor"), &UltraleapDeviceNode::get_run_in_editor);

    ClassDB::bind_method(D_METHOD("get_rigging_transform"), &UltraleapDeviceNode::get_rigging_transform);
    ClassDB::bind_method(D_METHOD("set_rigging_transform"), &UltraleapDeviceNode::set_rigging_transform);

    ClassDB::add_property(
        "UltraleapDeviceNode",
        PropertyInfo(
            Variant::NODE_PATH,
            "tracker",
            PROPERTY_HINT_NODE_TYPE,
            "UltraleapHandTracking"
        ),
        "set_tracker",
        "get_tracker"
    );

    ClassDB::add_property(
        "UltraleapDeviceNode",
        tracking_mode_info,
        "set_tracking_mode",
        "get_tracking_mode"
    );

    ClassDB::add_property(
        "UltraleapDeviceNode",
        PropertyInfo(
            Variant::BOOL,
            "run_in_editor"
        ),
        "set_run_in_editor",
        "get_run_in_editor"
    );

    ClassDB::add_property_group(
        "UltraleapDeviceNode",
        "Rigging",
        "rigging"
    );

    ClassDB::add_property(
        "UltraleapDeviceNode",
        PropertyInfo(
            Variant::TRANSFORM3D,
            "rigging_transform",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_NO_EDITOR
        ),
        "set_rigging_transform",
        "get_rigging_transform"
    );

    // Signals

    ClassDB::add_signal(
        "UltraleapDeviceNode",
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
        "UltraleapDeviceNode",
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

void UltraleapDeviceNode::set_tracker(NodePath value) {
    if (!tracker.is_empty()) {
        Node* tracking = get_node_or_null(tracker);
        if (tracking != NULL) {
            tracking->disconnect("device_added", Callable((Object*)this, "device_added"));
        }
    }

    Node* tracking = get_node_or_null(tracker);
    if (tracking != NULL) {
        tracking->connect("device_added", Callable((Object*)this, "device_added"));
    }
    tracker = value;
}

void UltraleapDeviceNode::_notification(int p_what) {
    if (p_what == Node::NOTIFICATION_READY) {
        Node *tracking_node = get_node_or_null(tracker);
        if (tracking_node != NULL) {
            UltraleapHandTracking* tracking = Object::cast_to<UltraleapHandTracking>(tracking_node);

            // Connect to tracking signals
            tracking->connect("device_added", Callable((Object*)this, "device_added"));
            tracking->connect("device_removed", Callable((Object*)this, "device_removed"));

            // Build a list of serial numbers to choose from
            for (int i = 0; i < tracking->devices->size(); i++) {
                Ref<UltraleapDevice> device = tracking->devices->get(i);
                if (!devices.has(device->serial)) {
                    devices.append(device->serial);
                }
            }
        }
    }
    else if (p_what == Node::NOTIFICATION_ENTER_TREE) {
        set_process_internal(true);
    }
    else if (p_what == Node::NOTIFICATION_INTERNAL_PROCESS) {
        if (current_device == NULL || !should_run()) {
            return;
        }

        if (current_device->tracking_mode != tracking_mode) {
            tracking_mode = current_device->tracking_mode;
            emit_signal("tracking_mode_changed", current_device->tracking_mode);
        }

        Ref<UltraleapFrame> new_frame = current_device->get_last_frame();

        if (new_frame == NULL) {
            return;
        }

        if (last_frame == NULL || last_frame->id != new_frame->id) {
            last_frame = new_frame;
            emit_signal("frame_received", last_frame);
        }
    }
}

void UltraleapDeviceNode::_get_property_list(List<PropertyInfo> *p_list) const {
    p_list->push_back(
        PropertyInfo(
            Variant::INT,
            "device",
            PROPERTY_HINT_ENUM,
            String(",").join(devices).insert(0, "Any,")
        )
    );

    p_list->push_back(
        PropertyInfo(
            Variant::INT,
            "rigging/presets",
            PROPERTY_HINT_ENUM,
            "Blender,Custom"
        )
    );

    if (rigging_transform_type == 1) {
        p_list->push_back(
            PropertyInfo(
                Variant::TRANSFORM3D,
                "rigging/transform"
            )
        );
    }
}

bool UltraleapDeviceNode::_get(const StringName &p_name, Variant &r_ret) const {
    String name = p_name;
    if (name == "device") {
        r_ret = chosen_serial;
        return true;
    }
    else if (name == "rigging/presets") {
        r_ret = rigging_transform_type;
        return true;
    }
    else if (name == "rigging/transform") {
        r_ret = rigging_transform;
        return true;
    }
    return false;
}

bool UltraleapDeviceNode::_set(const StringName &p_name, const Variant &p_value) {
    String name = p_name;
    if (name == "device") {
        chosen_serial = p_value;
        notify_property_list_changed();
        return true;
    }
    else if (name == "rigging/presets") {
        rigging_transform_type = p_value;
        notify_property_list_changed();
        return true;
    }
    else if (name == "rigging/transform") {
        rigging_transform = p_value;
        return true;
    }
    return false;
}

UltraleapHandTracking* UltraleapDeviceNode::get_tracking() {
    return Object::cast_to<UltraleapHandTracking>(get_node_or_null(tracker));
}

Ref<UltraleapFrame> UltraleapDeviceNode::get_last_frame() {
    return last_frame;
}

Ref<UltraleapFrame> UltraleapDeviceNode::get_interpolated_frame(int64_t time) {
    if (current_device == NULL) {
        return NULL;
    }
    return current_device->get_interpolated_frame(time);
}

void UltraleapDeviceNode::set_tracking_mode(UltraleapTypes::TrackingMode value) {
    // If we're in the editor, we just change the value displayed,
    // otherwise we make a request on the device to change the tracking mode
    if (!should_run()) {
        tracking_mode = value;
    }
    else {
        if (current_device != NULL) {
            current_device->set_tracking_mode(value);
        }
    }
}

bool UltraleapDeviceNode::should_run() {
    return (Engine::get_singleton()->is_editor_hint() && run_in_editor) || !Engine::get_singleton()->is_editor_hint();
}

void UltraleapDeviceNode::set_run_in_editor(bool value) {
    // If we are running in the editor and have a device, we close it
    if (should_run() && current_device != NULL) {
        // God knows what happens if it wasn't previously opened...
        unsubscribe_and_close();
    }

    run_in_editor = value;

    // Now if we should run, let open the device
    if (should_run() && current_device != NULL) {
        open_and_subscribe();
    }
}

Transform3D UltraleapDeviceNode::get_rigging_transform() {
    if (current_device != NULL) {
        return current_device->get_rigging_transform();
    }

    return rigging_transform;
}

void UltraleapDeviceNode::set_rigging_transform(Transform3D t) {
    if (current_device != NULL) {
        current_device->set_rigging_transform(t);
    }

    rigging_transform = t;
}