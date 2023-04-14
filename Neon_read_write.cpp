#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <sched.h>
#include <unistd.h>

//using namespace std;
/**
 * 编译：aarch64-linux-android26-clang base.cpp
 * 
 */

char sysMonApp[] = "/data/local/tmp/sysMonApp";
char logfilename[] = "/data/local/tmp/1.log";

typedef enum{
   CPU_CORE = 1,
   RUNTIME_SEC,
   READ_WRITE_MODE,
   MEM_BLOCK,
   LOOP_NUM,
   FILENAME_PRO,
}ARGS;
//0 0 0 0 0 log.txt log1.txt 
typedef struct{
   int cpu_core_idx;    // 0-7
   int runtime_sec;    //运行的程序类型（1 -- CPU, 2 -- GPU, 3 -- DSP)
   int read_write;	   //读（1），写（2），读写（3）
   int mem_block;	   //读写内存块大小 4K *small_block bytes,最大不能超过32M(<=8192)
   int Loop_num;	   //读写次数
   char* logfile4pro; //用于程序自身的记录
   int pid;
} Inputs;

void get_arm_load(const Inputs* paras){
   char cmd1[1024];	
   char cmd2[1024];	  
   char cmdx[1024];
   sprintf(cmdx, "echo =====ARM FREQ/LOAD===== >> %s", paras->logfile4pro);
   system(cmdx);
   sprintf(cmd1,"cat /sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq >> %s",paras->cpu_core_idx,paras->logfile4pro);
   sprintf(cmd2,"ps -eo pid,%%cpu,CMD --pid %d --sort=-%%cpu >> %s",paras->pid,paras->logfile4pro);
   system(cmd1);
   system(cmd2);
}

//从输入参数初始化参数列表
void init_paras(Inputs *paras, int argc, char* argv[]){
   if(argc != 7){
      printf("error args number");
      exit(0);
   }
   paras->cpu_core_idx = atoi(argv[CPU_CORE]);
   paras->runtime_sec = atoi(argv[RUNTIME_SEC]);
   paras->read_write = atoi(argv[READ_WRITE_MODE]);
   paras->mem_block = atoi(argv[MEM_BLOCK]);
   paras->Loop_num = atoi(argv[LOOP_NUM]);
   
   paras->logfile4pro = argv[FILENAME_PRO];
   paras->pid = getpid();
   printf("PID==%d\n",  paras->pid);
}

void set_system(const Inputs* paras)
{
	int i;
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(paras->cpu_core_idx, &set);   //绑定CPU	
	{
		char cmd[1024];
		sprintf(cmd,"taskset -ap %02x %d",1<<paras->cpu_core_idx,paras->pid);
		printf("cmd=%s\n", cmd);
		system(cmd);			
		
	}	
}
#ifdef __cplusplus
extern "C"{
#endif
void neon2_read(int x0,int x1,int x2,char *in,char *out);
void neon2_write(int x0,int x1,int x2,char *in,char *out);
void neon2_read_write(int x0,int x1,int x2,char *in,char *out);
#ifdef __cplusplus
}
#endif




int main(int argc, char* argv[])
{
   Inputs paras;
   time_t t,t1,t2,startime;
   char message[1024];
   int i;
   int No_loop = 0;
   int res = 0;
   int ret;
   float data[4] = {0.0,5.0,3.0,1.0};
   int mem_size;
   char *pi1,*po1,*pi,*po;
   
   init_paras(&paras, argc, argv);
   sprintf(message,"echo  param: %s %s %s %s %s > %s",argv[1],argv[2],argv[3],argv[4],argv[5],paras.logfile4pro);
   system(message);
   
   mem_size = paras.mem_block * 1024;
   pi1 = (char *)malloc(mem_size+256+1024);//32M
   po1 = (char *)malloc(mem_size+256+1024);//32M
   pi = (char *)(((long)pi1)&0xffffffffffffff00);
   po = (char *)(((long)po1)&0xffffffffffffff00);
   // for(i = 0;i<mem_size+1024;i++)
   // {
	//    pi[i] = i;
	//    po[i] = 0;//3
   // }	  
   
//--------系统设定---------------
  set_system(&paras);
//--------获取开始时间----------------
   t = time(NULL);
   t1 = t;
   sprintf(message,"echo curetime %ld >> %s",t,paras.logfile4pro);  //记录开始时间
   system(message);
//--------获取ARM核工作频率，工作占比---------
   get_arm_load(&paras);
   printf("===================Start 3==============\n");
   usleep(1*1000*1000);
   printf("===================Start 2==============\n");
   usleep(1*1000*1000);
   printf("===================Start 1==============\n");
   usleep(1*1000*1000);
   //startime = t = time(NULL);	 
   while(1)
   {
	  if(paras.read_write == 1) {
         neon2_read(8*paras.mem_block,paras.Loop_num,1,pi,po);
     }
	  if(paras.read_write == 2)
		  neon2_write(8*paras.mem_block,paras.Loop_num,1,pi,po);
	  if(paras.read_write == 3)
		  neon2_read_write(8*paras.mem_block,paras.Loop_num,1,pi,po);
     return 0;

 	  No_loop++;											//循环次数++
	  t2 = time(NULL);										//获取时间		
      if(t2>t+paras.runtime_sec)							//超过预计测试时间长度			
			break;
	  if((No_loop%100)==0)									//没100次循环记录一次时间，分析运行均匀度
	  {
	      sprintf(message,"echo curtime %ld No_loop %d >> %s",t2,No_loop,paras.logfile4pro);  //记录开始时间
		  system(message);
	  }
   }
   printf("===================End==============\n");
   usleep(30*1000*1000);
   printf("===================EXIT==============\n");




//--------获取结束时间-----------------------   
   t = time(NULL);
//--------获取ARM核工作频率，工作占比---------
   get_arm_load(&paras);

   
   sprintf(message,"echo startime %ld endtime %ld >> %s",startime,t,paras.logfile4pro);  //记录开始时间
   system(message);

   sprintf(message,"echo No_loop %d %d >> %s",No_loop,res,paras.logfile4pro);  //记录开始时间
   system(message);
   
    for(i = 0;i<paras.mem_block * 1024+16;i++)
   {
	   if(pi[i] != po[i])
	   {
		   printf("not same!%d %d %d\n",i,pi[i],po[i]);
		   break;
	   }
   }	  
   printf("same\n");
   return 0;
}