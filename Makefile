AutoMouseLock.exe : AutoMouseLock.obj Makefile
	link /nologo /ENTRY:main /NODEFAULTLIB /SUBSYSTEM:WINDOWS /MERGE:.text=.cyberml /MERGE:.rdata=.cyberml /MERGE:.data=.cyberml /SECTION:.cyberml,ERW AutoMouseLock.obj user32.lib kernel32.lib
#	ulink -Tpd AutoMouseLock.obj msvcrt.lib -e_DllMain@12 kernel32.lib user32.lib
AutoMouseLock.obj : AutoMouseLock.cpp
	cl /nologo /c /O1 /GS- AutoMouseLock.cpp

debug:
	cl -DDEBUG AutoMouseLock.cpp user32.lib

AutoMouseLock-manifest.exe : AutoMouseLock.exe AutoMouseLock.manifest
	cp -f AutoMouseLock.exe AutoMouseLock-tmp.exe
	mt -manifest AutoMouseLock.manifest -outputresource:AutoMouseLock-tmp.exe
	mv -f AutoMouseLock-tmp.exe AutoMouseLock-manifest.exe

AutoMouseLock-signed.exe : AutoMouseLock-manifest.exe
	cp -f AutoMouseLock-manifest.exe AutoMouseLock-tmp.exe
	signtool sign /n "Vladimir Panteleev" /d "AutoMouseLock" /du "https://github.com/CyberShadow/AutoMouseLock" /t http://time.certum.pl/ AutoMouseLock-tmp.exe
	mv -f AutoMouseLock-tmp.exe AutoMouseLock-signed.exe
