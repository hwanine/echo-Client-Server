# TCP echo Server/Client Program

# 개요

## 1. 개요

**TCP 에코 서버/클라이언트 프로그램**은 클라이언트가 보낸 데이터를 서버 쪽에서 받아들여, 클라이언트에게 그대로 다시 보내주는 프로그램이며, 전송방식은 TCP 방식으로 동작합니다.

또한 본 프로그램은 한 번에 하나의 클라이언트와 통신이 가능하도록 구현되었습니다.

<br>

## 2. 개발 환경

-	**OS**: Windows 10 home
-	**IDE**: Visual Studio 2019
-	**Language**: C 
-	**Library**: WS2_32.lib (windows socket)

<br>

## 3. 검증 환경

-	**OS**: Windows 10 home
-	**Build**: MinGW GCC Compiler, 
Visual Studio 2019
-	**Action**: Windows Dos
-	**Test Address**: 127.0.0.1
-	**Test Port**: 8090
-	**Test Server Max**: 2
-	**Test Client Max**: 5

<br>

# 동작

![image](https://user-images.githubusercontent.com/57826388/82476402-f1620d00-9b08-11ea-84b8-3c29a06bddaa.png)

<br>

## 1. 빌드

우선 C언어로 작성된 소스코드를 Windows에서 컴파일, 링크과정을 거쳐 실행파일을 생성해야 합니다. 여기서는 두 가지 방법을 소개합니다.

###	**Visual Studio 프로젝트 빌드**

![image](https://user-images.githubusercontent.com/57826388/82476528-3423e500-9b09-11ea-86ce-b41a1a633256.png)
![image](https://user-images.githubusercontent.com/57826388/82476534-35eda880-9b09-11ea-9a8d-c5405b841465.png)

<br>

Client 프로젝트와 Server 프로젝트를 각각 생성하여 소스코드를 넣은 뒤, 상단의 빌드 탭을 통해 빌드를 진행합니다.

<br>

### **GCC 빌드**

동작 환경이 Windows이기 때문에 MinGW를 설치하여**GCC** 컴파일러가 실행 가능한 환경을 만듭니다. 이후 소스파일이 있는 경로에서 다음과 같이 GCC 컴파일을 합니다.  
‘-lis2_32’를 통해 Windows 소켓 라이브러리를 링크합니다.

![image](https://user-images.githubusercontent.com/57826388/82476696-6af9fb00-9b09-11ea-8259-427033870ed5.png)

<br>

## 2. 프로그램 실행

실행파일이 생성된 경로에서 Server와 Client를 각각 실행합니다

![image](https://user-images.githubusercontent.com/57826388/82476820-8bc25080-9b09-11ea-8b18-31b20fccef5a.png)

<br>

만약 같은 포트를 가진 서버 두개를 열게 되면 어느 한쪽에서 바인드 에러가 발생하게 됩니다.

![image](https://user-images.githubusercontent.com/57826388/82476828-8e24aa80-9b09-11ea-8c1d-d0d210742b14.png)

<br>

## 3. 소켓 생성 및 연결

먼저 서버를 실행하여 포트번호를 입력하면 소켓을 생성됩니다. 그리고 소켓과 주소, 포트번호를 **바인딩**한 후, **listen()** 함수를 통해 클라이언트 연결 호출 요청을 대기합니다. 

클라이언트는 접속 주소와 서버 포트를 입력하여 소켓을 생성한 뒤, 서버 소켓에 연결요청을 합니다.

![image](https://user-images.githubusercontent.com/57826388/82476932-b0b6c380-9b09-11ea-8a35-5e8a761c8bd0.png)

<br>

올바르지 못한 주소와 포트번호로 접속을 시도할 경우, 다음의 에러를 출력합니다.

![image](https://user-images.githubusercontent.com/57826388/82476943-b2808700-9b09-11ea-8ac1-40c1f346c376.png)

<br>

## 4. 통신

서버/클라이언트가 서로 연결이 된 후, 클라이언트에서 메시지를 입력하면 **send()** 함수를 통해 메시지를 서버로 전송합니다.

 서버는 **recv()** 함수로 메시지를 받고 클라이언트에게 메시지를 전송합니다. 그리고 클라이언트는 서버로부터 전달받은 메시지를 응답하여 출력합니다.

통신 문자열 버퍼 크기를 **1024**로 지정해서 이 범위 내의 값만 송수신이 가능합니다.

![image](https://user-images.githubusercontent.com/57826388/82477062-db088100-9b09-11ea-9e9f-9b8b7ee65e20.png)

<br>

이 과정에서 또 다른 클라이언트가 서버에 연결하게 되면 한 번에 하나의 클라이언트와 통신이 가능하기 때문에 그 클라이언트는 현재 통신중인 연결이 종료될 때까지 대기하게 됩니다.  

이전에 통신 중이던 클라이언트가 종료하게 되면 대기중인 클라이언트와 바로 통신이 진행됩니다.

![image](https://user-images.githubusercontent.com/57826388/82477130-f83d4f80-9b09-11ea-9753-8cc5fc938472.png)

<br>

## 5. 종료 및 대기

클라이언트가 **‘Q’**를 입력하면 종료하게 되고 그와 동시에 서버는 종료할지, 다른 클라이언트의 접속을 대기할지 선택하게 됩니다.  

이 부분은 별도의 쓰레드로 동작하기 때문에 선택을 하는 도중에도 다른 클라이언트가 접속한다면 서버와 클라이언트의 통신은 계속됩니다.

서버에서 **‘S’**를 입력하면 서버가 종료됩니다.

![image](https://user-images.githubusercontent.com/57826388/82477235-2884ee00-9b0a-11ea-954c-a80e67db535c.png)

<br>
 
반면 아무것도 입력하지않거나 이외의 값을 입력하게 되면 다른 클라이언트의 접속을 기다리고 접속 요청이 올 경우, 이에 응답하여 연결됩니다.

![image](https://user-images.githubusercontent.com/57826388/82477356-55d19c00-9b0a-11ea-9b43-8ec927062724.png)
![image](https://user-images.githubusercontent.com/57826388/82477362-579b5f80-9b0a-11ea-8355-52ceee7476c2.png)

<br>

클라이언트와 통신 도중에 서버가 종료를 요청할 경우, 비정상적인 종료를 방지합니다.

![image](https://user-images.githubusercontent.com/57826388/82477266-376ba080-9b0a-11ea-8a34-1e569ea9ab41.png)

<br>

# 3. TCP vs UDP

만약 UDP 기반 에코 서버/클라이언트 프로그램을 작성한다고 했을 시, TCP 방식의 프로그램과 다른 몇 가지 차이점이 있습니다.

TCP 방식과 구별되는 UDP 방식의 프로그램만의 특징은 다음과 같습니다.

<br>

> 1. 연결 불필요

UDP 서버, 클라이언트는 TCP와 달리 사전 연결 설정 과정이 불필요합니다. 따라서 TCP와 같이 연결된 상태로 데이터를 송수신 하지 않습니다. 단, 연결을 아예 안하는 것이 아니라 send(), recv() 함수를 호출하는 시점에만 소켓에 연결할 하고, 함수가 종료되면 연결을 종료하는 것을 반복합니다.
따라서 UDP 기반에서는 TCP 에코 서버 구현 과정에서 거쳤던 listen() 함수와 accept() 함수의 호출은 불필요합니다. 오로지 UDP 소켓 생성과 주소 할당, 데이터의 송수신 과정만 존재합니다.
즉, UDP에서 connect() 함수는 단순히 소켓에 호스트의 IP와 임의의 Port를 할당하는 일만 담당하지만 TCP에서는 호스트의 IP와 Port를 할당하고 서버 연결 요청까지 한다는 차이가 있습니다.

<br>

> 2. 소켓간 관계

TCP에서는 소켓과 소켓의 관계가 1:1 입니다. 하지만 UDP는 서버와 클라이언트는 하나의 소켓만 존재하면 됩니다. 그렇기 때문에 UDP 소켓은 하나만 존재하여도 1:1은 물론, 1:M 데이터 송수신이 가능합니다.

<br>

> 3. 주소정보 전송

UDP 소켓은 연결상태를 유지하지 않는 대신, 데이터를 전송할 때 마다 목적지 주소정보를 추가로 사용합니다. TCP의 경우에는 연결이 된 상태에서 데이터를 송수신하기 때문에 주소 정보가 필요 없습니다.
