// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	//AT 기능 함수 GetAvatarActor() - GA의 아바타 액터를 반환
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	//4-6
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	
	Character->Jump();

	//점프가 언제 종료될지 모르니, Wait
	SetWaitingOnAvatar();
}

void UABAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	//4-6
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Super::OnDestroy(AbilityEnded);
}

void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	//AT의 델리게이트 브로드캐스팅 여부를 소유한 GA에게 알려줄지 말지의 옵션 
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();		
	}
}
