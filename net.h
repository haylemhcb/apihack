#ifndef NET_H
#define NET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "netproto.h"
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void net_wifi_set_channel(NET *n, const char *canal)
{
  char cmd[80] = {'\0'};
  strcpy(cmd, "iwconfig ");
  strcat(cmd, n->interf);
  strcat(cmd, " channel ");
  strcat(cmd, canal);
  printf("Poniendo canal %s via iwconfig\n", canal);
  system(cmd);


  strcpy(cmd, "iw ");
  strcat(cmd, n->interf);
  strcat(cmd, " set channel ");
  strcat(cmd, canal);
  printf("Poniendo canal %s via iw\n", canal);
  system(cmd);
}

void net_wifi_set_rate(NET *n, const char *rate)
{
  char cmd[80] = {'\0'};
  strcpy(cmd, "iwconfig ");
  strcat(cmd, n->interf);
  strcat(cmd, " rate ");
  strcat(cmd, rate);
  printf("Poniendo bit rate  a %s via iwconfig\n", rate);
  system(cmd);


  strcpy(cmd, "iw ");
  strcat(cmd, n->interf);
  strcat(cmd, " set bitrates legacy-2.4 ");
  strcat(cmd, rate);
  printf("Poniendo bit rate  a %s via iw\n", rate);
  system(cmd);
}

void net_wifi_set_retry_limit(NET *n, const char *retry)
{
  char cmd[80] = {'\0'};
  strcpy(cmd, "iwconfig ");
  strcat(cmd, n->interf);
  strcat(cmd, " retry ");
  strcat(cmd, retry);
  printf("Poniendo retry limit a %s\n", retry);
  system(cmd);
}

int net_is_internet(NET *nt)
{
  char b[20000] = {'\0'};
  FILE *f = NULL;
  int i = 0;
   f = fopen("/tmp/lnxgoog", "r");
	
  if(f == NULL)
   {
      printf("\n\nNo existe /tmp/lnxgoog, saliendo...\n\n");
	  ++contador;
      return 0;
   }
   puts("VERIFICANDO SI HAY INTERNET...");
 
   while(!feof(f))
   {
     b[i] = fgetc(f);
	 if(b[i] == '&') b[i] = '_';
	 if(b[i] == '|') b[i] = '_';
	 if(b[i] == '!') b[i] = '_';
	 if(b[i] == '>') b[i] = '_';
	 if(b[i] == '<') b[i] = '_';
     if(b[i] == EOF)
     {
       b[i] = '\0';
       break;
     }

     ++i;
     if(i >= (20000 - 1))
     {
       strcpy(b, "");
       i = 0;
	   break;
     }
  }
  pclose(f);
  if((strstr(b, "google") != NULL) || (strstr(b, "photos") != NULL))
  {
     puts("HAY INTERNET, GOOGLE DETECTADO");
	 internetok = 1;
	 contador = 0;
	 remove("/tmp/lnxgoog");
	 return 1;
  }
  else
  {
     puts("NO HAY INTERNET AUN!!!");
	 internetok = 0;
	 ++contador;
	 return 0;
  }
	
}

int net_isdown_interf(NET *nt)
{
  char cmd[250];
  char b[80] = {'\0'};
  FILE *f = NULL;
  int i = 0;

  strcpy(cmd, "ip link show dev ");
  strcat(cmd, nt->interf);

  strcpy(b, "");
	
  f = popen(cmd, "r");
	
  if(f == NULL)
   {
      printf("\n\nNo existe ip link, saliendo...\n\n");
      return 1;
   }
   puts("VERIFICANDO ESTADO ACTIVO DE INTERFACE...");
 
   while(!feof(f))
   {
     b[i] = fgetc(f);
     if(b[i] == EOF)
     {
       b[i] = '\0';
       break;
     }
	   
     if(i >= 240)
     {
       strcpy(b, "");
       i = 0;
	   break;
     }
	   ++i;
  }
  pclose(f);
  fprintf(stdout, "%s\n", b);
  if(strcmp(b, "") == 0) {puts("ESTADO INTERFACE INDEFINIDO"); return 1;}
  if(strstr(b, "LOWER_UP") != NULL)
	 {
	   puts("ESTADO INTERFACE ENCENDIDA"); return 0;
     }
   else
	{
      puts("ESTADO INTERFACE APAGADA"); return 1;
	}

}

