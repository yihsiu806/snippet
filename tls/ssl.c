#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sock, connfd, ret;
    SSL_CTX *ctx;
    const SSL_METHOD *method;
    SSL *ssl;
    struct sockaddr_in addr;
    const char reply[] = "Hello!\n";

    SSL_library_init();
    SSL_load_error_strings();
    method = TLSv1_2_server_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);
    SSL_CTX_set_cipher_list(ctx, "AES");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(993);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 1);

    for (;;) {
        int connfd = accept(sock, NULL, NULL);
        printf("handle new client\n");
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, connfd);
        SSL_accept(ssl);
        SSL_write(ssl, reply, strlen(reply));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(connfd);
        printf("finish client\n");
    }

    close(sock);
    SSL_CTX_free(ctx);

    return 0;
}
