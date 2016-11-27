## Little Instruction

***A socket program for green-hand learning***

*support multi-thread*    *support multi-client*    *not support synchronization*

There are two arrays - we can call pools - `threads` and  `cSock`.One stores `thread`, one stores `Client SOCKET`, and elements in them are corresponding.Were there a `SOCKET` taking over a `client`, a `thread` would manage session between the `client` and `server` itself.

A `SOCKET POOL` is necessary for situation: "*Client A* wants to talk to *Client B*, the only way is through *Server S* ,so *S* must find which socket B is".In the way mentioned up there, *S* gets *B's* No. from *A*, then notifies *B*.