## Little Instruction

***This is the <u>simplest</u> socket program for green-hand learning***

**No multithread    No synchronize    No multi-client**

Code fetched from [https://msdn.microsoft.com](https://msdn.microsoft.com/en-us/library/windows/desktop/ms737593(v=vs.85).aspx) and reformed to fit in concrete project

### function

```c++
void InitWinSock(); //Initialize windows socket
void InitCheckListenSock(); //Initialize a listening socket and double check whether it works or not
void BindSock(); //bind the listening socket created to aimed address or port
void ListenSock(); //Let listening socket listen
void AcceptSock(); //Create a ClientSocket and accept the listening socket
void PrepareSock(); //include all functions up here

void Receive(); //receive information from client
void Send(std::string sendStr); //send content you want and it MUST be of type std::string
void RecvAndSend(); //include functions up here

void ShutDown(); //shutdown connection and free resourses
bool GetSessionState(); //iResult>0 loop send and recv, iResult=0 closing connection, iResult<0 error
std::string GetRecvStr(); //transfer content of type c_str(char*) to what of type std::string
```
### How to use it?

*step1* `new` a `SocketServer` object and it possesses all variables and functions

*step2*  call for `Prepare`function and it will finish preparation so that you don't need to call every single function on your own

*step3* In a `do-while`loop, `Receive` and `Send` functions work.In addition, you need to tell `Send` what content to deal with and it is required to be type `std::string`

*step4* After send content connection should be shutdown.

