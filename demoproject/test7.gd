extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

@export var test_transform: Transform3D

func offset_arm_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	if !is_right:
		transform_offset = transform_offset.rotated_local(Vector3.UP,PI)
		transform_offset= transform_offset.rotated(Vector3.FORWARD, -PI)
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI).rotated(Vector3.FORWARD, -PI/2)
	
	return transform_offset

func offset_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	if !is_right:
		transform_offset = transform_offset.rotated(Vector3.UP, PI)
	
	transform_offset = transform_offset.rotated_local(Vector3.UP, -PI/2)
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI).rotated(Vector3.FORWARD,-PI/2).rotated(Vector3.UP, PI/2)
	
	return transform_offset


# TODO FINISH THE THUMBBBBBSSSS
func offset_thumb_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	if is_right:
		transform_offset = transform_offset.rotated_local(Vector3.UP, -PI/4).rotated(Vector3.UP, PI/2)
	else:
		transform_offset = transform_offset.rotated_local(Vector3.UP, PI/4).rotated(Vector3.UP, -PI/2)
	
	return transform_offset


func pose_finger(bone: UltraleapBone, bone_id: int, is_right: bool, is_thumb: bool = false) -> void:

	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# get global orientation in Goot global coordinates (rigging transform applied in extension)
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	# undo offset introduced by rigging transform
	bone_transform = offset_transform(bone_transform, is_right)
	
	# handle thumb offset
	if is_thumb:
		bone_transform = offset_thumb_transform(bone_transform, is_right)
	
	# get pose relative to rest
	bone_transform = reference_transform * bone_transform
	
	# get pose in parent bone space
	bone_transform = parent_transform.affine_inverse()*bone_transform

	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())
	
func pose_arm(bone: UltraleapBone, bone_id: int, is_right: bool) -> void:

	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# get global orientation in Goot global coordinates (rigging transform applied in extension)
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	# undo offset introduced by rigging transform
	bone_transform = offset_arm_transform(bone_transform, is_right)
	
	# get pose relative to rest
	bone_transform = reference_transform * bone_transform
	
	# get pose in parent bone space
	bone_transform = parent_transform.affine_inverse()*bone_transform
	
	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())

func pose_hand(hand: UltraleapHand, bone_id: int, is_right: bool) -> void:

	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# apply rigging transform. LeapC coordinates -> Godot global coordinates
	var hand_transform: Transform3D = hand.wrist*rigging_transform
	
	# get global orientation
	hand_transform =  Transform3D(hand_transform.basis)
	
	# undo offset introduced by rigging transform
	hand_transform = offset_transform(hand_transform, is_right)
	
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
		pose_arm(frame.right_hand.arm, 35, true)
		
		# hand
		pose_hand(frame.right_hand, 36, true)
		
		# fingers
		pose_finger(frame.right_hand.thumb.proximal, 37, true, true)
		#pose_finger(frame.right_hand.thumb.intermediate, 38, true, true)
		#pose_finger(frame.right_hand.thumb.distal, 39, true, true)
		
		pose_finger(frame.right_hand.index.metacarpal, 41, true)
		pose_finger(frame.right_hand.index.proximal, 42, true)
		pose_finger(frame.right_hand.index.intermediate, 43, true)
		pose_finger(frame.right_hand.index.distal, 44, true)
		
		pose_finger(frame.right_hand.middle.metacarpal, 45, true)
		pose_finger(frame.right_hand.middle.proximal, 46, true)
		pose_finger(frame.right_hand.middle.intermediate, 47, true)
		pose_finger(frame.right_hand.middle.distal, 48, true)
		
		pose_finger(frame.right_hand.ring.metacarpal, 49, true)
		pose_finger(frame.right_hand.ring.proximal, 50, true)
		pose_finger(frame.right_hand.ring.intermediate, 51, true)
		pose_finger(frame.right_hand.ring.distal, 52, true)
		
		pose_finger(frame.right_hand.pinky.metacarpal, 53, true)
		pose_finger(frame.right_hand.pinky.proximal, 54, true)
		pose_finger(frame.right_hand.pinky.intermediate, 55, true)
		pose_finger(frame.right_hand.pinky.distal, 56, true)
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_arm(frame.left_hand.arm, 11, false)
		
		# hand
		pose_hand(frame.left_hand, 12, false)
		
		#fingers
		pose_finger(frame.left_hand.thumb.proximal, 13, false, true)
		#pose_finger(frame.left_hand.thumb.intermediate, 14, false, true)
		#pose_finger(frame.left_hand.thumb.distal, 15, false, true)
		
		pose_finger(frame.left_hand.index.metacarpal, 17, false)
		pose_finger(frame.left_hand.index.proximal, 18, false)
		pose_finger(frame.left_hand.index.intermediate, 19, false)
		pose_finger(frame.left_hand.index.distal, 20, false)
		
		pose_finger(frame.left_hand.middle.metacarpal, 21, false)
		pose_finger(frame.left_hand.middle.proximal, 22, false)
		pose_finger(frame.left_hand.middle.intermediate, 23, false)
		pose_finger(frame.left_hand.middle.distal, 24, false)
		
		pose_finger(frame.left_hand.ring.metacarpal, 25, false)
		pose_finger(frame.left_hand.ring.proximal, 26, false)
		pose_finger(frame.left_hand.ring.intermediate, 27, false)
		pose_finger(frame.left_hand.ring.distal, 28, false)
		
		pose_finger(frame.left_hand.pinky.metacarpal, 29, false)
		pose_finger(frame.left_hand.pinky.proximal, 30, false)
		pose_finger(frame.left_hand.pinky.intermediate, 31, false)
		pose_finger(frame.left_hand.pinky.distal, 32, false)
