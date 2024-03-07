extends Node3D



# Properties Referencing External Resources

# The droplet scenes.
var _cs_droplet_scene: PackedScene = preload("res://cs_droplet/cs_droplet.tscn")
var _cpp_droplet_scene: PackedScene = preload("res://cpp_droplet/cpp_droplet.tscn")

# The droplet meshes.
var _cs_droplet_mesh: Mesh = preload("res://cs_droplet/cs_droplet_mesh.tres")
var _cpp_droplet_mesh: Mesh = preload("res://cpp_droplet/cpp_droplet_mesh.tres")



# Properties About Server Type

# Enum for the droplet server types.
enum DropletServerType {CS_SERVER, CPP_SERVER}

## Which type of droplet server to use.
@export
var droplet_server_type := DropletServerType.CS_SERVER



# Properties About Object Type

# Enum for the droplet object types.
enum DropletObjectType {SCENE, SERVER_RID}

## Which type of droplet object to use.
## SCENE will create instances of a .tscn file.
## SERVER_RID will create objects directly using the rendering and physics servers.
@export
var droplet_object_type := DropletObjectType.SCENE

# Keeping references to RIDs that are created when using SERVER_RID.
var _droplet_mesh_rids: Array[RID]
var _droplet_body_rids: Array[RID]
var _droplet_body_shape: RID



# Properties About Timing of Droplet Generation

## How much time between each droplet generation.
@export var generation_interval: float = 0.01

# Elapsed time since last droplet generation.
var _elapsed_time: float = 0.0



# Properties About Number of Droplets Generated

## The maximum number of droplets that can be generated.
## Should not be greater than CppDropletServer.MAX_DROPLETS or CsDropletServer.MaxDroplets.
const MAX_DROPLETS = 4000

## The number of droplets to generate.
@export_range(0, MAX_DROPLETS)
var droplets_to_generate: int = 2000

## The current number of droplets that have been generated.
var num_droplets: int = 0



# Built-In Methods

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	_droplet_body_shape = PhysicsServer3D.sphere_shape_create()
	PhysicsServer3D.shape_set_data(_droplet_body_shape, 0.167)

# Called every physics frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	_elapsed_time += delta
	# Check if we should generate a new droplet
	if _elapsed_time >= generation_interval and num_droplets < droplets_to_generate:
		_elapsed_time = _elapsed_time - generation_interval
		# If working with droplet scenes...
		if droplet_object_type == DropletObjectType.SCENE:
			_generate_droplet_scene(droplet_server_type)
		# Otherwise, if working with droplet RIDs
		elif droplet_object_type == DropletObjectType.SERVER_RID:
			_generate_droplet_rid(droplet_server_type)
		# Increment current number of droplets
		num_droplets += 1



# Methods for Generating a Droplet

# Generates a droplet from a scene.
func _generate_droplet_scene(server_type: DropletServerType)  -> void:
	# Create the appropriate droplet type
	var droplet_node: RigidBody3D
	if server_type == DropletServerType.CS_SERVER:
		droplet_node = _cs_droplet_scene.instantiate()
	elif server_type == DropletServerType.CPP_SERVER:
		droplet_node = _cpp_droplet_scene.instantiate()
	# Add the droplet to the scene
	get_parent().add_child(droplet_node)
	droplet_node.owner = owner
	# Set the droplet's position
	droplet_node.position = Vector3(randf_range(-1.0, 1.0),
									randf_range(-1.0, 1.0),
									randf_range(-1.0, 1.0))

# Generates a droplet as an RID using rendering and physics servers.
func _generate_droplet_rid(server_type: DropletServerType) -> void:
	# Figure out the droplet's transform
	var droplet_position := Vector3(randf_range(-1.0, 1.0),
									randf_range(-1.0, 1.0),
									randf_range(-1.0, 1.0))
	var droplet_transform := Transform3D(Basis(), droplet_position)
	# Create a renderable mesh
	var droplet_mesh: Mesh
	if server_type == DropletServerType.CS_SERVER:
		droplet_mesh = _cs_droplet_mesh
	elif server_type == DropletServerType.CPP_SERVER:
		droplet_mesh = _cpp_droplet_mesh
	var droplet_mesh_rid = RenderingServer.instance_create()
	RenderingServer.instance_set_scenario(droplet_mesh_rid, get_world_3d().scenario)
	RenderingServer.instance_set_base(droplet_mesh_rid, droplet_mesh)
	RenderingServer.instance_set_transform(droplet_mesh_rid, droplet_transform)
	_droplet_mesh_rids.append(droplet_mesh_rid)
	# Create a rigid body
	var droplet_body_rid = PhysicsServer3D.body_create()
	PhysicsServer3D.body_set_mode(droplet_body_rid, PhysicsServer3D.BODY_MODE_RIGID_LINEAR)
	PhysicsServer3D.body_add_shape(droplet_body_rid, _droplet_body_shape)
	PhysicsServer3D.body_set_space(droplet_body_rid, get_world_3d().space)
	PhysicsServer3D.body_set_state(droplet_body_rid, PhysicsServer3D.BODY_STATE_TRANSFORM, droplet_transform)
	PhysicsServer3D.body_set_param(droplet_body_rid, PhysicsServer3D.BODY_PARAM_FRICTION, 0.05)
	PhysicsServer3D.body_set_param(droplet_body_rid, PhysicsServer3D.BODY_PARAM_BOUNCE, 0.3)
	PhysicsServer3D.body_set_param(droplet_body_rid, PhysicsServer3D.BODY_PARAM_MASS, 50)
	PhysicsServer3D.body_set_state(droplet_body_rid, PhysicsServer3D.BODY_STATE_CAN_SLEEP, false)
	PhysicsServer3D.body_set_force_integration_callback(droplet_body_rid, _droplet_force_callback, num_droplets)
	_droplet_body_rids.append(droplet_body_rid)
	# Add the rigid body to the appropriate droplet server
	if server_type == DropletServerType.CS_SERVER:
		CsDropletServerAutoload.AddDroplet(droplet_body_rid)
	elif server_type == DropletServerType.CPP_SERVER:
		CppDropletServerAutoload.add_droplet(droplet_body_rid)

# Will be called when a server RID droplet moves.
func _droplet_force_callback(state: PhysicsDirectBodyState3D, index: int):
	RenderingServer.instance_set_transform(_droplet_mesh_rids[index], state.transform)
