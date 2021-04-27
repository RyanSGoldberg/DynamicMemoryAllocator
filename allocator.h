#pragma once

extern void *malloc(size_t size);

extern void free(void *ptr);

extern void *calloc(size_t nmemb, size_t size);

extern void *realloc(void *ptr, size_t size);

extern void *reallocarray(void *ptr, size_t nmemb, size_t size);

void heapdump();