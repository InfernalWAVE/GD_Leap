#include "frame.h"
#include "types.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void UltraleapFrame::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_left_hand"), &UltraleapFrame::get_left_hand);
    ClassDB::bind_method(D_METHOD("set_left_hand", "left_hand"), &UltraleapFrame::set_left_hand);

    ClassDB::bind_method(D_METHOD("get_right_hand"), &UltraleapFrame::get_right_hand);
    ClassDB::bind_method(D_METHOD("set_right_hand", "right_hand"), &UltraleapFrame::set_right_hand);

    ClassDB::bind_method(D_METHOD("get_is_left_hand_visible"), &UltraleapFrame::get_is_left_hand_visible);
    ClassDB::bind_method(D_METHOD("set_is_left_hand_visible", "is_left_hand_visible"), &UltraleapFrame::set_is_left_hand_visible);

    ClassDB::bind_method(D_METHOD("get_is_right_hand_visible"), &UltraleapFrame::get_is_right_hand_visible);
    ClassDB::bind_method(D_METHOD("set_is_right_hand_visible", "is_right_hand_visible"), &UltraleapFrame::set_is_right_hand_visible);

    ClassDB::bind_method(D_METHOD("get_framerate"), &UltraleapFrame::get_framerate);
    ClassDB::bind_method(D_METHOD("set_framerate", "framerate"), &UltraleapFrame::set_framerate);

    ClassDB::add_property(
        "UltraleapFrame",
        PropertyInfo(
            Variant::OBJECT,
            "left_hand",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapHand"
        ),
        "set_left_hand",
        "get_left_hand"
    );

    ClassDB::add_property(
        "UltraleapFrame",
        PropertyInfo(
            Variant::OBJECT,
            "right_hand",
            PROPERTY_HINT_RESOURCE_TYPE,
            "UltraleapHand"
        ),
        "set_right_hand",
        "get_right_hand"
    );

    ClassDB::add_property(
        "UltraleapFrame",
        PropertyInfo(
            Variant::FLOAT,
            "is_left_hand_visible"
        ),
        "set_is_left_hand_visible",
        "get_is_left_hand_visible"
    );

    ClassDB::add_property(
        "UltraleapFrame",
        PropertyInfo(
            Variant::FLOAT,
            "is_right_hand_visible"
        ),
        "set_is_right_hand_visible",
        "get_is_right_hand_visible"
    );

    ClassDB::add_property(
        "UltraleapFrame",
        PropertyInfo(
            Variant::FLOAT,
            "framerate"
        ),
        "set_framerate",
        "get_framerate"
    );
}

void UltraleapFrame::fill_frame_data(Ref<UltraleapFrame> ul_frame, const LEAP_TRACKING_EVENT* frame, Transform3D rigging_transform) {
    ul_frame->id = frame->tracking_frame_id;
    ul_frame->framerate = frame->framerate;
    ul_frame->is_left_hand_visible = false;
    ul_frame->is_right_hand_visible = false;

    for (size_t i = 0; i < frame->nHands; i++)
    {
        if (frame->pHands[i].type == eLeapHandType_Left) {
            if (!ul_frame->left_hand.is_valid()) {
                ul_frame->left_hand.instantiate();
            }
            UltraleapHand::fill_hand_data(ul_frame->get_left_hand(), &frame->pHands[i], rigging_transform);
            ul_frame->is_left_hand_visible = true;
        }
        else {
            if (!ul_frame->right_hand.is_valid()) {
                ul_frame->right_hand.instantiate();
            }
            UltraleapHand::fill_hand_data(ul_frame->get_right_hand(), &frame->pHands[i], rigging_transform);
            ul_frame->is_right_hand_visible = true;
        }
    }
}