# UDP_Flooding
UDP flooding using CPP (Socket Programming)

소켓 프로그래밍을 사용한 UDP Flooding 공격

1. C&C Server (ip: 114.70.37.17)의 10004 포트로 IMHACKER_2019111313 메세지를 UDP로 전송한다.
2. C&C Server로부터 공격 대상지 IP와 포트번호를 받고, 공격지에 UDP Flooding 공격을 수행한다. 이때 보내는 메세지는 2019111313을 10번 보낸다.
3. 이를 단독실행하는 exe 파일로 변경한 뒤, UPX를 사용한 패킹을 통해 난독화를 진행
