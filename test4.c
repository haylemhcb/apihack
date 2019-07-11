#include "apihack.h"


int main(int argc, char *argv[])
{

  const char *args[] = {"xterm" , "-bg", "blue", "-fg", "yellow", "-e", "top", NULL};

 NET *nt = NULL;

 sys_exec_timeout(args, 7); /* Ejecutar por 7 seg */

 nt = net_new();

net_set_interf(nt, "wlan0");
	net_set_gateway (nt, "10.240.0.1");
	net_cap_set_monitor(nt, "1");
	net_cap_set_port(nt, "https");
	net_cap_set_range(nt, "10.240");
net_cap_set_timeout(nt, 4);
       net_cap_dump(nt);	

printf("%s\n", nt->cap.bufer);  

puts(net_cap_get_ip(nt));
puts(net_cap_get_mac(nt));
puts(net_cap_get_bssid(nt));


 return 0;
}
