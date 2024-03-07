using Godot;

[GlobalClass]
public partial class CsDropletServer : Node
{
	// Properties

	// Max number of droplets
	public const int MaxDroplets = 4000;

	// An array of droplets and their properties
	private int _numDroplets = 0;
	private Rid[] _dropletRids = new Rid[MaxDroplets];
	private System.Numerics.Vector3[] _positions = new System.Numerics.Vector3[MaxDroplets];
	private System.Numerics.Vector3[] _forces = new System.Numerics.Vector3[MaxDroplets];
	private System.Threading.Mutex[] _mutexes = new System.Threading.Mutex[MaxDroplets];

	// The magnitude of the attraction force
	[Export]
	public float ForceMagnitude {get; set;}= 300.0f;

	// The distance that the attraction force is effective over
	private float _forceEffectiveDistance = 0.5f;
	private float _forceEffectiveDistanceSquared = 0.25f;
	[Export]
	public float ForceEffectiveDistance
	{
		get
		{
			return _forceEffectiveDistance;
		}
		set
		{
			_forceEffectiveDistance = Mathf.Abs(value);
			_forceEffectiveDistanceSquared = _forceEffectiveDistance * _forceEffectiveDistance;
		}
	}




	// Overridden Methods

    // Called every physics frame. 'delta' is the elapsed time since the previous frame.
    public override void _PhysicsProcess(double delta)
	{
		// Get the current positions
		for (int i = 0; i < _numDroplets; ++i)
		{
			_positions[i] = ToSystemVector3(GetDropletPosition(_dropletRids[i]));
		}
		// Sum up the forces, main loop
		System.Threading.Tasks.Parallel.For(0, _numDroplets, i =>
		{
			// Figure out start and end index for sub loop
			long start = i + 1;
			long end = start + _numDroplets / 2;
			if (_numDroplets % 2 == 0 && i * 2 >= _numDroplets)
				end--;
			// Sub loop
			for (long j = start; j < end; j++)
			{
				long k = j % _numDroplets;
				float distanceSquared = System.Numerics.Vector3.DistanceSquared(_positions[i], _positions[k]);
				if (distanceSquared < _forceEffectiveDistanceSquared)
				{
					var forceDirection = System.Numerics.Vector3.Normalize(_positions[i] - _positions[k]);
					_mutexes[i].WaitOne();
					_forces[i] += -ForceMagnitude * forceDirection;
					_mutexes[i].ReleaseMutex();
					_mutexes[k].WaitOne();
					_forces[k] += ForceMagnitude * forceDirection;
					_mutexes[k].ReleaseMutex();
				}
			}
		});
		// Apply the forces
		System.Threading.Tasks.Parallel.For(0, _numDroplets, i =>
		{
			PhysicsServer3D.BodyApplyCentralForce(_dropletRids[i], ToGodotVector3(_forces[i]));
			_forces[i] = System.Numerics.Vector3.Zero;
		});
	}



	// Other Methods

	// Custom method to convert a Godot Vector3 to a C# Vector3
	private static System.Numerics.Vector3 ToSystemVector3(Vector3 vec)
	{
		return new System.Numerics.Vector3(vec.X, vec.Y, vec.Z);
	}

	// Custom method to convert a C# Vector3 to a Godot Vector3
	private static Vector3 ToGodotVector3(System.Numerics.Vector3 vec)
	{
		return new Vector3(vec.X, vec.Y, vec.Z);
	}

	// Adds a droplet to the server's array
	public void AddDroplet(Rid dropletRid)
	{
		// Make sure the droplet is a rigid body
		if (!IsRigidBody(dropletRid))
		{
			GD.PrintErr("NOT A RIGID BODY: Could not add physics object ", dropletRid, " to ", this);
			return;
		}

		// Try to add it
		if (_numDroplets < MaxDroplets)
		{
			_dropletRids[_numDroplets] = dropletRid;
			_forces[_numDroplets] = System.Numerics.Vector3.Zero;
			_mutexes[_numDroplets] = new();
			_numDroplets++;
		}
		else
		{
			GD.PrintErr("MAX EXCEEDED: Could not add physics object ", dropletRid, " to ", this);
		}
	}

	// Removes a droplet from the server's array
	public void RemoveDroplet(Rid dropletRid)
	{
		// Make sure the droplet is a rigid body
		if (!IsRigidBody(dropletRid))
		{
			GD.PrintErr("NOT A RIGID BODY: Could not remove physics object ", dropletRid, " from ", this);
			return;
		}

		// Try to remove it
		for (int i = 0; i < _numDroplets; i++)
		{
			if (_dropletRids[i] == dropletRid)
			{
				_dropletRids[i] = _dropletRids[_numDroplets - 1];
				_positions[i] = _positions[_numDroplets - 1];
				_forces[i] = _forces[_numDroplets - 1];
				_dropletRids[_numDroplets - 1] = new Rid();
				_positions[_numDroplets - 1] = System.Numerics.Vector3.Zero;
				_forces[_numDroplets - 1] = System.Numerics.Vector3.Zero;
				_numDroplets--;
				return;
			}
		}
		GD.PrintErr("NOT FOUND: Could not remove physics object ", dropletRid, " from ", this);
	}

	// Confirms that an RID is for a rigid body
	private bool IsRigidBody(Rid dropletRid)
	{
		PhysicsServer3D.BodyMode bodyMode = PhysicsServer3D.BodyGetMode(dropletRid);
		return bodyMode == PhysicsServer3D.BodyMode.Rigid || bodyMode == PhysicsServer3D.BodyMode.RigidLinear;
	}

	// Gets the position of a droplet from its RID
	private Vector3 GetDropletPosition(Rid dropletRid)
	{
		Transform3D dropletTransform = (Transform3D)PhysicsServer3D.BodyGetState(dropletRid, PhysicsServer3D.BodyState.Transform);
		return dropletTransform.Origin;
	}
}
