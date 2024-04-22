 # ****************************************************************
 # * Copyright (c) 2024 Ryan Powell
 # *
 # * This software is released under the MIT License.
 # * See the LICENSE file in the project root for more information.
 # *****************************************************************

extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

func offset_basis(new_basis: Basis, is_right: bool) -> Basis:
	if is_right:
		return new_basis.rotated(Vector3.UP, -PI/2.0).rotated(Vector3.RIGHT, -PI/2.0)
	else:
		return new_basis.rotated(Vector3.UP, PI/2.0).rotated(Vector3.RIGHT, -PI/2.0)

func pose_bone(bone: UltraleapBone, bone_id: int, is_right: bool) -> void:
	# default Metahuman rig in Blender has opposite bone convention for L v R
	var offset_transform: Transform3D = Transform3D(offset_basis(Basis.IDENTITY, is_right))
	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# get global orientation in Goot global coordinates (rigging transform applied in extension)
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	# undo offset introduced by rigging transform
	bone_transform = offset_transform * bone_transform
	
	# get pose relative to rest
	bone_transform = reference_transform * bone_transform
	
	# get pose in parent bone space
	bone_transform = parent_transform.affine_inverse()*bone_transform
	
	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())

func pose_hand(hand: UltraleapHand, bone_id: int, is_right: bool) -> void:
	# default Metahuman rig in Blender has opposite bone convention for L v R
	var offset_transform: Transform3D = Transform3D(offset_basis(Basis.IDENTITY, is_right))
	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# apply rigging transform. LeapC coordinates -> Godot global coordinates
	var hand_transform: Transform3D = hand.wrist*rigging_transform
	
	# get global orientation
	hand_transform =  Transform3D(hand_transform.basis)
	
	# undo offset introduced by rigging transform
	hand_transform = offset_transform * hand_transform
	
	# get pose relative to rest
	hand_transform = reference_transform * hand_transform
	
	# get pose in parent bone space
	hand_transform = parent_transform.affine_inverse()*hand_transform
	
	# set pose
	skeleton.set_bone_pose_rotation(bone_id, hand_transform.basis.get_rotation_quaternion())

func _process(_delta):
	var frame : UltraleapFrame = get_interpolated_frame(Time.get_ticks_usec())
	if frame != null and frame.is_right_hand_visible:
		# arm
		pose_bone(frame.right_hand.arm, 33, true)
		
		# hand
		pose_hand(frame.right_hand, 34, true)
		
		# fingers
		pose_bone(frame.right_hand.index.proximal, 44, true)
		
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_bone(frame.left_hand.arm, 14, false)
		
		# hand
		pose_hand(frame.left_hand, 15, false)
