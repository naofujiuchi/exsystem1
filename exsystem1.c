#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 100

//create type 'System'
typedef struct{
  int proc;
  int multi;
  int delta;
  int vai;
}System;

//declare global variant
char systemname[10] = "cutting";
char mhorhat[10] = "hat";
double am0 = 0.0013;
double ad0 = 0.005;
double a = 1;
int pp = 200;
double ldw[13] = {0, 0.006726142, 0.008112798, 0.01781992, 0.044270472, 0.090650433, 0.154908796, 0.229757567, 0.302671767, 0.355889428, 0.3664116, 0.306002344, 0.141188735};
double dwr[13] = {0, 0.144246391, 0.109104463, 0.084636357, 0.069466302, 0.062218532, 0.061517275, 0.065986765, 0.074251231, 0.084934906, 0.096662019, 0.108056803, 0.117743488};
double ha[13] = {0, 8.433876504, 8.936554696, 9.841160263, 11.00765187, 12.29598818, 13.56612786, 14.67802957, 15.49165198, 15.86695376, 15.66389356, 14.74243006, 12.96252192};
double mh[13];
double hat[13];

/*
//read data
char filename[] = "leafdw1-12.txt";
FILE *fp;
double c;
if((fp = fopen(filename, "r")) == NULL){
  printf("open file error...\n", filename); 
  exit(1);
 }
int i = 1;
while((c = getc(fp)) != EOF){
  
 }
*/

//declare function
static void output(System *sys, double ad, double *out);
static void hfw_plant(double *hfw_p);
static void ha_plant(double *ha_p);
static double vaiplants(System *sys, double ad);
static double getdeltaarea(System *sys);
static void getSystem(System *sys);

double getdeltaarea(System *sys){
  double am;
  double ad;
  am = (am0 * sys->multi) * a / (am0 * sys->multi + ad0 * sys->delta);
  ad = a - am;
  return ad;
}

void getSystem(System *sys){
  if(!strcmp(systemname, "replace")){
    //replace property
    sys->proc = 6;
    sys->multi = 2;
    sys->delta = 4;
    sys->vai = 1;
  }else if(!strcmp(systemname, "recycle")){
    //recycle property
    sys->proc = 14;
    sys->multi = 2;
    sys->delta = 12;
    sys->vai = 5;
  }else if(!strcmp(systemname, "cutting")){ 
    //cutting property
    sys->proc = 14;
    sys->multi = 2;
    sys->delta = 12;
    sys->vai = 10;
  }
}

double vaiplants(System *sys, double ad){
  double vaip;
  vaip = pp * ad * sys->vai / sys->delta;
  return vaip;
}

void hfw_plant(double *hfw_p){
  int scheme;
  int i;
  int j;
  for(scheme = 1; scheme <= 10; scheme++){
    for(i = 1; i <= scheme; i++){
      j = 12 - (i - 1);
      hfw_p[scheme] = hfw_p[scheme] + ldw[j] / dwr[j];
    }
  }
}

void ha_plant(double *ha_p){
  int scheme;
  int i;
  int j;
  for(scheme = 1; scheme <= 10; scheme++){
    for(i = 1; i <= scheme; i++){
      j = 12 - (i - 1);
      ha_p[scheme] = ha_p[scheme] + ldw[j] * ha[j];
    }
  }
}

void output(System *sys, double ad, double *out){
  int i;
  double hfw_p[13];
  double ha_p[13];
  for(i = 0; i <= 12; i++){
    hfw_p[i] = 0;
    ha_p[i] = 0;
  }
  hfw_plant(hfw_p);
  ha_plant(ha_p);
  if(!strcmp(mhorhat, "mh")){
    for(i = 1; i <= 10; i++){
      out[i] = hfw_p[i] * vaiplants(sys, ad);
    }
  }else if(!strcmp(mhorhat, "hat")){
    for(i = 1; i <= 10; i++){
      out[i] = ha_p[i] * vaiplants(sys, ad);
    }
  }
}

int  main(int argc, const char* argv[]){
  System a;
  double ad;
  double out[11];
  int i;
  getSystem(&a);
  ad = getdeltaarea(&a);
  output(&a, ad, out);
  for(i = 1; i <= 10; i++){
    printf("%f\n", out[i]);
  }
  return 0;
}
