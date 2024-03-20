// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include "ArenaBattleGAS.h"
#include "ShaderCompiler.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/ABGameplayTag.h" //8-13

void UABGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		//특정 GA에 대한 이벤트만 분리해서 가져옴
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnMaxHealthChanged );

		//8-13
		//해당 태그가 생성, 제거될 때 델리게이트 브로드캐스트
		ASC->RegisterGameplayTagEvent(ABTAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UABGASHpBarUserWidget::OnInvinsibleTagChanged);
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		
		const UABCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();	
			}
		}

	}
}

void UABGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}

void UABGASHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}
