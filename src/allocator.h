/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <LeapC.h>
#include <godot_cpp/core/memory.hpp>

static void* allocate(uint32_t size, eLeapAllocatorType typeHint, void* state) {
    void* ptr = memalloc(size);
    return ptr;
}

static void deallocate(void* ptr, void* state) {
    if (!ptr) {
        return; 
    }
    memfree(ptr);
}

#endif