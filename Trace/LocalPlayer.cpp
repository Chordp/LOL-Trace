#include "Hero.hpp"

int LocalPlayer::IssueOrder(int Order, Vector* Loc, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown)
{
	static auto fnIssueOrder = reinterpret_cast<int(__thiscall*)(PVOID, int, Vector*, GameObject*, bool, bool, DWORD)>(Engine::GetBaseModule() + Function::IssueOrder);
	return fnIssueOrder(this, Order, Loc, Target, IsAccelerator, IsMinion, Unknown);
}
void LocalPlayer::MoveTo(Vector pos)
{
	if (this->GetPos().DistTo(pos) < 150.0f) {
		Vector test = pos - (pos - this->GetPos()) * (150.0f / this->GetPos().DistTo(pos));
		Vector maxERange = Vector(-test.X, -test.Y, -test.Z);

		IssueOrder(2, &maxERange, NULL, false, false, false);
	}
	else {

		IssueOrder(2, &pos, NULL, false, false, false);
	}
}
void LocalPlayer::Attack(GameObject* target, bool isMinion)
{
	IssueOrder(3, &target->GetPos(), target, 1, isMinion, 0);
}

float LocalPlayer::GetAttackDelay()
{
	static auto fnGetAttackDelay = reinterpret_cast<float(__cdecl*)(PVOID)>(Engine::GetBaseModule() + Function::GetAttackDelay);
	return fnGetAttackDelay(this);
}

float LocalPlayer::GetAttackCastDelay()
{
	static auto fnGetAttackCastDelay = reinterpret_cast<float(__cdecl*)(PVOID,int)>(Engine::GetBaseModule()+Function::GetAttackCastDelay);
	return fnGetAttackCastDelay(this,64);
}
