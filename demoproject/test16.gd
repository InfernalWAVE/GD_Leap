extends UltraleapDeviceNode

@export var debug_label: Label
@export var skeleton: Skeleton3D

@export var test_transform: Transform3D
@export var test_transform_2: Transform3D

# TODO map initial thumb position, add special case for desired offset rest
@export var thumb_roll: float
@export var thumb_yaw: float

var root_pose: Transform3D

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
		pose_thumb(frame.right_hand.thumb.proximal, 35)
		pose_thumb(frame.right_hand.thumb.intermediate, 36)
		pose_thumb(frame.right_hand.thumb.distal, 37)
		
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
		pose_thumb(frame.left_hand.thumb.proximal, 11)
		pose_thumb(frame.left_hand.thumb.intermediate, 12)
		pose_thumb(frame.left_hand.thumb.distal, 13)
		
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
	var bone_transform: Transform3D = new_transform 
	var rest_transform: Transform3D = skeleton.get_bone_global_rest(bone_id)
	var local_rest_transform: Transform3D = skeleton.get_bone_rest(bone_id)
	
	
	# undo offset from rigging transform.
	# TODO fix thumbs
	var tester = bone_transform * test_transform
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2)
	tester = test_transform_2*bone_transform
	bone_transform = bone_transform.rotated(Vector3.UP, PI)
	var checker = bone_transform
	
	# get pose relative to parent bone
	# TODO cache global transforms to avoid recalculations
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	bone_transform = parent_transform.affine_inverse()*bone_transform

	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())

# TODO just make a special case for thumbs, cut your losses
func pose_thumb(bone: UltraleapBone, bone_id: int) -> void:
	var new_transform: Transform3D = Transform3D(bone.get_global_transform().basis)
	var bone_transform: Transform3D = new_transform 
	var rest_transform: Transform3D = skeleton.get_bone_global_rest(bone_id)
	var local_rest_transform: Transform3D = skeleton.get_bone_rest(bone_id)
	
	
	# undo offset from rigging transform.
	# TODO fix thumbs
	var tester = bone_transform * test_transform
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2)
	tester = test_transform_2*bone_transform
	bone_transform = bone_transform.rotated(Vector3.UP, PI)
	var checker = bone_transform
	
	# get pose relative to parent bone
	# TODO cache global transforms to avoid recalculations
	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	bone_transform = parent_transform.affine_inverse()*bone_transform
	
	match bone_id:
		11:
			bone_transform = bone_transform.rotated_local(Vector3.UP, deg_to_rad(90.0)).rotated_local(Vector3.UP, deg_to_rad(-35.0))
			pass
		12,13:
			bone_transform = bone_transform.rotated(Vector3.UP, deg_to_rad(90.0)).rotated(Vector3.UP, deg_to_rad(-35.0))
			pass
		35:
			bone_transform = bone_transform.rotated_local(Vector3.UP, deg_to_rad(-90.0)).rotated_local(Vector3.UP, deg_to_rad(35.0))
			pass
		36,37:
			bone_transform = bone_transform.rotated(Vector3.UP, deg_to_rad(-90.0)).rotated(Vector3.UP, deg_to_rad(35.0))
			pass
	
	# set pose
	skeleton.set_bone_pose_rotation(bone_id, bone_transform.basis.get_rotation_quaternion())
