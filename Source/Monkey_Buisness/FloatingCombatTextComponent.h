// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FloatingCombatTextComponent.generated.h"

class AFloatingTextActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MONKEY_BUISNESS_API UFloatingCombatTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFloatingCombatTextComponent();

private:
	// the floating combat text actor to use to display the text
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComponent", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFloatingTextActor> FloatingTextActorClass = nullptr;

	// The amount of vertical space between each damage number
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComponent", Meta = (AllowPrivateAccess = "true"))
	float TextVerticalSpacing = 40.0f;

	// The amount of vertical offset to apply when spawning a new text actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FloatingCombatTextComponent", Meta = (AllowPrivateAccess = "true"))
	float TextVerticalOffset = 100.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Adds the specified text at the specified location in world space
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "FloatingCombatTextComponent")
	void AddFloatingText_CLIENT(const FText& text, FVector world_location);

private:
	// Callback for when a floating text actor is destroyed
	UFUNCTION()
	void OnTextDestroyed(AActor* destroyed_actor);

	// An array of all active floating text actors
	UPROPERTY()
	TArray<AFloatingTextActor*> ActiveTextActors;
};
