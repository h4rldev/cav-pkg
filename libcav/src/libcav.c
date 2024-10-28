#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "../include/libcav.h"
#include "../include/log.h"

typedef struct {
  char *memory;
  size_t size;
} RespMemory;

static size_t write_mem(void *cont, size_t size, size_t nmemb, void *userp) {
  size_t real_size = size * nmemb;
  RespMemory *mem = (RespMemory *)userp;

  cav_log(Debug, "reallocating %lu mem..\n", mem->size + real_size + 1);

  char *ptr = realloc(mem->memory, mem->size + real_size + 1);
  if (!ptr) {
    cav_log(Fatal, "Not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), cont, mem->size + real_size + 1);
  mem->size += real_size;
  mem->memory[mem->size] = 0;

  return real_size;
}

char *get_request(char *url, size_t data_size, int status_code) {
  CURL *curl;
  CURLcode resp_code;

  size_t return_buf_size = 0;
  char *return_buf = {0};

  RespMemory chunk;
  chunk.memory = (char *)malloc(1);
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if (!curl) {
    cav_log(Fatal, "CURL failed to easy initialize");
    free(chunk.memory);
    return 0;
  }

  cav_log(6, "Curl successfully initialized!");
  cav_log(Debug, "Setting opts..\n");

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_mem);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "cav @ libcurl-agent/1.0");

  cav_log(Debug, "Performing...\n");

  resp_code = curl_easy_perform(curl);
  if (resp_code != CURLE_OK) {
    cav_log(Fatal, "CURL failed to perform: %s\n",
            curl_easy_strerror(resp_code));
    free(chunk.memory);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
  }

  data_size = chunk.size;

  return_buf_size = chunk.size + 1;
  return_buf = (char *)malloc(return_buf_size);

  if (!return_buf) {
    cav_log(Fatal, "Failed to malloc for response.");
    free(chunk.memory);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
  }

  cav_log(Debug, "Writing response content to a buffer with size %lu\n",
          chunk.size);
  strlcpy(return_buf, chunk.memory, return_buf_size);

  cav_log(Debug, "Getting status code...\n");

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

  cav_log(Debug,
          "[Summary]: received %lu bytes of content with a status of %d\n",
          chunk.size, status_code);

  curl_easy_cleanup(curl);
  curl_global_cleanup();
  return return_buf;
}
