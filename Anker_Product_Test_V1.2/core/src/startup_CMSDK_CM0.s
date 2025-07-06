;/**************************************************************************//**
; * @file     startup_CMSDK_CM0.s
; * @brief    CMSIS Cortex-M0 Core Device Startup File for
; *           Device CMSDK_CM0
; * @version  V3.01
; * @date     06. March 2012
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000300

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
                DCD     UART_Handler              ; UART Handler
                DCD     TYPEC0_Handler            ; TYPE C 0 Handler
                DCD     I2CM_Handler              ; I2CM Handler
                DCD     PD_Handler                ; PD Handler
                DCD     DPDM_Handler              ; DPDM Handler
                DCD     I2CS_Handler              ; I2CS Handler
                DCD     GPIO_Handler              ; GPIO Handler
                DCD     POWER_Handler             ; POWER Handler
                DCD     TYPEC1_Handler            ; TYPE C 1 Handler
                DCD     TFCP_Handler              ; TFCP Handle
                DCD     SCP_Handler               ; SCP Handle					
                DCD     ADC_Handler               ; ADC Handle
                DCD     UFCS_Handler              ; UFCS Handler					
                DCD     BTM0_Handler              ; BTM0 handler
                DCD     BTM1_Handler              ; BTM1 handler
                DCD     BTM2_Handler              ; BTM2 handler					
                DCD     BTM3_Handler              ; BTM3 handler		
                DCD     BTM4_Handler              ; BTM4 handler		
                DCD     0                         ; Reserved		
                DCD     0                         ; Reserved		
                DCD     0                         ; Reserved	
                DCD     ATM0_Handler              ; ATM 0 handler
                DCD     ATM1_Handler              ; ATM 1 handler
                DCD     PLUG_Handler              ; PLUG handler
				DCD     ABN_Handler               ; ABN handler	
				DCD     VOOC_Handler              ; ABN handler						
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
				MOVS 	R0, #0
				MOVS 	R1, #0
				MOVS 	R2, #0
				MOVS 	R3, #0
				MOVS 	R4, #0
				MOVS 	R5, #0
				MOVS 	R6, #0
				MOV 	R7, R0
				MOV 	R8, R0
				MOV 	R9, R0
				MOV 	R10, R0
				MOV 	R11, R0 
				MOV 	R12, R0
				LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP
Default_Handler PROC
                EXPORT UART_Handler               [WEAK]
                EXPORT TYPEC0_Handler             [WEAK]
                EXPORT I2CM_Handler               [WEAK]
                EXPORT PD_Handler                 [WEAK]
                EXPORT DPDM_Handler               [WEAK]
				EXPORT I2CS_Handler               [WEAK]	
                EXPORT GPIO_Handler               [WEAK]
                EXPORT POWER_Handler              [WEAK]
				EXPORT TYPEC1_Handler             [WEAK]
				EXPORT TFCP_Handler               [WEAK]
				EXPORT SCP_Handler                [WEAK]					
				EXPORT ADC_Handler                [WEAK]
				EXPORT UFCS_Handler               [WEAK]
                EXPORT BTM0_Handler               [WEAK]					
                EXPORT BTM1_Handler               [WEAK]	
                EXPORT BTM2_Handler               [WEAK]	
                EXPORT BTM3_Handler               [WEAK]	
                EXPORT BTM4_Handler               [WEAK]					
                EXPORT ATM0_Handler               [WEAK]
                EXPORT ATM1_Handler               [WEAK]
				EXPORT PLUG_Handler               [WEAK]
				EXPORT ABN_Handler                [WEAK]	
				EXPORT VOOC_Handler               [WEAK]					
UART_Handler
TYPEC0_Handler
I2CM_Handler
PD_Handler
DPDM_Handler
I2CS_Handler
GPIO_Handler
POWER_Handler
TYPEC1_Handler
TFCP_Handler
SCP_Handler
ADC_Handler
UFCS_Handler
BTM0_Handler
BTM1_Handler
BTM2_Handler
BTM3_Handler
BTM4_Handler
ATM0_Handler
ATM1_Handler
PLUG_Handler
ABN_Handler
VOOC_Handler
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
