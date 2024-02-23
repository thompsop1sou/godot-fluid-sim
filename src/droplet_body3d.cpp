#include "droplet_body3d.h"
#include <godot_cpp/core/class_db.hpp>
#include <stdio.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void DropletBody3D::_bind_methods()
{
	// Property: droplet_force_mag
	ClassDB::bind_method(D_METHOD("get_droplet_force_mag"), &DropletBody3D::get_droplet_force_mag);
	ClassDB::bind_method(D_METHOD("set_droplet_force_mag", "p_droplet_force_mag"), &DropletBody3D::set_droplet_force_mag);
	ClassDB::add_property("DropletBody3D", PropertyInfo(Variant::FLOAT, "droplet_force_mag"), "set_droplet_force_mag", "get_droplet_force_mag");

	// Property: body_force_mag
	ClassDB::bind_method(D_METHOD("get_body_force_mag"), &DropletBody3D::get_body_force_mag);
	ClassDB::bind_method(D_METHOD("set_body_force_mag", "p_body_force_mag"), &DropletBody3D::set_body_force_mag);
	ClassDB::add_property("DropletBody3D", PropertyInfo(Variant::FLOAT, "body_force_mag"), "set_body_force_mag", "get_body_force_mag");

	// Methods: add_body and remove_body
	ClassDB::bind_method(D_METHOD("add_body", "p_body"), &DropletBody3D::add_body);
	ClassDB::bind_method(D_METHOD("remove_body", "p_body"), &DropletBody3D::remove_body);
}

// Constructor and destructor

DropletBody3D::DropletBody3D() :
	m_nearby_droplets(),
	m_nearby_bodies(),
	m_nearby_droplets_size(0),
	m_nearby_bodies_size(0),
	m_droplet_force_mag(300.0),
	m_body_force_mag(300.0),
	m_in_game()
{}

DropletBody3D::~DropletBody3D()
{}

// Overridden functions

void DropletBody3D::_ready()
{
	// Determine whether the game is running
	m_in_game = !Engine::get_singleton()->is_editor_hint();

	// Connect up to the enter/exit signals of a child Area3D node
	TypedArray<Node> areas = find_children("*", "Area3D");
	if (areas.size() == 0)
	{
		UtilityFunctions::printerr(this, " cannot find any child Area3D nodes");
	}
	else
	{
		for (int i = 0; i < areas.size(); i++)
		{
			Object* droplet_object = areas[i];
			Area3D* droplet_area = Object::cast_to<Area3D>(droplet_object);
			if (droplet_area != nullptr)
			{
				droplet_area->connect("body_entered", Callable(this, "add_body"));
				droplet_area->connect("body_exited", Callable(this, "remove_body"));
			}
		}
	}
}

void DropletBody3D::_physics_process(double delta)
{
	// Only process physics while the game is running
	if (m_in_game)
	{
		// Sum up the force from nearby droplets
		Vector3 droplet_force = Vector3(0.0, 0.0, 0.0);
		for (int i = 0; i < m_nearby_droplets_size; i++)
		{
			droplet_force += (get_position() - m_nearby_droplets[i]->get_position()).normalized();
		}

		// Sum up the force from nearby regular bodies
		Vector3 body_force = Vector3(0.0, 0.0, 0.0);
		for (int i = 0; i < m_nearby_bodies_size; i++)
		{
			body_force += (get_position() - m_nearby_bodies[i]->get_position()).normalized();
		}

		// Apply the total force to this droplet
		apply_central_force(m_droplet_force_mag * droplet_force + m_body_force_mag * body_force);
	}
}

// Functions for adding to or removing from nearby droplets/bodies arrays

void DropletBody3D::add_body(Node3D* p_body)
{
	// Try to cast it to a DropletBody3D
	DropletBody3D* droplet_body = Object::cast_to<DropletBody3D>(p_body);

	// If the cast did NOT work, add it to the regular bodies array
	if (droplet_body == nullptr)
	{
		if (m_nearby_bodies_size < MAX_NEARBY)
		{
			m_nearby_bodies[m_nearby_bodies_size] = p_body;
			m_nearby_bodies_size++;
		}
	}
	// If the cast did work, add it to the droplet bodies array (assuming it is not this droplet)
	else if (droplet_body != this)
	{
		if (m_nearby_droplets_size < MAX_NEARBY)
		{
			m_nearby_droplets[m_nearby_droplets_size] = droplet_body;
			m_nearby_droplets_size++;
		}
	}
}

void DropletBody3D::remove_body(Node3D* p_body)
{
	// Try to cast it to a DropletBody3D
	DropletBody3D* droplet_body = Object::cast_to<DropletBody3D>(p_body);

	// If the cast did NOT work, remove it from the regular bodies array
	if (droplet_body == nullptr)
	{
		for (int i = 0; i < m_nearby_bodies_size; i++)
		{
			if (m_nearby_bodies[i] == p_body)
			{
				m_nearby_bodies[i] = m_nearby_bodies[m_nearby_bodies_size - 1];
				m_nearby_bodies[m_nearby_bodies_size - 1] = nullptr;
				m_nearby_bodies_size--;
				break;
			}
		}
	}
	// If the cast did work, remove it from the droplet bodies array (assuming it is not this droplet)
	else if (droplet_body != this)
	{
		for (int i = 0; i < m_nearby_droplets_size; i++)
		{
			if (m_nearby_droplets[i] == droplet_body)
			{
				m_nearby_droplets[i] = m_nearby_droplets[m_nearby_droplets_size - 1];
				m_nearby_droplets[m_nearby_droplets_size - 1] = nullptr;
				m_nearby_droplets_size--;
				break;
			}
		}
	}
}

// Getters and setters for force magnitudes

void DropletBody3D::set_droplet_force_mag(float p_droplet_force_mag)
{
	m_droplet_force_mag = p_droplet_force_mag;
}

float DropletBody3D::get_droplet_force_mag() const
{
	return m_droplet_force_mag;
}

void DropletBody3D::set_body_force_mag(float p_body_force_mag)
{
	m_body_force_mag = p_body_force_mag;
}

float DropletBody3D::get_body_force_mag() const
{
	return m_body_force_mag;
}