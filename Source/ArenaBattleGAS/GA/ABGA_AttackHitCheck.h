// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_AttackHitCheck.generated.h"

UCLASS()
class ARENABATTLEGAS_API UABGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UABGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	//5-12
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	//7-1
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	//7-12
	float CurrentLevel;

	//8-16
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackBuffEffect;

	//10-7
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class AABTA_Trace> TargetActorClass;
};





































