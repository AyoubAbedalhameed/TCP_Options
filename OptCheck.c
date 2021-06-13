/* NES416 H.W 3: Checking Socket Options Program  
 > Apr 21 2021
 > Team members:- 
         * Ayoub Abedalhameed 124052
         * Zeina Falah 128652  */ 


#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
 #include <unistd.h>
 #include <errno.h>
 #include <sys/time.h>
 #include <bits/types.h>
 #include <sys/types.h>
 #include <stdbool.h>
 #include <sys/utsname.h>

#define INET6_ADDRSTRLEN 46 
typedef unsigned char u_char ; 



//To be passed for getsockop() functions. 
union val {
  int				i_val;
  long				l_val;
  struct linger		linger_val ;
  struct timeval 	timeval_val ;
  struct in_addr    address ;
  u_char              c_val ; 

  /*struct ip_mreq     mreq_val ; */

} val; 



static char	strres[128];






/*static char* sock_str_mreq (union val *ptr, int len)
{
	struct ip_mreq	*tvptr = &ptr->mreq_val;

	if (len != sizeof(struct ip_mreq))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct ip_mreq)", len);
	else
		snprintf(strres, sizeof(strres), "imr_multiaddr:%s , imr_interface:%s",
				 tvptr->imr_multiaddr, tvptr->imr_interface);
	return(strres);
}*/


/* ----------------------------------*/ 






//Function that deals with union value filled by (char);     
static char	*sock_str_char(union val *ptr, int len)
{
/* *INDENT-OFF* */
	if (len != sizeof(u_char))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(c_val)", len);
	else
		snprintf(strres, sizeof(strres), "%uc",ptr-> c_val );
	return(strres);
/* *INDENT-ON* */}

/* all the followng function are used to print the default value of an option which 
returned buy getsockopt func, every function can deals with a specific type*/

//Function that deals with union value filled by (IPV4 address);     
static char	*sock_str_addr(union val *ptr, int len)
{
/* *INDENT-OFF* */
	if (len != sizeof(struct in_addr))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(in_addr)", len);
	else
		snprintf(strres, sizeof(strres), "%s", inet_ntoa(ptr->address) );
	return(strres);
/* *INDENT-ON* */}

//Function that deals with union value filled by (int flag 0/1);     
static char	*sock_str_flag(union val *ptr, int len)
{
/* *INDENT-OFF* */
	if (len != sizeof(int))
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres),
				 "%s", (ptr->i_val == 0) ? "off" : "on");
	return(strres);
/* *INDENT-ON* */
}
/* end checkopts3 */

//Function that deals with union value filled by (IPV4 address);     

static char* sock_str_int(union val *ptr, int len)
{
	if (len != sizeof(int))         //first Check the returned size. 
		snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
	else
		snprintf(strres, sizeof(strres), "%d", ptr->i_val);
	return(strres);
}

//Function that deals with union value filled by (in) value;     
static char* sock_str_linger(union val *ptr, int len)
{
	struct linger	*lptr = &ptr->linger_val;

	if (len != sizeof(struct linger))
		snprintf(strres, sizeof(strres),"size (%d) not sizeof(struct linger)", len);
	else
		snprintf(strres, sizeof(strres), "l_onoff = %d, l_linger = %d",
				 lptr->l_onoff, lptr->l_linger);
	return(strres);
}

static char* sock_str_timeval(union val *ptr, int len)
{
	struct timeval	*tvptr = &ptr->timeval_val;

	if (len != sizeof(struct timeval))
		snprintf(strres, sizeof(strres),
				 "size (%d) not sizeof(struct timeval)", len);
	else
		snprintf(strres, sizeof(strres), "%d sec, %d usec",
				 tvptr->tv_sec, tvptr->tv_usec);
	return(strres);
}
/* ----------------------------------*/ 







