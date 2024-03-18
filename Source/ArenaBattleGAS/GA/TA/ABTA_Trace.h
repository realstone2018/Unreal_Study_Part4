// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ABTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AABTA_Trace();

	//타겟팅 시작
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	//타겟을 확정하고 남은 프로세스 진행
	virtual void ConfirmTargetingAndContinue() override;
	//디버깅, 타겟 판정 범위 시각화
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	//타겟데이터 핸들 반환
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
