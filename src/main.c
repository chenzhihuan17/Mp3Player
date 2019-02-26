#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "curl_get.h"
#include "debug.h"
#include "curl_get.h"

int main(void)
{
	pthread_t thread_get;
	#if 0
	gRecordQueue = create_empty_linkqueue();
	if(gRecordQueue == NULL) {
		ECHO_DBG("gRecordQueue create failed.");
		return;
	}
	#endif
	char *url = "http://protected-hd-01.0mzl.com/cia/musics/dvOUZDcmgOWHJlr8E2Y21fiBQVtu1UacidXLFCrrZq2ZXd7oiVhncTIFb6Q46S3c.mp3";
	int ret = pthread_create(&thread_get, NULL, (void *)get_http_content, (void*)url);
	if(ret != 0){
		CZH_DBG("Create pthread error!\n");
		exit(1); 
	}

	pthread_join(thread_get, NULL); 
	return 0;
}

