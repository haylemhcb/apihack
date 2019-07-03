#include "apihack.h"


int main(int argc, char *argv[])
{

 char *cad, *mac1 = NULL;
 const char **dns;
 int y;

 NET *nt = NULL;

 nt = net_new();

 net_set_interf(nt, "wlp16s0");
 net_activate_interf(nt);

 cad = net_wifi_get_ssid(nt);
 puts(cad);
 cad = net_wifi_get_signal(nt);
 puts(cad);
 cad = net_get_ip(nt);
 puts(cad);
 cad = net_get_mac(nt);
 puts(cad);
 cad = net_get_gateway(nt);
 puts(cad);

 dns = net_get_dns_list();
 while(dns[y] != NULL)
   puts(dns[y++]);


 cad = net_get_mtu(nt);
 puts(cad);

 return 0;
}
