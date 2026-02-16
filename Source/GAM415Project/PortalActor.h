// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PortalActor.generated.h"

UCLASS()
class YOURPROJECT_API APortalActor : public AActor
{
    GENERATED_BODY()

public:
    APortalActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // === Components ===
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* PortalMesh;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere)
    USceneCaptureComponent2D* SceneCapture;

    // === Portal Linking ===
    UPROPERTY(EditAnywhere, Category = "Portal")
    APortalActor* LinkedPortal;

    // === Render Target ===
    UPROPERTY(EditAnywhere, Category = "Portal")
    UTextureRenderTarget2D* RenderTarget;

protected:
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void UpdateSceneCapture();
};
