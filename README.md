## Little Instruction

***This is the <u>simplest</u> socket program for green-hand learning***

**No multithread    No synchronize    No multi-client**

Code fetched from [https://msdn.microsoft.com](https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591(v=vs.85).aspx) and reformed to fit in concrete project

### function

```c++
void PrepareSock();
void InitWinSock();
void InitCheckListenSock();
void BindSock();
void ListenSock();
void AcceptSock();
void RecvAndSend();
void Receive();
void ClearRecvBuf();
void Send(std::string sendStr);
void ShutDown();
bool GetSessionState();
std::string GetRecvStr()
```

### How to use it?

*step1* `new` a `SocketServer` object and it possesses all variables and functions

*step2*  call for `Prepare`function and it will finish preparation so that you don't need to call every single function on your own

*step3* Call for `Send`function to send content you want

*step4* Receive content from server and free resources

