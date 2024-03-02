extends Node3D



# Properties

# Alias for the 3D physics server singleton
var PS3D := PhysicsServer3D

# Whether gravity is currently turned on
var is_gravity_on: bool = true:
	get:
		return is_gravity_on
	set(val):
		is_gravity_on = val
		var gravity = 9.8 if is_gravity_on else 0.0
		PS3D.area_set_param(get_viewport().find_world_3d().space,
							PS3D.AREA_PARAM_GRAVITY,
							gravity)



# Methods

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Make sure gravity is set correctly
	is_gravity_on = is_gravity_on

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# Update the label for the current frame
	$LabelsBox/ProcessFPS.text = " Process: " + str(roundf(1.0 / delta)) + " FPS"
	$LabelsBox/Droplets.text = " Droplets: " + str($DropletGenerator.num_droplets)

# Called every physics frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	# Update the label for the current physics frame
	$LabelsBox/PhysicsFPS.text = " Physics: " + str(roundf(1.0 / delta)) + " FPS"

# Called when there is an input event.
func _input(event: InputEvent) -> void:
	# Turn gravity on or off
	if event.is_action_pressed("gravity_toggle"):
		is_gravity_on = not is_gravity_on
	# Change the direction of gravity
	elif event.is_action_pressed("gravity_up"):
		_set_gravity_vector(Vector3.UP)
	elif event.is_action_pressed("gravity_down"):
		_set_gravity_vector(Vector3.DOWN)
	elif event.is_action_pressed("gravity_left"):
		_set_gravity_vector(Vector3.LEFT)
	elif event.is_action_pressed("gravity_right"):
		_set_gravity_vector(Vector3.RIGHT)

# Helper function sets the direction of gravity
func _set_gravity_vector(gravity_vector: Vector3) -> void:
	PS3D.area_set_param(get_viewport().find_world_3d().space,
						PS3D.AREA_PARAM_GRAVITY_VECTOR,
						gravity_vector)
