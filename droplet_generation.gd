extends Node3D

@export var use_servers: bool = true

var droplet_bodies: Array[RID]
var droplet_body_shape: RID

var droplet_meshes: Array[RID]
var droplet_mesh_shape: SphereMesh = preload("res://droplet_mesh.tres")

var droplet_scene: PackedScene = preload("res://droplet.tscn")

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
			# Create a renderable mesh
			var droplet_mesh = RenderingServer.instance_create()
			RenderingServer.instance_set_scenario(droplet_mesh, get_world_3d().scenario)
			RenderingServer.instance_set_base(droplet_mesh, droplet_mesh_shape)
			RenderingServer.instance_set_transform(droplet_mesh, droplet_transform)
			droplet_meshes.append(droplet_mesh)
			# Create a physics body
			var droplet_body = PhysicsServer3D.body_create()
			PhysicsServer3D.body_set_mode(droplet_body, PhysicsServer3D.BODY_MODE_RIGID_LINEAR)
			PhysicsServer3D.body_add_shape(droplet_body, droplet_body_shape)
			PhysicsServer3D.body_set_space(droplet_body, get_world_3d().space)
			PhysicsServer3D.body_set_state(droplet_body, PhysicsServer3D.BODY_STATE_TRANSFORM, droplet_transform)
			PhysicsServer3D.body_set_param(droplet_body, PhysicsServer3D.BODY_PARAM_FRICTION, 0.0)
			PhysicsServer3D.body_set_param(droplet_body, PhysicsServer3D.BODY_PARAM_BOUNCE, 0.4)
			PhysicsServer3D.body_set_param(droplet_body, PhysicsServer3D.BODY_PARAM_MASS, 50)
			PhysicsServer3D.body_set_state(droplet_body, PhysicsServer3D.BODY_STATE_CAN_SLEEP, false)
			PhysicsServer3D.body_set_force_integration_callback(droplet_body, _droplet_moved, total_droplets)
			droplet_bodies.append(droplet_body)
		else:
			var droplet_node = droplet_scene.instantiate()
			get_parent().add_child(droplet_node)
			droplet_node.transform = droplet_transform
		total_droplets += 1

func _droplet_moved(state: PhysicsDirectBodyState3D, index: int):
	RenderingServer.instance_set_transform(droplet_meshes[index], state.transform)
