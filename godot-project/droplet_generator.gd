extends Node3D



# Properties

# The different types of droplet scenes
var cs_droplet_scene: PackedScene = preload("res://cs_droplet/cs_droplet.tscn")
var cpp_droplet_scene: PackedScene = preload("res://cpp_droplet/cpp_droplet.tscn")

# Enum for the various droplet server types
enum DropletServerType {CS_SERVER, CPP_SERVER}

## Which type of droplet server to use
@export
var droplet_server_type := DropletServerType.CPP_SERVER

## How much time between each droplet generation
@export var generation_interval: float = 0.01

# Elapsed time since last droplet generation
var _elapsed_time: float = 0.0

## The maximum number of droplets that can be generated.
## Should not be greater than CppDropletServer.MAX_DROPLETS or CsDropletServer.MaxDroplets
const MAX_DROPLETS = 4000

## The number of droplets to generate
@export_range(0, MAX_DROPLETS)
var droplets_to_generate: int = 2000

## The current number of droplets that have been generated
var num_droplets: int = 0



# Methods

# Called every physics frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	_elapsed_time += delta
	# Check if we should generate a new droplet
	if _elapsed_time > generation_interval and num_droplets < droplets_to_generate:
		_elapsed_time = 0.0
		# Create the appropriate droplet type
		var droplet_node: RigidBody3D
		match droplet_server_type:
			DropletServerType.CS_SERVER:
				droplet_node = cs_droplet_scene.instantiate()
			DropletServerType.CPP_SERVER:
				droplet_node = cpp_droplet_scene.instantiate()
		# Add the droplet to the scene
		get_parent().add_child(droplet_node)
		droplet_node.owner = owner
		# Set the droplet's transform
		var droplet_position := Vector3(randf_range(-1.0, 1.0),
										randf_range(-1.0, 1.0),
										randf_range(-1.0, 1.0))
		droplet_node.transform = Transform3D(Basis(), droplet_position)
		# Increment current number of droplets
		num_droplets += 1
