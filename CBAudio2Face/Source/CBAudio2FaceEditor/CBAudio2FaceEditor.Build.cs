using UnrealBuildTool;

public class CBAudio2FaceEditor : ModuleRules
{
    public CBAudio2FaceEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "MainFrame",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "CBAudio2Face",
                "UnrealEd",
            }
        );
    }
}