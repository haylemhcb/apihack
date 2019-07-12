/*
 * apihack.h
 *
 * Copyright (C) 2019 - yo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* Creado por Haylem Candelario Bauza */

#ifndef APIHACK_H
#define APIHACK_H
#include "net.h"
#include <sys/stat.h>

void file_save_data(const char *filename, const char *data);
void sys_tcp_low_latency(void);
void * sys_copy_file(const char *orig, const char *dest);
void * deb_pack_paq(const char *dirstructura, const char *dirfinal);
void sys_wifi_set_country(const char *country); /* US, ES, etc */


void sys_wifi_set_country(const char *country)
{
  char cmd[80] = {'\0'};
  strcpy(cmd, "iw reg set ");
  strcat(cmd, country);
  printf("Nacionalidad puesta en %s\n", country);
  system(cmd);
}


void sys_tcp_low_latency(void)
{
 system("sysctl net.ipv4.tcp_low_latency=1");
}

void * sys_crea_desktop_file(const char *nombre, const char *destino, const char *coment,
                           const char *descripcion, const char *exec,
                           const char *icopath, const char *categor,
                           int terminal /* 0 No  1 Si*/, const char *cmdroot /* NULL o gksu aplicacion, etc */,
                           int notify /* 1 si  0 no*/)
{

  char pth[500] = {'\0'};
  FILE *dk = NULL;

  strcpy(pth, destino);
  strcat(pth, "/");
  strcat(pth, nombre);
  strcat(pth, "/usr/share/applications/");
  strcat(pth, nombre);
  strcat(pth, ".desktop");

  dk = fopen(pth, "w");
  if(dk == NULL) return NULL;

  fprintf(dk, "[Desktop Entry]\n");
  fprintf(dk, "Name=%s\n", nombre);
  fprintf(dk, "Comment=%s\n", coment);
  fprintf(dk, "GenericName=%s\n", nombre);
  fprintf(dk, "X-GNOME-FullName=%s\n", nombre);
  if(cmdroot == NULL)
   fprintf(dk, "Exec=%s\n", nombre);
  else
   fprintf(dk, "Exec=%s %s\n", cmdroot, nombre);

  if(terminal == 1)
   fprintf(dk, "Terminal=%s\n", "true");
  else
   fprintf(dk, "Terminal=%s\n", "false");

  fprintf(dk, "Type=%s\n", "Application");
  fprintf(dk, "Icon=%s\n", icopath);
  fprintf(dk, "Categories=%s;\n", categor);
  fprintf(dk, "StartupWMClass=%s\n", nombre);
  if(notify == 1)
   fprintf(dk, "StartupNotify=%s\n", "true");
  else
   fprintf(dk, "StartupNotify=%s\n", "false");

  fclose(dk);

  return NULL;
}

