extends Node3D

@export var use_servers: bool = true

var droplet_bodies: Array[RID]
var droplet_body_shape: RID

var droplet_meshes: Array[RID]
var droplet_mesh_shape: SphereMesh = preload("res://droplet_mesh.tres")

var droplet_scene: PackedScene = preload("res://droplet.tscn")
var server_droplet_scene: PackedScene = preload("res://server_droplet.tscn")

var elapsed_time: float = 0.0
@export var interval: float = 0.01

var total_droplets: int = 0
@export var max_droplets: int = 4000

func _ready():
	droplet_body_shape = PhysicsServer3D.sphere_shape_create()
	PhysicsServer3D.shape_set_data(droplet_body_shape, 0.25)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	elapsed_time += delta
	if elapsed_time > interval and total_droplets < max_droplets:
		elapsed_time = 0.0
		var droplet_transform := Transform3D(Basis(), Vector3(randf_range(-1.0, 1.0), randf_range(-1.0, 1.0), randf_range(-1.0, 1.0)))
		if use_servers:
			var droplet_node = server_droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		else:
			var droplet_node = droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		total_droplets += 1

func _droplet_moved(state: PhysicsDirectBodyState3D, index: int):
	RenderingServer.instance_set_transform(droplet_meshes[index], state.transform)
