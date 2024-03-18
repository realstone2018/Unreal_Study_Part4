// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABGE_AttackDamage.h"
#include "Attribute/ABCharacterAttributeSet.h"

UABGE_AttackDamage::UABGE_AttackDamage()
{
	//한프레임내 효과 적용 완료
	DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo HealthModifier;

	//변경할 AttributeSet 클래스 및 필드 설정
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UABCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UABCharacterAttributeSet, Health)));
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	//실제 적용할 값
	FScalableFloat DamageAmount(-30.0f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	HealthModifier.ModifierMagnitude = ModMagnitude;
	Modifiers.Add(HealthModifier);
}




































