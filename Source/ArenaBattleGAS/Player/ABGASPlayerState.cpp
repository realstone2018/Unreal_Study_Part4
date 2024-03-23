// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "Attribute/ABCharacterSkillAttributeSet.h" //10-12

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//네트워크 멀티플레이인 경우, 서버에서 클라로 계속 갱신시켜 줘야 한다.
	//ASC->SetIsReplicated(true);

	//6-4
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("AttributeSet"));

	//10-12
	SkillAttributeSet = CreateDefaultSubobject<UABCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));

}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

























