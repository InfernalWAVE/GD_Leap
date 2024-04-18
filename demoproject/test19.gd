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
@export var xr_skeleton: Skeleton3D

# TODO map initial thumb position, add special case for desired offset rest
@export var thumb_roll: float = 35.0
@export var thumb_yaw: float = 90.0
@export var leap_origin: Vector3 = Vector3(0.22, -0.185, -0.44) # this is what works well for my desk/chair/body setup
const RIGGING_TRANSFORM: Transform3D = Transform3D(Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(-1.0, 0.0, 0.0), Vector3.ZERO)

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

enum BM {BONE_ID}

@export var bone_map: Dictionary = {
	XRBodyTracker.JOINT_LEFT_SHOULDER: {
		BM.BONE_ID: 7,
	},
	XRBodyTracker.JOINT_LEFT_UPPER_ARM: {
		BM.BONE_ID: 8,
	},
	XRBodyTracker.JOINT_LEFT_LOWER_ARM: {
		BM.BONE_ID: 9,
	},
	XRBodyTracker.JOINT_LEFT_HAND: {
		BM.BONE_ID: 10,
	},
	XRBodyTracker.JOINT_RIGHT_SHOULDER: {
		BM.BONE_ID: 31,
	},
	XRBodyTracker.JOINT_RIGHT_UPPER_ARM: {
		BM.BONE_ID: 32,
	},
	XRBodyTracker.JOINT_RIGHT_LOWER_ARM: {
		BM.BONE_ID: 33,
	},
	XRBodyTracker.JOINT_RIGHT_HAND: {
		BM.BONE_ID: 34,
	},
	
}

func _ready() -> void:
	rigging_transform = RIGGING_TRANSFORM
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
			##shoulder
			#body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_SHOULDER, Transform3D(skeleton.get_bone_global_pose(3).basis*skeleton.get_bone_pose(31).basis, skeleton.get_bone_rest(31).origin))
			#body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_SHOULDER, JOINT_TRACKING)
			#
			##upper arm
			#body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, Transform3D(skeleton.get_bone_global_pose(31).basis*skeleton.get_bone_pose(32).basis, skeleton.get_bone_rest(32).origin))
			#body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, JOINT_TRACKING)
			
			#lower arm
			pose_xr(Transform3D(frame.right_hand.arm.get_global_transform()), XRBodyTracker.JOINT_RIGHT_LOWER_ARM)
			pose_xr(Transform3D(frame.right_hand.wrist*rigging_transform), XRBodyTracker.JOINT_RIGHT_HAND)
		
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

	var parent_transform: Transform3D = skeleton.get_bone_global_pose(skeleton.get_bone_parent(bone_id))
	
	bone_transform = new_transform.rotated(Vector3.UP,PI) # correct Z front
	bone_transform = parent_transform.affine_inverse()*bone_transform # in parent bone space
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2) # correct for humaoid rigging convention
	
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
				#position_proxy_skeleton.set_bone_pose_rotation(bone_id, Transform3D(parent_rest_transform.affine_inverse()*skeleton.get_bone_global_pose(bone_id)*local_rest_transform).basis.get_rotation_quaternion())
				#var arm_length: float = (parent_rest_transform.origin - rest_transform.origin).length()
				#var arm_offset: Vector3 = Vector3(0.0, arm_length, 0.0)
				#var arm_position: Vector3 = new_transform.origin
				#arm_position = Vector3(-arm_position.z-leap_origin.z, arm_position.x-leap_origin.x, -arm_position.y-leap_origin.y)
				#arm_position = arm_position * arm_move_scale
				#arm_position = arm_offset + arm_position
				#
				#position_proxy_skeleton.set_bone_pose_position(bone_id, arm_position)
				#
				#var target_transform: Transform3D = (position_proxy_skeleton.get_bone_global_pose(bone_id))
				#ik_proxy_ra.set_target_transform(target_transform)
				#
				#var upper_arm_transform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_rest(bone_id-2).affine_inverse()*ik_proxy_skeleton.get_bone_global_pose(bone_id-1)*parent_local_rest_transform*local_rest_transform)
				#var shoulder_tramsform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_pose(bone_id-2))
				#skeleton.set_bone_pose_rotation(bone_id-2, shoulder_tramsform.basis.get_rotation_quaternion())
				#skeleton.set_bone_pose_rotation(bone_id-1, upper_arm_transform.basis.get_rotation_quaternion())
				pass
	

