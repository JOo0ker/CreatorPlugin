@echo off

set "source_file=E:\LL\Github\CreatorPlugin\Selection\Selection_v1.0\Selection\Debug\Selection.dll"
set "target_folder=E:\Program Files\Creator\Creator_4_2\config\plugins\Sim-support"
set "app_path=E:\Program Files\Creator\Creator_4_2\bin\mgfltx.exe"

rem 创建目标文件夹（如果不存在）
if not exist "%target_folder%" mkdir "%target_folder%"

rem 复制文件到目标文件夹
xcopy /y "%source_file%" "%target_folder%"

rem 打开应用程序
start "" "%app_path%"
