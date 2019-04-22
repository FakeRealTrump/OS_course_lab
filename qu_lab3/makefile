GCC := g++

CPPFLAGES := -W"switch" -W"no-deprecated-declarations" -W"empty-body" \
-W"conversion" -W"return-type" -W"parentheses" -W"no-pointer-sign" -W"no-format" \
-W"uninitialized" -W"unreachable-code" -W"unused-function" -W"unused-value" \
-W"unused-variable" -std=c++11 -Wall -fno-strict-aliasing -g2 -gdwarf-2 

CFLAGES :=  -O0  -fthreadsafe-statics \
-W"switch" -W"no-deprecated-declarations" -W"empty-body" -W"conversion" \
-W"return-type" -W"parentheses" -W"no-format" -W"uninitialized" -W"unreachable-code" \
-W"unused-function" -W"unused-value" -W"unused-variable" \
-frtti -fno-omit-frame-pointer -std=c11 -fexceptions 

LKFLAGES:= -Wl,-z,now -Wl,-z,relro -Wl,-z,noexecstack -Wl,--no-undefined -pthread


all: socketTest pipeTest shmTest
	

socketTest:sockettest.cpp
	g++ $(CPPFLAGES) sockettest.cpp -o "./socketTest.out" $(LKFLAGES)

pipeTest: pipetest.cpp
	g++ $(CPPFLAGES)  pipetest.cpp -o "./pipeTest.out" $(LKFLAGES)

shmTest: shmtest.cpp
	g++ $(CPPFLAGES) shmtest.cpp -o "./shmTest.out" $(LKFLAGES)