void * deb_crea_paq(const char *ruta, const char * ejecutableapp,
                  const char *prioridad /* optional */,
                  const char *seccion /* misc, x11, net */,
                  const char *mantenedor /* fulano <ful@gmail.com> */,
                  const char *arquit /* amd64, i686, i386, all */,
                  const char *version /* Version ej 1.0 */,
                  const char *dependencias /* Dependencias ej tcpdump, liballegro */,
                  const char *recomend /* Recomendaciones ej libc6 */,
                  const char *suger /* Sugeridos ej libc6 */,
                  const char *paginaini /*Pag de inicio ej www.google.co */,
                  const char *descorta /* Descripcion corta */,
                  const char *deslarga /* Descripcion corta */
                  )
{
   char cad[200] = {'\0'};
   char controlpath[200] = {'\0'};
   char optpath[200] = {'\0'};
   char ico[200] = {'\0'};
   char binpath[200] = {'\0'};
   char desktoppath[200] = {'\0'};
   char usrpath[200] = {'\0'};
   char sharepath[200] = {'\0'};
   char pixmappath[200] = {'\0'};
   char rutascript[200] = {'\0'};

   
   FILE *control = NULL, *scriptrun = NULL;



  strcpy(cad, ruta);
  strcat(cad, "/");
  strcat(cad, ejecutableapp);
  mkdir(cad, 0777);

  strcpy(optpath, cad);
  strcat(optpath, "/opt");
  mkdir(optpath, 0777);

  strcpy(usrpath, cad);
  strcat(usrpath, "/usr");
  mkdir(usrpath, 0777);

  strcpy(binpath, cad);
  strcat(binpath, "/usr/bin");
  mkdir(binpath, 0777);
	
  strcpy(sharepath, cad);
  strcat(sharepath, "/opt/");
  strcat(sharepath, ejecutableapp);
  mkdir(sharepath, 0777);
  strcat(sharepath, "/share");
  mkdir(sharepath, 0777);

  strcpy(sharepath, cad);
  strcat(sharepath, "/usr/share");
  mkdir(sharepath, 0777);


  strcpy(desktoppath, cad);
  strcat(desktoppath, "/usr/share/applications");
  mkdir(desktoppath, 0777);

  strcpy(pixmappath, cad);
  strcat(pixmappath, "/usr/share/pixmaps");
  mkdir(pixmappath, 0777);
	
  strcat(cad, "/DEBIAN/");
  mkdir(cad, 0777);

  strcpy(controlpath, cad);
  strcat(controlpath, "control");

  control = fopen(controlpath, "w");
  if(control == NULL) return NULL;

   fprintf(control, "Package: %s\n", ejecutableapp);
   fprintf(control, "Priority: %s\n", prioridad);
   fprintf(control, "Section: %s\n", seccion);
   fprintf(control, "Maintainer: %s\n", mantenedor);
   fprintf(control, "Architecture: %s\n", arquit);
   fprintf(control, "Version: %s\n", version);
   fprintf(control, "Depends: %s\n", dependencias);
   fprintf(control, "Recommends: %s\n", recomend);
   fprintf(control, "Suggest: %s\n", suger);
   fprintf(control, "HomePage: %s\n", paginaini);
   fprintf(control, "Description: %s\n", descorta);
   fprintf(control, " %s\n", deslarga);
   fprintf(control, "\n");

  fclose(control);

  /* Crear script de ejecucion */
  strcpy(rutascript, ruta);
  strcat(rutascript, "/");
  strcat(rutascript, ejecutableapp);
  strcat(rutascript, "/usr/bin/");
  strcat(rutascript, ejecutableapp);
  strcat(rutascript, "-run");
  scriptrun = fopen(rutascript, "w");
  if(scriptrun == NULL) return NULL;
  fprintf(scriptrun, "#!/bin/bash\n");
  fprintf(scriptrun, "export PATH=/opt/%s/:$PATH\n", ejecutableapp);
  fprintf(scriptrun, "export LD_LIBRARY_PATH=/opt/%s/:$LD_LIBRARY_PATH\n", ejecutableapp);
  fprintf(scriptrun, "export XDG_DATA_DIRS=/opt/%s/share:$XDG_DATA_DIRS\n", ejecutableapp);
  fprintf(scriptrun, "/opt/%s/%s\n", ejecutableapp, ejecutableapp);
  fclose(scriptrun);
  chmod(rutascript, 0755);

  strcpy(ico, "/opt/");
  strcat(ico, ejecutableapp);
  sys_crea_desktop_file(ejecutableapp, ruta, descorta, deslarga, ejecutableapp, ico, "GNOME", 1, NULL, 1);
  
}


void * sys_copy_file(const char *orig, const char *dest)
{
  FILE *fl = NULL;
  char cmd[1000] = {'\0'};

  strcpy(cmd, "cp -f ");
  strcat(cmd, orig);
  strcat(cmd, " ");
  strcat(cmd, dest);

  fl = popen(cmd, "r");

  if(fl == NULL) return NULL;

	while(!feof(fl))
	{
		fgetc (fl);
	}
		
	pclose(fl);


}

void * deb_pack_paq(const char *dirstructura, const char *dirfinal)
{
  char cmd[2000] = {'\0'};
  FILE *fl = NULL, *svf = NULL;
  char c = '\0';
  int nc = 0;
	
  strcpy(cmd, "dpkg -b \"");
  strcat(cmd, dirstructura);
  strcat(cmd, "\" \"");
  strcat(cmd, dirfinal);
  strcat(cmd, "\"");

   fl = popen(cmd, "r");
   svf = fopen("/tmp/logpqdeb", "w");

  if(fl == NULL) return NULL;
  if(svf == NULL) return NULL;

	while(!feof(fl))
	{
		c = fgetc (fl);
                if(c == EOF) break;
                fputc(c, svf);
                ++nc;
                if(nc >= 2000) break; 
                if(c == '\0') break;

	}

        fprintf(svf, "\n\nOK Terminado.");
		
	pclose(fl);
        fclose(svf);

}

void file_save_data(const char *filename, const char *data)
{
   FILE *f = NULL;
   f = fopen(filename, "w");
   if(f == NULL) {puts("No se puede crear el archivo para save data"); return;}
   fputs(data, f);
   fclose(f);
}

#endif
