obj-m += one.o one_1.o

default:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

install:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules_install
		
