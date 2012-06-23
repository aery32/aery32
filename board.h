#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#define OSC0_FREQ 12000000UL

#ifndef F_CPU
#define F_CPU 66000000UL
#endif

#define HSBMASK_DEFAULT 0xFFFFFFFF
#define PBAMASK_DEFAULT 0xFFFFFFFF
#define PBBMASK_DEFAULT 0xFFFFFFFF

void init_board(void);

#ifdef __cplusplus
}
#endif

#endif
