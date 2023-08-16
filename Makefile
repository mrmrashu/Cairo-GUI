# Compiler
COMP = gcc

# Source file
APP = cross-p

# Output executable names
EXEC_WIN = cp
EXEC_LINUX = cp

# Default target (Linux)
default: linux

# Windows target
win:
	${COMP} ${APP}.c -o ${EXEC_WIN}.exe -I/mingw64/include/cairo -L/mingw64/lib -lcairo -lgdi32

# Linux target
linux:
	${COMP} ${APP}.c -o ${EXEC_LINUX} -I/usr/include/cairo -lcairo -lX11

# Clean target to remove executables
clear:
	rm -f ${EXEC_WIN}.exe ${EXEC_LINUX}