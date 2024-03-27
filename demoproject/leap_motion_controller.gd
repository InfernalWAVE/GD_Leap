extends UltraleapDeviceNode

@export var tracked_node: Node3D

@export var tracker_name: String

var xr_tracker: XRBodyTracker = XRBodyTracker.new()

func _ready() -> void:
	pass

func _process(_delta):
	var frame : UltraleapFrame = get_interpolated_frame(Time.get_ticks_usec())
	if frame != null and frame.is_right_hand_visible:
		tracked_node.position = frame.right_hand.index.distal.next_joint * 10.0
		tracked_node.quaternion = frame.right_hand.index.distal.orientation
