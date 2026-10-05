// Copyright © 2026 AETHERIS. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAetherisCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace Aetheris
{
	// Deterministic, stable ID helpers. IDs must not depend on UObject pointers.
	uint64 GenerateSeedId(const FString& SeedName);
	uint64 GetEntityId(const FString& EntityType, int32 Index);

	// Logging helpers.
	void LogCore(const FString& Message);
	void LogWarning(const FString& Message);
	void LogError(const FString& Message);
}
