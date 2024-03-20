// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASItemBox.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/ABCollision.h"
#include "AbilitySystemBlueprintLibrary.h" //9-6

AABGASItemBox::AABGASItemBox()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AABGASItemBox::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASItemBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);
	
	//이벤트 중복 발생 방지
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	//2초 뒤 상자 액터 제거
	SetLifeSpan(2.0f);
}

void AABGASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ASC->InitAbilityActorInfo(this, this);

}

void AABGASItemBox::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			//타겟의 ASC에서 실행함으로 ToSelf
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AABGASItemBox::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;	//타겟으로부터 발동된 것이라 Instigator 설정
	Param.Location = GetActorLocation();

	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}

