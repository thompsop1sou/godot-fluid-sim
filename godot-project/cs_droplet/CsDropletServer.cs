using Godot;

[GlobalClass]
public partial class CsDropletServer : Node
{
	// Properties

	// Max number of droplets
	public const int MaxDroplets = 4000;

	// A struct to hold information about each droplet
	private struct Droplet
	{
		public Rid PhysRid;
		public System.Numerics.Vector3 Position;
		public System.Numerics.Vector3 Force;
		public System.Threading.Mutex Mut;
	}

	// An array of Droplet structs
	private int _numDroplets = 0;
	private Droplet[] _droplets = new Droplet[MaxDroplets];

	// The magnitude of the attraction force
	[Export]
	public float ForceMagnitude {get; set;}= 300.0f;

	// The distance that the attraction force is effective over
	private float _forceEffectiveDistanceSquared = 0.25f;
	private float _forceEffectiveDistance = 0.5f;
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
		// Get the current position of each droplet
		for (int i = 0; i < _numDroplets; ++i)
		{
			_droplets[i].Position = ToSystemVector3(GetDropletPosition(_droplets[i].PhysRid));
		}
		// Sum up the forces by looping over pairs of droplets
		// Outer loop to get first droplet
		System.Threading.Tasks.Parallel.For(0, _numDroplets, i =>
		{
			// Figure out start and end index for inner loop
			long start = i + 1;
			long end = start + _numDroplets / 2;
			if (_numDroplets % 2 == 0 && i * 2 >= _numDroplets)
				end--;
			// Inner loop to get second droplet
			for (long j = start; j < end; j++)
			{
				long k = j % _numDroplets;
				// Apply cohesive forces if the droplets are close enough
				float distanceSquared = System.Numerics.Vector3.DistanceSquared(_droplets[i].Position, _droplets[k].Position);
				if (distanceSquared < _forceEffectiveDistanceSquared)
				{
					var forceDirection = System.Numerics.Vector3.Normalize(_droplets[i].Position - _droplets[k].Position);
					_droplets[i].Mut.WaitOne();
					_droplets[i].Force += -ForceMagnitude * forceDirection;
					_droplets[i].Mut.ReleaseMutex();
					_droplets[k].Mut.WaitOne();
					_droplets[k].Force += ForceMagnitude * forceDirection;
					_droplets[k].Mut.ReleaseMutex();
				}
			}
		});
		// Apply the forces for each droplet
		System.Threading.Tasks.Parallel.For(0, _numDroplets, i =>
		{
			PhysicsServer3D.BodyApplyCentralForce(_droplets[i].PhysRid, ToGodotVector3(_droplets[i].Force));
			_droplets[i].Force = System.Numerics.Vector3.Zero;
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
			_droplets[_numDroplets].PhysRid = dropletRid;
			_droplets[_numDroplets].Position = ToSystemVector3(GetDropletPosition(dropletRid));
			_droplets[_numDroplets].Force = System.Numerics.Vector3.Zero;
			_droplets[_numDroplets].Mut = new System.Threading.Mutex();
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
			if (_droplets[i].PhysRid == dropletRid)
			{
				_droplets[i].PhysRid = _droplets[_numDroplets - 1].PhysRid;
				_droplets[i].Position = _droplets[_numDroplets - 1].Position;
				_droplets[i].Force = _droplets[_numDroplets - 1].Force;
				_droplets[_numDroplets - 1].PhysRid = new Rid();
				_droplets[_numDroplets - 1].Position = System.Numerics.Vector3.Zero;
				_droplets[_numDroplets - 1].Force = System.Numerics.Vector3.Zero;
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
