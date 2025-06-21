#include "config.h"
#include "arguments.h"
//#include "server.h" lo comento para hacer pruebas
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>





int main(int argc, char *argv[]){
    struct arguments args; //ver de donde saco esa libreria 'arguments'

    if(parse_arguments(argc, argv, &args) != 0){
        return EXIT_FAILURE;
    }

    printf("Start server on %s:%d\n", args.address, args.port); // es para debuggear

    int master_socket = server_init(args.address, args.port);

    if(master_socket < 0){
        return EXIT_FAILURE;
    }

    //setup_signals

    while(1){
        //struct sockaddr_in slave_addr; esto prueba de no usarlo en la llamda server_accept
        int slave_socket = server_accept(master_socket);

        if(slave_socket < 0){
            continue;
        }

        server_loop(slave_socket);

    }


/*
    int port; //esto lo saco el profe pero me tira error en el htons(port) por ahora

    int master_socket, slave_socket;
    struct sockaddr_in master_addr, slave_addr;
    socklen_t slave_addr_len;
    char user_name[BUFSIZE];
    char user_pass[BUFSIZE];
    char buffer[BUFSIZE];
    char command[BUFSIZE];
    int data_len;

    master_socket = socket(AF_INET, SOCK_STREAM, 0);
    master_addr.sin_family = AF_INET;
    master_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    master_addr.sin_port = htons(port);
    bind(master_socket, (struct sockaddr *)&master_addr, sizeof(master_addr)) < 0;
    listen(master_socket, 5);
    while (1) {
        slave_addr_len = sizeof(slave_addr);
        slave_socket = accept(master_socket, (struct sockaddr *)&slave_addr, &slave_addr_len);
        printf("%s\n",MSG_220);
        if (send(slave_socket, MSG_220, sizeof(MSG_220) - 1, 0) != sizeof(MSG_220) - 1) {
            close(slave_socket);
            fprintf(stderr, "Error: no se pudo enviar el mensaje.\n");
            break;
        }

        if (recv_cmd(slave_socket, command, user_name) != 0) {
            close(slave_socket);
            fprintf(stderr, "Error: no se pudo recibir el comando USER.\n");
            break;
        }
        if (strcmp(command, "USER") != 0) {
            close(slave_socket);
            fprintf(stderr, "Error: se esperaba el comando USER.\n");
            continue;
        }
        data_len = snprintf(buffer, BUFSIZE, MSG_331, user_name);
        if (send(slave_socket, buffer, data_len, 0) < 0) {
            close(slave_socket);
            fprintf(stderr, "Error: no se pudo enviar el mensaje MSG_331.\n");
            break;
        }
        
        if (recv_cmd(slave_socket, command, user_pass) != 0) {
            close(slave_socket);
            fprintf(stderr, "Error: no se pudo recibir el comando PASS.\n");
            break;
        }
        if (strcmp(command, "PASS") != 0) {
            close(slave_socket);
            fprintf(stderr, "Error: se esperaba el comando PASS.\n");
            continue;
        }
        if (!check_credentials(user_name, user_pass)) {
            data_len = snprintf(buffer, BUFSIZE, MSG_530);
            if (send(slave_socket, buffer, data_len, 0) < 0) {
                close(slave_socket);
                fprintf(stderr, "Error: no se pudo enviar el mensaje MSG_530.\n");
                break;
            }
            close(slave_socket);
            continue;
        }
        data_len = snprintf(buffer, BUFSIZE, MSG_230, user_name);
        if (send(slave_socket, buffer, data_len, 0) < 0) {
            close(slave_socket);
            fprintf(stderr, "Error: no se pudo enviar el mensaje MSG_230.\n");
            break;
        }

        while (1) {
            if (recv_cmd(slave_socket, command, buffer) != 0) {
                close(slave_socket);
                fprintf(stderr, "Error: no se pudo recibir el comando %s.\n", command);
                break;
            }
            if (strcmp(command, "QUIT") == 0) {
                if (send(slave_socket, MSG_221, sizeof(MSG_221) - 1, 0) < 0) {
                    close(slave_socket);
                    fprintf(stderr, "Error: no se pudo enviar el mensaje MSG_221.\n");
                    break;
                }
                close(slave_socket);
                break;
            }
            if (strcmp(command, "SYST") == 0) {
                if (send(slave_socket, MSG_215, sizeof(MSG_215) - 1, 0) < 0) {
                    close(slave_socket);
                    fprintf(stderr, "Error: no se pudo enviar el mensaje SYST.\n");
                    break;
                }
                continue;
            }
            if (strcmp(command, "FEAT") == 0) {
                if (send(slave_socket, MSG_502, sizeof(MSG_502) - 1, 0) < 0) {
                    close(slave_socket);
                    fprintf(stderr, "Error: no se pudo enviar el mensaje FEAT.\n");
                    break;
                }
                continue;
            }
            // Aquí se pueden manejar otros comandos FTP si es necesario.
        }

    }
    close(master_socket);
*/
    return 0;
}