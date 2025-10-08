
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTags.h"
#include "InputActionValue.h"
#include "GenericTeamAgentInterface.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class UAuraInputConfig;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockHit, bool bCritHit);

	//Generic Team Agent Interface Functions
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;
	// //

	
protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
private:

	FGenericTeamId TeamId;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<class UInputAction> ShiftAction;

	bool bShiftKeydown{false};

	void ShiftPressed(const FInputActionValue& InputActionValue);
	void ShiftReleased(const FInputActionValue& InputActionValue);

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();

	FHitResult CursorHit;

	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination{FVector::ZeroVector};
	float FollowTime{0.f};
	float ShortPressThreshold{0.5f};
	bool bAutoRunning{false};
	bool bTargeting{false};
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius{50.f};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};


