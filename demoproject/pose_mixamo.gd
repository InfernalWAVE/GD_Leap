 # ****************************************************************
 # * Copyright (c) 2024 Ryan Powell
 # *
 # * This software is released under the MIT License.
 # * See the LICENSE file in the project root for more information.
 # *****************************************************************

extends UltraleapDeviceNode

@export var debug_label: Label
@export var debug_mesh_1: MeshInstance3D
@export var debug_location: Vector3
@export var skeleton: Skeleton3D

@export var arm_move_scale: float = 1.0
@export var should_position_arms: bool = false
@export var should_use_xr_tracker: bool = false
@export var ik_proxy_skeleton: Skeleton3D
@export var ik_proxy_ra: SkeletonIK3D
@export var ik_proxy_la: SkeletonIK3D
@export var position_proxy_skeleton: Skeleton3D

# TODO map initial thumb position, add special case for desired offset rest
@export var thumb_roll: float
@export var thumb_yaw: float

var root_pose: Transform3D
@export var leap_origin: Vector3

var body_tracker: XRBodyTracker = XRBodyTracker.new()
## Body tracking flags
const BODY_TRACKING := \
	XRBodyTracker.BODY_FLAG_UPPER_BODY_SUPPORTED | \
	XRBodyTracker.BODY_FLAG_LOWER_BODY_SUPPORTED | \
	XRBodyTracker.BODY_FLAG_HANDS_SUPPORTED

## Joint tracking flags
const JOINT_TRACKING := \
	XRBodyTracker.JOINT_FLAG_ORIENTATION_TRACKED | \
	XRBodyTracker.JOINT_FLAG_ORIENTATION_VALID | \
	XRBodyTracker.JOINT_FLAG_POSITION_TRACKED | \
	XRBodyTracker.JOINT_FLAG_POSITION_VALID


enum Joint {
	LEFT_LOWER_ARM,
	LEFT_HAND,
	LEFT_THUMB_METACARPAL,
	LEFT_THUMB_PROXIMAL,
	LEFT_THUMB_DISTAL,
	LEFT_INDEX_PROXIMAL,
	LEFT_INDEX_INTERMEDIATE,
	LEFT_INDEX_DISTAL,
	LEFT_MIDDLE_PROXIMAL,
	LEFT_MIDDLE_INTERMEDIATE,
	LEFT_MIDDLE_DISTAL,
	LEFT_RING_PROXIMAL,
	LEFT_RING_INTERMEDIATE,
	LEFT_RING_DISTAL,
	LEFT_LITTLE_PROXIMAL,
	LEFT_LITTLE_INTERMEDIATE,
	LEFT_LITTLE_DISTAL,
	RIGHT_LOWER_ARM,
	RIGHT_HAND,
	RIGHT_THUMB_METACARPAL,
	RIGHT_THUMB_PROXIMAL,
	RIGHT_THUMB_DISTAL,
	RIGHT_INDEX_PROXIMAL,
	RIGHT_INDEX_INTERMEDIATE,
	RIGHT_INDEX_DISTAL,
	RIGHT_MIDDLE_PROXIMAL,
	RIGHT_MIDDLE_INTERMEDIATE,
	RIGHT_MIDDLE_DISTAL,
	RIGHT_RING_PROXIMAL,
	RIGHT_RING_INTERMEDIATE,
	RIGHT_RING_DISTAL,
	RIGHT_LITTLE_PROXIMAL,
	RIGHT_LITTLE_INTERMEDIATE,
	RIGHT_LITTLE_DISTAL,
}

