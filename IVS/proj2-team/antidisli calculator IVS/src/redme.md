REQUIRE CMAKE https://cmake.org/download/

qt required !!! download qt extension in VS

downlod qt:

https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4

https://drive.google.com/drive/folders/1IbOUeXZAHsjcT-JnTik4lljWfC4s2MWS?fbclid=IwAR3cL8ahfCxhdRPpevSoamcK1HHbcinlPNRtUmcAxSG5Wj2jjuHM_KV7c_8

download last version... vs2017 recomended

my fake login: kukikuk147258369@seznam.cz aaaaaaaaaaA1

create build folder cmd cmake-gui .. recomended VS 2019 x64

set path for qt.. something like: C:\Qt\5.14.2\msvc2015_64\lib\cmake\Qt5

buld\Debug needs:
Qt5Guid.dll 	<- C:\Qt\5.14.2\msvc2015_64\bin\Qt5Guid.dll
Qt5Widgetsd.dll <- C:\Qt\5.14.2\msvc2015_64\bin\Qt5Widgetsd.dll
Qt5Cored.dll 	<- C:\Qt\5.14.2\msvc2015_64\bin\Qt5Cored.dll
platforms\qwindowsd.dll <- C:\Qt\5.14.2\msvc2015_64\plugins\platforms\qwindowsd.dll

for instalator add to buld\DebugstDir:  
build\Debug\vcruntime140d.dll
build\Debug\msvcp140d.dll 
build\Debug\vcruntime140_1d.dll 
build\Debug\ucrtbased.dll
(can be found in system32)
