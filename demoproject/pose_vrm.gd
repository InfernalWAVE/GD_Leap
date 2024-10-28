 # ****************************************************************
 # * Copyright (c) 2024 Ryan Powell
 # *
 # * This software is released under the MIT License.
 # * See the LICENSE file in the project root for more information.
 # *****************************************************************

extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

@export var arm_move_scale: float = 1.0
@export var should_position_arms: bool = false
@export var ik_proxy_skeleton: Skeleton3D
@export var ik_proxy_ra: SkeletonIK3D
@export var ik_proxy_la: SkeletonIK3D
@export var position_proxy_skeleton: Skeleton3D

# TODO map initial thumb position, add special case for desired offset rest
@export var thumb_roll: float = 35.0
@export var thumb_yaw: float = 90.0
@export var leap_origin: Vector3 = Vector3(0.265, -0.505, -0.44) # this is what works well for my desk/chair/body setup
const RIGGING_TRANSFORM: Transform3D = Transform3D(Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(-1.0, 0.0, 0.0), Vector3.ZERO)

func _ready() -> void:
	rigging_transform = RIGGING_TRANSFORM
	if should_position_arms:
		ik_proxy_ra.start()
		ik_proxy_la.start()

func _process(_delta: float) -> void:
	var frame : UltraleapFrame = get_interpolated_frame(Time.get_ticks_usec())
	if frame != null and frame.is_right_hand_visible:
		# arm
		pose_bone(frame.right_hand.arm, 29)
		
		# hand
		pose_hand(frame.right_hand, 30)
		
		# TODO create bone ID map and pose bones according to map
		# TODO create function to auto-map bone IDs using skeleton.find_bone(bone_name)
		# TODO add XRBodyTracker support so you dont need bone IDs
		
		# fingers
		pose_bone(frame.right_hand.thumb.proximal, 31)
		pose_bone(frame.right_hand.thumb.intermediate, 32)
		pose_bone(frame.right_hand.thumb.distal, 33)
		
		pose_bone(frame.right_hand.index.proximal, 34)
		pose_bone(frame.right_hand.index.intermediate, 35)
		pose_bone(frame.right_hand.index.distal, 36)
		
		pose_bone(frame.right_hand.middle.proximal, 37)
		pose_bone(frame.right_hand.middle.intermediate, 38)
		pose_bone(frame.right_hand.middle.distal, 39)
		
		pose_bone(frame.right_hand.ring.proximal, 40)
		pose_bone(frame.right_hand.ring.intermediate, 41)
		pose_bone(frame.right_hand.ring.distal, 42)
		
		pose_bone(frame.right_hand.pinky.proximal, 43)
		pose_bone(frame.right_hand.pinky.intermediate, 44)
		pose_bone(frame.right_hand.pinky.distal, 45)
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_bone(frame.left_hand.arm, 10)
		
		# hand
		pose_hand(frame.left_hand, 11)
		
		#fingers
		pose_bone(frame.left_hand.thumb.proximal, 12)
		pose_bone(frame.left_hand.thumb.intermediate, 13)
		pose_bone(frame.left_hand.thumb.distal, 14)
		
		pose_bone(frame.left_hand.index.proximal, 15)
		pose_bone(frame.left_hand.index.intermediate, 16)
		pose_bone(frame.left_hand.index.distal, 17)
		
		pose_bone(frame.left_hand.middle.proximal, 18)
		pose_bone(frame.left_hand.middle.intermediate, 19)
		pose_bone(frame.left_hand.middle.distal, 20)
		
		pose_bone(frame.left_hand.ring.proximal, 21)
		pose_bone(frame.left_hand.ring.intermediate, 22)
		pose_bone(frame.left_hand.ring.distal, 23)
		
		pose_bone(frame.left_hand.pinky.proximal, 24)
		pose_bone(frame.left_hand.pinky.intermediate, 25)
		pose_bone(frame.left_hand.pinky.distal, 26)

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
	var bone_transform: Transform3D = new_transform # transform from leap
	
	var rest_transform: Transform3D = skeleton.get_bone_global_rest(bone_id) # global rest
	var local_rest_transform: Transform3D = skeleton.get_bone_rest(bone_id) # local pose
	
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id)) # parent global pose
	var parent_rest_transform: Transform3D = skeleton.get_bone_global_rest(skeleton.get_bone_parent(bone_id)) # parent global rest
	var parent_local_rest_transform: Transform3D = skeleton.get_bone_rest(skeleton.get_bone_parent(bone_id)) # parent local rest

	bone_transform = new_transform.rotated(Vector3.UP,PI) # correct Z front
	bone_transform = parent_transform.affine_inverse()*bone_transform # in parent space
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2) # undo offset introduced by rigging transform
	
	# correct for thumb roll
	match bone_id:
		31: # right 1st
			bone_transform = bone_transform.rotated_local(Vector3.UP, -deg_to_rad(thumb_roll))
		32, 33: # right 2nd & 3rd
			bone_transform = bone_transform.rotated(Vector3.UP, -deg_to_rad(thumb_roll))
		12: # left 1st
			bone_transform = bone_transform.rotated_local(Vector3.UP, deg_to_rad(thumb_roll))
		13, 14: # left 2nd & 3rd
			bone_transform = bone_transform.rotated(Vector3.UP, deg_to_rad(thumb_roll))
	
	# set pose rotation
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())
	
	# arms position
	if should_position_arms:
		match bone_id:
			10: # left
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
				skeleton.set_bone_pose_rotation(bone_id-1, upper_arm_transform.basis.get_rotation_quaternion())
				pass
				
			29: # right
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
				skeleton.set_bone_pose_rotation(bone_id-1, upper_arm_transform.basis.get_rotation_quaternion())
	
