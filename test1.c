#include "apihack.h"


int main(int argc, char *argv[])
{

 char *cad, *mac1 = NULL;
 const char **dns;
 int y;

 NET *nt = NULL;

 nt = net_new();

 net_set_interf(nt, "wlx30b5c2117c3f");
 net_activate_interf(nt);

 cad = net_wifi_get_ssid(nt);
 puts(cad);
 cad = net_wifi_get_signal(nt);
 puts(cad);
 cad = net_get_ip(nt);
 puts(cad);
 mac1 = net_get_mac(nt); /* Salvar mac anterior para restaurar luego */
 puts(mac1);
 cad = net_get_gateway(nt);
 puts(cad);
 cad = net_get_mtu(nt);
 puts(cad);

 dns = net_get_dns_list();
 while(dns[y] != NULL)
   puts(dns[y++]);


 net_stop_nmanager();


 net_set_mac(nt, "54:04:a6:45:d4:80");
 net_set_ip(nt, "7.7.7.24");
 net_set_gateway(nt, "7.7.7.1");
 net_set_mtu(nt, "2304");
 net_wifi_set_ssid(nt, "wifimeliacohiba");
 net_apply_config(nt);
 cad = net_get_mac(nt);
 puts(cad);
 cad = net_get_mtu(nt);
 puts(cad);

 net_set_mac(nt, mac1);
 net_apply_config(nt);
 cad = net_get_mac(nt);
 puts(cad);



net_start_nmanager();

 return 0;
}
