#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "record.h"
#include "echo_post.h"
#include "echo_debug.h"
#include "queue.h"
#include "vad.h"

#define PCM_BUFF_SIZE 		AUDIO_BUFF_SIZE // http一次上传的pcm数据大小
#define VAD_BUFF_SIZE		320
#define MAX_NO_VOICE_COUNT  20
#define BUFF_SIZE 			640			 //每次调用record_pcm_start的pcm数据大小
#define MAX_VOICE_COUNT		(PCM_BUFF_SIZE / BUFF_SIZE)

LinkQueue* gRecordQueue;//存放pcm数据的队列
int pcm_record_count;
int voice_start;
void record_pcm()
{

	int no_voice_count = 0;
	int voice_count = 0;
	int voice_stop = 0;
	FILE *fpOutput;
	//short vad_buff[VAD_BUFF_SIZE] = {0};
	VadInst* vad_handle = NULL;
	int mode = 0; // 模式
	// 1.初始化 设置模式
	WebRtcVad_Create(&vad_handle);
	WebRtcVad_Init(vad_handle);
	WebRtcVad_set_mode(vad_handle,mode);
	fpOutput = fopen("./aa.pcm","wb");
	if(fpOutput == NULL)
	{
		ECHO_DBG("open fpOutput file err!!!!\n");
		exit(1); 
	}

	int rc, size = BUFF_SIZE;
	char* buff = malloc(BUFF_SIZE);
	if(buff == NULL)
	{
		fprintf(stderr, "malloc buff err!!!\n");
		exit(1);
	}
	record_pcm_init();
	int ret = 0;
	char *pcm_buff = NULL;
	
	while(1){
		pcm_buff = malloc(PCM_BUFF_SIZE);
		if(pcm_buff == NULL)
		{	
			ECHO_DBG("malloc pcm_buff err!!\n");
			break;
		}
		for(int i = 0; i < MAX_VOICE_COUNT; i++)
		{
			memset(buff, 0 ,BUFF_SIZE);
			record_pcm_start(buff);	
			memcpy(pcm_buff + BUFF_SIZE * i, buff, BUFF_SIZE);
			rc = fwrite(buff, 1, BUFF_SIZE ,fpOutput);
			if (rc != size) fprintf(stderr, "Short write: wrote %d bytes\n", rc);
			//memcpy(vad_buff, buff, BUFF_SIZE);
			ret = vad_main_process(vad_handle, VAD_BUFF_SIZE, 16000, (short*)buff);//检测一帧数据是否有声音
			if(ret == 0){
				no_voice_count++;//没有声音
				voice_count = 0;
			}
			else{
				voice_count++;//有声音
				no_voice_count = 0;
			}
			if(!voice_start && (voice_count >= MAX_VOICE_COUNT)){
				ECHO_DBG("Recode Start !!!\n");
				voice_start = 1;
				voice_stop = 0;
			}

			if(voice_start && (no_voice_count >= MAX_NO_VOICE_COUNT))//stop recode
			{
				ECHO_DBG("Recode Stop !!!\n");
				push_linkqueue(gRecordQueue, "record_end");
				voice_stop = 1;
				voice_start = 0;
				break;
			}
		}
		
		if(voice_start)
		{
			push_linkqueue(gRecordQueue, pcm_buff);//pcm_buff数据入队
			pcm_record_count++;
			printf("pcm_data address = %p, count = %d\n", pcm_buff, pcm_record_count);
		}
		if(voice_stop)
			break;

	}

	WebRtcVad_Free(vad_handle);
	fclose(fpOutput);


	record_pcm_stop();
	
}


int main(void)
{
	pthread_t thread_record, thread_httpc;
	
	gRecordQueue = create_empty_linkqueue();
	if(gRecordQueue == NULL) {
		ECHO_DBG("gRecordQueue create failed.");
		return;
	}
	
    //建立两个线程，通过gRecordQueue队列去传送数据。
    int ret = pthread_create(&thread_record, NULL, (void *)record_pcm, NULL);
	if(ret != 0){
		ECHO_DBG("Create pthread error!\n");
		exit(1); 
	}
	while(!pcm_record_count){NULL;}
	ret = pthread_create(&thread_httpc, NULL, (void *)httpc_post, NULL);
	if(ret != 0){
		ECHO_DBG("Create pthread error!\n");
		exit(1); 
	}
	
	pthread_join(thread_record, NULL); 
	pthread_join(thread_httpc, NULL); 
	return 0;
}

