
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;
class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	public:
	
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	int32 GetPlayerLevel() const {return Level;};
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);

	int32 GetPlayerXP() const {return XP;};
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	int32 GetAttributePoints() const;
	void AddToAttributePoints(int32 InAttributePoints);
	void SetAttributePoints(int32 InAttributePoints);

	int32 GetSpellPoints() const;
	void AddToSpellPoints(int32 InSpellPoints);
	void SetSpellPoints(int32 InSpellPoints);

	protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_Level")
	int32 Level{1};

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_XP")
	int32 XP{0};

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_AttributePoints")
	int32 AttributePoints{0};

	UPROPERTY(VisibleAnywhere, ReplicatedUsing="OnRep_SpellPoints")
	int32 SpellPoints{0};

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldPoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldPoints);
	
};
