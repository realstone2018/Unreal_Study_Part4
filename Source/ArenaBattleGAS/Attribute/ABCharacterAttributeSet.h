// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ABCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ARENABATTLEGAS_API UABCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UABCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UABCharacterAttributeSet, Damage);	//7-8

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//7-8 Attribute Set 변경은 직접 접근이 아닌, 오직 GE를 통해서만 
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	
protected:
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	//7-1 Modifier에서 필드 접근이 가능하게 friend (데이터 클래스의 필드를 public으로 두는건 지양)
	friend class UABGE_AttackDamage;

	//7-8
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	//7-9
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;


};














