#ifndef ULTRALEAP_DEVICE_H
#define ULTRALEAP_DEVICE_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <LeapC.h>

#include <mutex>

#include "types.h"
#include "frame.h"

using namespace godot;

class UltraleapDevice: public Resource{
    GDCLASS(UltraleapDevice, Resource);

public:
    ~UltraleapDevice();

    uint32_t baseline;
    String serial;
    uint32_t id;

    UltraleapTypes::TrackingMode tracking_mode = UltraleapTypes::TrackingMode::Desktop;

    Ref<UltraleapFrame> last_frame_ref;

    Ref<UltraleapFrame> get_last_frame();
    Ref<UltraleapFrame> get_interpolated_frame(int64_t time);

    UltraleapTypes::TrackingMode get_tracking_mode() { return tracking_mode; }
    void set_tracking_mode(UltraleapTypes::TrackingMode value);

    uint32_t get_baseline() { return baseline; }
    void set_baseline(uint32_t value) { baseline = value; }

    String get_serial() { return serial; }
    void set_serial(String value) { serial = value; }

    uint32_t get_id() { return id; }
    void set_id(uint32_t value) { id = value; }

    Transform3D get_rigging_transform() { return rigging_transform; }
    void set_rigging_transform(Transform3D t) { rigging_transform = t; }

    void subscribe();
    void unsubscribe();

    void open();
    void close();

    LEAP_DEVICE_REF device_ref;
    LEAP_DEVICE device;
    LEAP_CONNECTION* connection = NULL;
    LEAP_CLOCK_REBASER rebaser;

    void tracking_mode_changed(UltraleapTypes::TrackingMode value);

    void on_frame_received(const LEAP_TRACKING_EVENT* frame);

    void on_lost();

    bool is_opened() { return opened; }
    bool is_subscribed() { return subscribed; }

protected:
    static void _bind_methods();

private:
    std::mutex frame_mutex;
    std::mutex image_mutex;

    bool opened = false;
    bool subscribed = false;
    bool interpolation_available = false;
    
    Transform3D rigging_transform;

};

#endif