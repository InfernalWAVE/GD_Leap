extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

@export var desired_offset_rest: Transform3D

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

func pose_hand(hand: UltraleapHand, bone_id: int) -> void:
	# extension doesn't apply rigging transform to hand, we'll do it here
	var hand_transform: Transform3D = hand.wrist*rigging_transform
	
	pose_from_transfrom(hand_transform, bone_id)

func pose_bone(bone: UltraleapBone, bone_id: int) -> void:
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	pose_from_transfrom(bone_transform, bone_id)

func pose_from_transfrom(new_transform: Transform3D, bone_id: int) -> void:
	var bone_transform: Transform3D = new_transform.basis
	
	# undo offset from rigging transform.
	var world_rest = bone_transform.affine_inverse() * desired_offset_rest
	var b = world_rest.basis
	bone_transform = Transform3D(Basis(Vector3(-b.z.z, -b.y.z, -b.x.z),  Vector3(-b.z.y, -b.y.y, -b.x.y), Vector3(b.z.x, b.y.x, b.x.x)))
	
	# get pose relative to parent bone
	# TODO cache global transforms to avoid recalculations
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	bone_transform = parent_transform.affine_inverse()*bone_transform

	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())
