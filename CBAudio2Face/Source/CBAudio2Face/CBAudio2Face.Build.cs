// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class CBAudio2Face : ModuleRules
{
	public CBAudio2Face(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CBHTTP",
				"CBCommon",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"Json",
				"HTTP",
				"Projects"
			}
			);
		
		string ExtrasDir = Path.Combine(PluginDirectory, "Extras");
		foreach (string DataFile in Directory.EnumerateFiles(ExtrasDir, "*", SearchOption.AllDirectories))
		{
			RuntimeDependencies.Add(DataFile);
		}
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
