// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "ABGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASHpBarUserWidget : public UABGASUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();

	//8-13
	virtual void OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
protected:
	//meta = (BindWidget): 변수이름의(PbHpBar) 위젯과 자동으로 바인딩 된다. 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;
	
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;	//나누기 연산시 0을 나누는 경우가 없도록 0.1로 초기화

	//8-13
	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinsibleColor = FLinearColor::Blue;
};
