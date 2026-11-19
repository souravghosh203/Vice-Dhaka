using UnrealBuildTool;
using System.Collections.Generic;

public class ViceDhakaEditorTarget : TargetRules
{
    public ViceDhakaEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        ExtraModuleNames.Add("ViceDhaka");
    }
}
