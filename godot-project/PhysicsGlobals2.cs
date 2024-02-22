using Godot;

public partial class PhysicsGlobals2 : Node
{
	public const uint MaxDroplets = 2000;
	private CustomDroplet2[] _customDroplets = new CustomDroplet2[MaxDroplets];
	private System.Numerics.Vector3[] _customPositions = new System.Numerics.Vector3[MaxDroplets];
	private System.Numerics.Vector3[] _customForces = new System.Numerics.Vector3[MaxDroplets];
	private System.Threading.Mutex[] _customMutexes = new System.Threading.Mutex[MaxDroplets];
	private uint _numCustomDroplets = 0;
	private float forceMag = 300.0f;

	public void RegisterCustomDroplet(CustomDroplet2 droplet)
	{
		_customDroplets[_numCustomDroplets] = droplet;
		_customForces[_numCustomDroplets] = System.Numerics.Vector3.Zero;
		_customMutexes[_numCustomDroplets] = new();
		_numCustomDroplets++;
	}

	private static System.Numerics.Vector3 ToSystemVector3(Vector3 vec)
	{
		return new System.Numerics.Vector3(vec.X, vec.Y, vec.Z);
	}

	private static Vector3 ToGodotVector3(System.Numerics.Vector3 vec)
	{
		return new Vector3(vec.X, vec.Y, vec.Z);
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _PhysicsProcess(double delta)
	{
		for (int i = 0; i < _numCustomDroplets; ++i)
		{
			_customPositions[i] = ToSystemVector3(_customDroplets[i].Position);
		}
		System.Threading.Tasks.Parallel.For(0, _numCustomDroplets, i =>
		{
			var dropletAPosition = _customPositions[i];
			System.Threading.Tasks.Parallel.For(i + 1, _numCustomDroplets, j =>
			{
				var dropletBPosition = _customPositions[j];
				if (System.Numerics.Vector3.DistanceSquared(dropletAPosition, dropletBPosition) < 0.5f)
				{
					System.Numerics.Vector3 forceDirec = System.Numerics.Vector3.Normalize(dropletAPosition - dropletBPosition);
					_customMutexes[i].WaitOne();
					_customForces[i] += -forceMag * forceDirec;
					_customMutexes[i].ReleaseMutex();
					_customMutexes[j].WaitOne();
					_customForces[j] += forceMag * forceDirec;
					_customMutexes[j].ReleaseMutex();
				}
			});
		});
		System.Threading.Tasks.Parallel.For(0, _numCustomDroplets, i =>
		{
			var force = ToGodotVector3(_customForces[i]);
			_customDroplets[i].ApplyCentralForce(force);
			_customForces[i] = System.Numerics.Vector3.Zero;
		});
	}
}
