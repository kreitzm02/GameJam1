// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

UStateMachine::UStateMachine(AActor* a_InOwner)
{
	m_Owner = a_InOwner;
}

UStateMachine::~UStateMachine() = default;

void UStateMachine::SetInitial(IBaseState* a_State)
{
	m_Current = a_State;
	m_PendingEnter = true;
}

void UStateMachine::AddTransition(FTransition a_Transition)
{
	m_Transitions.Add(MoveTemp(a_Transition));
}

void UStateMachine::Update(float a_DeltaTime)
{
	if (!m_Owner || !m_Current) return;

	if (m_PendingEnter)
	{
		m_Current->OnEnter(m_Owner);
		m_PendingEnter = false;
	}

	if (IBaseState* next = TryFindTransition(nullptr)) SwitchTo(next);
	if (IBaseState* next = TryFindTransition(m_Current)) SwitchTo(next);

	m_Current->OnUpdate(m_Owner, a_DeltaTime);
}

IBaseState* UStateMachine::TryFindTransition(IBaseState* a_From)
{
	IBaseState* next = nullptr;

	for (FTransition& t : m_Transitions)
	{
		if (t.m_From != a_From) continue;
		if (!t.m_To || !t.m_Condition) continue;
		if (t.m_Condition())
		{
			next = t.m_To;
			break;
		}
	}
	return next;
}

void UStateMachine::SwitchTo(IBaseState* a_Next)
{
	if (!a_Next || a_Next == m_Current) return;

	m_Current->OnExit(m_Owner);
	m_Current = a_Next;
	m_PendingEnter = true;
}

IBaseState* UStateMachine::GetCurrent() const
{
	return m_Current;
}
