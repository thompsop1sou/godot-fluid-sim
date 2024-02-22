extends Node3D

var is_gravity_on: bool = true:
	get:
		return is_gravity_on
	set(val):
		is_gravity_on = val
		var gravity = 9.8 if is_gravity_on else 0.0
		PhysicsServer3D.area_set_param(get_viewport().find_world_3d().space, PhysicsServer3D.AREA_PARAM_GRAVITY, gravity)

func _ready():
	is_gravity_on = is_gravity_on

func _process(delta):
	$LabelsBox/ProcessFPS.text = " Process: " + str(roundf(1.0 / delta)) + " FPS"
	$LabelsBox/Droplets.text = " Droplets: " + str($DropletGenerator.total_droplets)

func _physics_process(delta):
	$LabelsBox/PhysicsFPS.text = " Physics: " + str(roundf(1.0 / delta)) + " FPS"

func _input(event):
	if event.is_action_pressed("gravity_toggle"):
		is_gravity_on = not is_gravity_on
	elif event.is_action_pressed("gravity_up"):
		PhysicsServer3D.area_set_param(get_viewport().find_world_3d().space, PhysicsServer3D.AREA_PARAM_GRAVITY_VECTOR, Vector3.UP)
	elif event.is_action_pressed("gravity_down"):
		PhysicsServer3D.area_set_param(get_viewport().find_world_3d().space, PhysicsServer3D.AREA_PARAM_GRAVITY_VECTOR, Vector3.DOWN)
	elif event.is_action_pressed("gravity_left"):
		PhysicsServer3D.area_set_param(get_viewport().find_world_3d().space, PhysicsServer3D.AREA_PARAM_GRAVITY_VECTOR, Vector3.LEFT)
	elif event.is_action_pressed("gravity_right"):
		PhysicsServer3D.area_set_param(get_viewport().find_world_3d().space, PhysicsServer3D.AREA_PARAM_GRAVITY_VECTOR, Vector3.RIGHT)
