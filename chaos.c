#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <complex.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/resource.h>


//======User Definables==========//
int xSize=1680; //1680 by 2 bytes/pixel. xSize and ySize should match the size of your frame buffer.
int ySize=1056;
long double yMin=-10;
long double yMax=10;
long double xMin=-15.9090909090;
long double xMax; //=xMin+((long double) (xSize))/ySize*(yMax-yMin)
	  //xMax is defined in main to keep the axes on the same scale.
long double convergenceEpsilon=0.0001L;
#define f(z) csinl(1/z)

char fb_dir[10]="/dev/fb0"; //path to frame buffer.
//===============================//


int scrn_fd; //File descriptor for the framebuffer.
unsigned char* black;
unsigned char* fb;

void killHandler(int sig) {
	//Save framebuffer to file here.
	int saveFile=open("saved_fb",O_WRONLY|O_TRUNC|O_CREAT);
	write(saveFile,fb,xSize*ySize*2);
	close(saveFile);
	//Gracefully blackens the screen before killing the program to avoid GUI bleedthroughs...
	lseek(scrn_fd,0,SEEK_SET);
	write(scrn_fd,black,xSize*ySize*2);
	close(scrn_fd);
	exit(0);
}

int main() {
	printf("Hello\n");

	xMax=xMin+((long double) (xSize))/ySize*(yMax-yMin);
	printf("x:%Lf %Lf\ny:%Lf %Lf",xMin,xMax,yMin,yMax);

	struct rlimit rl;
	getrlimit(RLIMIT_STACK,&rl);
	rl.rlim_cur=100L*1024L*1024L; //100MB stack size, since I am storing a framebuffer's worth of complex long doubles.
	setrlimit(RLIMIT_STACK,&rl);

	complex long double x[xSize*ySize];
	unsigned char _black[xSize*ySize*2]; black=((unsigned char*) (_black));
	unsigned char _fb[xSize*ySize*2]; fb=((unsigned char*) (_fb));
	unsigned char white[xSize*ySize*2];
	int i;
	for(i=0;i<xSize*ySize*2;i++) {
		white[i]=0xFF;
		fb[i]=0xFF;
		black[i]=0;
	}
	for(i=0;i<xSize*ySize;i++) {
		x[i]=xMin+((long double) (xMax-xMin))/(xSize-1)*(i%xSize)+I*(yMax-((long double) (yMax-yMin))/(ySize-1)*(i/xSize)); //Box with Re(x) in [xMin,xMax], and Im(x) in [yMin,yMax].
	}

	scrn_fd=open(fb_dir, O_RDWR|O_SYNC); //FrameBuffer
	signal(SIGINT,killHandler); //Exiting Gracefully

	/*printf("%Lf+%Lfi\n",creall(x[0]),cimagl(x[0]));
	printf("%Lf+%Lfi\n",creall(x[xSize-2]),cimagl(x[xSize-2]));
	printf("_%Lf+%Lfi\n",creall(x[xSize-1]),cimagl(x[xSize-1]));
	printf("%Lf+%Lfi\n",creall(x[xSize]),cimagl(x[xSize]));
	printf("%Lf+%Lfi\n",creall(x[2*xSize-2]),cimagl(x[2*xSize-2]));
	printf("_%Lf+%Lfi\n",creall(x[2*xSize-1]),cimagl(x[2*xSize-1]));
	printf("%Lf+%Lfi\n",creall(x[2*xSize]),cimagl(x[2*xSize]));*/


	//whiten the screen.
	lseek(scrn_fd,0,SEEK_SET);
	write(scrn_fd,white,xSize*ySize*2);

	while(1) {
		complex long double tmp;
		lseek(scrn_fd,0,SEEK_SET);
		for(i=0;i<xSize*ySize;i++) {
			tmp=f(x[i]);
			if(cabsl(x[i]-tmp)<convergenceEpsilon)
				fb[2*i]=fb[2*i+1]=0;
				//write(scrn_fd,black,2);
			else
				fb[2*i]=fb[2*i+1]=0xFF;
				//write(scrn_fd,white,2);
			x[i]=tmp;
		}
		write(scrn_fd,fb,xSize*ySize*2);
		//printf(".");
	}

	return 0;
}


