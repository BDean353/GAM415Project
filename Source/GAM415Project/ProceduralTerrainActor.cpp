// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralTerrainActor.h"

AProceduralTerrainActor::AProceduralTerrainActor()
{
    PrimaryActorTick.bCanEverTick = false;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;

    ProceduralMesh->bUseAsyncCooking = true;
}

void AProceduralTerrainActor::BeginPlay()
{
    Super::BeginPlay();
    GenerateTerrain();
}

void AProceduralTerrainActor::GenerateTerrain()
{
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
    Tangents.Empty();

    for (int32 Y = 0; Y <= YSize; Y++)
    {
        for (int32 X = 0; X <= XSize; X++)
        {
            float Height = FMath::PerlinNoise2D(
                FVector2D(X * NoiseScale, Y * NoiseScale)
            ) * HeightMultiplier;

            Vertices.Add(FVector(X * 100, Y * 100, Height));
            UVs.Add(FVector2D((float)X / XSize, (float)Y / YSize));
        }
    }

    for (int32 Y = 0; Y < YSize; Y++)
    {
        for (int32 X = 0; X < XSize; X++)
        {
            int32 Index = X + Y * (XSize + 1);

            Triangles.Add(Index);
            Triangles.Add(Index + XSize + 1);
            Triangles.Add(Index + 1);

            Triangles.Add(Index + 1);
            Triangles.Add(Index + XSize + 1);
            Triangles.Add(Index + XSize + 2);
        }
    }

    Normals.Init(FVector::UpVector, Vertices.Num());
    Tangents.Init(FProcMeshTangent(1, 0, 0), Vertices.Num());

    GenerateMesh();
}

void AProceduralTerrainActor::GenerateMesh()
{
    ProceduralMesh->CreateMeshSection(
        0,
        Vertices,
        Triangles,
        Normals,
        UVs,
        TArray<FColor>(),
        Tangents,
        true
    );
}

void AProceduralTerrainActor::ModifyTerrainAtLocation(
    const FVector& HitLocation,
    float Radius,
    float Strength)
{
    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        FVector WorldVertex = GetActorTransform().TransformPosition(Vertices[i]);
        float Distance = FVector::Dist(WorldVertex, HitLocation);

        if (Distance < Radius)
        {
            float Falloff = 1.0f - (Distance / Radius);
            Vertices[i].Z += Strength * Falloff;
        }
    }

    GenerateMesh();
}


