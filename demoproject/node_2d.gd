extends Node2D

@export var hello_world_node: HelloWorld

func _ready() -> void:
	hello_world_node.hello_world("this is from GDScript!")


func _on_hello_world_hello_world_signal(data):
	print("data from signal is " + data)
