#ifndef INCLUDE_LER_CSV_H_
#define INCLUDE_LER_CSV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1048576 // 2^20

void ler_csv(char*, int, int, double**);

#endif /* INCLUDE_LER_CSV_H_ */