struct sock_opts {
  const char	   *opt_str;
  int		opt_level;
  int		opt_name;
  char   *(*opt_val_str)(union val *, int);
} sock_opts[] = {
	{ "SO_BROADCAST",		SOL_SOCKET,	SO_BROADCAST,	sock_str_flag },
	{ "SO_DEBUG",			SOL_SOCKET,	SO_DEBUG,		sock_str_flag },
	{ "SO_DONTROUTE",		SOL_SOCKET,	SO_DONTROUTE,	sock_str_flag },
	{ "SO_ERROR",			SOL_SOCKET,	SO_ERROR,		sock_str_int },
	{ "SO_KEEPALIVE",		SOL_SOCKET,	SO_KEEPALIVE,	sock_str_flag },
	{ "SO_LINGER",			SOL_SOCKET,	SO_LINGER,		sock_str_linger },
	{ "SO_OOBINLINE",		SOL_SOCKET,	SO_OOBINLINE,	sock_str_flag },
	{ "SO_RCVBUF",			SOL_SOCKET,	SO_RCVBUF,		sock_str_int },
	{ "SO_SNDBUF",			SOL_SOCKET,	SO_SNDBUF,		sock_str_int },
	{ "SO_RCVLOWAT",		SOL_SOCKET,	SO_RCVLOWAT,	sock_str_int },
	{ "SO_SNDLOWAT",		SOL_SOCKET,	SO_SNDLOWAT,	sock_str_int },
	{ "SO_RCVTIMEO",		SOL_SOCKET,	SO_RCVTIMEO,	sock_str_timeval },
	{ "SO_SNDTIMEO",		SOL_SOCKET,	SO_SNDTIMEO,	sock_str_timeval },
	{ "SO_REUSEADDR",		SOL_SOCKET,	SO_REUSEADDR,	sock_str_flag },
#ifdef	SO_REUSEPORT
	{ "SO_REUSEPORT",		SOL_SOCKET,	SO_REUSEPORT,	sock_str_flag },
#else
	{ "SO_REUSEPORT",		0,			0,				NULL },
#endif
	{ "SO_TYPE",			SOL_SOCKET,	SO_TYPE,		sock_str_int },
	

#ifdef SO_USELOOPBACK 
    {"SO_USELOOPBACK",		SOL_SOCKET,	SO_USELOOPBACK,	sock_str_flag },
	#else
    	{ "SO_USELOOPBACK",		0,			0,				NULL },
    #endif 

/*                      ------[End of SOL_SOCKET Level (17) options]-----                */  




/*                          *******[IPPROTO_IP Level]******                            */

    { "IP_TOS",				IPPROTO_IP,	IP_TOS,			sock_str_int },
	{ "IP_TTL",				IPPROTO_IP,	IP_TTL,			sock_str_int },
    { "IP_HDRINCL",				IPPROTO_IP,	IP_HDRINCL,			sock_str_int },
#ifdef IP_RECVIF
	{ "IP_RECVIF",				IPPROTO_IP,	IP_RECVIF,			sock_str_int },
#else 
		{ "IP_RECVIF",				0,	0,			NULL },
#endif 

#ifdef IP_RECVDSTADDR
	{ "IP_RECVDSTADDR",				IPPROTO_IP,	IP_RECVDSTADDR,			sock_str_int },
#else 
		{ "IP_RECVDSTADDR",				0,	0 ,			NULL },
#endif 

		{ "IP_MULTICAST_IF",				IPPROTO_IP,	IP_MULTICAST_IF,			sock_str_addr },

		{ "IP_MULTICAST_TTL",				IPPROTO_IP,	IP_MULTICAST_TTL,			sock_str_int },
		{ "IP_MULTICAST_LOOP",				IPPROTO_IP,	IP_MULTICAST_LOOP,			sock_str_int },

		/*#ifdef IP_ADD_MEMBERSHIP
{ "IP_ADD_MEMBERSHIP",				IPPROTO_IP,	IP_ADD_MEMBERSHIP,			sock_str_mreq },
#else
{ "IP_ADD_MEMBERSHIP",				0,	0 ,			NULL },
#endif

	#ifdef IP_DROP_MEMBERSHIP
{ "IP_DROP_MEMBERSHIP",				IPPROTO_IP,	IP_DROP_MEMBERSHIP,			sock_str_mreq },
#else
{ "IP_DROP_MEMBERSHIP",				0,	0 ,			NULL },
#endif*/ 


   /*                       ------ [End Of IPPROTO_IP Level  (8 (+2) Options)]                  */


/*                          *******[IPV6_DONTFRAG Level]******                            */



#ifdef	IPV6_CHECKSUM

	{ "IPV6_CHECKSUM",		IPPROTO_IPV6,IPV6_CHECKSUM,	sock_str_int },
#else
	{ "IPV6_CHECKSUM",		0,			0,				NULL },
#endif


#ifdef	IPV6_DONTFRAG
	{ "IPV6_DONTFRAG",		IPPROTO_IPV6,IPV6_DONTFRAG,	sock_str_flag },
#else
	{ "IPV6_DONTFRAG",		0,			0,				NULL },
#endif

//IPV6_RECVDSTOPTS
//

	{ "IPV6_RECVDSTOPTS",		IPPROTO_IPV6,IPV6_RECVDSTOPTS,	sock_str_flag }, 
	{ "IPV6_RECVHOPLIMIT",		IPPROTO_IPV6,IPV6_RECVHOPLIMIT,	sock_str_flag },
	{ "IPV6_RECVHOPOPTS",		IPPROTO_IPV6,IPV6_RECVHOPOPTS,	sock_str_flag },
	{ "IPV6_RECVPATHMTU",		IPPROTO_IPV6,IPV6_RECVPATHMTU,	sock_str_flag },
	{ "IPV6_RECVRTHDR ",		IPPROTO_IPV6,IPV6_RECVRTHDR ,	sock_str_flag },
    { "IPV6_RECVPKTINFO ",		IPPROTO_IPV6,IPV6_RECVPKTINFO ,	sock_str_flag },
    { "IPV6_V6ONLY ",		IPPROTO_IPV6,IPV6_V6ONLY ,	sock_str_flag },
    { "IPV6_MULTICAST_HOPS ",		IPPROTO_IPV6,IPV6_MULTICAST_HOPS ,	sock_str_flag },

#ifdef	IPV6_UNICAST_HOPS
	{ "IPV6_UNICAST_HOPS",	IPPROTO_IPV6,IPV6_UNICAST_HOPS,sock_str_int },
#else
	{ "IPV6_UNICAST_HOPS",	0,			0,				NULL },
#endif
#ifdef	IPV6_V6ONLY
	{ "IPV6_V6ONLY",		IPPROTO_IPV6,IPV6_V6ONLY,	sock_str_flag },
#else
	{ "IPV6_V6ONLY",		0,			0,				NULL },
#endif

#ifdef TCP_MAXSEG
	{ "TCP_MAXSEG",			IPPROTO_TCP,TCP_MAXSEG,		sock_str_int },
#else 
{ "TCP_MAXSEG",		0,			0,				NULL },
#endif

	
#ifdef TCP_NODELAY
    { "TCP_NODELAY",		IPPROTO_TCP,TCP_NODELAY,	sock_str_flag },
#else 
{ "TCP_NODELAY",		0,			0,				NULL },
#endif

#ifdef	SCTP_AUTOCLOSE
	{ "SCTP_AUTOCLOSE",		IPPROTO_SCTP,SCTP_AUTOCLOSE,sock_str_int },
#else
	{ "SCTP_AUTOCLOSE",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXBURST
	{ "SCTP_MAXBURST",		IPPROTO_SCTP,SCTP_MAXBURST,	sock_str_int },
#else
	{ "SCTP_MAXBURST",		0,			0,				NULL },
#endif
#ifdef	SCTP_MAXSEG
	{ "SCTP_MAXSEG",		IPPROTO_SCTP,SCTP_MAXSEG,	sock_str_int },
#else
	{ "SCTP_MAXSEG",		0,			0,				NULL },
#endif
#ifdef	SCTP_NODELAY
	{ "SCTP_NODELAY",		IPPROTO_SCTP,SCTP_NODELAY,	sock_str_flag },
#else
	{ "SCTP_NODELAY",		0,			0,				NULL },
#endif
	{ NULL,					0,			0,				NULL }
};



int main(int argc, char **argv)
{

struct utsname Mine ; 

if ( uname(&Mine) == -1  ) {perror("Error While calling uname sys. call: "); return -1 ;}

printf("\n********\nOS:%s\n",Mine.sysname );
printf("Release: %s\n",Mine.release );
printf("Version: %s\n********\n            ********Checking Default Options*******\n\n",Mine.version );



	int	counter = 0, fd;
	socklen_t			len;
	struct sock_opts	*ptr;

	for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
		printf("->%d: %s: ", ++counter ,ptr->opt_str);
		if (ptr->opt_val_str == NULL)
			printf("(undefined)\n");
		else {
			switch(ptr->opt_level) {
			case SOL_SOCKET:
			case IPPROTO_IP:
			case IPPROTO_TCP:
				fd = socket(AF_INET, SOCK_STREAM, 0);
				break;

			case IPPROTO_IPV6:
				fd = socket(AF_INET6, SOCK_STREAM, 0);
				break;


#ifdef	IPPROTO_SCTP
			case IPPROTO_SCTP:
				fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
				break;
#endif
			default:
				printf("Can't create fd for level %d\n, Continue to next option ... \n", ptr->opt_level);
                continue ; 
			}

			len = sizeof(val);
			if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1) {
            /*if(errno=92) {printf("(undefined)\n"); continue ;} */
				perror("getsockopt error:"); printf("Continue to next option ... \n",errno) ; continue;}
                 else {
					 printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
					/* if(ptr->opt_name ==SO_RCVLOWAT ) 
			{NLen = sizeof(NVal) ; getsockopt(fd, SOL_SOCKET, SO_RCVLOWAT, &NVal, &NLen); 
					 printf("Again the value of SO_RCVLOWAT is (%d)\n", NVal); return 0;}*/
			}
			close(fd);
		}
	}
		printf("\n           ******End of Options*****        \n\n") ; 

	int ReceiveBuffS, SendBuffS, SendLWM,  ReceiveLWM ; 
    socklen_t NewLength ; 
	int TestSocket ;
	bool  flag1 = 0 , flag2 = 0 ; 

		// Openning Test Socket: 
		TestSocket = socket(AF_INET, SOCK_STREAM, 0 );
