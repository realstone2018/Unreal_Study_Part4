// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"	//6-6
#include "Attribute/ABCharacterAttributeSet.h" //6-6
#include "AbilitySystemBlueprintLibrary.h"	//6-6
#include "ArenaBattleGAS.h" //6-6

AABTA_Trace::AABTA_Trace()
{
}

void AABTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	//가장 먼저 멤버변수인 SourceActor를 초기화 해준다.
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AABTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		//TargetDataReadyDelegate 델리게이트를 이용해 타겟 데이터 핸들 브로드캐스팅
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	//6-6 어트리뷰트 셋 사용
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UABCharacterAttributeSet* AttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
	if (!AttributeSet)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ABCharacterAttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	FHitResult OutHitResult;
	
	//6-6
	// const float AttackRange = 100.0f;
	// const float AttackRadius = 50.f;
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start  + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		//FGameplayAbilityTargetData_SingleTargetHit: 시작지점과 끝지점을 넘기지 않아도 되도록 랩핑된 타겟 데이터 클래스
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		//TSharedPtr를 사용해서 추가해줌으로, 래퍼런스가 유지되는한 객체가 삭제되지 않음
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);		
	}
#endif

	return DataHandle;
}





















	