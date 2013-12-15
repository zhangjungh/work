Environment:
CPU: Pentium Dual-Core T4200 2.0G
GPU: GS9300M
OS: Win7 Home x86
IDE: VC++ 2010 Express
DirectX: DXSDK_Jun10 9.29.1962 http://www.microsoft.com/en-us/download/details.aspx?id=6812

Dir&Files:
--Debug
	Debug build and the resources need by the application
	Please rename the file to Exe, it's not allowed to send exe file directly in zip
--PickingOutlineDemo
	The source code and VS2010 project
--PickingOutlineDemo.sln
	The solution file
--ReadMe.txt
	The file itself

Description:
the demo is designed to show outline of the object

objects include primitive object such as teapot and box and objects from x file
only use one x file model to demonstrate the function

object in game is renderred with per-pixing light effect (no time for shadows)
the terrain is just a flat plane, no sky, no background

you can use mouse to click the objects (terrain can't be chosen)
the selected object will turn red outline
if multi objects selected, only the closest will turn red (only allow one object to select)
if no object has been selected after click, then no object has red outline

there are some info on screen help u to manipulate the scene

please note the program set as multi-byte character set, not unicode

the demo is tested under above environment, please let me know if you meet any compiling, linking issue

Reference:
MSDN website
Introduction to 3D Game Programming with DirectX 9.0 framework
Google

Narrow Down the task:
1.Dx FrameWork -- Dx Init, Hardward capability, Msg Loop
2.Resource Load/Unload -- terrain, character model, texture and so on
3.Shader program Interface
4.Text and infos -- CPU, GPU, FPS and others
5.Dx Select Objects -- Dx Input handling
6.Edge detecting and rendering -- shader
7.Object Hierachy
8.Scene graph