// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "AbilitySystemComponent.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass)
{
	//5-9 TargetActorClass 파라미터 추가
	// UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);
	// return NewTask;
	
	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	//5-10
	SpawnAndInitializeTargetActor();
	//5-11
	FinalizeTargetActor();

	//지연? - 어떤 목적인지, 없어도 되는건지 확인 필요
	SetWaitingOnAvatar();

	// 5-8: 개발 도중에는 테스트가 가능하도록, 임시로 시작 즉시 종료 처리
	//		타겟 데이터 핸들 흐름이 완성되었으니 OnTargetDataReadyCallback콜백으로 옮겨준다. 
	// if (ShouldBroadcastAbilityTaskDelegates())
	// {
	// 	OnComplete.Broadcast(DataHandle );
	// }
	//
	// EndTask();
}

void UABAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	//Super가 마지막에 오는점 주의
	Super::OnDestroy(AbilityEnded);
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}

void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	//TA 지연 생성, TargetDataReadyDelegate 구독
	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);
	}
}

void UABAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		//ASC의 TA관리용 TArray에 등록
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();	// -> ABTA_Trace의 ConfirmTargetingAndContinue()가 호출됨  -> 델리게이트 콜백으로 ABAT_Trace의 SpawnAndInitializeTargetActor가 호출된다. 
	}
}





















