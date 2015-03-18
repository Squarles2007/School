README

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
NEW Kernel : 
	Linux 3.15.8
	from boot options select: CentOS Linux (3.15.8) 7 (Core)

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 


* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Kernal Installation:
	The Kernel installation process was very straight foward.
	We didn't encounter any error in the installation process.
Installation:
	See instructions below.
	
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Running - Using Provide MakeFile
	'#': a prefix that suggest a command to run
	For this project we had to run each program 5 times with 
	different data sizes. Running our make file will run all
	three programs 5 times for the current data size, and store
	each programs timing data into a text file named 
	PROGRAMNAME_timing.txt. 
NEED TO KNOW:
	To change data size open each program and comment out your current data size and uncomment your desired data size. Make sure you move the timing text files out of your current working directory before you run the makefile again. IT WILL OVERWRITE THEM!!!!
To Run:
	In terminal make sure that you are in the correct directory.
	to change directory 
		# cd FILEPATH 
	(FILEPATH = the path you want) in the correct directory 
		# make
	the make file will compile and run each program. Then store
	their time data into their text files

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Running - Without MakeFile
	'#': a prefix that suggest a command to run
	Same Need To Know applies from MakeFile Section.

	Compile:
	gcc -o run_mp mp.c
	gcc -o run_mt mt.c -pthread
	gcc -o run_serial  serial.c

	Run Instructions: //store the time data in timing file.
	# /usr/bin/time -o mp_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_mp; \
	# /usr/bin/time -o mt_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_mt; \
	# /usr/bin/time -o serial_timings.txt -a -f 'Total Execution Time: %E\nTotal User Time: %U\n\n' ./run_serial; \

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *




* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Installation Instructions:
	'#': a prefix that suggest a command to run
	
	-- Prepare the packages for installation as root
	# yum groupinstall "Development Tools"
	# yum install ncurses-devel
	# yum install hmaccalc zlib-devel binutils-devel elfutils-libelf-devel

	-- Prepare the source code 
	# tar zxf linux-3.15.8.tar.gz
	# cd linux-3.15.8

	-- Configure the kernel
	# make menuconfig
	-- Do not need change anything. Use left arrow key and right arrow key to control the cursor.
	-- Choose "save", then choose "exit".
	# make oldconfig

	-- Compile the kernel
	# make all

	-- In case of compilation problems, you can try one of the configuration
	-- files from the system. They are located under /boot/. For example, you can try
	-- the following commands to re-configure and re-compile.

	# cp /boot/config-3.10.0-123.13.2.el7.x86_64 .config
	# make oldconfig
	# make all

	-- Become the root 
	# su -

	-- Install the kernel modules
	# cd linux-3.15.8 
	# make modules_install 

	--Install the kernel
	# make install

	-- Reboot and load the system with the new kernel 
	-- Usually the new kernel has become the default kernel after rebooting.
	-- You can follow the following steps to check.

	-- Determine the current version of the kernel 
	# uname -r 

	-- List all available kernels in your system
	# egrep ^menuentry /etc/grub2.cfg | cut -f 2 -d \'

	-- Config your default kernel 
	-- NAME refers to one of the entries you get from previous step
	-- # grub2-set-default "NAME" For example, on my machine, I enabled:
	# grub2-set-default "CentOS Linux (3.15.8) 7 (Core)"

	-- Verify your configuration
	# vim /boot/grub2/grubenv 

	-- Reboot the system
	# reboot

	-- Confirm the version of the new kernel
	# uname -r

	-- Check the number of CPU/cores on the system.
	# cat /proc/cpuinfo

	-- Running the same kernel in a mode that supports only one CPU.
	-- Open the file /boot/grub2/grub.cfg 

	-- Append this option 'maxcpus=1' to the 'linux16' line in grub.cfg. 
	-- For example, if the linux16 line reads as follows:
	        linux16 /vmlinuz-3.15.8 root=/dev/mapper/centos-root ro rd.lvm.lv=centos/swap vconsole.font=latarcyrheb-sun16 rd.lvm.lv=centos/root crashkernel=auto  vconsole.keymap=us rhgb quiet LANG=en_US.UTF-8 

	-- then, it should be modified to be
	        linux16 /vmlinuz-3.15.8 root=/dev/mapper/centos-root ro rd.lvm.lv=centos/swap vconsole.font=latarcyrheb-sun16 rd.lvm.lv=centos/root crashkernel=auto  vconsole.keymap=us rhgb quiet LANG=en_US.UTF-8 maxcpus=1

	-- Reboot the system and confirm the kernel version
	# reboot
	# uname -r

	-- Confirm the number of CPUs. You should see only one CPU entry using this command: 
	# cat /proc/cpuifo
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *