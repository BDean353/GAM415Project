#include "GAM415ProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

AGAM415ProjectProjectile::AGAM415ProjectProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAM415ProjectProjectile::OnHit);
	RootComponent = CollisionComp;

	// ---------- PROJECTILE MESH ----------
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ---------- MOVEMENT ----------
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AGAM415ProjectProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material + random color
	if (BaseMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		ProjectileMesh->SetMaterial(0, DynamicMaterial);

		ProjectileColor = FLinearColor::MakeRandomColor();
		DynamicMaterial->SetVectorParameterValue("Color", ProjectileColor);
	}
}

void AGAM415ProjectProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	// ---------- DECAL ----------
	if (SplatDecalMaterials.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, SplatDecalMaterials.Num() - 1);

		UMaterialInstanceDynamic* DecalMID =
			UMaterialInstanceDynamic::Create(SplatDecalMaterials[Index], this);

		DecalMID->SetVectorParameterValue("Color", ProjectileColor);

		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalMID,
			DecalSize,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			DecalLife
		);
	}

	// ---------- PARTICLES ----------
	if (SplatParticles)
	{
		auto NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SplatParticles,
			Hit.ImpactPoint,
			FRotator::ZeroRotator
		);

		if (NiagaraComp)
		{
			NiagaraComp->SetVariableLinearColor("User.Color", ProjectileColor);
		}
	}

	// ---------- PHYSICS ----------
	if (OtherActor && OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
	}

	Destroy();
}
