
#include<stdio.h>
#include<winsock2.h>
#include <ws2tcpip.h>

#define DADOS 512 
#define PORT 8888   

int main()
{
    
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server, si_other;
    int slen = sizeof(si_other);  
    int iResult;               
    
    char buf[DADOS];

	printf("-----------SERVIDOR---------------\n");
    printf("Iniciando Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("WSAStartup falhou com o erro: %d\n",WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("(inicializado com sucesso!)\n");

    char hostname[NI_MAXHOST];        
    gethostname(hostname,NI_MAXHOST); 
    printf("Nome da maquina: %s\n",hostname);

    
    PHOSTENT phe = gethostbyname(hostname);     
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Endereco IP (%d): %s\n",i, inet_ntoa(addr));        
        
    }
     
     
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Nao pode criar o socket. Erro: %d\n", WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("Socket criado.\n");
     
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
     
     
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind falhou com o codigo : %d\n" , WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("Bind funcionando.\n");
 
 
 
    while(1)
    {
        printf("--------------------------------\n");
        printf("Aguardando as mensagens dos clientes...\n");
         
         
        memset(buf,'\0', DADOS);
         
         
        if ((iResult = recvfrom(s, buf, DADOS, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
         
        printf("Pacote recebido de %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Dados recebidos: %s\n", buf);
         
         
        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() falhou com o erro : %d" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
    }
 
 
    printf("-----------------------\n");
    printf("Fechando socket...\n");
    system("pause");
    closesocket(s);
    WSACleanup();
     
    return 0;
}
