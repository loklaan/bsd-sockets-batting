// -----------------------------------------------
// File: console.h
// Purpose: Macros for use in terminal. Includes things like
// debugging logs and messages specific to the Batting Stats
// system.
// Author: Lochlan Bunn
//         n8509719
// -----------------------------------------------
// 

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

// =======---------=======
//       Debug Macros
// Disabled with -DNDEBUG flag
// 
#ifdef NDEBUG
#define debug(M, ...)
#define dbg_client_id(M, ...)
#define dbg_server(M, ...)
#define dbg_client(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define dbg_client_id(M, ...) msg_client_id(M, ##__VA_ARGS__)
#define dbg_server(M, ...) msg_server(M, ##__VA_ARGS__)
#define dbg_client(M, ...) msg_client(M, ##__VA_ARGS__)
#endif /* NDEBUG */
//
// -------=========-------

// =======---------=======
//    Error/Info Macros
// Some may not be used in this project
// 
#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }
#define check_mem(A) check((A), "Out of memory.")
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }
//
// -------=========-------


// =======---------=======
//     Message Macros
// Specific to this project
// 
#define msg(M, ...) printf("[MSG] " M "...\n", ##__VA_ARGS__)
#define msg_client_id(ID, M, ...) printf("[CLIENT_MSG:ID_%d] " M "...\n", ID, ##__VA_ARGS__)
#define msg_server(M, ...) printf("[SERVER_MSG] " M "...\n", ##__VA_ARGS__)
#define msg_client(M, ...) printf("[CLIENT_MSG] " M "...\n", ##__VA_ARGS__)
//
// -------=========-------

#endif /* CONSOLE_H_ */
