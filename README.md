# ga2017-final
Alvin Leung's Final Project for RPI Game Architecture 2017.

Implementation of Snapshot system from Quake's network model.

To run the server:
./ga.exe server 9000

To run the client:
./ga.exe client \<port number\>

On the client, you should be able to move the cube with the I J K L keys. The client will then send the key command to the server which will move the cube and send the differences in snapshots back to the client. The client then sends an ACK back to the server when it receies the difference.