@export var bone_map: Dictionary = {
	Joint.LEFT_LOWER_ARM: {
		"joint_name" : "LeftLowerArm",
		"joint_id" : 9,
	},
	Joint.LEFT_HAND: {
		"joint_name" : "LeftHand",
		"joint_id" : 10,
	},
	Joint.LEFT_THUMB_METACARPAL: {
		"joint_name" : "LeftThumbMetacarpal",
		"joint_id" : 11,
	},
	Joint.LEFT_THUMB_PROXIMAL: {
		"joint_name" : "LeftThumbProximal",
		"joint_id" : 12,
	},
	Joint.LEFT_THUMB_DISTAL: {
		"joint_name" : "LeftThumbDistal",
		"joint_id" : 13,
	},
	Joint.LEFT_INDEX_PROXIMAL: {
		"joint_name" : "LeftIndexProximal",
		"joint_id" : 15,
	},
	Joint.LEFT_INDEX_INTERMEDIATE: {
		"joint_name" : "LeftIndexIntermediate",
		"joint_id" : 16,
	},
	Joint.LEFT_INDEX_DISTAL: {
		"joint_name" : "LeftIndexDistal",
		"joint_id" : 17,
	},
	Joint.LEFT_MIDDLE_PROXIMAL: {
		"joint_name" : "LeftMiddleProximal",
		"joint_id" : 19,
	},
	Joint.LEFT_MIDDLE_INTERMEDIATE: {
		"joint_name" : "LeftMiddleIntermediate",
		"joint_id" : 20,
	},
	Joint.LEFT_MIDDLE_DISTAL: {
		"joint_name" : "LeftMiddleDistal",
		"joint_id" : 21,
	},
	Joint.LEFT_RING_PROXIMAL: {
		"joint_name" : "LeftRingProximal",
		"joint_id" : 23,
	},
	Joint.LEFT_RING_INTERMEDIATE: {
		"joint_name" : "LeftRingIntermediate",
		"joint_id" : 24,
	},
	Joint.LEFT_RING_DISTAL: {
		"joint_name" : "LeftRingDistal",
		"joint_id" : 25,
	},
	Joint.LEFT_LITTLE_PROXIMAL: {
		"joint_name" : "LeftLittleProximal",
		"joint_id" : 27,
	},
	Joint.LEFT_LITTLE_INTERMEDIATE: {
		"joint_name" : "LeftLittleIntermediate",
		"joint_id" : 28,
	},
	Joint.LEFT_LITTLE_DISTAL: {
		"joint_name" : "LeftLittleDistal",
		"joint_id" : 29,
	},
	Joint.RIGHT_LOWER_ARM: {
		"joint_name" : "RightLowerArm",
		"joint_id" : 33,
	},
	Joint.RIGHT_HAND: {
		"joint_name" : "RightHand",
		"joint_id" : 34,
	},
	Joint.RIGHT_THUMB_METACARPAL: {
		"joint_name" : "RightThumbMetacarpal",
		"joint_id" : 35,
	},
	Joint.RIGHT_THUMB_PROXIMAL: {
		"joint_name" : "RightThumbProximal",
		"joint_id" : 36,
	},
	Joint.RIGHT_THUMB_DISTAL: {
		"joint_name" : "RightThumbDistal",
		"joint_id" : 37,
	},
	Joint.RIGHT_INDEX_PROXIMAL: {
		"joint_name" : "RightIndexProximal",
		"joint_id" : 39,
	},
	Joint.RIGHT_INDEX_INTERMEDIATE: {
		"joint_name" : "RightIndexIntermediate",
		"joint_id" : 40,
	},
	Joint.RIGHT_INDEX_DISTAL: {
		"joint_name" : "RightIndexDistal",
		"joint_id" : 41,
	},
	Joint.RIGHT_MIDDLE_PROXIMAL: {
		"joint_name" : "RightMiddleProximal",
		"joint_id" : 43,
	},
	Joint.RIGHT_MIDDLE_INTERMEDIATE: {
		"joint_name" : "RightMiddleIntermediate",
		"joint_id" : 44,
	},
	Joint.RIGHT_MIDDLE_DISTAL: {
		"joint_name" : "RightMiddleDistal",
		"joint_id" : 45,
	},
	Joint.RIGHT_RING_PROXIMAL: {
		"joint_name" : "RightRingProximal",
		"joint_id" : 47,
	},
	Joint.RIGHT_RING_INTERMEDIATE: {
		"joint_name" : "RightRingIntermediate",
		"joint_id" : 48,
	},
	Joint.RIGHT_RING_DISTAL: {
		"joint_name" : "RightRingDistal",
		"joint_id" : 49,
	},
	Joint.RIGHT_LITTLE_PROXIMAL: {
		"joint_name" : "RightLittleProximal",
		"joint_id" : 51,
	},
	Joint.RIGHT_LITTLE_INTERMEDIATE: {
		"joint_name" : "RightLittleIntermediate",
		"joint_id" : 52,
	},
	Joint.RIGHT_LITTLE_DISTAL: {
		"joint_name" : "RightLittleDistal",
		"joint_id" : 53,
	},
}

func _ready() -> void:
	root_pose = Transform3D(skeleton.get_bone_global_pose(0).basis)
	if should_position_arms:
		ik_proxy_ra.start()
		ik_proxy_la.start()
	if should_use_xr_tracker:
		XRServer.add_body_tracker("/leap/body", body_tracker)

