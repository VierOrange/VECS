#include "VECS/core/vecs.hpp"

#include "VECS/syss/LoggingSystem.hpp"
#include "VECS/syss/RenderSystem.hpp"
#include "VECS/syss/InputSystem.hpp"

namespace VECS
{

std::shared_ptr<internal::World> WorldPtr;
bool Exit;
std::string AssetsPath;

void Init()
{
	WorldPtr				 = std::make_shared<internal::World>();
	internal::Entity::pWorld = WorldPtr;
	Exit = false;
	AssetsPath = std::string{"../assets/images/"};
}
void Build()
{
	internal::World &World = *WorldPtr;
	// Register all components here
	World.RegisterComponent<Render>();
	World.RegisterComponent<Controllable>();

	// Register all systems here
	Signature signature;

	signature.set(World.GetComponentType<Render>());
	World.RegisterSystem<RenderSystem>();
	World.SetSystemSignature<RenderSystem>(signature);

	signature.reset();

	signature.set(World.GetComponentType<Controllable>());
	World.RegisterSystem<InputSystem>();
	World.SetSystemSignature<InputSystem>(signature);

	signature.set(World.GetComponentType<Render>());
	World.RegisterSystem<LoggingSystem>();
	World.SetSystemSignature<LoggingSystem>(signature);

	// Create initial entities here
	
	VECS::internal::Entity background = World.Create();
	background.AddComponent<Render>(Render{0,0});
	background.AddComponent<Controllable>(Controllable{0,1});
}
void Run()
{

	internal::World &World = *WorldPtr;
	while (!Exit)
//	for(int i = 0;i<60;++i)
	{
		World.GetSystem<InputSystem>()->Update();
		World.GetSystem<LoggingSystem>()->Update();
		World.GetSystem<RenderSystem>()->Update();
	}
}

} // namespace VECS
