using UnrealBuildTool;
using System.Collections.Generic;

public class ViceDhakaTarget : TargetRules
{
    public ViceDhakaTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        ExtraModuleNames.Add("ViceDhaka");
    }
}
