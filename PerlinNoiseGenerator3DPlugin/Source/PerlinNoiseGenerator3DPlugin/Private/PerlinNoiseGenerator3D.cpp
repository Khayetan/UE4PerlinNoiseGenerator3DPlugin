// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinNoiseGenerator3D.h"


UPerlinNoiseGenerator3D::UPerlinNoiseGenerator3D()
{
	GenerateHashTable(Seed, RepeatLength);
}

void UPerlinNoiseGenerator3D::SetSeed(int32 NewSeed)
{
	Seed = NewSeed;
	GenerateHashTable(Seed, RepeatLength);
}

void UPerlinNoiseGenerator3D::SetRepeatLength(int32 NewRepeatLength)
{
	RepeatLength = NewRepeatLength;
	GenerateHashTable(Seed, RepeatLength);
}

// Used to smooth output.
float UPerlinNoiseGenerator3D::Fade(float T)
{
	// 6t^5 - 15t^4 + 10t^3
	return T * T * T * (T * (T * 6 - 15) + 10);
}

int32 UPerlinNoiseGenerator3D::Inc(int32 Num)
{
	Num++;
	if (Num > RepeatLength - 1)
	{
		Num = 0;
	}
	return Num;
}

float UPerlinNoiseGenerator3D::Grad(int32 Hash, float X, float Y, float Z)
{
	switch (Hash & 0xF)
	{
	case 0x0: return  X + Y;
	case 0x1: return -X + Y;
	case 0x2: return  X - Y;
	case 0x3: return -X - Y;
	case 0x4: return  X + Z;
	case 0x5: return -X + Z;
	case 0x6: return  X - Z;
	case 0x7: return -X - Z;
	case 0x8: return  Y + Z;
	case 0x9: return -Y + Z;
	case 0xA: return  Y - Z;
	case 0xB: return -Y - Z;
	case 0xC: return  Y + X;
	case 0xD: return -Y + Z;
	case 0xE: return  Y - X;
	case 0xF: return -Y - Z;
	default: return 0;
	}
}

float UPerlinNoiseGenerator3D::Lerp(float A, float B, float X)
{
	return A + X * (B - A);
}

void UPerlinNoiseGenerator3D::GenerateHashTable(int32 InputSeed, int32 InputRepeatLength)
{
	Seed = InputSeed;
	FRandomStream PGen = FRandomStream(this->Seed);
	RepeatLength = InputRepeatLength;
	P.Init(0, RepeatLength * 2);
	for (int i = 0; i < RepeatLength * 2; i++)
	{
		P[i] = PGen.RandRange(0, RepeatLength - 1);
	}
}

float UPerlinNoiseGenerator3D::GradientNoise(FVector Input)
{
	// Input world coords. Negative values are translated to positive ones.
	float X = Input.X < 0.0f ? RepeatLength + (Input.X - ((int32)(Input.X / RepeatLength) * RepeatLength)) : Input.X;
	float Y = Input.Y < 0.0f ? RepeatLength + (Input.Y - ((int32)(Input.Y / RepeatLength) * RepeatLength)) : Input.Y;
	float Z = Input.Z < 0.0f ? RepeatLength + (Input.Z - ((int32)(Input.Z / RepeatLength) * RepeatLength)) : Input.Z;

	// Cube coords. Mod to prevent breaching repeat point.
	int32 CX = (int32)X % RepeatLength;
	int32 CY = (int32)Y % RepeatLength;
	int32 CZ = (int32)Z % RepeatLength;

	// Inner coords.
	float IX = X - (int32)X;
	float IY = Y - (int32)Y;
	float IZ = Z - (int32)Z;

	// Faded inner coords.
	float U, V, W;
	U = Fade(IX);
	V = Fade(IY);
	W = Fade(IZ);

	// Select a value from the hash table for each vert of the unit cube.
	// Doing it this way ensures that the values are the same for neighbouring cubes.
	int32 AAA, ABA, AAB, ABB, BAA, BBA, BAB, BBB;
	AAA = P[P[P[CX] + CY] + CZ];
	ABA = P[P[P[CX] + Inc(CY)] + CZ];
	AAB = P[P[P[CX] + CY] + Inc(CZ)];
	ABB = P[P[P[CX] + Inc(CY)] + Inc(CZ)];
	BAA = P[P[P[Inc(CX)] + CY] + CZ];
	BBA = P[P[P[Inc(CX)] + Inc(CY)] + CZ];
	BAB = P[P[P[Inc(CX)] + CY] + Inc(CZ)];
	BBB = P[P[P[Inc(CX)] + Inc(CY)] + Inc(CZ)];

	// Calculate the final noise value.
	float X1, X2, Y1, Y2;
	X1 = Lerp(Grad(AAA, IX, IY, IZ), Grad(BAA, IX - 1, IY, IZ), U);
	X2 = Lerp(Grad(ABA, IX, IY - 1, IZ), Grad(BBA, IX - 1, IY - 1, IZ), U);
	Y1 = Lerp(X1, X2, V);

	X1 = Lerp(Grad(AAB, IX, IY, IZ - 1), Grad(BAB, IX - 1, IY, IZ - 1), U);
	X2 = Lerp(Grad(ABB, IX, IY - 1, IZ - 1), Grad(BBB, IX - 1, IY - 1, IZ - 1), U);
	Y2 = Lerp(X1, X2, V);

	return (Lerp(Y1, Y2, W) + 1) / 2;
}

float UPerlinNoiseGenerator3D::Perlin3D(FVector Input, int32 Octaves, float Persistence)
{
	float Total = 0.0f;
	float Frequency = 1.0f;
	float Amplitude = 1.0f;
	// Used for normalizing result to 0.0 - 1.0.
	float MaxValue = 0.0f;

	for (int i = 0; i < Octaves; i++) {
		Total += GradientNoise(Input * Frequency) * Amplitude;
		MaxValue += Amplitude;
		Amplitude *= Persistence;
		Frequency *= 2;
	}

	return Total / MaxValue;
}

