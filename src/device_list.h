/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ULTRALEAP_DEVICE_LIST_H
#define ULTRALEAP_DEVICE_LIST_H

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "device.h"

using namespace godot;

class UltraleapDeviceList : public Resource {
    GDCLASS(UltraleapDeviceList, Resource);

public:
    ~UltraleapDeviceList();

    Array devices;
    
    bool add_device(Ref<UltraleapDevice> device);
    bool remove_device(Ref<UltraleapDevice> device);
    bool has_device(Ref<UltraleapDevice> device);
    int find(Ref<UltraleapDevice> device, int from);
    int size() { return devices.size(); }

    Ref<UltraleapDevice> get(int index) { return ((Ref<UltraleapDevice>)devices[index]); }

    Ref<UltraleapDevice> find_by_id(uint32_t id);
    Ref<UltraleapDevice> find_by_serial(String serial);

protected:
    static void _bind_methods();
};

#endif