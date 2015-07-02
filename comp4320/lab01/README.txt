**README

Group: 
Drew Hoover
Davis Quarles
Kullen Williams

**GENERAL INSTRUCTIONS

You must be logged into 2 tux machines in the Davis Lab.
You can either ssh into 2 machine via terminal or SecureCRT.
Take note of the IP address for each machine.
Machine 1 = client machine
Machine 2 = server machine

**COMPILE INSTRUCTIONS

Navigate to the correct directory 
Machine 1 client directory
Machine 2 server directory
Then run the provided make file on both machines.

**RUN INSTRUCTIONS

On server machine (machine 2) run ftp_server by entering : ./ftp_server
(it will wait and listen to one of the assigned ports)

On the client machine (machine 1) run ftp_client by entering: 
./ftp_client d <damaged packets%> l <lost packets%> s <machine2's ip address>
