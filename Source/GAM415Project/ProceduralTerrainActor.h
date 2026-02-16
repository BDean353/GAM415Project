// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralTerrainActor.generated.h"

UCLASS()
class YOURPROJECT_API AProceduralTerrainActor : public AActor
{
    GENERATED_BODY()

public:
    AProceduralTerrainActor();

protected:
    virtual void BeginPlay() override;

public:
    // === Procedural Mesh ===
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Terrain")
    UProceduralMeshComponent* ProceduralMesh;

    // === Terrain Size ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int32 XSize = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    int32 YSize = 100;

    // === Noise Settings ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float NoiseScale = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
    float HeightMultiplier = 50.0f;

    // === Terrain Data ===
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    // === Functions ===
    void GenerateTerrain();
    void GenerateMesh();
    void ModifyTerrainAtLocation(const FVector& HitLocation, float Radius, float Strength);
};
