#include "pi.h"
#include "server.h"
#include "responses.h"
//#include "utils.h" esto no se de donde goma sale y me tira error

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <strings.h>

static ftp_command_t ftp_commands[] = {
    {"USER", handle_USER},
    {"PASS", handle_PASS},
    {"QUIT", handle_QUIT},
    {"SYST", handle_SYST},
    {"TYPE", handle_TYPE},
    {"PORT", handle_PORT},
    {"RETR", handle_RETR},
    {"STOR", handle_STOR},
    {"NOOP", handle_NOOP},
    {NULL, NULL}
};

//no definio la estructura ftp_session_t solo hizo la ftp_command_t
int welcome(ftp_session_t *sess){
    //mando mensaje FTP bienvenida
    if(safe_dprintf(sess->control_sock, MSG_220) != sizeof(MSG_220) -1){ //safe_dprintf no se de donde sale
        fprintf(stderr, "Send error\n");
        close_fd(sess->control_sock, "cliente socket"); //no se donde esta definida close_fd
        return -1;
    }
    return 0;
}

int get_exe_command(ftp_session_t *sess){
    char buffer[BUFSIZE];

    //recive string del command chanel
    ssize_t len = recv(sess->control_sock, buffer, sizeof(buffer) -1, 0);
    if(len < 0){
        perror("Receive fail: ");
        close_fd(sess->control_sock, "cliente socket");
        return -1;
    }

    //la conexion se cerro inapropiadamente y nosotros la cerramos bien
    if(len == 0){
        sess->current_user[0] = '\0'; //cierro sesion
        close_fd(sess->control_sock, "client socket");
        sess->control_sock = -1;
        return -1;
    }

    buffer[len] = '\0';

    //strip CRLF
    char *cr = strchr(buffer, '\r');
    if(cr) *cr = '\0';
    char *lf = strchr(buffer, '\n');
    if(lf) *lf = '\0';

    //separo comando y argumento
    char *arg = NULL;
    char *cmd = buffer;

    //caso de comando nulo
    if(cmd[0] == '\0'){
        safe_dprintf(sess->control_sock, "500 Empty command. \r\n");
        return 0;
    }

    char *space = strchr(buffer, ' ');
    if(space){
        *space = '\0';
        arg = space + 1;
        while(*arg == ' ') arg++;
    }

    ftp_command_t *entry = ftp_commands;
    while(entry->name){
        if(strcasecmp(entry->name, cmd) == 0){
            entry->handler(arg ? arg : "");
            return (sess->control_sock < 0) ? -1 : 0;
        }
        entry++;
    }

    safe_dprintf(sess->control_sock, "502 Command not implemented.\r\n");
    return 0;

}


// --- DE ACA PARA ABAJO ES TODO EL CODIGO QUE EL FUE REEMPLAZANDO ---
/*
int is_valid_command(const char *command) {
    int i = 0;
    while (valid_commands[i] != NULL) {
        if (strcmp(command, valid_commands[i]) == 0) {
            return arg_commands[i];
        }
        i++;
    }
    return -1;
}
*/

/* 
 * Función recv_cmd recibe un comando del cliente y lo separa en operación y parámetro.
 *
 * @param socket_descriptor: Descriptor del socket desde el cual se recibe el comando.
 * @param operation: Cadena donde se almacenará la operación del comando 
 *                   (comandos básicos de ftp, por ejemplo: "USER", "PASS", "LIST", etc.).
 * @param param: Cadena donde se almacenará el parámetro del comando, si existe.
 * @return 0 si se recibió correctamente, 1 si hubo un error.
 */

/*
int recv_cmd(int socket_descriptor, char *operation, char *param) {
    char buffer[BUFSIZE];
    char * token;
    int args_number;

    if (recv(socket_descriptor, buffer, BUFSIZE, 0) < 0) {
        fprintf(stderr, "Error: no se pudo recibir el comando.\n");
        return 1;
    }
    buffer[strcspn(buffer, "\r\n")] = 0;
    token = strtok(buffer, " ");
    if (token == NULL || strlen(token) < 3 || (args_number = is_valid_command(token)) < 0) {
        fprintf(stderr, "Error: comando no válido.\n");
        return 1;
    }
    strcpy(operation, token);
    if (!args_number)
        return 0;
    token = strtok(NULL, " ");
    #if DEBUG 
    printf("par %s\n", token);
    #endif
    if (token != NULL) 
        strcpy(param, token);
    else {
        fprintf(stderr, "Error: se esperaba un argumento para el comando %s.\n", operation);
        return 1;
    }
    return 0;
}
*/