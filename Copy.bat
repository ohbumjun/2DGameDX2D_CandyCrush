cd..
cd..
xcopy .\GameEngine\Bin\*.* .\Client2D\Bin\ /s /d /y 
xcopy .\GameEngine\Bin\*.* .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.* .\Editor\Bin\ /s /d /y 

xcopy .\GameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.inl .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.hpp .\Engine\Include\ /s /d /y