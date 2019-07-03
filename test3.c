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

 net_wifi_set_ssid(nt, "wifimeliacohiba");
 net_wifi_asoc_open(nt);
 net_add_dns(nt, "1.1.1.1");
 net_wifi_deasoc(nt);


 return 0;
}
