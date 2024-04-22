#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demoproject/bin/GD_Leap.{}.{}.framework/GD_Leap.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demoproject/bin/GD_Leap{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

    env.Append(LIBS=['LeapC'])
    env.Append(LIBPATH = ['C:/Program Files/Ultraleap/LeapSDK/lib/x64'])
    env.Append(CPPPATH = ["C:/Program Files/Ultraleap/LeapSDK/include"])

    post_actions = [
        Copy("demoproject/bin/LeapC.dll", "C:/Program Files/Ultraleap/LeapSDK/lib/x64/LeapC.dll")
]

if library is not None:
    for post_action in post_actions:
            AddPostAction(library, post_action)

    Default(library)
else:
    print("Nothing to build, weird")