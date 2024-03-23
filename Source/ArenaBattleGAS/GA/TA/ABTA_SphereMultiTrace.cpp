// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/TA/ABTA_SphereMultiTrace.h"
#include "ArenaBattleGAS.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute/ABCharacterSkillAttributeSet.h" //10-12

FGameplayAbilityTargetDataHandle AABTA_SphereMultiTrace::MakeTargetData() const
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	//10-12
	const UABCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UABCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("SkillAttribute not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	//충돌 후 결과를 Overlaps에 임시저장 -> TargetDataHandle에 저장
	TArray<FOverlapResult> Overlaps;

	//10-12
	//const float SkillRadius = 600.0f;
	const float SkillRadius = SkillAttribute->GetSkillRange();
	
	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, Character);	
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	//아래의 FGameplayAbilityTargetData_ActorArray가 TWeakObjectPtr형태임으로 변환해준다.
	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}
	
	//FGameplayAbilityTargetData_ActorArray: 다수의 액터를 저장하는 타겟 데이터 타입
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif
	
	return FGameplayAbilityTargetDataHandle(ActorsData);
}
