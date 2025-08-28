// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <functional>

class IBaseState
{
public:
	virtual ~IBaseState() = default;
	virtual void OnEnter(AActor* a_Owner) {}
	virtual void OnUpdate(AActor* a_Owner, float a_DeltaTime) {}
	virtual void OnExit(AActor* a_Owner) {}
};

class FTransition
{
public:
	IBaseState* m_From = nullptr;
	IBaseState* m_To = nullptr;
	TFunction<bool()> m_Condition;
};

class GAMEJAM1_API UStateMachine
{
public:
	explicit UStateMachine(AActor* a_InOwner);
	~UStateMachine();

	void SetInitial(IBaseState* a_State);
	IBaseState* GetCurrent() const;
	void AddTransition(FTransition a_Transition);
	void Update(float a_DeltaTime);

private:
	IBaseState* TryFindTransition(IBaseState* a_From);
	void SwitchTo(IBaseState* a_Next);

	AActor* m_Owner = nullptr;
	IBaseState* m_Current = nullptr;
	bool m_PendingEnter = false;
	TArray<FTransition> m_Transitions;
};
