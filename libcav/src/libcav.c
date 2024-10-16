#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/libcav.h"

typedef struct {
  char *memory;
  size_t size;
} RespMemory;

static size_t write_mem(void *cont, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    RespMemory *mem = (RespMemory *)userp;

    size_t req_size = mem->size + real_size + 1; 
    
    char *ptr = realloc(mem->memory, req_size);
    if (!ptr) {
	fprintf(stderr, "! Not enough memory (realloc returned NULL)\n");
	return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), cont, req_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;

    return real_size;
}

int get_request(char *url, char* resp, size_t resplen, int status_code) {
    CURL *curl;
    CURLcode resp_code;
    RespMemory chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
	fprintf(stderr, "! CURL failed to easy initialize\n");
	free(chunk.memory);
	return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "cav @ libcurl-agent/1.0");

    resp_code = curl_easy_perform(curl);
    if (resp_code != CURLE_OK) {
	fprintf(stderr, "! CURL failed to perform: %s\n", curl_easy_strerror(resp_code));
	free(chunk.memory);
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return -1;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
    
    printf("CURL: received %lu bytes of content with a status of %d\n", chunk.size, status_code);
    strlcpy(resp, chunk.memory, resplen);
    
    return 0;
}
