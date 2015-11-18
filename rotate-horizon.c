#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/***** 32 pixels along X, 16 along Y ********/
/***** Each pixel has 3 bytes, UV, Green, Blue *****/
/******Stored along X. then along Y. That is if <X,Y> then 0,0 ---> 31,0 ***/
/******then 0,1 ----> 31,1..... 0,15 ----> 31,15 ***************/
/****** 0,0 top left and 0,15 bottom left ****************/

void read_base_panel(void);
void write_base_panel(void);
double find_horizon_rotation(int);
double find_pixel_rotation(int,int);
double find_pixel_modulus(int,int);
char find_pixel_color(double,double);

FILE *fp1,*fp2;
int sample;
int frame=1;
double angle_pixel,angle_time,angle_differential;
double modulus_pixel;
int ch_U,ch_G,ch_B;
char base_panel[40][40];


/********************************************************************/
main()
{

int X,Y;
int T; /* in 1 msec, i.e. 1000 frame per sec. */
char C; /*Pixel color */

 read_base_panel();
// write_base_panel();
// exit(0);

 fp1=fopen("rotate-horizon-5sec.img", "w+");
 fp2=fopen("rotate-horizon-5sec.values", "w+");

 for(T=0;T<5000;T++)
 {
  angle_time=find_horizon_rotation(T);
//  printf("%f\n",angle_time);
  for(Y=0;Y<16; Y++)
  {
   for(X=0;X<32; X++)
   {
    angle_pixel=find_pixel_rotation(X,Y);

	modulus_pixel=sqrt((((double)X-15.5)*((double)X-15.5))
	                   +(((double)Y-7.5)*((double)Y-7.5)));

    angle_pixel=angle_pixel+angle_time;
    while(angle_pixel<0.0)
     angle_pixel+=2.0*M_PI;
    while(angle_pixel>2.0*M_PI)
     angle_pixel-=2.0*M_PI;
	C=find_pixel_color(angle_pixel,modulus_pixel);

    if(C=='s')
    {
     ch_U=255;
     ch_G=0;
     ch_B=0;
    }
    else if(C=='g')
    {
     ch_U=0;
     ch_G=255;
     ch_B=0;
    }
    else if(C=='c')
    {
     ch_U=127;
     ch_G=0;
     ch_B=100;
    }
    else
    {
     printf("Yikes\n");
    }
     
    fputc(ch_U, fp1); // Each character is an int 0 to 255, hence a byte 
    fputc(ch_G, fp1); // Each character is an int 0 to 255, hence a byte 
    fputc(ch_B, fp1); // Each character is an int 0 to 255, hence a byte 
    if(C=='s')
     fprintf(fp2,"+ ");
    else if(C=='g')
     fprintf(fp2,"  ");
    else if(C=='c')
     fprintf(fp2,"  ");
    //fprintf(fp2,"%d|%d|%d ",ch_U,ch_G,ch_B);
    //fprintf(fp2,"%f ",angle_pixel);
   }
   fprintf(fp2,"\n");
  }
  fprintf(fp2,"Frame=%d *********************************************************\n", frame);
 frame++;
 }

 fclose(fp1);
 fclose(fp2);

}

/********************************************************************/

void read_base_panel( void)
{
 int X,Y;
 FILE *fp_p;

 fp_p=fopen("panel.base","r");
 for(Y=0;Y<41; Y++)
 {
  for(X=40;X>-1; X--)
   fscanf(fp_p,"%c",&(base_panel[X][Y]));
  fscanf(fp_p,"\n");
 }
 fclose(fp_p);
 printf("Done reading\n");
}

/********************************************************************/

void write_base_panel(void)
{
 int X,Y;
 FILE *fp_p;

 fp_p=fopen("panel.base_new","w+");

 for(X=0;X<41; X++)
 {
  for(Y=0;Y<41; Y++)
   fprintf(fp_p,"%c",base_panel[X][Y]);
  fprintf(fp_p,"\n");
 }
 fclose(fp_p);
 printf("Done writing\n");
}

/********************************************************************/

double find_horizon_rotation(int t)
{
 double angle;
 double t_;

 if(t<=1250)
 {
  t_=t;
  t_=t_/1250.0;
  angle=atan2(15.5*(t_), 7.5);
 }
 else if(t<=2500)
 {
  t_=t-1250;
  t_/=1250.0;
  angle=atan2(15.5*(1.0-t_), 7.5);
 }
 else if(t<=3750)
 {
  t_=t-2500;
  t_/=1250.0;
  angle=atan2(-15.5*(t_), 7.5);
 }
 else
 {
  t_=t-3750;
  t_/=1250.0;
  angle=atan2(-15.5*(1.0-t_), 7.5);
 }

 while(angle<0.0)
  angle+=2.0*M_PI;
 while(angle>2.0*M_PI)
  angle-=2.0*M_PI;

 return angle;

}

/********************************************************************/

double find_pixel_rotation(int y,int x) /*coordinates rotated*/
{
 double angle;

 angle=atan2(((double)y-15.5),((double)x-7.5));

 while(angle<0.0)
  angle+=2.0*M_PI;
 while(angle>2.0*M_PI)
  angle-=2.0*M_PI;

 return angle;

}

/********************************************************************/

char find_pixel_color(double theta,double r)
{
 char c;
 int x,y;

 x=round(21.0+r*(cos(theta)));
 y=round(21.0+r*(sin(theta)));
 c=base_panel[x][y];
 return c;

}