if ( TestSocket == -1) {perror("Error While Openning Test Sock:"); return -1;}	
 

//Getting Buffures Size:
 NewLength = sizeof(ReceiveBuffS) ; 
if(getsockopt(TestSocket, SOL_SOCKET, SO_RCVBUF , &ReceiveBuffS, &NewLength) == -1 ){perror("Error While Getting Receive Buff Size opt:"); return -1;}

NewLength = sizeof(SendBuffS) ; 
if(getsockopt(TestSocket, SOL_SOCKET, SO_SNDBUF, &SendBuffS, &NewLength) == -1 ){perror("Error While Getting Receive Buff Size opt:"); return -1;}

//Test : 
//printf("Receive Buff, Send Buff : (%d),(%d)", ReceiveBuffS, SendBuffS) ; 
		
	int bug =0 ; 
while(1)
{
	printf("Please Enter New RCVLOWAT, SendLOWAT to change default values OR ENTER -1 to cancel\n") ; 
	
	
	scanf("%d", &ReceiveLWM) ; 
	if(ReceiveLWM == -1) {printf("Exiting .. \n"); return 0 ; } 
	if(ReceiveLWM < 0 || ReceiveLWM > SendBuffS) {scanf("%d",&bug);printf("Invalid Value Please Try Again\n"); continue;} 
	scanf("%d", &SendLWM) ; 
	if(SendLWM < 0 || SendLWM > ReceiveBuffS) { printf("Invalid Value Please Try Again\n"); continue;} 
break;     
}	

