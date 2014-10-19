Network Simulation in C

I couldn't get the processes to work perfectly within one terminal. When
I started working on the assignment I thought we had to run all nodes
in their own terminal, so that we could see each node doing their own work.
You can try running 'make run' this will compile and run all of the nodes for you.
It can be a little buggy running it this way. RUnning the different nodes in their own terminals
will work better with my program.

THe netwrok protcol I designed is using and array with 4 characters,
that represent my packets.

Here is the break down of my packet:
[0] - The character that is to be sent (data)
[1] - Destination node number (2, 3, 4, 5, 6, 7)
[2] - Where in the message (S-Start , T-Middle, F-End)
[3] - Killswitch (T- ON, F- OFF)

Each parent node is the creater and destroyer of the link it shares with its child nodes.