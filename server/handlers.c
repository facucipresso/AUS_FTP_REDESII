//hanlers.c
#include "responses.h"
#include "pi.h"
#include "dtp.h"
#include "session.h" //estas dos no estan definidas en ningun lado
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_USER(const char *args){
    ftp_session_t *sess = session_get();
    
    if(!args || strlen(args) == 0){
        safe_dprintf(sess->control_sock, MSG_501); //error de sintaxis en los parametros
        return;
    }

    strncpy(sess->current_user, args, sizeof(sess->current_user) -1);
    sess->current_user[sizeof(sess->current_user) -1] = '\0';
    safe_dprintf(sess->control_sock, MSG_331); //usuario correcto, se necesita la contraseña
}

void handle_PASS(const char *args){
    ftp_session_t *sess = session_get();
    if(sess->current_user[0] == '\0'){
        safe_sprintf(sess->control_sock, MSG_503); //mala secuencia de comandos
        return;
    }

    if(!args || strlen(args) == 0){
        safe_dprintf(sess->control_sock, MSG_501); //error de sintaxis en los parametros
        return;
    }

    if(check_credentials(sess->current_user, (char *)args) == 0){
        sess->logged_in = 1;
        safe_dprintf(sess->control_sock, MSG_230); //usuario logueado
    }else{
        safe_dprintf(sess->control_sock, MSG_530); // no logueado
        sess->current_user[0] = '\0'; //resetea usuario por login fallido
        sess->logged_in = 0;
    }
}

void handle_QUIT(const char *args){
    ftp_session_t *sess = session_get();
    (void)args; //unused

    safe_dprintf(sess->control_sock, MSG_221); // goodbye
    sess->current_user[0] = '\0'; // cierra sesion
    close_fd(sess->control_sock, "client socket"); //cierra el socket
    sess->control_sock = -1;
}

void handle_SYST(const char *args){
    ftp_session_t *sess = session_get();
    (void)args; //unused

    safe_dprintf(sess->control_sock, MSG_215); // system type
}

void handle_TYPE(const char *args){
    ftp_session_t *sess = session_get();
    (void)args;
    (void)sess;


    if (!args || strlen(args) != 1) {
    	safe_dprintf(sess->control_sock, MSG_501);
    	return;
    }

    if(args[0] == 'I'){
        safe_dprintf(sess->control_sock, "200 Modo Binario");
    }else if(args[0] == 'A' || args[0] == 'E' || args [0] == 'L'){
        safe_dprintf(sess->control_sock, MSG_504); // comando no implementado para ese parametro
    }else{
        safe_dprintf(sess->control_sock, MSG_501);//error de sintaxis
    }

}

void handle_PORT(const char *args){
    ftp_session_t *sess = session_get();
    (void)args;
    (void)sess;

    if(!sess->logged_in){
        safe_dprintf(sess->control_sock, MSG_530);//No esta loggeado el usuario
    }

    if(!args || strlen(args) == 0){
        safe_dprintf(sess->control_sock, MSG_501); //Error de sintaxis
    }

    int ip1, ip2, ip3, ip4, p1, p2;
    if(sscanf(args, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &p1, &p2) != 6){
        safe_dprintf(sess->control_sock, MSG_501);//Error de sintaxis
    }

    //pongo todos ceros en la estructura
    memset(&sess->data_addr, 0, sizeof(sess->data_addr));
    sess->data_addr.sin_family = AF_INET;
    sess->data_addr.sin_port = htons(p1 *256 + p2);

    char ip_str[INET_ADDRSTRLEN];
    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    if(inet_pton(AF_INET, ip_str, &sess->data_addr.sin_addr) <= 0){
        safe_dprintf(sess->control_sock, MSG_501);
        return;
    }
    safe_dprintf(sess->control_sock, MSG_200);
}

void handle_RETR(const char *args){
    ftp_session_t *sess = session_get();
    (void)args;
    (void)sess;

    //placeholder
}

void handle_STOR(const char *args){
    ftp_session_t *sess = session_get();
    (void)args;
    (void)sess;

    //placeholder
}

void handle_NOOP(const char *args){
    ftp_session_t *sess = session_get();
    (void)args;
    (void)sess;

    safe_dprintf(sess->control_sock, MSG_200); //mensaje sin operacion para no cortar la conexion

}

