#Compiler and Flags
CC = gcc
CFLAGS = -std=c99 -pthread -lm -g3 -pedantic -Wall -O2

#Sources
SRCS = *.c

#Headers
HEADERS = 
ifdef *.h
	*.h
endif

#Objects
OBJS = *.o

#Documentation
DOCS = *.pdf

#Programs
PROG = mutexSum

#Compressed File
TAR = cs.tar.bz2

#####################################################
# BUILD and TAR
# ###################################################

prog: ${OBJS} ${HEADERS}
	${CC} ${CFLAGS} ${OBJS} -o ${PROG}

${OBJS}: ${SRCS}
	${CC} ${CFLAGS} -c ${@:.o=.c}

tar:
	tar cvjf ${TAR} ${SRCS} ${HEADERS} ${DOCS} makefile

###################
#CLEAN
###################

clean:
	rm -f ${PROG} *.o *~
