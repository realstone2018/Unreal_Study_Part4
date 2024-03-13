// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "ABGASCharacterPlayer.generated.h"

UCLASS()
class ARENABATTLEGAS_API AABGASCharacterPlayer : public AABCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASCharacterPlayer();

	//3-3
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//3-4 컨트롤러가 폰에 빙의될 때 호출
	virtual void PossessedBy(AController* NewController) override;

	//3-6
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//3-6
	void SetupGasInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);
	
protected:
	//3-3
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	//3-4
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	//3-8 입력에 대한 어빌리티들을 따로 관리, 모든 입력으로 확장할 수 있게 설계 변경
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
};


























