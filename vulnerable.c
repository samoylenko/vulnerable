#include <stdio.h>
#include <winsock2.h>

int main( int argc, char **argv )
{
	int s, len, i;
	struct sockaddr_in sa;

	WSADATA wsaData;
	WORD wVersionRequested;

	char buf[1024];

	printf( "vulnerable programm\r\n" );

	wVersionRequested = MAKEWORD(1, 1);
	if( WSAStartup( wVersionRequested, &wsaData ) < 0 )
	{
		fprintf( stderr, "WSAStartup() error!\r\n" );
		return 1;
	}

	if( ( s = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
	{
		fprintf( stderr, "socket() error!\r\n" );
		WSACleanup();
		return 1;
	}
	
	ZeroMemory( &sa, sizeof( struct sockaddr_in ) );

	sa.sin_family       = AF_INET;
	sa.sin_addr.s_addr  = htonl( INADDR_ANY );
	sa.sin_port         = htons( 2929 );

	if( bind( s, ( const struct sockaddr * )&sa, sizeof( struct sockaddr_in ) ) )
	{
		fprintf( stderr, "bind() error!\r\n" );
		closesocket( s );
		WSACleanup();
		return 1;
	}

	printf( "runs on udp port 2929\r\n" );

	ZeroMemory( buf, 1024 );
	for( ; ; )
	{
		if( len = recv( s, buf, 5, 0 ) )
		{
			if( !strncmp( buf, "die", 3 ) )
			{
				printf( "got quit command\r\n" );
				closesocket( s );
				WSACleanup();
				return 0;
			}

			printf( "received data: " );
			for( i = 0; i < len - 1; i++ )
			{
				printf( "0x%02x, ", buf[i] );
			}

			printf( "0x%02x\r\n", buf[i] );

			ZeroMemory( buf, 1024 );
		}
	}
}