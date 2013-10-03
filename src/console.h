#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

// ---------------------------------
// Debug Prints
// 
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
//
// ---------------------------------



// ---------------------------------
//  Message Prints
//
#define msg(M, ...) printf("[MSG] " M "...\n", ##__VA_ARGS__);
#define msg_client_cpid(M, ...) printf("[CLIENT_MSG:%d] " M "...\n", getpid(), ##__VA_ARGS__);
#define msg_server_cpid(M, ...) printf("[CLIENT_MSG:%d] " M "...\n", getpid(), ##__VA_ARGS__);
#define msg_server(M, ...) printf("[SERVER_MSG] " M "...\n", ##__VA_ARGS__);
#define msg_client(M, ...) printf("[CLIENT_MSG] " M "...\n", ##__VA_ARGS__);
//
// ---------------------------------
