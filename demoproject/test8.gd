extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

@export var test_transform: Transform3D

# TODO FINISH THE THUMBBBBBSSSS
func offset_thumb_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	if is_right:
		transform_offset = transform_offset.rotated_local(Vector3.UP, -PI/4).rotated_local(Vector3.RIGHT, PI)
	else:
		transform_offset = transform_offset.rotated_local(Vector3.UP, PI/4).rotated(Vector3.UP, -PI/2)
	
	return transform_offset

func offset_arm_transform(new_transform: Transform3D) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI).rotated(Vector3.FORWARD,-PI/2).rotated(Vector3.UP, PI/2)
	
	return transform_offset

func pose_arm(bone: UltraleapBone, bone_id: int) -> void:
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	# undo offset introduced by rigging transform
	bone_transform = offset_arm_transform(bone_transform)
	
	pose_from_transfrom(bone_transform, bone_id)

func pose_hand(hand: UltraleapHand, bone_id: int) -> void:
	var hand_transform: Transform3D = hand.wrist*rigging_transform # extension doesn't apply rigging transform to hand
	
	hand_transform = offset_transform(hand_transform) # undo offset from rigging transform
	
	pose_from_transfrom(hand_transform, bone_id)

func pose_finger(bone: UltraleapBone, bone_id: int, is_thumb: bool = false) -> void:
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	bone_transform = offset_transform(bone_transform) # undo offset from rigging transform
	
	pose_from_transfrom(bone_transform, bone_id)

func offset_transform(new_transform: Transform3D) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	transform_offset = transform_offset.rotated_local(Vector3.UP, -PI/2)
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI).rotated(Vector3.FORWARD,-PI/2).rotated(Vector3.UP, PI/2)
	
	return transform_offset

func pose_from_transfrom(new_transform: Transform3D, bone_id) -> void:
	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	var bone_transform: Transform3D = new_transform

	# get pose relative to rest
	bone_transform = test_transform * bone_transform
	
	# get pose in parent bone space
	bone_transform = parent_transform.affine_inverse()*bone_transform

	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())

func _process(_delta):
	var frame : UltraleapFrame = get_interpolated_frame(Time.get_ticks_usec())
	if frame != null and frame.is_right_hand_visible:
		# arm
		pose_arm(frame.right_hand.arm, 35)
		
		# hand
		pose_hand(frame.right_hand, 36)
		
		# fingers
		pose_finger(frame.right_hand.thumb.proximal, 37, true)
		pose_finger(frame.right_hand.thumb.intermediate, 38, true)
		pose_finger(frame.right_hand.thumb.distal, 39, true)
		
		#pose_finger(frame.right_hand.index.metacarpal, 41)
		pose_finger(frame.right_hand.index.proximal, 41)
		pose_finger(frame.right_hand.index.intermediate, 42)
		pose_finger(frame.right_hand.index.distal, 43)
		
		#pose_finger(frame.right_hand.middle.metacarpal, 45)
		pose_finger(frame.right_hand.middle.proximal, 45)
		pose_finger(frame.right_hand.middle.intermediate, 46)
		pose_finger(frame.right_hand.middle.distal, 47)
		
		#pose_finger(frame.right_hand.ring.metacarpal, 49)
		pose_finger(frame.right_hand.ring.proximal, 49)
		pose_finger(frame.right_hand.ring.intermediate, 50)
		pose_finger(frame.right_hand.ring.distal, 51)
		
		#pose_finger(frame.right_hand.pinky.metacarpal, 53)
		pose_finger(frame.right_hand.pinky.proximal, 53)
		pose_finger(frame.right_hand.pinky.intermediate, 54)
		pose_finger(frame.right_hand.pinky.distal, 55)
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_arm(frame.left_hand.arm, 11)
		
		# hand
		pose_hand(frame.left_hand, 12)
		
		#fingers
		pose_finger(frame.left_hand.thumb.proximal, 13, true)
		pose_finger(frame.left_hand.thumb.intermediate, 14, true)
		pose_finger(frame.left_hand.thumb.distal, 15, true)
		
		#pose_finger(frame.left_hand.index.metacarpal, 17)
		pose_finger(frame.left_hand.index.proximal, 17)
		pose_finger(frame.left_hand.index.intermediate, 18)
		pose_finger(frame.left_hand.index.distal, 19)
		
		#pose_finger(frame.left_hand.middle.metacarpal, 21)
		pose_finger(frame.left_hand.middle.proximal, 21)
		pose_finger(frame.left_hand.middle.intermediate, 22)
		pose_finger(frame.left_hand.middle.distal, 23)
		
		#pose_finger(frame.left_hand.ring.metacarpal, 25)
		pose_finger(frame.left_hand.ring.proximal, 25)
		pose_finger(frame.left_hand.ring.intermediate, 26)
		pose_finger(frame.left_hand.ring.distal, 27)
		
		#pose_finger(frame.left_hand.pinky.metacarpal, 29)
		pose_finger(frame.left_hand.pinky.proximal, 29)
		pose_finger(frame.left_hand.pinky.intermediate, 30)
		pose_finger(frame.left_hand.pinky.distal, 31)
