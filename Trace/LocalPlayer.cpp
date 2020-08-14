#include "Hero.hpp"


int LocalPlayer::IssueOrder(IssueOrderType Order, Vector* Loc, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown)
{
	static auto fnIssueOrder = reinterpret_cast<int(__thiscall*)(PVOID, IssueOrderType, Vector*, GameObject*, bool, bool, DWORD)>(Engine::GetBaseModule() + Function::IssueOrder);
	return fnIssueOrder(this, Order, Loc, Target, IsAttackMove, IsMinion, Unknown);
}
void LocalPlayer::MoveTo(Vector pos)
{
	if (this->GetPos().DistTo(pos) < 150.0f) {
		Vector test = pos - (pos - this->GetPos()) * (150.0f / this->GetPos().DistTo(pos));
		Vector maxERange = Vector(-test.X, -test.Y, -test.Z);

		IssueOrder(IssueOrderType::MoveTo, &maxERange, nullptr, false, false, false);
	}
	else {

		IssueOrder(IssueOrderType::MoveTo, &pos, nullptr, false, false, false);
	}
}
void LocalPlayer::Attack(GameObject* target)
{
	IssueOrder(IssueOrderType::AttackUnit, &target->GetPos(), target, true, false, false);
}

float LocalPlayer::GetAttackDelay()
{
	static auto fnGetAttackDelay = reinterpret_cast<float(__cdecl*)(PVOID)>(Engine::GetBaseModule() + Function::GetAttackDelay);
	return fnGetAttackDelay(this);
}

float LocalPlayer::GetAttackCastDelay()
{
	static auto fnGetAttackCastDelay = reinterpret_cast<float(__cdecl*)(PVOID)>(Engine::GetBaseModule()+Function::GetAttackCastDelay);

	return fnGetAttackCastDelay(this);
}
