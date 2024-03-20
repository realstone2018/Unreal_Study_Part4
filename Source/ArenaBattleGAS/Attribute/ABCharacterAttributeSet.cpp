// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterAttributeSet.h"
#include "ArenaBattleGAS.h"
#include "GameplayEffectExtension.h"	//7-9
#include "Tag/AbGameplayTag.h"	//8-6

UABCharacterAttributeSet::UABCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//7-8 Hp 직접 접근 제거, 데미지를 통해 계산되도록
	// if (Attribute == GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	// }

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;	
	}
}

// 7-9 Attribute Set 변경은 직접 접근이 아닌, 오직 GE를 통해서만 
// void UABCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
// {
// 	if (Attribute == GetHealthAttribute())
// 	{
// 		ABGAS_LOG(LogABGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
// 	}
// }

bool UABCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(ABTAG_CHARACTER_INVINSIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}	
	}
	
	return true;
}

void UABCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access: %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		ABGAS_LOG(LogABGAS, Warning, TEXT("Damage: %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);	//데미지를 계산마다 초기화하지말고 휘발성 데이터로 관리하는게 좋아보임
	}

	//8-5
	if(GetHealth() <= 0.0f && !bOutOfHealth)
	{
		//8-6 Target에 IsDead 태그를 넣어준다.
		Data.Target.AddLooseGameplayTag(ABTAG_CHARACTER_ISDEAD);
		
		OnOutOfHealth.Broadcast();
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
}









































