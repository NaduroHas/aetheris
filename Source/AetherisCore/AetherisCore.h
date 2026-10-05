// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAetherisCoreModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// AETHERIS Core API — exported functions for ID generation, logging, time
namespace Aetheris
{
	// Generate a deterministic UUID from a string seed (64-bit hash-based)
	uint64 GenerateSeedId(const FString& SeedName);

	// Generate a random UUID (64-bit)
	uint64 GenerateRandomId();

	// Get deterministic ID for an entity type + index
	uint64 GetEntityId(const FString& EntityType, int32 Index);

	// Simulation time tracking
	double GetSimulationTime();
	int64 GetSimulationFrame();

	// Logging helpers
	void LogCore(const FString& Message);
	void LogWarning(const FString& Message);
	void LogError(const FString& Message);
}
