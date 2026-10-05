// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisCore.h"
#include "Misc/DateTime.h"
#include "HAL/IConsoleManager.h"

static FName CoreLogCategory(TEXT("Aetheris.Core"));

void FAetherisCoreModule::StartupModule()
{
	// Initialize static simulation state
	Aetheris::SimulationTime = 0.0;
	Aetheris::SimulationFrame = 0;
}

void FAetherisCoreModule::ShutdownModule()
{
	// Nothing to clean up
}

namespace Aetheris
{
	// Static state
	double SimulationTime = 0.0;
	int64 SimulationFrame = 0;

	// Simple hash function (FNV-1a variant)
	static uint64 HashString(const FString& Str)
	{
		uint64 Hash = 14695981039346656037ull;
		for (char32 c : Str)
		{
			Hash ^= static_cast<uint64>(c);
			Hash *= 1099511628211ull;
		}
		return Hash;
	}

	uint64 GenerateSeedId(const FString& SeedName)
	{
		return HashString(SeedName);
	}

	uint64 GenerateRandomId()
	{
		return FMath::RandRange(0, MAX_uint64);
	}

	uint64 GetEntityId(const FString& EntityType, int32 Index)
	{
		FString Composite = FString::Printf(TEXT("%s_%d"), *EntityType, Index);
		return HashString(Composite);
	}

	double GetSimulationTime()
	{
		return SimulationTime;
	}

	int64 GetSimulationFrame()
	{
		return SimulationFrame;
	}

	void LogCore(const FString& Message)
	{
		UE_LOG(CoreLogCategory, Log, TEXT("%s"), *Message);
	}

	void LogWarning(const FString& Message)
	{
		UE_LOG(CoreLogCategory, Warning, TEXT("%s"), *Message);
	}

	void LogError(const FString& Message)
	{
		UE_LOG(CoreLogCategory, Error, TEXT("%s"), *Message);
	}
}

IMPLEMENT_MODULE(AetherisCore, AetherisCore)
