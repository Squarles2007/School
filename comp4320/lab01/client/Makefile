#make file for ftp_client.cpp
# compiles only
#
# after compile run command : ./ftp_client  d <damage %> 1 <loss %> s <server ipaddress>

ftp: ftp_client.cpp 
	g++ -o ftp_client ftp_client.cpp

info:
$(info         )
$(info ***************************************)
$(info WHEN RUNNING: )
$(info Use one of the following syntax)
$(info ./ftp_client l <lost packets> d <damaged packets> s <ip address>)
$(info OR ./ftp_client d <damaged packets> s <ip address>)
$(info OR ./ftp_client l <lost packets> s <ip address> )
$(info OR ./ftp_client s <ip address> )
$(info )
$(info FOR HELP ENTER ./ftp_client h)
$(info *************************************** )
$(info )


clean:
	rm -rf *.o
