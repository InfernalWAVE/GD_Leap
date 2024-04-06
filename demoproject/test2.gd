extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

func offset_arm_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI)
	
	if is_right:
		transform_offset = transform_offset.rotated(Vector3.FORWARD, -PI/2)
	else:
		transform_offset = transform_offset.rotated_local(Vector3.UP,PI).rotated(Vector3.FORWARD, PI/2)
	
	
	
	return transform_offset

func offset_transform(new_transform: Transform3D, is_right: bool) -> Transform3D:
	var transform_offset: Transform3D = new_transform
	
	transform_offset = transform_offset.rotated(Vector3.RIGHT, PI)
	
	if is_right:
		transform_offset = transform_offset.rotated_local(Vector3.UP, -PI/2).rotated(Vector3.FORWARD,-PI/2).rotated(Vector3.UP, PI/2)
	else:
		transform_offset = transform_offset.rotated_local(Vector3.UP,PI).rotated_local(Vector3.UP, PI/2).rotated(Vector3.FORWARD,PI/2).rotated(Vector3.UP, -PI/2)
	
	return transform_offset

func pose_bone(bone: UltraleapBone, bone_id: int, is_right: bool) -> void:
	# default Metahuman rig in Blender has opposite bone convention for L v R
	#var offset_transform: Transform3D = offset_transform(Basis.IDENTITY, is_right)
	var reference_transform: Transform3D = Transform3D(skeleton.get_bone_global_rest(bone_id).basis)
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	# get global orientation in Goot global coordinates (rigging transform applied in extension)
	var bone_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	
	# undo offset introduced by rigging transform
	bone_transform = offset_transform(bone_transform, is_right)
	
	# get pose relative to rest
	bone_transform = reference_transform * bone_transform
	
	# get pose in parent bone space
	bone_transform = parent_transform.affine_inverse()*bone_transform
	
	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())
	
func pose_arm(bone: UltraleapBone, bone_id: int, is_right: bool) -> void:
	# default Metahuman rig in Blender has opposite bone convention for L v R
	#var offset_transform: Transform3D = offset_transform(Basis.IDENTITY, is_right)
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
	# default Metahuman rig in Blender has opposite bone convention for L v R
	#var offset_transform: Transform3D = offset_transform(Basis.IDENTITY, is_right)
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
		pose_arm(frame.right_hand.arm, 33, true)
		
		# hand
		pose_hand(frame.right_hand, 34, true)
		
		# fingers
		pose_bone(frame.right_hand.index.proximal, 36, true)
		
	
	if frame != null and frame.is_left_hand_visible:
		# arm
		pose_arm(frame.left_hand.arm, 10, false)
		
		# hand
		pose_hand(frame.left_hand, 11, false)
