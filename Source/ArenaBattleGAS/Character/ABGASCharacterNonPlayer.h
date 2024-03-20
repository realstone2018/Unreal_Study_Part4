// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGASCharacterNonPlayer.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASCharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASCharacterNonPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	//6-4
	UPROPERTY()
	TObjectPtr<class UABCharacterAttributeSet> AttributeSet;

	//7-17 시작시, GA없이 발동시킬 GE
	UPROPERTY(EditAnywhere, Category= GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	//8-4
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UABGASWidgetComponent> HpBar;

	//8-7
	UFUNCTION()
	virtual void OnOutOfHealth();
};









































