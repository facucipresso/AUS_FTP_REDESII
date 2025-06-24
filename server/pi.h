#pragma once

//#include "responses.h" esto lo saco
#include <stddef.h>
#include "session.h"



/* todo esto lo mando a responses.h, no le voy a poner todos los mensajes que le puso el.
#define MSG_220 "220 srvFtp version " VERSION "\r\n"
#define MSG_331 "331 Password required for %s\r\n"
#define MSG_230 "230 User %s logged in\r\n"
#define MSG_530 "530 Login incorrect\r\n"
#define MSG_221 "221 Goodbye\r\n"
#define MSG_550 "550 %s: no such file or directory\r\n"
#define MSG_299 "299 File %s size %ld bytes\r\n"
#define MSG_226 "226 Transfer complete\r\n"
#define MSG_502 "502 Command not implemented\r\n"
#define MSG_215 "215 UNIX Type: L8\r\n"
*/

//int recv_cmd(int, char *, char *);


//sigo sin tener definida la estructura ftp_session_t
int welcome(ftp_session_t *sess);
int get_exe_command(ftp_session_t *sess);

typedef struct{
  const char *name;
  void (*handler) (const char *args);
} ftp_command_t;

void handle_USER(const char *args);
void handle_PASS(const char *args);
void handle_QUIT(const char *args);
void handle_SYST(const char *args);
void handle_TYPE(const char *args);
void handle_PORT(const char *args);
void handle_RETR(const char *args);
void handle_STOR(const char *args);
void handle_NOOP(const char *args);
