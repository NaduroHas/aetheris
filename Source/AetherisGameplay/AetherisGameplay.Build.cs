// Copyright © 2026 AETHERIS. All rights reserved.

using UnrealBuildTool;

public class AetherisGameplay : ModuleRules
{
	public AetherisGameplay(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AetherisCore",
			"AetherisSimulation"
		});
	
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