func _process(_delta: float) -> void:
	var frame : UltraleapFrame = get_interpolated_frame(Time.get_ticks_usec())
	if frame != null and frame.is_right_hand_visible:
		# arm
		pose_bone(frame.right_hand.arm, 33)
		
		# hand
		pose_hand(frame.right_hand, 34)
		
		# TODO create bone ID map and pose bones according to map
		# TODO create function to auto-map bone IDs using skeleton.find_bone(bone_name)
		# TODO add XRBodyTracker support so you dont need bone IDs
		
		# fingers
		pose_bone(frame.right_hand.thumb.proximal, 35)
		pose_bone(frame.right_hand.thumb.intermediate, 36)
		pose_bone(frame.right_hand.thumb.distal, 37)
		
		pose_bone(frame.right_hand.index.proximal, 39)
		pose_bone(frame.right_hand.index.intermediate, 40)
		pose_bone(frame.right_hand.index.distal, 41)
		
		pose_bone(frame.right_hand.middle.proximal, 43)
		pose_bone(frame.right_hand.middle.intermediate, 44)
		pose_bone(frame.right_hand.middle.distal, 45)
		
		pose_bone(frame.right_hand.ring.proximal, 47)
		pose_bone(frame.right_hand.ring.intermediate, 48)
		pose_bone(frame.right_hand.ring.distal, 49)
		
		pose_bone(frame.right_hand.pinky.proximal, 51)
		pose_bone(frame.right_hand.pinky.intermediate, 52)
		pose_bone(frame.right_hand.pinky.distal, 53)
		
		#debug_mesh_1.set_global_transform(Transform3D(frame.right_hand.arm.get_global_transform().rotated(Vector3.UP, PI).basis, debug_location))
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_bone(frame.left_hand.arm, 9)
		
		# hand
		pose_hand(frame.left_hand, 10)
		
		#fingers
		pose_bone(frame.left_hand.thumb.proximal, 11)
		pose_bone(frame.left_hand.thumb.intermediate, 12)
		pose_bone(frame.left_hand.thumb.distal, 13)
		
		pose_bone(frame.left_hand.index.proximal, 15)
		pose_bone(frame.left_hand.index.intermediate, 16)
		pose_bone(frame.left_hand.index.distal, 17)
		
		pose_bone(frame.left_hand.middle.proximal, 19)
		pose_bone(frame.left_hand.middle.intermediate, 20)
		pose_bone(frame.left_hand.middle.distal, 21)
		
		pose_bone(frame.left_hand.ring.proximal, 23)
		pose_bone(frame.left_hand.ring.intermediate, 24)
		pose_bone(frame.left_hand.ring.distal, 25)
		
		pose_bone(frame.left_hand.pinky.proximal, 27)
		pose_bone(frame.left_hand.pinky.intermediate, 28)
		pose_bone(frame.left_hand.pinky.distal, 29)
	
	if should_use_xr_tracker:
		if frame != null and frame.is_right_hand_visible:
			#shoulder
			body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_SHOULDER, Transform3D(skeleton.get_bone_global_pose(3).basis*skeleton.get_bone_pose(31).basis, skeleton.get_bone_rest(31).origin))
			body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_SHOULDER, JOINT_TRACKING)
			
			#upper arm
			body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, Transform3D(skeleton.get_bone_global_pose(31).basis*skeleton.get_bone_pose(32).basis, skeleton.get_bone_rest(32).origin))
			body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, JOINT_TRACKING)
			
			#lower arm
			body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_LOWER_ARM, Transform3D(skeleton.get_bone_global_pose(32).basis*skeleton.get_bone_pose(33).basis, skeleton.get_bone_rest(33).origin))
			body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_LOWER_ARM, JOINT_TRACKING)
		
			#hand
			body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_HAND, Transform3D(skeleton.get_bone_global_pose(33).basis*skeleton.get_bone_pose(34).basis, skeleton.get_bone_rest(34).origin))
			body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_HAND, JOINT_TRACKING)
		
		body_tracker.set_joint_transform(XRBodyTracker.Joint.JOINT_ROOT, Transform3D(Basis.IDENTITY, Vector3(1.086, 0, -0.624)))
		body_tracker.set_joint_flags(XRBodyTracker.Joint.JOINT_ROOT, JOINT_TRACKING)
		
		body_tracker.body_flags = BODY_TRACKING
		body_tracker.has_tracking_data = true

func pose_hand(hand: UltraleapHand, bone_id: int) -> void:
	# extension doesn't apply rigging transform to hand, we'll do it here
	var hand_transform: Transform3D = hand.wrist*rigging_transform
	
	pose_from_transform(hand_transform, bone_id)
	
	if bone_id == 34:
		debug_label.text = "grab strength: " + str(hand.grab_strength)
		debug_label.text = debug_label.text + "\npinch strength: " + str(hand.pinch_strength)

func pose_bone(bone: UltraleapBone, bone_id: int) -> void:
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform())
	
	pose_from_transform(bone_transform, bone_id)

