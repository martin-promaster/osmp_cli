JDK8_INC=/usr/java/jdk1.8.0_301-amd64/include
JDK8_INC_LINUX=/usr/java/jdk1.8.0_301-amd64/include/linux
USER_INC=-I./include
PRJ_INC=-I${JDK8_INC} -I${JDK8_INC_LINUX} ${USER_INC}
PRJ_LIBS=-L. -lpmscore -lcurl
LIB_DIR=./lib
BIN_DIR=./bin
SRC = src/pmscore.c src/linked_list.c src/xsql.c src/xhttp.c src/cJSON.c src/cJSON_Utils.c
OSMP_INC=-I./include -I/usr/include/curl
OSMP_LIB=-L. -L/usr/lib64/mysql -lm -lcurl -lmysqlclient -lpmscore
CC=gcc

.c.o:
	gcc ${PROJ_INC} -c $<

all: osmp
	rm -rf ${LIB_DIR}/*
	mv *.so ${LIB_DIR}
	mv osmp ${BIN_DIR}
	
osmp: libpmsinvoker.so
	${CC} -g ${OSMP_INC} ${OSMP_LIB} -o osmp src/osmp.c

libpmsinvoker.so: libpmscore.so
	${CC} -fPIC -shared ${PRJ_INC} ${PRJ_LIBS} -o libpmsinvoker.so src/pmsinvoker.c

libpmscore.so: ${SRC}
	${CC} -fPIC -shared -L/usr/lib64/mysql -lmysqlclient -o libpmscore.so $^

.PHONY: clean

clean:
	rm -rf lib*.so
	rm -rf osmp
