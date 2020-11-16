#include "Trace.h"
#include "Prediction.h"
Prediction::Prediction()
{
}

Prediction* Prediction::GetInstance()
{
	static auto r = new Prediction();
	return r;
}

Prediction& Prediction::GetReference()
{
	return *GetInstance();
}


Prediction::~Prediction()
{
}

Vector LinePrediction::Calculate(Hero* target, float range, float missilespeed, float casttime)
{
	float t = Vector(target->GetPos() - Engine::GetLocalPlayer()->GetPos()).Length() / missilespeed;
	t += casttime;
	auto aim = target->GetAIManager();
	auto veloc = aim->GetVelocity();
	veloc.y = 0.f;
	Vector orientation = veloc.Normalize();

	if (!(target && target->GetHealth() > 0.f && target->GetPos().DistTo(Me->GetPos())< range)) {
		return Vector(0.f, 0.f, 0.f);
	}

	if (veloc.x == 0.f && veloc.z == 0.f)
	{
		return target->GetPos();
	}

	Vector result = target->GetPos() + (orientation * target->GetMoveSpeed() * t);

	if (result.DistTo(Engine::GetLocalPlayer()->GetPos()) > range) {
		return Vector(0.f, 0.f, 0.f);
	}

	return result;
}

Vector CirclePrediction::Calculate(Hero* target, float range, float casttime)
{
	auto aim = target->GetAIManager();
	auto veloc = aim->GetVelocity();
	veloc.y = 0.f;
	Vector orientation = veloc.Normalize(); //target->GetAllShield->vVelocity.Normalized();

	if (!(target && target->GetHealth() > 0.f && target->GetPos().DistTo(Me->GetPos()) < range)) {
		return Vector(0.f, 0.f, 0.f);
	}

	if (veloc.x== 0.f && veloc.z == 0.f)
	{
		return target->GetPos();
	}

	Vector result = target->GetPos() + (orientation * target->GetMoveSpeed() * casttime);

	return result;
}