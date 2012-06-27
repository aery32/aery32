#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#define F_OSC0 12000000UL
#define F_CPU 66000000UL

#define HSBMASK_DEFAULT 0xFFFFFFFF
#define PBAMASK_DEFAULT 0xFFFFFFFF
#define PBBMASK_DEFAULT 0xFFFFFFFF

void init_board(void);

#ifdef __cplusplus
}
#endif

#endif
