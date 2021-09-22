// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCombatTextComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "FloatingTextActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFloatingCombatTextComponent::UFloatingCombatTextComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFloatingCombatTextComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFloatingCombatTextComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner<APawn>()->IsLocallyControlled() == false)
		return;

	// Get the viewport size
	const FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	// Check for bad viewport size
	if (viewportSize.X <= 0.0f || viewportSize.Y <= 0.0f)
		return;

	// Get the viewport scale (the DPI scale)
	const int32 viewportX = FGenericPlatformMath::FloorToInt(viewportSize.X);
	const int32 viewportY = FGenericPlatformMath::FloorToInt(viewportSize.Y);
	const float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportX, viewportY));

	// Check for bad scale
	if (viewportScale <= 0.0f)
		return;
	
	// Get the owning player's camera location
	const FVector cameraLocation = GetOwner<APawn>()->GetController<APlayerController>()->PlayerCameraManager->GetCameraLocation();


	// Adjust the vertical location of the active text actors by the viewport size and scale so that they appear evenly stacked on the screen
	for (int32 i = 1; i < ActiveTextActors.Num(); ++i)
	{
		// Calculate the vertical offset based on how far we (the camera) are from the floating text actor
		AFloatingTextActor* textActor = ActiveTextActors[i];
		const float distance = FVector::Dist(cameraLocation, textActor->GetAnchorLocation());
		const float verticalOffset = distance / (viewportSize.X / viewportScale);

		// Adjusting the floating text actor's location by the calculated amount
		//textActor->SetActorLocation(textActor->GetAnchorLocation() + FVector(0.0f, 0.0f, i * verticalOffset * TextVerticalSpacing));
	}
}

void UFloatingCombatTextComponent::AddFloatingText_CLIENT_Implementation(const FText& text, FVector world_location)
{
	if (FloatingTextActorClass == nullptr)
		return;

	// Spawn the new text actor at the specified location
	const FTransform spawnTransform = FTransform(FRotator::ZeroRotator, world_location + FVector(0.0f, 0.0f, TextVerticalOffset));
	AFloatingTextActor* newTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FloatingTextActorClass, spawnTransform, GetOwner());
	if (newTextActor == nullptr)
		return;

	// Initialize and finish spawning the actor
	newTextActor->Initialize(text);
	newTextActor->OnDestroyed.AddDynamic(this, &UFloatingCombatTextComponent::OnTextDestroyed);
	UGameplayStatics::FinishSpawningActor(newTextActor, spawnTransform);

	// Add the new text actor to the beginning of the ActiveTextActors array 
	ActiveTextActors.Insert(newTextActor, 0);
}

void UFloatingCombatTextComponent::OnTextDestroyed(AActor* destroyed_actor)
{
	ActiveTextActors.Pop();
}

