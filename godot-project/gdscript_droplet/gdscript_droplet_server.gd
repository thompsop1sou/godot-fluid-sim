class_name GDScriptDropletServer extends Node



# Properties

# An array of _droplets and their properties
var _num_droplets: int = 0
var _droplets: Array[RigidBody3D]
var _forces: Array[Vector3]

## The magnitude of the attraction force
@export
var force_magnitude: float = 300.0

## The distance that the attraction force is effective over
var _force_effective_distance_squared: float = 0.25
@export
var force_effective_distance: float = 0.5:
	get:
		return force_effective_distance
	set(value):
		force_effective_distance = absf(value)
		_force_effective_distance_squared = force_effective_distance * force_effective_distance

# Whether currently in-game
var _in_game := false



# Overridden Methods

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	_in_game = not Engine.is_editor_hint()

# Called every physics frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(_delta: float) -> void:
	# Only run if in game
	if _in_game:
		# Sum up the forces, main loop
		var i: int = 0
		var j: int = 0
		while i < _num_droplets:
			# Figure out start and end index for sub loop
			j = i + 1
			@warning_ignore("integer_division")
			var end: int = j + int(_num_droplets / 2)
			if (_num_droplets % 2 == 0 and i * 2 >= _num_droplets):
				end -= 1
			# Sub loop
			while j < end:
				var k: int = j % _num_droplets
				var distance_squared = _droplets[i].position.distance_squared_to(_droplets[k].position)
				if distance_squared < _force_effective_distance_squared:
					var force_direction = (_droplets[i].position - _droplets[k].position).normalized()
					_forces[i] += -force_magnitude * force_direction
					_forces[k] += force_magnitude * force_direction
				j += 1
			i += 1
		i = 0
		# Apply the forces
		while i < _num_droplets:
			_droplets[i].apply_central_force(_forces[i])
			_forces[i] = Vector3.ZERO
			i += 1



# Other Methods

## Adds a droplet to the server's array
func add_droplet(droplet: RigidBody3D):
	_droplets.append(droplet)
	_forces.append(Vector3.ZERO)
	_num_droplets += 1

## Removes a droplet from the server's array
func remove_droplet(droplet: RigidBody3D):
	_droplets.erase(droplet)
	_forces.pop_back()
	_num_droplets -= 1
