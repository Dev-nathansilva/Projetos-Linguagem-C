#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#define SERVER "127.0.0.1"  
#define DADOS 512          
#define PORT 8888           

int main(void)
{
    WSADATA wsa;

    SOCKET s;
    
    struct sockaddr_in si_other;
    int slen=sizeof(si_other); 
    char buf[DADOS];
    char message[DADOS];

	printf("-------------CLIENTE---------------\n");
    printf("Inicializando o Winsock...");
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

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("Nao pode criar o socket. Erro: %d" , WSAGetLastError());
        system("pause"); exit(EXIT_FAILURE);
    }
    printf("Socket criado.\n");
     
     
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER); 
    
    
    while(1)
    {
        printf("----------------------------------\n");
        printf("Digite uma mensagem : ");
        gets(message);
         
       
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
         
        memset(buf,'\0', DADOS);

        if (recvfrom(s, buf, DADOS, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() falhou com o erro : %d\n" , WSAGetLastError());
            system("pause"); exit(EXIT_FAILURE);
        }
        
        printf("mensagem enviada com sucesso\n");
    }
    
    
    printf("---------------------------\n");
    printf("Fechando socket.\n");
    closesocket(s);
    WSACleanup();
    system("pause");
 
    return 0;
}
