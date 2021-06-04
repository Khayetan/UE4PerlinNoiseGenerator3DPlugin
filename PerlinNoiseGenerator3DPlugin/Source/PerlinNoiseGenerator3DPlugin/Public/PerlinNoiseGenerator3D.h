#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PerlinNoiseGenerator3D.generated.h"

/**
 * 3D Perlin noise generator object.
 */
UCLASS(Blueprintable, BlueprintType)
class PERLINNOISEGENERATOR3DPLUGIN_API UPerlinNoiseGenerator3D : public UObject
{
	GENERATED_BODY()

public:

	UPerlinNoiseGenerator3D();

	/** Octaved 3D Perlin noise. For 2D just ignore the Z axis. */
	UFUNCTION(BlueprintPure, Category = "Noise")
	float Perlin3D(FVector Input, int32 Octaves, float Persistence);
	/** Set the seed used by the generator. */
	UFUNCTION(BlueprintCallable, Category = "Noise")
	void SetSeed(int32 NewSeed);
	/** 
	 * RepeatLength is the number of cells at which the noise starts to loop. 
	 * The default is 256.
	 */
	UFUNCTION(BlueprintCallable, Category = "Noise")
	void SetRepeatLength(int32 NewRepeatLength);

private:

	// The number of cells at which the noise starts to loop.
	int32 RepeatLength = 256;
	int32 Seed = 0;
	// Value lookup table for random generation.
	TArray<int32> P;
	// Smoothes the noise.
	float Fade(float T);
	int32 Inc(int32 Num);
	// Calculates a gradient value based on the hash.
	float Grad(int32 Hash, float X, float Y, float Z);
	float Lerp(float A, float B, float X);
	// Generates the lookup table.
	void GenerateHashTable(int32 Seed, int32 NoiseSize);
	// 3D Perlin noise.
	float GradientNoise(FVector Input);
};