void net_cap_set_timeout(NET *nt, int seg)
{
   nt->cap.timeout = seg;
}

void sys_exec_timeout(const char *args[] , int seg)
{
 pid_t pid;
 pid = fork();
 if(pid == -1) {perror("Error creando proceso fork"); return;}
 if(pid == 0)
 {
    execvp(args[0], args);
 }
 else
 {
   sleep(seg);
   kill(pid, SIGKILL);
   wait(NULL);
   fprintf(stdout, "Proceso terminado\n");
 }
}

DOWNLOAD *download_new(void)
{
  strcpy(download.src, "http://www.google.com");
  strcpy(download.dst, "/tmp/apihack_fileout");
  return &download;
}

void download_set_src(DOWNLOAD *dw, const char *src)
{
  if(src == NULL)
  {
    strcpy(dw->src, "http://www.google.com");
  }
  else
  {
    strcpy(dw->src, src);
  }
}

void download_set_dst(DOWNLOAD *dw,const char *dst)
{
  if(dst == NULL)
  {
    strcpy(dw->dst, "/tmp/apihack_fileout");
  }
  else
  {
    strcpy(dw->dst, dst);
  }
}

void download_file(DOWNLOAD *dw)
{
  char cmd[2000] = {'\0'};
  strcpy(cmd, "wget -T 3 -t 1 ");
  strcat(cmd, dw->src);
  strcat(cmd, " -O ");
  strcat(cmd, dw->dst);
  system(cmd);
}

NET *net_new(void)
{
  VARS.traffic = 1;
  VARS.emergencyexit = 0;
  net.cap.timeout = 10;
  net.cap.working = 0;
  strcpy(net.ip, "0.0.0.0");
  strcpy(net.interf, "wlan0");
  strcpy(net.mac, "54:04:a6:45:d5:50");
  strcpy(net.mtu, "1500");
  strcpy(net.resolvfile, "/etc/resolv.conf");
  strcpy(net.w.rate, "1M");
  strcpy(net.w.rts, "off");
  strcpy(net.w.frag, "off");
  strcpy(net.w.txpower, "auto");
  strcpy(net.w.essid, "WIFI_ETECSA");
  strcpy(net.cap.port, "https");
  strcpy(net.cap.monitor, "0");
  strcpy(net.cap.range, "0.0.0.0");
  return &net;
}

void verificacon(void)
{
  const char *argum[] = {"wget", "-T", "2", "-t", "1", "http://www.google.com", "-O", "/tmp/lnxgoog", 0};
  if(net.cap.working == 1) return;
  remove("/tmp/lnxgoog");
  sys_exec_timeout (argum, 2);
  puts("TIEMPO VERIFICACION GOOGLE TERMINADO.");
}

void fnw3m(void)
{
  const char *argum[] = {"w3m", "http://www.google.com", 0};
  if(net.cap.working == 1) return;
  sys_exec_timeout (argum, 2);
  puts("TIEMPO VERIFICACION W3M TERMINADO.");
}

void net_stop_traffic_gen(void)
{
  VARS.traffic = 0;
}

int net_keep_traffic_active(void)
{
  pthread_t vgoogle;
  pthread_t w3m;
  int res = 0;



  while(VARS.traffic == 1)
  {
   res = pthread_create(&vgoogle, NULL, &verificacon, NULL);
   if(res != 0) return 1;
   res = pthread_create(&w3m, NULL, &fnw3m, NULL);
   if(res != 0) return 1;
   sleep(2);

   /* Verifica si hay conexion */
   net_is_internet(&net);
   

   if(contador >= 100) {contador = 0; return;}
   if(contador == 50) { net_wifi_deasoc(&net); }
  } 
}

