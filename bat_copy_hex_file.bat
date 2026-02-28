@echo off
:: 获取当前文件夹的名称
set current_dir=%cd%
for %%i in ("%current_dir%") do set folder_name=%%~nxi

:: 输出当前文件夹名称
echo Current folder name: %folder_name%

:: 设置 MDK-ARM 文件夹路径
set mdk_dir=%current_dir%\MDK-ARM\%folder_name%

:: 检查 MDK-ARM 中的文件夹是否存在
if not exist "%mdk_dir%" (
    echo Folder "%mdk_dir%" does not exist. Exiting...
    exit /b
)

:: 检查 .hex 文件是否存在
set hex_file=%mdk_dir%\*.hex
if not exist "%hex_file%" (
    echo No .hex files found in "%mdk_dir%". Exiting...
    exit /b
)

:: 设置目标文件夹路径（假设目标目录是当前目录）
set target_dir=%current_dir%

:: 直接复制唯一的 .hex 文件
echo Copying %hex_file% to %target_dir%
copy /y "%hex_file%" "%target_dir%\hex_file.hex"

:: 确认复制并重命名
if exist "%target_dir%\hex_file.hex" (
    echo File copied and renamed to hex_file.hex
) else (
    echo File copy failed.
)

echo File copy and rename completed.

pause
exit