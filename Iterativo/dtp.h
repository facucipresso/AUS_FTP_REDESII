#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "session.h"

#define PWDFILE "/etc/aus_ftp/ftpusers" //esto lo saco

int check_credentials(char *user, char *pass);
int send_file(ftp_session_t *sess, const char *nombre_archivo);
int receive_file(ftp_session_t *sess, const char *nombre_archivo);
