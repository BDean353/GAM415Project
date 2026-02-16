// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

APortalActor::APortalActor()
{
    PrimaryActorTick.bCanEverTick = true;

    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    RootComponent = PortalMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("OverlapAll"));

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(RootComponent);
    SceneCapture->bCaptureEveryFrame = true;
    SceneCapture->bCaptureOnMovement = false;
}

void APortalActor::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &APortalActor::OnOverlapBegin
    );

    if (RenderTarget)
    {
        SceneCapture->TextureTarget = RenderTarget;
    }
}

void APortalActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateSceneCapture();
}

void APortalActor::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!LinkedPortal) return;

    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character) return;

    // Convert player transform into portal local space
    FTransform FromPortal = GetActorTransform();
    FTransform ToPortal = LinkedPortal->GetActorTransform();

    FTransform RelativeTransform =
        Character->GetActorTransform().GetRelativeTransform(FromPortal);

    // Rotate 180 degrees to face forward out of destination portal
    FRotator RotationOffset(0.f, 180.f, 0.f);
    RelativeTransform.ConcatenateRotation(RotationOffset.Quaternion());

    // Apply to destination portal
    FTransform NewWorldTransform =
        RelativeTransform * ToPortal;

    Character->SetActorTransform(NewWorldTransform);
}

void APortalActor::UpdateSceneCapture()
{
    if (!LinkedPortal) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;

    UCameraComponent* PlayerCamera =
        PlayerPawn->FindComponentByClass<UCameraComponent>();
    if (!PlayerCamera) return;

    // Camera relative to this portal
    FTransform CameraRelative =
        PlayerCamera->GetComponentTransform()
        .GetRelativeTransform(GetActorTransform());

    // Apply to linked portal
    FTransform CaptureTransform =
        CameraRelative * LinkedPortal->GetActorTransform();

    SceneCapture->SetWorldLocationAndRotation(
        CaptureTransform.GetLocation(),
        CaptureTransform.GetRotation()
    );
}
