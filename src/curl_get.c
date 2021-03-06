/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Multiplexed HTTP/2 downloads over a single connection
 * </DESC>
 */

#include <stdlib.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "curl_get.h"

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out = (struct FtpFile *)stream;
  if(out && !out->stream) {
    /* open file for writing */
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }
  return fwrite(buffer, size, nmemb, out->stream);
}

 
int my_progress_func(char *progress_data,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
  //printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0/t);
  return 0;
}

size_t get_size_struct(void* ptr, size_t size, size_t nmemb, void* data){
    return (size_t)(size * nmemb);
}					 
 
int get_http_content(void* url)
{
  CURL *curl;
  CURLcode res, ret;
  struct FtpFile ftpfile = {
    "./download.mp3", /* name to store the file as if successful */
    NULL
  };
  char *url_tmp = (char*)url;
  char *progress_data = "* ";
  printf("url = %s\n", url_tmp);
  curl_global_init(CURL_GLOBAL_DEFAULT); 
  curl = curl_easy_init();
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//显示详细信息
    curl_easy_setopt(curl, CURLOPT_URL, url_tmp);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

    //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);                        //0L打开进度表，1L关闭进度表
    //curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);   //进度条回调函数
    //curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);    //传递给进度回调的自定义指针 
    res = curl_easy_perform(curl);
   	if(CURLE_OK != res) {
	 	/* we failed */
	 	fprintf(stderr, "curl told us %d\n", res);
   	}   

	/* extract the content-type */
    char *ct = NULL;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if(!res && ct) {
      printf("####Content-Type: %s\n", ct);
    }
	/* check the size */
	double cl;
    ret = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cl);
    if(!ret) {
      printf("####Content-Length: %.0f\n", cl);
    }
   	fprintf(stderr, "curl_easy_getinfo %d\n", ret);
	/* always cleanup */
   	curl_easy_cleanup(curl);
  }
  if(ftpfile.stream)
    fclose(ftpfile.stream); /* close the local file */

  curl_global_cleanup();

  return 0;

}




