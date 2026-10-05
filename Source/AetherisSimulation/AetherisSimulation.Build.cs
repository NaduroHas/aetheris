// Copyright © 2026 AETHERIS. All rights reserved.

using UnrealBuildTool;

public class AetherisSimulation : ModuleRules
{
	public AetherisSimulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AetherisCore"
		});
	
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