void capt(NET *nt) /* funcion de hilo */
{
   FILE *tcp = NULL;
   int i = 0;
	
   if(nt->cap.working == 1) return;
   nt->cap.id = pthread_self();

   nt->cap.working = 1;
   net_activate_interf(nt);
   tcp = popen(nt->cap.cmd, "r");
   if(tcp == NULL)
   {
      printf("\n\nNo existe tcpdump, saliendo...\n\n");
      return;
   }
   puts("CAPTURANDO...");
 
   while(!feof(tcp))
   {
     nt->cap.bufer[i] = fgetc(tcp);
     if(nt->cap.bufer[i] == EOF)
     {
       nt->cap.bufer[i] = '\0';
       break;
     }
     ++i;
     if(i >= 1000)
     {
       strcpy(nt->cap.bufer, "");
       i = 0;
	   break;
     }
  }
  pclose(tcp);
  nt->cap.working = 0;
  puts(nt->cap.bufer);
  if(strcmp(nt->cap.bufer, "") == 0) puts("NADA CAPTURADO AUN"); else puts("SE CAPTURARON DATOS");
}


const char *net_cap_get_bssid(NET *nt)
{
  char b[10000] = {'\0'};
  char *token1, *token2, *token3, *token4, *token5, *token6;
  char bssid[40];
  char *resbssid = NULL;
  strcpy(b, nt->cap.bufer);

  if(strcmp(b, "") == 0) {puts("BSSID VICTIMA NO CAPTURADA"); return "NOVICTIMBSSID";}
 
  strcpy(b, strstr(b, "BSSID:"));
  strtok(b, " ");
  strtok(b, ":");
  token1 = strtok(NULL, ":");
  token2 = strtok(NULL, ":");
  token3 = strtok(NULL, ":");
  token4 = strtok(NULL, ":");
  token5 = strtok(NULL, ":");
  token6 = strtok(NULL, ":");

  strcpy(bssid, token1);
  strcat(bssid, ":");
  strcat(bssid, token2);
  strcat(bssid, ":");
  strcat(bssid, token3);
  strcat(bssid, ":");
  strcat(bssid, token4);
  strcat(bssid, ":");
  strcat(bssid, token5);
  strcat(bssid, ":");
  strcat(bssid, token6);


  resbssid = bssid;

  if(resbssid == NULL) {puts("BSSID VICTIMA NO CAPTURADA"); return "NOBSSID";}

  fprintf(stdout, "SE HA CAPTURADO UNA VICTIMA CONECTADA AL AP  %s\n", resbssid);
  return strdup(resbssid);
}

const char *net_cap_get_mac(NET *nt)
{
  char b[10000] = {'\0'};
  char *token1, *token2, *token3, *token4, *token5, *token6;
  char mac[40];
  char *resmac = NULL;
  strcpy(b, nt->cap.bufer);

  if(strcmp(b, "") == 0) {puts("MAC NO CAPTURADA"); return "NOMAC";}
  strcpy(b, strstr(b, "DA:"));
  strtok(b, " ");
  strtok(b, ":");
  token1 = strtok(NULL, ":");
  token2 = strtok(NULL, ":");
  token3 = strtok(NULL, ":");
  token4 = strtok(NULL, ":");
  token5 = strtok(NULL, ":");
  token6 = strtok(NULL, ":");

  strcpy(mac, token1);
  strcat(mac, ":");
  strcat(mac, token2);
  strcat(mac, ":");
  strcat(mac, token3);
  strcat(mac, ":");
  strcat(mac, token4);
  strcat(mac, ":");
  strcat(mac, token5);
  strcat(mac, ":");
  strcat(mac, token6);

  resmac = mac;

  if(resmac == NULL) {puts("MAC NO CAPTURADA"); return "NOMAC";}

  fprintf(stdout, "SE HA CAPTURADO UNA VICTIMA SU MAC ES %s\n", resmac);
  return strdup(resmac);

}

const char *net_cap_get_ip(NET *nt)
{
  char b[10000] = {'\0'};
  char *token = NULL, *token1 = NULL, *token2 = NULL, *token3 = NULL, *token4 = NULL;
  char ip[40];

  strcpy(b, nt->cap.bufer);

  if(strcmp(b, "") == 0) {puts("IP NO CAPTURADA"); return "NOIP";}

  strcpy(b, strstr(b, "ethertype"));
  strcpy(b, strstr(b, ">"));
  strcpy(b, strstr(b, " "));

  token1 = strtok(b, ".");
  token2 = strtok(NULL, ".");
  token3 = strtok(NULL, ".");
  token4 = strtok(NULL, ".");

  strcpy(ip, token1);
  strcat(ip, ".");
  strcat(ip, token2);
  strcat(ip, ".");
  strcat(ip, token3);
  strcat(ip, ".");
  strcat(ip, token4);
  strcpy(ip, strstr(ip, " "));

  token = ip;

  if(token == NULL) {puts("IP NO CAPTURADA"); return "NOIP";}

  fprintf(stdout, "SE HA CAPTURADO UNA VICTIMA SU IP ES %s\n", token);
  return strdup(token);

}

void net_wifi_monitor_off(NET *nt)
{
   net_deactivate_interf(nt);
   net_wifi_mode_managed(nt);
   net_activate_interf(nt);
}


void net_wifi_mode_managed(NET *nt)
{
   FILE *f;
   char cmd[250] = {'\0'};

   strcpy(cmd, "iwconfig ");
   strcat(cmd, nt->interf);
   strcat(cmd, " mode Managed");

   f = popen(cmd, "r");

   if(f == NULL) return;

   while(!feof(f)) fgetc(f);
   fclose(f);


   strcpy(cmd, "iw ");
   strcat(cmd, nt->interf);
   strcat(cmd, " set type managed");

   f = popen(cmd, "r");

   if(f == NULL) return;

   while(!feof(f)) fgetc(f);
   fclose(f);
}

void net_cap_dump(NET *n)
{ 
   void capt(NET *nt);
   int res = 0;
   strcpy(n->cap.cmd, "tcpdump");
   if(strcmp(n->cap.monitor, "1") == 0)
		   strcat(n->cap.cmd, " -I");

   strcat(n->cap.cmd, " -i ");
   strcat(n->cap.cmd, n->interf);
   strcat(n->cap.cmd, " -n -c 1 -l -eq dst net ");
   strcat(n->cap.cmd, n->cap.range);
   strcat(n->cap.cmd, " and port ");
   strcat(n->cap.cmd, n->cap.port);
   strcat(n->cap.cmd, " and tcp and not host ");
   strcat(n->cap.cmd, n->gateway);

   res = pthread_create(&n->cap.hilocaptura, NULL, &capt, n);
   if(res != 0) return;
   sleep(n->cap.timeout);
   pthread_cancel(n->cap.id);
   if(pthread_cancel(n->cap.id) == 0)
   {
     puts("Hilo de captura terminado.");
     fprintf(stdout, "ID del hilo de captura %d\n", n->cap.id); 
   }
	 sys_pkill("tcpdump");
     net_wifi_monitor_off(n);

}

void net_cap_set_range(NET *n, const char *range)
{
  if(range == NULL)  strcpy(n->cap.range, "0.0.0.0"); 
  strcpy(n->cap.range, range);
}

void net_cap_set_port(NET *n, const char *port)
{
  if(port == NULL)  strcpy(n->cap.port, "80"); 	
  strcpy(n->cap.port, port);
}

void net_cap_set_monitor(NET *n, const char *monitor)
{
  if(monitor == NULL)  strcpy(n->cap.monitor, "0"); 
  strcpy(n->cap.monitor, monitor);
}

void net_set_interf(NET *n, const char *interf)
{
 if(interf == NULL) strcpy(n->interf, "wlan0");
 strcpy(n->interf, interf);
}


void net_wifi_dump(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;
  system("echo \"\">/tmp/datoswifi"); 
  strcpy(cmd, "iw dev ");
  strcat(cmd, n->interf);
  strcat(cmd, " link");

  f = popen(cmd, "r");

  if(f == NULL)
   ;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

 f = fopen("/tmp/datoswifi", "w");

 if(f == NULL)
  ;
 else
 {
  fputs(buf, f);
  fclose(f);
 }
}

void net_dump(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  strcpy(cmd, "ip addr show dev ");
  strcat(cmd, n->interf);

  f = popen(cmd, "r");

  if(f == NULL)
   ;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

 f = fopen("/tmp/datosnet", "w");

 if(f == NULL)
  ;
 else
 {
  fputs(buf, f);
  fclose(f);
 }
}

