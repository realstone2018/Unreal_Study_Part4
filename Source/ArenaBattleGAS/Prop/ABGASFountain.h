// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"	//2-2
#include "ABGASFountain.generated.h"//2-5

UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASFountain();

	//2-2
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

protected:
	//2-1 기존대로 액터에서 기능 구현
	UPROPERTY(VisibleAnywhere, Category=Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;
	
	UPROPERTY(EditAnywhere, Category=Timer)
	float ActionPeriod;

	FTimerHandle ActionTimer;

	//2-2 GAS를 이용한 기능 구현
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	//2-5 시작시 활성화할 GA들을 TArray로 관리, 블루프린트로 관리 
	UPROPERTY(EditAnywhere, Category=GAS)
 	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
};













