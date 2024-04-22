/****************************************************************
 * Copyright (c) 2024 Ryan Powell
 *
 * This software is released under the MIT License.
 * See the LICENSE file in the project root for more information.
 ****************************************************************/

#ifndef HELLOWORLD_REGISTER_TYPES_H
#define HELLOWORLD_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_hello_world(ModuleInitializationLevel p_level);
void uninitialize_hello_world(ModuleInitializationLevel p_level);

#endif