char * net_wifi_get_ssid(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_wifi_dump(n);

  strcpy(cmd, "cat /tmp/datoswifi|grep \"SSID\"|cut -f2 -d\":\"");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      if(buf[i] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

  if(strcmp(buf, "") == 0) return "Desconectado";

  return strdup(buf);
}

char * net_wifi_get_bssid(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_wifi_dump(n);

  strcpy(cmd, "cat /tmp/datoswifi|grep \"(\"|cut -d\" \" -f3|grep -v bytes");
 
  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      if(buf[i] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

  if(strcmp(buf, "") == 0) return "Desconectado";

  return strdup(buf);

}

char * net_wifi_get_signal(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;
  if(VARS.emergencyexit == 1) return "Parado";
  net_wifi_dump(n);

  strcpy(cmd, "cat /tmp/datoswifi|grep \"sign\"|cut -f2 -d\":\"");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;
	  if(VARS.emergencyexit == 1) return "Parado";
      if(buf[0] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);


  if(strcmp(buf, "") == 0) return "Desconectado";

  return strdup(buf);
}

char * net_get_ip(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_dump(n);

  strcpy(cmd, "cat /tmp/datosnet|grep inet|grep global|cut -d\"/\" -f1|cut -d\"t\" -f2");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      if(buf[0] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

  if(strcmp(buf, "") == 0) return "Desconectado";

  return strdup(buf);
}

char * net_get_mac(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_dump(n);

  strcpy(cmd, "cat /tmp/datosnet|grep \"link/\"|cut -d\"r\" -f2|cut -d\" \" -f2");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      if(buf[i] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);

  if(strcmp(buf, "") == 0) return "no mac";
  return strdup(buf);
}

char * net_get_gateway(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_dump(n);
  strcpy(cmd, "ip route show dev ");
  strcat(cmd, n->interf);
  strcat(cmd, "|grep default|cut -d\" \" -f3");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      if(buf[i] == ' ') buf[i] = '\r';
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);


  if(strcmp(buf, "") == 0) return "no pasarela";

  return strdup(buf);
}

void net_activate_interf(NET *n)
{
 char cmd[80] = {'\0'};
 FILE *f = NULL;

 strcpy(cmd, "ip link set ");
 strcat(cmd, n->interf);
 strcat(cmd, " up");

 f = popen(cmd, "r");
 if(f == NULL) ; else
 {
  printf("Activando tarjeta de red %s\n", n->interf); 
  sleep(1);
 }
 pclose(f);

}


void net_deactivate_interf(NET *n)
{
 char cmd[80] = {'\0'};
 FILE *f = NULL;

 strcpy(cmd, "ip link set ");
 strcat(cmd, n->interf);
 strcat(cmd, " down");

 f = popen(cmd, "r");
 if(f == NULL) ; else
 {
  printf("Desactivando tarjeta de red %s\n", n->interf); 
  sleep(2);
 }
 pclose(f);

 net_flush_ip(n); /* Liberar la ips de la tarjeta evita problemas de parseo */
 sleep(1);
}

void net_flush_ip(NET *n)
{
    char cmd[80] = {'\0'};
    FILE *f = NULL;

    strcpy(cmd, "ip address flush dev ");
    strcat(cmd, n->interf);
    strcat(cmd, " scope global");

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Liberando IP en %s\n", n->interf, n->ip); 
    }

   pclose(f);


}

void net_set_ip(NET *n, const char *ip)
{
   strcpy(n->ip, ip);
}

void net_set_mac(NET *n, const char *mac)
{
   strcpy(n->mac, mac);
}

void net_apply_config(NET *n)
{
    char cmd[80] = {'\0'};
    FILE *f = NULL;
	char *c;

	if(VARS.emergencyexit == 1) return;
    net_deactivate_interf(n);
	if(VARS.emergencyexit == 1) return;

    /* Poner MAC */
    strcpy(cmd, "ip link set dev ");
    strcat(cmd, n->interf);
    strcat(cmd, " address ");
    strcat(cmd, n->mac);

    f = popen(cmd, "r");
    if(f == NULL) return; else
    {
     printf("Definiendo MAC via ip en %s = %s\n", n->interf, n->mac); 
    }
   while(!feof(f)) c = fgetc(f);
   pclose(f);
   if(VARS.emergencyexit == 1) return;

   strcpy(cmd, "ifconfig  ");
   strcat(cmd, n->interf);
   strcat(cmd, " hw ether ");
   strcat(cmd, n->mac);

   f = popen(cmd, "r");
   if(f == NULL) return; else
   {
    printf("Definiendo MAC via ifconfig en %s = %s\n", n->interf, n->mac); 
   }
   while(!feof(f)) c = fgetc(f);
   pclose(f);
   if(VARS.emergencyexit == 1) return;

 
  /*--------------------------------------------------------------------*/

    /* Poner IP */
    net_flush_ip(n);
    strcpy(cmd, "ip address change global ");
    strcat(cmd, n->ip);
    strcat(cmd, "/24 dev ");
    strcat(cmd, n->interf);

    f = popen(cmd, "r");
    if(f == NULL) return; else
    {
     printf("Definiendo IP via ip en %s = %s\n", n->interf, n->ip); 
    }
   while(!feof(f)) c = fgetc(f);
   pclose(f);

    strcpy(cmd, "ifconfig ");
    strcat(cmd, n->interf);
    strcat(cmd, " ");
    strcat(cmd, n->ip);

    f = popen(cmd, "r");
    if(f == NULL) return; else
    {
     printf("Definiendo IP via ifconfig en %s = %s\n", n->interf, n->ip); 
    }
   while(!feof(f)) c = fgetc(f);
   pclose(f);

  /*------------------------------------------------------------------------*/
  /* Poner rutas */

    strcpy(cmd, "route add default gw ");
    strcat(cmd, n->gateway);
    strcat(cmd, " dev ");
    strcat(cmd, n->interf);

    f = popen(cmd, "r");
    if(f == NULL) return; else
    {
     printf("Definiendo pasarela via route en %s = %s\n", n->interf, n->gateway); 
    }
   while(!feof(f)) c = fgetc(f);
    pclose(f);

    strcpy(cmd, "ip route add default via ");
    strcat(cmd, n->gateway);
    strcat(cmd, " dev ");
    strcat(cmd, n->interf);

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Definiendo pasarela via ip en %s = %s\n", n->interf, n->gateway); 
    }
   while(!feof(f)) c = fgetc(f);
    pclose(f);

   /*--------------------------------------------------------------------------*/
   /* Poner mtu */
   strcpy(cmd, "ifconfig ");
    strcat(cmd, n->interf);
    strcat(cmd, " mtu ");
    strcat(cmd, n->mtu);

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Definiendo MTU via ifconfig en %s = %s\n", n->interf, n->mtu); 
    }
   while(!feof(f)) c = fgetc(f);
    pclose(f);

    strcpy(cmd, "ip link set dev ");
    strcat(cmd, n->interf);
    strcat(cmd, " mtu ");
    strcat(cmd, n->mtu);

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Definiendo MTU via ip en %s = %s\n", n->interf, n->mtu); 
    }
   while(!feof(f)) c = fgetc(f);
    pclose(f);

	if(VARS.emergencyexit == 1) return;

    net_activate_interf(n);
    if(VARS.emergencyexit == 1) return;

}

