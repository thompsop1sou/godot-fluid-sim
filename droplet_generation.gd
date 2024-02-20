extends Node3D

@export_enum("Regular", "Server", "Custom", "Custom2") var droplet_type: int = 0

var droplet_scene: PackedScene = preload("res://droplet.tscn")
var server_droplet_scene: PackedScene = preload("res://server_droplet.tscn")
var custom_droplet_scene: PackedScene = preload("res://custom_droplet.tscn")
var custom_droplet_2_scene: PackedScene = preload("res://custom_droplet_2.tscn")

var elapsed_time: float = 0.0
@export var interval: float = 0.01

var total_droplets: int = 0
@export var max_droplets: int = 4000

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta):
	elapsed_time += delta
	if elapsed_time > interval and total_droplets < max_droplets:
		elapsed_time = 0.0
		var droplet_transform := Transform3D(Basis(), Vector3(randf_range(-1.0, 1.0), randf_range(-1.0, 1.0), randf_range(-1.0, 1.0)))
		if droplet_type == 0:
			var droplet_node = droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		elif droplet_type == 1:
			var droplet_node = server_droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		elif droplet_type == 2:
			var droplet_node = custom_droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		elif droplet_type == 3:
			var droplet_node = custom_droplet_2_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		total_droplets += 1
