// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_Trace.generated.h"

//5-8 TA의 TargetDataReadyDelegate로 받은 타겟 데이터 핸들을 GA에게 전달할 수 있도록 파라미터 추가  
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTraceResultDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

UCLASS()
class ARENABATTLEGAS_API UABAT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UABAT_Trace();

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="WaitForTrace",HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;
	
protected:
	//5-8 TA의 TargetDataReadyDelegate 콜백
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

protected:
	//5-9
	//인자로 받은 생성할 TA 클래스 정보 저장 
	UPROPERTY()
	TSubclassOf<class AABTA_Trace> TargetActorClass;

	//TargetActorClass를 사용해 생성한 인스턴스 저장
	UPROPERTY()
	TObjectPtr<class AABTA_Trace> SpawnedTargetActor;

public:
	//5-10
	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();
};



























