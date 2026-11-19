using UnrealBuildTool;

public class ViceDhaka : ModuleRules
{
    public ViceDhaka(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore"
        });
    }
}