//Test:
//printf("ReceiveLWM, SendLWM, %d, %d\n" , ReceiveLWM, SendLWM) ; 	
	
	
//Setting New Values: 

if(setsockopt(TestSocket, SOL_SOCKET,SO_RCVLOWAT, &ReceiveLWM,  sizeof(ReceiveLWM)) == -1) {flag1 = 1 ; perror("Error While Setting New SO_RCVLOWAT: "); } 
if(setsockopt(TestSocket, SOL_SOCKET,SO_SNDLOWAT, &SendLWM,     sizeof(SendLWM))    == -1)       {flag2=1 ; perror("Error While Setting New SO_SNDLOWAT: "); } 



// Check if the values have been changed. 
int New_ReceiveLWM , New_SendLWM ; 
 NewLength = sizeof(New_ReceiveLWM) ; 
if(getsockopt(TestSocket, SOL_SOCKET, SO_RCVLOWAT , &New_ReceiveLWM, &NewLength) == -1 ){perror("Error While Getting Receive Buff Size opt:"); return -1;}

NewLength = sizeof(New_SendLWM) ; 
if(getsockopt(TestSocket, SOL_SOCKET, SO_SNDLOWAT, &New_SendLWM, &NewLength) == -1 ){perror("Error While Getting Receive Buff Size opt:"); return -1;}

if(flag1) printf("SO_RCVBUF has not changed due an error, ") ;
printf("SO_RCVBUF Value is: (%d)\n", New_ReceiveLWM ) ;


if(flag2) printf("SendLWM value has not changed, You cant change Low_WaterMark for stream protocol\n") ;
printf("New SendLWM Value is: (%d)\n", New_SendLWM ) ;



printf("\n        **********Execution ends********* .. \n\n");


	exit(0);
}
































































