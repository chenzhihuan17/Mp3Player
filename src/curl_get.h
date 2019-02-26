#ifndef _CURL_GET_H
#define _CURL_GET_H

struct FtpFile {
  const char *filename;
  FILE *stream;
};

int get_http_content(void* url);


#endif //_CURL_GET_H


