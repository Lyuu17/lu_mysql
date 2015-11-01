Module_objs = SQMain.o SQFuncs.o

CC = gcc
CFLAGS = -Wall -O2 -DNDEBUG -fPIC -I. -I../SDK/ -I./mysql/linux/include/

all: lu_mysql.so

default: all

lu_mysql.so: $(Module_objs)
	$(CC) -o ../bin/$@ $(Module_objs) -shared -s -L/usr/lib/mysql -lmysqlclient

SQMain.o: SQMain.c SQMain.h SQFuncs.h
	$(CC) $(CFLAGS) -c SQMain.c -o $@

SQFuncs.o: SQFuncs.c SQFuncs.h
	$(CC) $(CFLAGS) -c SQFuncs.c -o $@

clean:
	rm -f $(Module_objs) lu_mysql.so

