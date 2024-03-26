#include "VECS/core/vecs.hpp"
#include <chrono>
#include <ratio>

#include "utils.hpp"
#include "VECS/comps/Velocity.hpp"
#include "VECS/comps/Collider.hpp"
#include "VECS/comps/Transform.hpp"
#include "VECS/syss/InputSystem.hpp"
#include "VECS/syss/CombatSystem.hpp"
#include "VECS/syss/CollisionSystem.hpp"
#include "VECS/syss/LoggingSystem.hpp"
#include "VECS/syss/PhysicsSystem.hpp"
#include "VECS/syss/RenderSystem.hpp"

namespace VECS
{

std::shared_ptr<internal::World> WorldPtr;
bool Exit;
std::string AssetsPath;

void Init()
{
	WorldPtr				 = std::make_shared<internal::World>();
	internal::Entity::pWorld = WorldPtr;
	Exit					 = false;
	AssetsPath				 = std::string{"../assets/images/"};
}
void Build()
{
	internal::World &World = *WorldPtr;
	// Register all components here
	World.RegisterComponent<Render>();
	World.RegisterComponent<Controllable>();
	World.RegisterComponent<Transform>();
	World.RegisterComponent<Velocity>();
	World.RegisterComponent<Collider>();
	World.RegisterComponent<Health>();

	// Register all systems here
	Signature signature;

	signature.set(World.GetComponentType<Render>());
	World.RegisterSystem<RenderSystem>();
	World.SetSystemSignature<RenderSystem>(signature);

	signature.reset();

	signature.set(World.GetComponentType<Controllable>());
	World.RegisterSystem<InputSystem>();
	World.SetSystemSignature<InputSystem>(signature);

	signature.reset();

	signature.set(World.GetComponentType<Transform>());
	signature.set(World.GetComponentType<Velocity>());
	World.RegisterSystem<PhysicsSystem>();
	World.SetSystemSignature<PhysicsSystem>(signature);

	signature.reset();

	signature.set(World.GetComponentType<Transform>());
	signature.set(World.GetComponentType<Collider>());
	World.RegisterSystem<CollisionSystem>();
	World.SetSystemSignature<CollisionSystem>(signature);

	signature.reset();

	signature.set(World.GetComponentType<Collider>());
	signature.set(World.GetComponentType<Health>());
	World.RegisterSystem<CombatSystem>();
	World.SetSystemSignature<CombatSystem>(signature);

	//	World.RegisterSystem<LoggingSystem>();
	//	World.SetSystemSignature<LoggingSystem>(signature);

	// Create initial entities here

	// VECS::internal::Entity background = World.Create();
	// background.AddComponent<Render>(Render{0,0});
	// background.AddComponent<Controllable>(Controllable{0,1});
	//
	VECS::internal::Entity player = World.Create();
	player.AddComponent<Render>(Render{PLAYER, SILVER,40,40});
	player.AddComponent<Controllable>(Controllable{CPLAYER, true});
	player.AddComponent<Transform>(Transform{10, 10});
	player.AddComponent<Velocity>(Velocity{60, 0});
	player.AddComponent<Collider>(Collider{40,40,0,DYNAMIC});
	player.AddComponent<Health>(Health{100000,100000});
	//	player.AddComponent<>({});
	Collider tileCollider{10,10,0,STATIC};
	Health tileHealth{1,1};
	for (int i = 0; i < 3072; ++i)
	{
		VECS::internal::Entity tile = World.Create();

		tile.AddComponent<Render>(Render{TILE,genRand(0,4),10,10});
		tile.AddComponent<Collider>(tileCollider);
		tile.AddComponent<Health>(tileHealth);
		tile.AddComponent<Transform>(Transform{
			10.f * static_cast<int>(i % 64), 10.f * static_cast<int>(i / 64)});
	}
}
void Run()
{

	internal::World &World = *WorldPtr;
	auto last			   = std::chrono::high_resolution_clock::now();
	auto now			   = std::chrono::high_resolution_clock::now();
	float dt			   = 0;

	while (!Exit)
	//	for(int i = 0;i<60;++i)
	{
		now	 = std::chrono::high_resolution_clock::now();
		dt	 = std::chrono::duration<float, std::ratio<1>>(now - last).count();
		last = now;

		World.GetSystem<InputSystem>()->Update();
		World.GetSystem<PhysicsSystem>()->Update(dt);
		World.GetSystem<CollisionSystem>()->Update();
		World.GetSystem<CombatSystem>()->Update();
		//		World.GetSystem<LoggingSystem>()->Update(dt);
		World.GetSystem<RenderSystem>()->Update();
	}
}

} // namespace VECS
