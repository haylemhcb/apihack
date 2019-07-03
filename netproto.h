#ifndef NETPROTO_H
#define NETPROTO_H

struct WIRELESS
{
  char rate[20];
  char rts[20];
  char frag[20];
  char txpower[20];
  char essid[20];
  char signal[20];
};

struct CAPTURE
{
  char port[20];
  char monitor[20]; /* Poner tcpdump modo monitor si 1 o no 0 */
  char range[20]; /* Rango de red ej 12.168.0.0 */
  char bufer[9000]; 
  pthread_t hilocaptura; /* Para poder cancelar la captura facilmente */
  char cmd[1000]; /* comando a ejecutar */
  pthread_t id;
  char *ip[20];
};

struct SNET
{
  char ip[20];
  char interf[20];
  char mac[20];
  char mtu[20];
  char gateway[20];
  char resolvfile[20];
  struct WIRELESS w;
  struct CAPTURE cap;
};

typedef struct SNET NET;
NET net;


NET *net_new(void);
void net_set_interf(NET *n, const char *interf);
void net_wifi_dump(NET *n);
void net_wifi_set_ssid(NET *n, const char *ssid);
void net_wifi_asoc_open(NET *nt);
void net_wifi_deasoc(NET *nt);
void net_wifi_mode_managed(NET *nt);
void * net_wifi_monitor_off(NET *nt);
void net_dump(NET *n);
char * net_wifi_get_ssid(NET *n);
char * net_wifi_get_signal(NET *n);
char * net_get_ip(NET *n);
char * net_get_mac(NET *n);
char * net_get_gateway(NET *n);
char * net_get_mtu(NET *n);
void net_add_dns(NET *nt, const char *dns);
void net_activate_interf(NET *n);
void net_deactivate_interf(NET *n);
void net_set_ip(NET *n, const char *ip);
void net_set_mac(NET *n, const char *mac);
void net_set_mtu(NET *n, const char *mtu);
void net_set_gateway(NET *, const char *gateway);
void net_flush_ip(NET *n);
void net_apply_config(NET *n);
void net_stop_nmanager(void);
void net_start_nmanager(void);
void net_cap_set_port(NET *n, const char *port);
void net_cap_set_monitor(NET *n, const char *monitor);
void net_cap_set_range(NET *n, const char *range);
void net_cap_dump(NET *n);
const char * net_cap_get_ip(NET *n);
const char **net_get_dns_list();
void sys_pkill(const char *process);
const char *net_cap_get_ip(NET *nt);
const char *net_cap_get_mac(NET *nt);
const char *net_cap_get_bssid(NET *nt); /* Obtiene bssid a la que se asocia la victima */
#endif
