#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <ppl.h>

#include "cpp_droplet_server.h"

using namespace godot;

// Needed for exposing stuff to Godot
void CppDropletServer::_bind_methods()
{
	// Property: force_magnitude
	ClassDB::bind_method(D_METHOD("get_force_magnitude"), &CppDropletServer::get_force_magnitude);
	ClassDB::bind_method(D_METHOD("set_force_magnitude", "p_force_magnitude"), &CppDropletServer::set_force_magnitude);
	ClassDB::add_property("CppDropletServer", PropertyInfo(Variant::FLOAT, "force_magnitude"), "set_force_magnitude", "get_force_magnitude");

	// Property: force_effective_distance
	ClassDB::bind_method(D_METHOD("get_force_effective_distance"), &CppDropletServer::get_force_effective_distance);
	ClassDB::bind_method(D_METHOD("set_force_effective_distance", "p_force_effective_distance"), &CppDropletServer::set_force_effective_distance);
	ClassDB::add_property("CppDropletServer", PropertyInfo(Variant::FLOAT, "force_effective_distance"), "set_force_effective_distance", "get_force_effective_distance");

	// Methods: add_droplet and remove_droplet
	ClassDB::bind_method(D_METHOD("add_droplet", "droplet_rid"), &CppDropletServer::add_droplet);
	ClassDB::bind_method(D_METHOD("remove_droplet", "droplet_rid"), &CppDropletServer::remove_droplet);
}



// Constructor and Destructor

CppDropletServer::CppDropletServer() :
	m_num_droplets(0),
	m_droplets(),
	m_force_magnitude(300.0),
	m_force_effective_distance(0.5),
	m_force_effective_distance_squared(0.25),
	m_in_game(false),
	m_physics_server(nullptr)
{}

CppDropletServer::~CppDropletServer()
{}



// Overridden Functions

// Called when the node enters the scene tree for the first time.
void CppDropletServer::_ready()
{
	// Determine whether the game is running
	m_in_game = !Engine::get_singleton()->is_editor_hint();
	// Get a reference to the physics server singleton
	m_physics_server = PhysicsServer3D::get_singleton();
}

// Called every physics frame. 'delta' is the elapsed time since the previous frame.
void CppDropletServer::_physics_process(double delta)
{
    // Only run if in game
    if (m_in_game)
    {
		// Get the current position of each droplet
		for (int i = 0; i < m_num_droplets; i++)
		{
			m_droplets[i].position = Vec3(get_droplet_position(m_droplets[i].rid));
		}
		// Sum up the forces by looping over pairs of droplets
		// Outer loop to get first droplet
		concurrency::parallel_for(0, m_num_droplets, [this](int i)
		{
			// Figure out start and end index for inner loop
			int start = i + 1;
			int end = start + m_num_droplets / 2;
			if (m_num_droplets % 2 == 0 && i * 2 >= m_num_droplets)
				end--;
			// Inner loop to get second droplet
			for (int j = start; j < end; j++)
			{
				int k = j % m_num_droplets;
				// Apply cohesive forces if the droplets are close enough
				float distance_squared = m_droplets[i].position.distance_squared(m_droplets[k].position);
				if (distance_squared < m_force_effective_distance_squared)
				{
					Vec3 force_direction = (m_droplets[i].position - m_droplets[k].position).normalized();
					m_droplets[i].mut.lock();
					m_droplets[i].force += -m_force_magnitude * force_direction;
					m_droplets[i].mut.unlock();
					m_droplets[k].mut.lock();
					m_droplets[k].force += m_force_magnitude * force_direction;
					m_droplets[k].mut.unlock();
				}
			}
		});
		// Apply the forces for each droplet
		concurrency::parallel_for(0, m_num_droplets, [this](int i)
		{
			Droplet* droplet = &m_droplets[i];
			m_physics_server->body_apply_central_force(droplet->rid, Vector3(droplet->force));
			droplet->force = Vec3::ZERO;
		});
    }
}



// Other Functions

// Adds a droplet to the server's array
void CppDropletServer::add_droplet(RID droplet_rid)
{
	// Make sure the droplet is a rigid body
	if (!is_rigid_body(droplet_rid))
	{
		UtilityFunctions::printerr("NOT A RIGID BODY: Could not add physics object ", droplet_rid, " to ", this);
		return;
	}

	// Try to add it
	if (m_num_droplets < MAX_DROPLETS)
	{
		m_droplets[m_num_droplets].rid = droplet_rid;
		m_droplets[m_num_droplets].position = Vec3(get_droplet_position(droplet_rid));
		m_droplets[m_num_droplets].force = Vec3::ZERO;
		m_num_droplets++;
	}
	else
	{
		UtilityFunctions::printerr("MAX EXCEEDED: Could not add physics object ", droplet_rid, " to ", this);
	}
}

// Removes a droplet from the server's array
void CppDropletServer::remove_droplet(RID droplet_rid)
{
	// Make sure the droplet is a rigid body
	if (!is_rigid_body(droplet_rid))
	{
		UtilityFunctions::printerr("NOT A RIGID BODY: Could not remove physics object ", droplet_rid, " from ", this);
		return;
	}

	// Try to remove it
	for (int i = 0; i < m_num_droplets; i++)
	{
		if (m_droplets[i].rid == droplet_rid)
		{
			m_droplets[i].rid = m_droplets[m_num_droplets - 1].rid;
			m_droplets[i].position = m_droplets[m_num_droplets - 1].position;
			m_droplets[i].force = m_droplets[m_num_droplets - 1].force;
			m_droplets[m_num_droplets - 1].rid = RID();
			m_droplets[m_num_droplets - 1].position = Vec3::ZERO;
			m_droplets[m_num_droplets - 1].force = Vec3::ZERO;
			m_num_droplets--;
			return;
		}
	}
	UtilityFunctions::printerr("NOT FOUND: Could not remove physics object", droplet_rid, " from ", this);
}

// Getters and setters for force magnitude

float CppDropletServer::get_force_magnitude() const
{
	return m_force_magnitude;
}

void CppDropletServer::set_force_magnitude(const float p_force_magnitude)
{
	m_force_magnitude = p_force_magnitude;
}

// Getters and setters for force effective distance

float CppDropletServer::get_force_effective_distance() const
{
	return m_force_effective_distance;
}

void CppDropletServer::set_force_effective_distance(const float p_force_effective_distance)
{
	if (p_force_effective_distance < 0)
		m_force_effective_distance = 0;
	else
		m_force_effective_distance = p_force_effective_distance;
	m_force_effective_distance_squared = m_force_effective_distance * m_force_effective_distance;
}

// Confirms that an RID is for a rigid body
bool CppDropletServer::is_rigid_body(const RID &droplet_rid) const
{
	auto droplet_body_mode = m_physics_server->body_get_mode(droplet_rid);
	return droplet_body_mode == m_physics_server->BODY_MODE_RIGID || droplet_body_mode == m_physics_server->BODY_MODE_RIGID_LINEAR;
}

// Gets the position of a droplet from its RID
Vector3 CppDropletServer::get_droplet_position(const RID &droplet_rid) const
{
	return Transform3D(m_physics_server->body_get_state(droplet_rid, m_physics_server->BODY_STATE_TRANSFORM)).origin;
}
