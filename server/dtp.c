#define _GNU_SOURCE
#include "dtp.h"




int check_credentials(char *user, char *pass) {
    FILE *file;
    char *path = PWDFILE, *line = NULL, cred[100];
    size_t len = 0;
    int found = -1;

    // make the credential string
    sprintf(cred, "%s:%s", user, pass);

    // check if it is present in any ftpusers line
    file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo de usuarios.\n");
        return -1;
    }
    while (getline(&line, &len, file) != -1) {
        strtok(line, "\n");
        if (strcmp(line, cred) == 0) {
            found = 0;
            break;
        }
    }
    fclose(file);
    if (line) free(line);
    return found;
}

int send_file(ftp_session_t *sess, const char *nombre_archivo) {
    //abro en modo binario
    FILE *fp = fopen(nombre_archivo, "rb");
    if (!fp) {
      perror("fopen");
      return -1;
    }
    
    //creacion del sockete
    int data_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (data_sock < 0) {
      perror("socket");
      fclose(fp);
      return -1;
    }

    //creo la conexion con el cliente el modo activo
    if (connect(data_sock, (struct sockaddr *)&sess->data_addr, sizeof(sess->data_addr)) < 0) {
      perror("connect");
      close(data_sock);
      fclose(fp);
      return -1;
    }

    char buffer[1024];
    size_t bytes_read;
    ssize_t bytes_written;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
      bytes_written = write(data_sock, buffer, bytes_read);
      if (bytes_written < 0) {
        perror("write");
        close(data_sock);
        fclose(fp);
        return -1;
      } else if ((size_t)bytes_written != bytes_read) {
        fprintf(stderr, "Error: no se escribieron todos los bytes\n");
        close(data_sock);
        fclose(fp);
        return -1;
      }
    }

    if (ferror(fp)) {
      fprintf(stderr, "Error: en la lectura del archivo\n");
      close(data_sock);
      fclose(fp);
      return -1;
    }

    fprintf(stderr,"Archivo enviado: %s\n", nombre_archivo);

    close(data_sock);
    fclose(fp);
    return 0;
}

int receive_file(ftp_session_t *sess, const char *nombre_archivo) {
  //Abro archivo en modo de escritura binaria
  FILE *fp = fopen(nombre_archivo, "wb");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  int data_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (data_sock < 0) {
    perror("socket");
    fclose(fp);
    return -1;
  }

  if (connect(data_sock, (struct sockaddr *)&sess->data_addr, sizeof(sess->data_addr)) < 0) {
    perror("connect");
    
    close(data_sock);
    fclose(fp);
    return -1;
  }
  
  char buffer[1024];
  ssize_t bytes;
  while ((bytes = read(data_sock, buffer, sizeof(buffer))) > 0) {
    if (fwrite(buffer, 1, bytes, fp) != (size_t)bytes) {
      perror("fwrite");
      fclose(fp);
      close(data_sock);
      return -1;
    }
  }

  if (bytes < 0) {
    perror("read");
    fclose(fp);
    close(data_sock);
    return -1;
  }

  fclose(fp);
  close(data_sock);

  // Verificar si el archivo quedó con error
  if (ferror(fp)) {
    fprintf(stderr, "Error en el archivo luego del cierre.\n");
    return -1;
  }

  return 0;
}
