#include "VECS/core/vecs.hpp"

#include "VECS/syss/HurtingSystem.hpp"
#include "VECS/syss/LoggingSystem.hpp"

namespace VECS
{

std::shared_ptr<internal::World> WorldPtr;

void Init()
{
	WorldPtr				 = std::make_shared<internal::World>();
	internal::Entity::pWorld = WorldPtr;
}
void Build()
{
	internal::World &World = *WorldPtr;
	// Register all components here
	World.RegisterComponent<Health>();

	// Register all systems here
	Signature signature;

	signature.set(World.GetComponentType<Health>());
	World.RegisterSystem<HurtingSystem>();
	World.SetSystemSignature<HurtingSystem>(signature);
	World.RegisterSystem<LoggingSystem>();
	World.SetSystemSignature<LoggingSystem>(signature);

	// Create initial entities here
	VECS::internal::Entity alien = World.Create();
	alien.AddComponent<Health>(Health{100, 100});
}
void Run()
{

	internal::World &World = *WorldPtr;
	while (true)
	{
		World.GetSystem<HurtingSystem>()->Update();
		World.GetSystem<LoggingSystem>()->Update();
	}
}

} // namespace VECS
