#include "VECS/core/vecs.hpp"

#include "VECS/syss/HurtingSystem.hpp"
#include "VECS/syss/LoggingSystem.hpp"
#include "VECS/syss/RenderSystem.hpp"
#include "VECS/syss/InputSystem.hpp"

namespace VECS
{

std::shared_ptr<internal::World> WorldPtr;
bool Exit;

void Init()
{
	WorldPtr				 = std::make_shared<internal::World>();
	internal::Entity::pWorld = WorldPtr;
	Exit = false;
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
	World.RegisterSystem<RenderSystem>();
	World.SetSystemSignature<RenderSystem>(signature);
	World.RegisterSystem<InputSystem>();
	World.SetSystemSignature<InputSystem>(signature);

	// Create initial entities here
	VECS::internal::Entity alien = World.Create();
	alien.AddComponent<Health>(Health{100, 100});
}
void Run()
{

	internal::World &World = *WorldPtr;
	while (!Exit)
	//for(int i = 0;i<6000;++i)
	{
		World.GetSystem<InputSystem>()->Update();
		World.GetSystem<HurtingSystem>()->Update();
		World.GetSystem<LoggingSystem>()->Update();
		World.GetSystem<RenderSystem>()->Update();
	}
}

} // namespace VECS