void net_stop_nmanager(void)
{
    char cmd[80] = {'\0'};
    FILE *f = NULL;

    strcpy(cmd, "systemctl stop network-manager");

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Cerrando administrador de red NetworkManager\n"); 
    }

   pclose(f);
   sleep(2);
}

void net_start_nmanager(void)
{
    char cmd[80] = {'\0'};
    FILE *f = NULL;

    strcpy(cmd, "systemctl start network-manager");

    f = popen(cmd, "r");
    if(f == NULL) ; else
    {
     printf("Corriendo administrador de red NetworkManager\n\n"); 
    }

   pclose(f);
   sleep(2);
}

void net_set_gateway(NET *n, const char *gateway)
{
  strcpy(n->gateway, gateway);
}


const char **net_get_dns_list()
{
  char cmd[80] = {'\0'};
  FILE *f = NULL;
  static const char *lst[100];
  int k = 0, i = 0;
  char buf[2000] = {'\0'};

  strcpy(cmd, "cat /etc/resolv.conf |grep -v search|grep -v \"#\"|cut -d\" \" -f2");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;

  while(!feof(f))
  {
    buf[k] = fgetc(f);
    if(buf[k] == EOF) { buf[k] = '\0'; buf[k - 1] = '\0' /* Quia linea blanco */; break; }

   ++k;
  }

  pclose(f);

  puts("=== DNS archivo local ===");
  puts(buf);
  puts("===== DNS a bufer =======");

  lst[i] = strtok(buf, "\n");  
  while(lst[i] != NULL)
  {
    lst[++i] = strtok(NULL, "\n");
  }

 return lst;

}

