#ifndef HIREDISPOOL_H
#define HIREDISPOOL_H

#ifdef __cplusplus
extern "C" {
#endif


/* Constants */
#define HIREDISPOOL_MAJOR 0
#define HIREDISPOOL_MINOR 1
#define HIREDISPOOL_PATCH 1
#define HIREDISPOOL_SONAME 0.1

/* Types */
typedef struct redis_endpoint {
    char host[256];
    int port;
} REDIS_ENDPOINT;

typedef struct redis_config {
    REDIS_ENDPOINT* endpoints;
    int num_endpoints;
    int connect_timeout;
    int net_readwrite_timeout;
    int num_redis_socks;
    int connect_failure_retry_delay;
} REDIS_CONFIG;

typedef struct redis_socket {
    int id;
    int backup;
    pthread_mutex_t mutex;
    int inuse;
    struct redis_socket* next;
    enum { sockconnected, sockunconnected } state;
    void* conn;
} REDIS_SOCKET;

typedef struct redis_instance {
    time_t connect_after;
    REDIS_SOCKET* redis_pool;
    REDIS_SOCKET* last_used;
    REDIS_CONFIG* config;
} REDIS_INSTANCE;

/* Functions */
int redis_pool_create(const REDIS_CONFIG* config, REDIS_INSTANCE** instance);
int redis_pool_destroy(REDIS_INSTANCE* instance);

REDIS_SOCKET* redis_get_socket(REDIS_INSTANCE* instance);
int redis_release_socket(REDIS_INSTANCE* instance, REDIS_SOCKET* redisocket);

void* redis_command(REDIS_SOCKET* redisocket, REDIS_INSTANCE* instance, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif/*HIREDISPOOL_H*/