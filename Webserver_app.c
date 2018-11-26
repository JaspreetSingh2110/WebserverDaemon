#include <microhttpd.h>
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo</body></html>"
#define ERROR_STR "<html><body>An internal server error has occured!</body></html>"
#define IMAGE_PATH "/Users/jaspreetsingh/Desktop/workspace/Practice/WebserverApplication/Images/"

#define FILENAME "/Users/jaspreetsingh/Desktop/workspace/Practice/WebserverApplication/Images/Jaspreet.png"
#define MIMETYPE "image/png"

int print_out_key (void *cls, enum MHD_ValueKind kind,
	const char *key, const char *value)
{
	printf ("Key %s: Value: %s -----------\n", key, value);
	return MHD_YES;
}

static int
ahc_echo (void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr)
{
  static int aptr;
  const char *me = cls;
  struct MHD_Response *response;
  int ret;
  (void)url;               /* Unused. Silent compiler warning. */
  (void)version;           /* Unused. Silent compiler warning. */
  (void)upload_data;       /* Unused. Silent compiler warning. */
  (void)upload_data_size;  /* Unused. Silent compiler warning. */
  
  printf ("New %s request for %s using version %s\n", method, url, version);
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);

  if (0 != strcmp (method, "GET")) {
    printf ("Unhandled request type: %s.\n", method);
    return MHD_NO;              /* unexpected method */
  }

  if (&aptr != *ptr)
    {
      /* do never respond on first call */
      *ptr = &aptr;
      return MHD_YES;
    }
  *ptr = NULL;                  /* reset when done */


	int fd;
	struct stat sbuf;

	if ( (-1 == (fd = open (FILENAME, O_RDONLY))) || (0 != fstat (fd, &sbuf)) ) {
		/* error accessing file */
		if (fd != -1) 
		  close (fd);
		
		response = MHD_create_response_from_buffer (strlen (ERROR_STR),	(void *) ERROR_STR, MHD_RESPMEM_PERSISTENT);
		if (response)
		{
		  ret = MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
		  MHD_destroy_response (response);
		  return MHD_YES;
		} else
		  return MHD_NO;

		if (!ret) {
			response = MHD_create_response_from_buffer (strlen (ERROR_STR),	(void *) ERROR_STR, MHD_RESPMEM_PERSISTENT);
			if (response)
			{
			ret = MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
			MHD_destroy_response (response);
			return MHD_YES;
			}
			else return MHD_NO;
		}
	}

	response = MHD_create_response_from_fd_at_offset64 (sbuf.st_size, fd, 0);
	MHD_add_response_header (response, "Content-Type", MIMETYPE);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);

  // response = MHD_create_response_from_buffer (strlen (me), (void *) me, MHD_RESPMEM_PERSISTENT);
  return ret;
}

static int print_client_info (void *cls,
		const struct sockaddr *addr,
		socklen_t addrlen) {
        char client_ip[INET_ADDRSTRLEN];
        struct sockaddr_in *csa = (struct sockaddr_in *) addr;
	//memcpy (&csa, &addr, addrlen);
        //getaddrinfo(argv[1], NULL, NULL, &_addrinfo);
	inet_ntop(AF_INET, (&(csa->sin_addr)), client_ip, INET_ADDRSTRLEN);

	printf ("New client connected to Webserver. IP: %s, Port: %d\n", client_ip, 10);
        return MHD_YES;
}

int
main (int argc, char *const *argv)
{
  struct MHD_Daemon *d;

  if (argc != 2)
    {
      printf ("%s PORT\n", argv[0]);
      return 1;
    }
  d = MHD_start_daemon (/* MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, */
                        MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
                        /* MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL, */
                        /* MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG | MHD_USE_POLL, */
                        /* MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, */
                        atoi (argv[1]),
                        &print_client_info, NULL, &ahc_echo, PAGE,
                        MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
                        MHD_OPTION_STRICT_FOR_CLIENT, (int) 1,
                        MHD_OPTION_END);
  if (d == NULL)
    return 1;
  (void) getchar ();
  MHD_stop_daemon (d);
  return 0;
}
