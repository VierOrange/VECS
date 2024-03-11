#include "VECS/comps/Health.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/core/ecs.hpp"

int main()
{
	VECS world{};
	world.RegisterComponent<Health>();
	world.RegisterComponent<Transform>();

	Entity me				= world.Create();
	Health healthComp		= Health{100};
	Transform transformComp = Transform{0, 0, 0};
	world.AddComponent(me, healthComp);
	world.AddComponent(me, transformComp);
	return 0;
}
