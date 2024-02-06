#include "VECS/core/ecs.hpp"
#include "VECS/comps/Health.hpp"
#include "VECS/comps/Transform.hpp"
#include <iostream>

int main()
{
	EntityManager em = EntityManager();
	ComponentManager cm = ComponentManager();
	cm.RegisterComponent<Health>();
	cm.RegisterComponent<Transform>();

	Entity me = em.AddEntity();
	Health healthComp = Health{100};
	Transform transformComp = Transform{0,0,0};
	cm.AddComponent(me,healthComp);
	cm.AddComponent(me,transformComp);
	cm.PrintComponents();
	return 0;
}
