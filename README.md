# IATTracer

프로세스에 DLL 모듈 형태로 인젝션되어, 지정된 Win32 API를 통해 호출되는 파라미터를 출력하는 프로그램입니다.


현재 일반적으로 가장 많이 사용되는 File I/O 관련 함수의 Hook이 탑재되어 있습니다.
- LoadLibraryA, LoadLibraryW
- CreateFileA, CreateFileW
- MessageBoxA, MessageBoxW
