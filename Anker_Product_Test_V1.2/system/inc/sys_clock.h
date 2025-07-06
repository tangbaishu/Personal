/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file sys_clock.h
 * @brief Soc clock define
 * @author David
 ****************************************************************************/

#ifndef SYS_CLOCK_H
#define SYS_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------- Clock Declaration ------------------------------*/
#define RC2_50M   50000000
#define RC2_60M   60000000
#define RC2_70M   70000000
#define RC2_80M   80000000

#define RC2_CLK   RC2_50M
#define FCLK      RC2_CLK/2
#define AHBCLK    FCLK
#define APBCLK    FCLK/2
#define DEEP_SLEEP_CLK  125000
#define APB_DEEP_SLEEP_CLK  DEEP_SLEEP_CLK/2

#ifdef __cplusplus
}
#endif
#endif
