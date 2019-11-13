#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*----INPUT----*/
// File, Mode select, Instruction cache size(KB), Instruction cahe block size(B), Instruction associativity
// Data cache size (KB), Data cache block size (B), Data associativity

/*----OUTPUT----*/
// Access, Miss, Miss ratio (Instruction cache)
// Access, Miss, Miss ratio (Data cache)


// 0 Read Data
// 1 Write Data
// 2 Instruction Fetch

//  KB: 32 ~512
//
//	32KB 15비트
//	64KB 16비트
//	128KB 17비트
//	256KB 18비트
//	512KB 19비트
//
//	B : 4 ~64
//
//	4B 2비트
//	8B 3비트
//	16B 4비트
//	32B 5비트
//	64B 6비트


//변수
FILE *file;
char fileName[12];
int modeSelect;

typedef struct instruction {
	int tag;
	int indexBit;
	int cachesize;
	int cacheBlockSize;
	int associativity;
}instruction;

typedef struct data {
	int tag;
	int indexBit;
	int cachesize;
	int cacheBlockSize;
	int associativity;
}data;

typedef struct Cache {
	int valid;
	int tag;
	int data;
}Cache;

char bin[9];

const char *hextobin(int x)
{
	static const char *B[] = {
	"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
	"1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111",
	};
	if ('A' <= x && x <= 'F') x = x - 'A' + 10;
	else if ('a' <= x && x <= 'f') x = x - 'a' + 10;
	else x -= '0';

	return (0 <= x && x <= 15) ? B[x] : NULL;
}


int main() {
	instruction ins;
	data data;


	/* cache output*/
	int cacheAccess = 0;
	int cacheMiss = 0;
	double cacheRatio = 0.0;
	/* data output */
	int dataAccess = 0;
	int dataMiss = 0;
	double dataRatio = 0.0;

	printf("File : ");
	scanf("%s", &fileName);

	//printf("Mode select (LRU : 1 / FIFO : 2) : ");
	//scanf("%d", &modeSelect);

	//printf("Instruction cache size (KB) : ");
	//scanf("%d", &ins.cachesize);

	//printf("Instruction cache block size (B) : ");
	//scanf("%d", &ins.cacheBlockSize);

	//printf("Instruction associativity : ");
	//scanf("%d", &ins.associativity);

	//printf("Data cache size (KB) : ");
	//scanf("%d", &data.cachesize);

	//printf("Data cache block size (B) : ");
	//scanf("%d", &data.cacheBlockSize);

	//printf("Data associativity : ");
	//scanf("%d", &data.associativity);

	int addressBit = 32;
	ins.indexBit = (int)log2(ins.cachesize / ins.associativity) + 10;
	int blockOffset = (int)log2(ins.cacheBlockSize);
	ins.tag = addressBit - ins.indexBit - blockOffset;

	data.indexBit = (int)log2(data.cachesize / data.associativity) + 10;
	int blockOffset = (int)log2(data.cacheBlockSize);
	data.tag = addressBit - data.indexBit - blockOffset;
	
	file = fopen(fileName, "r");
	if (file == NULL) {
		printf("파일이 없습니다.");
		return -1;
	}

	int label;
	int address;
	int i;
	char binary[32] = { 0 };

	while (!feof(file)) {
		fscanf(file, "%d %x", &label, &address);

		sprintf(bin, "%08x", address);

		for (i = 0; i < 8; i++) {
			strcat(binary, hextobin(bin[i]));
		}

		if (label = 2) {

		}
		else {

		}

	}

	fclose(file);
	return 0;
}