func pose_xr(new_transform: Transform3D, joint: XRBodyTracker.Joint) -> void:
	var bone_transform: Transform3D = new_transform 
	
	var rest_transform: Transform3D = xr_skeleton.get_bone_global_rest(bone_map[joint][BM.BONE_ID])
	var local_rest_transform: Transform3D = xr_skeleton.get_bone_rest(bone_map[joint][BM.BONE_ID])
	
	var parent_transform: Transform3D = xr_skeleton.get_bone_global_pose(xr_skeleton.get_bone_parent(bone_map[joint][BM.BONE_ID]))
	var parent_rest_transform: Transform3D = xr_skeleton.get_bone_global_rest(xr_skeleton.get_bone_parent(bone_map[joint][BM.BONE_ID]))
	var parent_local_rest_transform: Transform3D = xr_skeleton.get_bone_rest(xr_skeleton.get_bone_parent(bone_map[joint][BM.BONE_ID]))
	
	bone_transform = new_transform.rotated(Vector3.UP,PI) # correct z-front
	bone_transform = parent_transform.affine_inverse()*bone_transform # position in parent space
	bone_transform = bone_transform.rotated_local(Vector3.UP, -PI/2) # correct for humanoid rigging convention
	
	# thumbs
	match bone_map[joint][BM.BONE_ID]:
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
	body_tracker.set_joint_transform(joint, Transform3D(xr_skeleton.get_bone_global_pose(xr_skeleton.get_bone_parent(bone_map[joint][BM.BONE_ID])).basis*bone_transform.basis, xr_skeleton.get_bone_rest(bone_map[joint][BM.BONE_ID]).origin))
	body_tracker.set_joint_flags(joint, JOINT_TRACKING)
	
	
	# arms position
	if should_position_arms:
		match bone_map[joint][BM.BONE_ID]:
			33:
				#position_proxy_skeleton.set_bone_pose_rotation(bone_map[joint][BM.BONE_ID], Transform3D(parent_rest_transform.affine_inverse()*xr_skeleton.get_bone_global_pose(bone_map[joint][BM.BONE_ID])*local_rest_transform).basis.get_rotation_quaternion())
				#var arm_length: float = (parent_rest_transform.origin - rest_transform.origin).length()
				#var arm_offset: Vector3 = Vector3(0.0, arm_length, 0.0)
				#var arm_position: Vector3 = new_transform.origin
				#arm_position = Vector3(-arm_position.z-leap_origin.z, arm_position.x-leap_origin.x, -arm_position.y-leap_origin.y)
				#arm_position = arm_position * arm_move_scale
				#arm_position = arm_offset + arm_position
				#
				#position_proxy_skeleton.set_bone_pose_position(bone_map[joint][BM.BONE_ID], arm_position)
				#
				#var target_transform: Transform3D = (position_proxy_skeleton.get_bone_global_pose(bone_map[joint][BM.BONE_ID]))
				#ik_proxy_ra.set_target_transform(target_transform)
				#
				#var upper_arm_transform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_rest(bone_map[joint][BM.BONE_ID]-2).affine_inverse()*ik_proxy_skeleton.get_bone_global_pose(bone_map[joint][BM.BONE_ID]-1)*parent_local_rest_transform*local_rest_transform)
				#var shoulder_tramsform: Transform3D = Transform3D(ik_proxy_skeleton.get_bone_global_pose(bone_map[joint][BM.BONE_ID]-2))
				
				
				
				#body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, Transform3D(xr_skeleton.get_bone_global_pose(xr_skeleton.get_bone_parent(bone_map[XRBodyTracker.JOINT_RIGHT_UPPER_ARM][BM.BONE_ID])).basis*upper_arm_transform.basis, xr_skeleton.get_bone_rest(bone_map[XRBodyTracker.JOINT_RIGHT_UPPER_ARM][BM.BONE_ID]).origin))
				#body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_UPPER_ARM, JOINT_TRACKING)
				#
				#body_tracker.set_joint_transform(XRBodyTracker.JOINT_RIGHT_SHOULDER, Transform3D(xr_skeleton.get_bone_global_pose(xr_skeleton.get_bone_parent(bone_map[XRBodyTracker.JOINT_RIGHT_SHOULDER][BM.BONE_ID])).basis*shoulder_tramsform.basis, xr_skeleton.get_bone_rest(bone_map[XRBodyTracker.JOINT_RIGHT_SHOULDER][BM.BONE_ID]).origin))
				#body_tracker.set_joint_flags(XRBodyTracker.JOINT_RIGHT_SHOULDER, JOINT_TRACKING)
				pass
