extends Camera3D

@export var input_action_trigger: String

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventKey:
		if event.is_action_pressed(input_action_trigger):
			make_current()
