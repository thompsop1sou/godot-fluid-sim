class_name ServerDroplet extends RigidBody3D

var nearby_body_rids: Array[RID]
var body_force_magnitude: float = -300

var outer_radius: float = 0.375
var droplet_body_rid: RID
var droplet_area_rid: RID
var droplet_area_shape_rid: RID

func _ready() -> void:
	droplet_body_rid = get_rid()
	# Create a physics area
	droplet_area_shape_rid = PhysicsServer3D.sphere_shape_create()
	PhysicsServer3D.shape_set_data(droplet_area_shape_rid, outer_radius)
	droplet_area_rid = PhysicsServer3D.area_create()
	PhysicsServer3D.area_set_monitorable(droplet_area_rid, true)
	PhysicsServer3D.area_add_shape(droplet_area_rid, droplet_area_shape_rid)
	PhysicsServer3D.area_set_space(droplet_area_rid, get_world_3d().space)
	PhysicsServer3D.area_set_transform(droplet_area_rid, transform)
	PhysicsServer3D.area_set_monitor_callback(droplet_area_rid, area_monitor_callback)

func _exit_tree():
	PhysicsServer3D.free_rid(droplet_area_shape_rid)
	PhysicsServer3D.free_rid(droplet_area_rid)

func area_monitor_callback(status: int, body_rid: RID, _instance_id: int,
							_body_shape_idx: int, _self_shape_idx: int) -> void:
	if body_rid != droplet_body_rid:
		if status == PhysicsServer3D.AREA_BODY_ADDED:
			nearby_body_rids.append(body_rid)
		elif status == PhysicsServer3D.AREA_BODY_REMOVED:
			nearby_body_rids.erase(body_rid)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(_delta: float) -> void:
	var body_force = Vector3()
	for body_rid in nearby_body_rids:
		var body_position = PhysicsServer3D.body_get_direct_state(body_rid).transform.origin
		body_force += (position - body_position).normalized()
	apply_central_force(body_force_magnitude * body_force)
	PhysicsServer3D.area_set_transform(droplet_area_rid, transform)
