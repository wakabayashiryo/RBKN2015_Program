/* 
 * File:   IOC.h
 * Author: evaota
 *
 * Created on 2015/10/22, 18:51
 */

#ifndef IOC_H
#define	IOC_H

extern unsigned char IOC5f;

void IOC_Init(unsigned char UpEdge,unsigned char DownEdge);
void Inter_IOC(void);
void EN_IOC(void);
void DI_IOC(void);

#endif	/* IOC_H */

