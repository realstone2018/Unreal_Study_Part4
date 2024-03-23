// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h" //3-3
#include "ABGASPlayerState.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;
	 
	//6-4
	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;

	//10-12
	UPROPERTY()
	TObjectPtr<class UABCharacterSkillAttributeSet> SkillAttributeSet;
};





























