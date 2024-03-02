#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "droplet_server.h"

using namespace godot;

void DropletServer::_bind_methods()
{
	// Property: force_magnitude
	ClassDB::bind_method(D_METHOD("get_force_magnitude"), &DropletServer::get_force_magnitude);
	ClassDB::bind_method(D_METHOD("set_force_magnitude", "p_force_magnitude"), &DropletServer::set_force_magnitude);
	ClassDB::add_property("DropletServer", PropertyInfo(Variant::FLOAT, "force_magnitude"), "set_force_magnitude", "get_force_magnitude");

	// Property: force_effective_distance
	ClassDB::bind_method(D_METHOD("get_force_effective_distance"), &DropletServer::get_force_effective_distance);
	ClassDB::bind_method(D_METHOD("set_force_effective_distance", "p_force_effective_distance"), &DropletServer::set_force_effective_distance);
	ClassDB::add_property("DropletServer", PropertyInfo(Variant::FLOAT, "force_effective_distance"), "set_force_effective_distance", "get_force_effective_distance");

	// Methods: add_droplet and remove_droplet
	ClassDB::bind_method(D_METHOD("add_droplet", "p_droplet"), &DropletServer::add_droplet);
	ClassDB::bind_method(D_METHOD("remove_droplet", "p_droplet"), &DropletServer::remove_droplet);
}

// Constructor and Destructor

DropletServer::DropletServer() :
    m_num_droplets(0),
    m_droplets(),
    m_positions(),
    m_forces(),
    m_force_magnitude(300.0),
    m_force_effective_distance(0.5),
    m_force_effective_distance_squared(0.25),
    m_in_game(false)
{}

DropletServer::~DropletServer()
{}

// Overridden Functions

void DropletServer::_ready()
{
	// Determine whether the game is running
	m_in_game = !Engine::get_singleton()->is_editor_hint();
}

void DropletServer::_physics_process(double delta)
{
    // Get the current positions
    for (int i = 0; i < m_num_droplets; i++)
    {
        m_positions[i] = Vec3(m_droplets[i]->get_global_position());
    }
    // Sum up the forces
    for (int i = 0; i < m_num_droplets; i++)
    {
        Vec3 droplet_a_position = m_positions[i];
        for (int j = i + 1; j < m_num_droplets; j++)
        {
            Vec3 droplet_b_position = m_positions[j];
            float distance_squared = droplet_a_position.distance_squared(droplet_b_position);
            if (distance_squared < m_force_effective_distance_squared)
            {
                Vec3 force_direction = (droplet_a_position - droplet_b_position).normalized();
                m_forces[i] += -m_force_magnitude * force_direction;
                m_forces[j] += m_force_magnitude * force_direction;
            }
        }
    }
    // Apply the forces
    for (int i = 0; i < m_num_droplets; i++)
    {
        m_droplets[i]->apply_central_force(godot::Vector3(m_forces[i]));
        m_forces[i] = Vec3::ZERO;
    }
}

// Functions for adding to or removing from the droplets array

void DropletServer::add_droplet(RigidBody3D* p_droplet)
{
    if (m_num_droplets < MAX_DROPLETS)
    {
        m_droplets[m_num_droplets] = p_droplet;
        m_positions[m_num_droplets] = Vec3(p_droplet->get_global_position());
        m_forces[m_num_droplets] = Vec3::ZERO;
        m_num_droplets++;
        UtilityFunctions::print("adding ", p_droplet);
    }
}

void DropletServer::remove_droplet(RigidBody3D* p_droplet)
{
    for (int i = 0; i < m_num_droplets; i++)
    {
        if (m_droplets[i] == p_droplet)
        {
            m_droplets[i] = m_droplets[m_num_droplets - 1];
            m_positions[i] = m_positions[m_num_droplets - 1];
            m_forces[i] = m_forces[m_num_droplets - 1];
            m_droplets[m_num_droplets - 1] = nullptr;
            m_positions[m_num_droplets - 1] = Vec3::ZERO;
            m_forces[m_num_droplets - 1] = Vec3::ZERO;
            m_num_droplets--;
            UtilityFunctions::print("removing ", p_droplet);
            break;
        }
    }
}

// Getters and setters for force magnitude and effective distance

void DropletServer::set_force_magnitude(float p_force_magnitude)
{
    m_force_magnitude = p_force_magnitude;
}

float DropletServer::get_force_magnitude() const
{
    return m_force_magnitude;
}

void DropletServer::set_force_effective_distance(float p_force_effective_distance)
{
    if (p_force_effective_distance < 0)
        m_force_effective_distance = 0;
    else
        m_force_effective_distance = p_force_effective_distance;
    m_force_effective_distance_squared = m_force_effective_distance * m_force_effective_distance;
    UtilityFunctions::print("force effective distance: ", m_force_effective_distance);
    UtilityFunctions::print("force effective distance squared: ", m_force_effective_distance_squared);
}

float DropletServer::get_force_effective_distance() const
{
    return m_force_effective_distance;
}
