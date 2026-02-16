#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GAM415ProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS(config = Game)
class AGAM415ProjectProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// -------- MESH ----------
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	// -------- MATERIAL ----------
	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	UMaterialInterface* BaseMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	FLinearColor ProjectileColor;

	// -------- DECALS ----------
	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	TArray<UMaterialInterface*> SplatDecalMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	FVector DecalSize = FVector(20, 20, 20);

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	float DecalLife = 10.f;

	// -------- PARTICLES ----------
	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	UNiagaraSystem* SplatParticles;

public:
	AGAM415ProjectProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
