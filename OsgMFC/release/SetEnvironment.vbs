Dim oWshShell, sOldPath, sNewPath ,csMyPath
Set oWshShell = WScript.CreateObject("WScript.Shell") 
sOldPath=oWshShell.RegRead("HKLM\SYSTEM\ControlSet001\Control\Session Manager\Environment\path") 

if( right(sOldPath, 1) <> ";" ) Then
 sOldPath = sOldPath & ";"
end if

csMyPath = left(Wscript.ScriptFullName,len(Wscript.ScriptFullName)-len(Wscript.ScriptName)) & "bin;"

sNewPath=sOldPath+csMyPath

oWshShell.RegWrite "HKLM\SYSTEM\ControlSet001\Control\Session Manager\Environment\path", sNewPath, "REG_SZ" 

msgbox ( csMyPath & " 已经成功添加到了环境变量Path。")