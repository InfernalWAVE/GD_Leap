#include "device_list.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include "device.h"

using namespace godot;

UltraleapDeviceList::~UltraleapDeviceList() {
    UtilityFunctions::print("Destroying device list");
}

bool UltraleapDeviceList::has_device(Ref<UltraleapDevice> device) {
    for (int index = 0; index < devices.size(); index++) {
        Ref<UltraleapDevice> dev = devices[index];
        if (dev->serial == device->serial) {
            return true;
        }
    }

    return false;
}

bool UltraleapDeviceList::add_device(Ref<UltraleapDevice> device) {
    devices.append(device);
    return true;
}

bool UltraleapDeviceList::remove_device(Ref<UltraleapDevice> device) {
    int index = find(device, 0);
    if (index != -1) {
        devices.remove_at(index);
    }
    return true;
}

int UltraleapDeviceList::find(Ref<UltraleapDevice> device, int from = 0) {
    for (int index = from; index < devices.size(); index++) {
        Ref<UltraleapDevice> dev = devices[index];
        if (dev->serial == device->serial) {
            return index;
        }
    }

    return -1;
}

Ref<UltraleapDevice> UltraleapDeviceList::find_by_id(uint32_t id) {
    for (int index = 0; index < devices.size(); index++) {
        Ref<UltraleapDevice> dev = devices[index];
        if (dev->id == id) {
            return dev;
        }
    }

    return NULL;
}

Ref<UltraleapDevice> UltraleapDeviceList::find_by_serial(String serial) {
    for (int index = 0; index < devices.size(); index++) {
        Ref<UltraleapDevice> dev = devices[index];
        if (dev->serial == serial) {
            return dev;
        }
    }

    return NULL;
}

void UltraleapDeviceList::_bind_methods() {
}