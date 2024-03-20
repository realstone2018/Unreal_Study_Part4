// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Chaos/GeometryParticlesfwd.h"

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//빈 Payload 이벤트 데이터 전달
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, WeaponEventTag, FGameplayEventData());	
}
