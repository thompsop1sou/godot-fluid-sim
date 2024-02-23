#ifndef DROPLET_BODY3D_H
#define DROPLET_BODY3D_H

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>

namespace godot
{
	class DropletBody3D : public RigidBody3D
	{
		GDCLASS(DropletBody3D, RigidBody3D)

	private:
		// Keep an array of all the nearby droplets and physics bodies
		static const size_t MAX_NEARBY = 32;
		DropletBody3D* m_nearby_droplets[MAX_NEARBY];
		Node3D* m_nearby_bodies[MAX_NEARBY];
		size_t m_nearby_droplets_size;
		size_t m_nearby_bodies_size;

		// The magnitude of the attraction forces
		float m_droplet_force_mag;
		float m_body_force_mag;

		// Whether currently in-game
		bool m_in_game;

	protected:
		static void _bind_methods();

	public:
		// Constructor and destructor
		DropletBody3D();
		~DropletBody3D();

		// Overridden functions
		void _ready() override;
		void _physics_process(double delta) override;

		// Functions for adding to or removing from nearby droplets/bodies arrays
		void add_body(Node3D* body);
		void remove_body(Node3D* body);

		// Getters and setters for force magnitudes
		void set_droplet_force_mag(float p_droplet_force_mag);
		float get_droplet_force_mag() const;
		void set_body_force_mag(float p_body_force_mag);
		float get_body_force_mag() const;
	};
}

#endif