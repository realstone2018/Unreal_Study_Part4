// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemBlueprintLibrary.h" //5-12
#include "GA/AT/ABAT_Trace.h"	//5-12
#include "GA/TA/ABTA_Trace.h"	//5-12
#include "ArenaBattleGAS.h"	//6-7
#include "Attribute/ABCharacterAttributeSet.h"
#include "Tag/ABGameplayTag.h" //7-5

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;	
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//5-11 OnTraceResultCallback()에서 종료되도록 이전
	// bool bReplicatedEndAbility = true;
	// bool bWasCancelled = false;
	// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

	//7-12
	CurrentLevel = TriggerEventData->EventMagnitude;
	
	//5-12
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, AABTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	//전달받은 타겟 데이터 핸들 처리 
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		//6-7 -> 7-1 GE 적용으로, 직접 Attribute에 접근하여 값을 변경할 필요 없어짐
		// UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		// UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		// if (!SourceASC || !TargetASC)
		// {
		// 	ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		// 	return;
		// }
		//
		// const UABCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABCharacterAttributeSet>();
		// UABCharacterAttributeSet* TargetAttribute = const_cast<UABCharacterAttributeSet*>(TargetASC->GetSet<UABCharacterAttributeSet>());
		// if(!SourceAttribute || !TargetAttribute)
		// {
		// 	ABGAS_LOG(LogABGAS, Error, TEXT("Attribute not found!"));
		// 	return;
		// }
		//
		// //매크로를 이용해 어트리뷰트셋의 값을 가져옴 (GetAttackRate함수를 따로 정의하지 않았음)
		// const float AttackDamage = SourceAttribute->GetAttackRate();
		// TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

		//7-1 -> 7-5
		// FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		// if (EffectSpecHandle.IsValid())
		// {
		// 	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		// }

		//7-5 GE의 Set by Caller 사용 (임시 지정임으로 세부 예외처리 생략)
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UABCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABCharacterAttributeSet>();

		//FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		//7-12
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}

		
	}
	
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}































