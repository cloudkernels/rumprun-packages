#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h> 

//extern int main(int argc, char *argv[]);

/*
 * TODO: Rumprun redefines main symbol with rumprun_main[1-9]
 * The number is assigned in incremental order, with the first 
 * object containing main getting the rumprun_main1 symbol.
 * We might want to find a better way to handle this
 * in the future.
 */
extern int rumprun_main2(int argc, char *argv[]);

#define LISTEN_PORT 8123
static const char reply_head[] = "HTTP/1.1 200 OK\r\n" \
			    "Content-Type: text/plain;\r\n" \
			    "Connection: close;\r\n" \
			    "\r\n";

#define BUFLEN 2048
static char recvbuf[BUFLEN];

int main(int argc, char *argv[])
{
	int rc = 0;
	int srv, client;
	ssize_t n;
	struct sockaddr_in srv_addr;
	int saved_output;

	srv = socket(AF_INET, SOCK_STREAM, 0);
	if (srv < 0) {
		fprintf(stderr, "Failed to create socket: %d\n", errno);
		goto out;
	}

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addr.sin_port = htons(LISTEN_PORT);

	rc = bind(srv, (struct sockaddr *) &srv_addr, sizeof(srv_addr));
	if (rc < 0) {
		fprintf(stderr, "Failed to bind socket: %d\n", errno);
		goto out;
	}

	/* Accept one simultaneous connection */
	rc = listen(srv, 1);
	if (rc < 0) {
		fprintf(stderr, "Failed to listen on socket: %d\n", errno);
		goto out;
	}

	saved_output = dup(STDOUT_FILENO);
	printf("%s: Listening on port %d...\n", __func__, LISTEN_PORT);
	while (1) {
		int func_ret = 0;
		int fpipe[2];
		ssize_t rbytes = 0;
		char reply_body[BUFLEN];

		rc = pipe(fpipe);
		if (rc < 0) {
			fprintf(stderr, "Failed to create pipe\n");
			return -1;
		}

		rc = dup2(fpipe[1], STDOUT_FILENO);
		if (rc < 0) {
			fprintf(stderr, "Failed to redirect stdout\n");
			return -1;
		}

		client = accept(srv, NULL, 0);
		if (client < 0) {
			fprintf(stderr,
				"Failed to accept incoming connection: %d\n",
				errno);
			goto out;
		}

		/* Receive some bytes (ignore errors) */
		read(client, recvbuf, BUFLEN);

		func_ret = rumprun_main2(argc, argv);

		fflush(stdout);
		rc = dup2(saved_output, STDOUT_FILENO);
		if (rc < 0) {
			fprintf(stderr, "Failed to redirect stdout\n");
			return -1;
		}
		printf("Faunction returned %d\n", func_ret);
		close(fpipe[1]);
		rbytes = read(fpipe[0], reply_body, BUFLEN);
		if (rbytes <= 0) {
			fprintf(stderr, "Failed to read redirected output %ld\n", rbytes);
			return -1;
		}
		close(fpipe[0]);

		
		/* Send reply */
		n = write(client, reply_head, strlen(reply_head));
		if (n < 0)
			fprintf(stderr, "Failed to send a reply\n");
		n = write(client, reply_body, rbytes);
		if (n < 0)
			fprintf(stderr, "Failed to send a reply\n");
		else
			printf("Sent a reply\n");

		close(client);
	}

out:
	close(saved_output);
	return rc;
}