char * net_get_mtu(NET *n)
{
  char cmd[80] = {'\0'};
  char buf[1024] = {'\0'};
  int i = 0;
  FILE *f = NULL;

  net_dump(n);
  strcpy(cmd, "cat /tmp/datosnet |grep \"mtu\"|cut -d\">\" -f2|cut -d\" \" -f3");

  f = popen(cmd, "r");

  if(f == NULL)
   return NULL;
  else
  {
    while(!feof(f))
    {
      buf[i] = fgetc(f);
      if(buf[i] == EOF)
       break;

      /*if(buf[i] == ' ') buf[i] = '\r';*/
      if(buf[i] == '\n') buf[i] = '\r';

      ++i;
    }
    buf[i] = '\0';
  }
  pclose(f);


  if(strcmp(buf, "") == 0) return "no mtu";

  return strdup(buf);
}
  

void net_set_mtu(NET *n, const char *mtu)
{
  strcpy(n->mtu, mtu);
}

void net_wifi_set_ssid(NET *n, const char *ssid)
{
   strcpy(n->w.essid, ssid);
   
}

void net_wifi_asoc_open(NET *nt)
{
   char cmd[80] = {'\0'};
   FILE *f = NULL;

   strcpy(cmd, "iw dev ");
   strcat(cmd, nt->interf);
   strcat(cmd, " connect ");
   strcat(cmd, nt->w.essid);


   f = popen(cmd, "r");
   if(f == NULL) 
   {
       puts("Comando iw no encontrado.");
      return;
   }

  printf("Asociando con iw a %s.\n", nt->w.essid);
  while(!feof(f))
  {
     fgetc(f);
  }
  pclose(f);

   strcpy(cmd, "iwconfig ");
   strcat(cmd, nt->interf);
   strcat(cmd, " essid ");
   strcat(cmd, nt->w.essid);


   f = popen(cmd, "r");
   if(f == NULL) 
   {
       puts("Comando iwconfig no encontrado.");
      return;
   }

  printf("Asociando con iwconfig a %s.\n", nt->w.essid);
  while(!feof(f))
  {
     fgetc(f);
  }
  pclose(f);

}

void net_wifi_deasoc(NET *nt)
{
   char cmd[80] = {'\0'};
   FILE *f = NULL;

   strcpy(cmd, "iw dev ");
   strcat(cmd, nt->interf);
   strcat(cmd, " disconnect ");



   f = popen(cmd, "r");
   if(f == NULL) 
   {
       puts("Comando iw no encontrado.");
      return;
   }

   printf("Desasociando con iw.\n");
   while(!feof(f))
   {
     fgetc(f);
   }

   strcpy(cmd, "iwconfig ");
   strcat(cmd, nt->interf);
   strcat(cmd, " essid ");
   strcat(cmd, "off");

 
   f = popen(cmd, "r");
   if(f == NULL) 
   {
       puts("Comando iwconfig no encontrado.");
      return;
   }

  printf("Desasociando con iwconfig.\n");
  while(!feof(f))
  {
     fgetc(f);
  }
  pclose(f);


}

void net_clean_dns_file(NET *nt)
{
   FILE *f = NULL;

   /* Vaciar el archivo de dns */
   f = fopen(nt->resolvfile, "w");
   if(f == NULL) 
   {
       printf("No se pudo escribir en %s.\n", nt->resolvfile);
      return;
   }
   fprintf(f, "");
   fclose(f);
}

void net_add_dns(NET *nt, const char *dns)
{
   FILE *f = NULL;

   /*------------------------------------------*/

   f = fopen(nt->resolvfile, "a");
   if(f == NULL) 
   {
       printf("No se pudo escribir en %s.\n", nt->resolvfile);
      return;
   }

   fprintf(f, "nameserver %s\n", dns);
   fclose(f);
  
}

void sys_pkill(const char *process)
{
   FILE *f;
   char cmd[250] = {'\0'};

   strcpy(cmd, "pkill ");
   strcat(cmd, process);

   f = popen(cmd, "r");

   if(f == NULL) return;

   while(!feof(f)) fgetc(f);
   fclose(f);
}

#endif
