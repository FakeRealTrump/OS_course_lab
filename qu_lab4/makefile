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

objects := main.o

all: $(objects)
	g++ $(LKFLAGES) $(objects) -o "./qu_lab4"

$(objects): %.o: %.cpp
	$(GCC) -c $(CPPFLAGES) $< -o $@