# TODO cache global transforms to avoid recalculations
func pose_from_transform(new_transform: Transform3D, bone_id: int) -> void:
	var bone_transform: Transform3D = new_transform 
	
	var rest_transform: Transform3D = skeleton.get_bone_global_rest(bone_id)
	var local_rest_transform: Transform3D = skeleton.get_bone_rest(bone_id)
	
	var parent_rest_transform: Transform3D = skeleton.get_bone_global_rest(skeleton.get_bone_parent(bone_id))
	var parent_local_rest_transform: Transform3D = skeleton.get_bone_rest(skeleton.get_bone_parent(bone_id))
	var inverse_parent_local_rest = parent_local_rest_transform.affine_inverse()

	var rest_offset: Transform3D = parent_rest_transform.affine_inverse()*rest_transform
	var local_rest_offset: Transform3D = parent_local_rest_transform.affine_inverse()*local_rest_transform
	var inverse_rest_offset: Transform3D = rest_offset.affine_inverse()
	var inverse_local_rest_offset: Transform3D = local_rest_offset.affine_inverse()
	
	var rest_offset_offset: Transform3D = rest_offset.affine_inverse()*local_rest_offset

	var world_bone = new_transform.rotated(Vector3.UP,PI) # correct Z front
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	bone_transform = world_bone
	bone_transform = parent_transform.affine_inverse()*bone_transform
	
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2)
	
	var rig_transform = rigging_transform
	
	# thumbs
	match bone_id:
		35:
			bone_transform = bone_transform.rotated_local(Vector3.UP, -deg_to_rad(thumb_roll))
		36,37:
			bone_transform = bone_transform.rotated(Vector3.UP, -deg_to_rad(thumb_roll))
		11:
			bone_transform = bone_transform.rotated_local(Vector3.UP, deg_to_rad(thumb_roll))
		12,13:
			bone_transform = bone_transform.rotated(Vector3.UP, deg_to_rad(thumb_roll))
	
	
	## set pose rotation
	var pre_position_rotation = bone_transform.basis.get_rotation_quaternion()
	skeleton.set_bone_pose_rotation(bone_id, pre_position_rotation)
	
	
	# arms position
	if should_position_arms:
		match bone_id:
			9:
				position_proxy_skeleton.set_bone_pose_rotation(bone_id, Transform3D(parent_rest_transform.affine_inverse()*skeleton.get_bone_global_pose(bone_id)*local_rest_transform).basis.get_rotation_quaternion())
				var arm_length: float = (parent_rest_transform.origin - rest_transform.origin).length()
				var arm_offset: Vector3 = Vector3(0.0, arm_length, 0.0)
				var arm_position: Vector3 = new_transform.origin
				arm_position = Vector3(arm_position.z+leap_origin.z, -arm_position.x-leap_origin.x, -arm_position.y-leap_origin.y)
				arm_position = arm_position * arm_move_scale
				arm_position = arm_offset + arm_position
				
				position_proxy_skeleton.set_bone_pose_position(bone_id, arm_position)
				
				var target_transform: Transform3D = position_proxy_skeleton.get_bone_global_pose(bone_id)
				ik_proxy_la.set_target_transform(target_transform)
				
				var upper_arm_transform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_rest(bone_id-2).affine_inverse()*ik_proxy_skeleton.get_bone_global_pose(bone_id-1)*parent_local_rest_transform*local_rest_transform)
				var shoulder_tramsform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_pose(bone_id-2))
				skeleton.set_bone_pose_rotation(bone_id-2, shoulder_tramsform.basis.get_rotation_quaternion())
				skeleton.set_bone_pose_rotation(bone_id-1, upper_arm_transform.basis.get_rotation_quaternion())
				
			33:
				position_proxy_skeleton.set_bone_pose_rotation(bone_id, Transform3D(parent_rest_transform.affine_inverse()*skeleton.get_bone_global_pose(bone_id)*local_rest_transform).basis.get_rotation_quaternion())
				var arm_length: float = (parent_rest_transform.origin - rest_transform.origin).length()
				var arm_offset: Vector3 = Vector3(0.0, arm_length, 0.0)
				var arm_position: Vector3 = new_transform.origin
				arm_position = Vector3(-arm_position.z-leap_origin.z, arm_position.x-leap_origin.x, -arm_position.y-leap_origin.y)
				arm_position = arm_position * arm_move_scale
				arm_position = arm_offset + arm_position
				
				position_proxy_skeleton.set_bone_pose_position(bone_id, arm_position)
				
				var target_transform: Transform3D = (position_proxy_skeleton.get_bone_global_pose(bone_id))
				ik_proxy_ra.set_target_transform(target_transform)
				
				var upper_arm_transform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_rest(bone_id-2).affine_inverse()*ik_proxy_skeleton.get_bone_global_pose(bone_id-1)*parent_local_rest_transform*local_rest_transform)
				var shoulder_tramsform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_pose(bone_id-2))
				skeleton.set_bone_pose_rotation(bone_id-2, shoulder_tramsform.basis.get_rotation_quaternion())
				skeleton.set_bone_pose_rotation(bone_id-1, upper_arm_transform.basis.get_rotation_quaternion())
	


