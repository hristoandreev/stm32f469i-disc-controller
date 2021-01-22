//
// Created by ivan on 16.5.2019 г..
//

#ifndef USPCUTILS_DBG_H
#define USPCUTILS_DBG_H

#ifdef __cplusplus
extern "C" {
#endif

int dbgPrint(const char* fmt, ...);
int dbgInit();
void dbg_abort(void);

#ifdef __cplusplus
}
#endif

#endif //USPCUTILS_DBG_H
