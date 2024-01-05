#include "ECS/ecs.h"
#include <iostream>

int main()
{
	EntityManager em = EntityManager();
	ComponentManager cm = ComponentManager();
	cm.RegisterComponent<Health>();

	Entity me = em.AddEntity();
	Entity he = em.AddEntity();
	Entity she = em.AddEntity();
	Entity it = em.AddEntity();
	Health healthComp = Health{100};
	cm.PrintComponents();
	cm.AddComponent(me,healthComp);
	cm.PrintComponents();
	cm.AddComponent(he,healthComp);
	cm.PrintComponents();
	cm.AddComponent(she,healthComp);
	cm.PrintComponents();
	cm.RemoveEntity(me);
	cm.PrintComponents();
	cm.AddComponent(it,healthComp);
	cm.PrintComponents();
	return 0;
}
