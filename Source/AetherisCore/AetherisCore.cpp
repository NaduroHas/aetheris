// Copyright © 2026 AETHERIS. All rights reserved.

#include "AetherisCore.h"

DEFINE_LOG_CATEGORY_STATIC(LogAetherisCore, Log, All);

void FAetherisCoreModule::StartupModule()
{
}

void FAetherisCoreModule::ShutdownModule()
{
}

namespace Aetheris
{
	static uint64 HashString(const FString& Str)
	{
		// FNV-1a over UTF-32 code points. This is deterministic across runs/platforms.
		uint64 Hash = 14695981039346656037ull;
		for (TCHAR Character : Str)
		{
			Hash ^= static_cast<uint64>(Character);
			Hash *= 1099511628211ull;
		}
		return Hash;
	}

	uint64 GenerateSeedId(const FString& SeedName)
	{
		return HashString(SeedName);
	}

	uint64 GetEntityId(const FString& EntityType, int32 Index)
	{
		return HashString(FString::Printf(TEXT("%s_%d"), *EntityType, Index));
	}

	void LogCore(const FString& Message)
	{
		UE_LOG(LogAetherisCore, Log, TEXT("%s"), *Message);
	}

	void LogWarning(const FString& Message)
	{
		UE_LOG(LogAetherisCore, Warning, TEXT("%s"), *Message);
	}

	void LogError(const FString& Message)
	{
		UE_LOG(LogAetherisCore, Error, TEXT("%s"), *Message);
	}
}

IMPLEMENT_MODULE(FAetherisCoreModule, AetherisCore)
