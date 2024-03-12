// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h" //2-2
#include "GameplayAbilitySpec.h"	//2-3
//#include "GA/ABGA_Rotate.h"	// 2-3 -> 2-5 ASC와 GA간의 의존성 제거, 태그로 대체
#include "Tag/ABGameplayTag.h"	//2-5
#include "Abilities/GameplayAbility.h"	//2-5

AABGASFountain::AABGASFountain()
{
	//2-1
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;

	//2-2
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//2-1
	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	//2-2 InnOwnerActor: ASC를 구동하고 데이터 처리 및 작업을 하는 액터, 데이터 처리를 하지 않지만 비주얼만 수행해주는 액터 
	ASC->InitAbilityActorInfo(this, this);

	//2-3 GA스펙을 생성해, ASC에게 부여 -> 2-5 ASC와 GA간의 의존성 제거, 태그로 대체
	// FGameplayAbilitySpec StartSpec(UABGA_Rotate::StaticClass());
	// ASC->GiveAbility(StartSpec);
	
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	//2-1
	// if (!RotatingMovement->IsActive())
	// {
	// 	RotatingMovement->Activate(true);
	// }
	// else
	// {
	// 	RotatingMovement->Deactivate();
	// }

	//2-2 -> 2-5 ASC와 GA간의 의존성 제거, 태그로 대체
	// FGameplayAbilitySpec* RotateGaSpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	// if (!RotateGaSpec)
	// {
	// 	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	// 	return;
	// }
	//
	// if (!RotateGaSpec->IsActive())
	// {
	// 	//Spec의 핸들을 이용해, GA 활성화
	// 	ASC->TryActivateAbility(RotateGaSpec->Handle);
	// }
	// else
	// {
	// 	ASC->CancelAbilityHandle(RotateGaSpec->Handle);
	// }
	
	//2-5
	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);
	
	if(!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		//Cancel시 &연산자 주의 
		ASC->CancelAbilities(&TargetTag);
	}
}



























