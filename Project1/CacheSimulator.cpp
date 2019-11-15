#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<list>
using namespace std;
/*----INPUT----*/
// File, Mode select, Instruction cache size(KB), Instruction cahe block size(B), Instruction associativity
// Data cache size (KB), Data cache block size (B), Data associativity

/*----OUTPUT----*/
// Access, Miss, Miss ratio (Instruction cache)
// Access, Miss, Miss ratio (Data cache)

typedef struct CacheNway {
	int valid;
	char tag[32];
	int cdata;
}CacheNway;

typedef struct cache {
	list<int> *LRU;
	CacheNway *cacheNWay;
}Cache;

typedef struct instruction {
	int tagBit;
	int indexBit;
	int cachesize;
	int blockOffset;
	int cacheBlockSize;
	int associativity;
	Cache *insCache;
}Instruction;

typedef struct data {
	int tagBit;
	int indexBit;
	int cachesize;
	int blockOffset;
	int cacheBlockSize;
	int associativity;
	Cache *cdataCache;
}cData;


/* --- 변수 --- */
FILE *file;
char fileName[12] = "trace_1.txt";
int modeSelect;
char bin[9];
Instruction ins;
cData cdata;

/* cache output*/
int instrcutionAccess = 0;
int instrcutionMiss = 0;
double instrcutionRatio = 0.0;
/* cdata output */
int dataAccess = 0;
int dataMiss = 0;
double dataRatio = 0.0;

char binary[32] = { 0 };

/* --- 함수 --- */
void check();
void check2();
void initialize();
const char *hextobin(long long x);
int twoConvertTen(long long num) ;
/* ----------- */



int main() {

	printf("File : ");
	scanf("%s", &fileName);

	printf("Mode select (LRU : 1 / FIFO : 2) : ");
	scanf("%d", &modeSelect);

	printf("Instruction cache size (KB) : ");
	scanf("%d", &ins.cachesize);

	printf("Instruction cache block size (B) : ");
	scanf("%d", &ins.cacheBlockSize);

	printf("Instruction associativity : ");
	scanf("%d", &ins.associativity);

	printf("Data cache size (KB) : ");
	scanf("%d", &cdata.cachesize);

	printf("Data cache block size (B) : ");
	scanf("%d", &cdata.cacheBlockSize);

	printf("Data associativity : ");
	scanf("%d", &cdata.associativity);

	//ins.cachesize = 128;
	//ins.cacheBlockSize = 8;
	//ins.associativity = 4;

	//cdata.cachesize = 64;
	//cdata.cacheBlockSize = 4;
	//cdata.associativity = 2;

	int addressBit = 32;

	ins.indexBit = (int)log2((ins.cachesize<<10) / ins.cacheBlockSize / ins.associativity); // cache line
	ins.blockOffset = (int)log2(ins.cacheBlockSize); //offset
	ins.tagBit = addressBit - ins.indexBit - ins.blockOffset; //tag size

	cdata.indexBit = (int)log2((cdata.cachesize<<10) / cdata.cacheBlockSize / cdata.associativity);
	cdata.blockOffset = (int)log2(cdata.cacheBlockSize);
	cdata.tagBit = addressBit - cdata.indexBit - cdata.blockOffset;

	initialize();

	file = fopen(fileName, "r");
	if (file == NULL) {
		printf("파일이 없습니다.");
		return -1;
	}

	int label;
	int address;
	int i;

	while (!feof(file)) {
		fscanf(file, "%d %x", &label, &address);

		sprintf(bin, "%08x", address);
		binary[0] = '\0';
		for (i = 0; i < 8; i++) {
			strcat(binary, hextobin(bin[i]));
		}
		if (label == 2) {
			check();
		}
		else {
			check2();
		}
	}

	instrcutionRatio = (double)instrcutionMiss / (double)(instrcutionAccess + instrcutionMiss);
	printf("\nInstrcution cache\n");
	printf("Access : %d\n", instrcutionAccess);
	printf("Miss : %d\n", instrcutionMiss);
	printf("Miss ratio : %.4f\n\n", instrcutionRatio);

	dataRatio = (double)dataMiss / (double)(dataAccess + dataMiss);
	printf("Data cache\n");
	printf("Access : %d\n", dataAccess);
	printf("Miss : %d\n", dataMiss);
	printf("Miss ratio : %.4f\n",dataRatio);

	fclose(file);
	return 0;
}

void check() {
	char tag1[32];
	char index[32];

	memcpy(tag1, &binary[0], ins.tagBit);
	tag1[ins.tagBit] = '\0';
	memcpy(index, &binary[ins.tagBit], ins.indexBit);
	index[ins.indexBit] = '\0';

	int tIndex = twoConvertTen(atoll(index));

	for (int i = 0; i < ins.associativity; i++) {
		//MISS
		if (!ins.insCache[tIndex].cacheNWay[i].valid) {
				ins.insCache[tIndex].cacheNWay[i].valid = 1;
				strcpy(ins.insCache[tIndex].cacheNWay[i].tag, tag1);
				ins.insCache[tIndex].LRU->push_back(i);
				instrcutionMiss++;
				return ;
		}
		//HIT
		if (!strcmp(ins.insCache[tIndex].cacheNWay[i].tag, tag1)) {
				ins.insCache[tIndex].LRU->remove(i);
				ins.insCache[tIndex].LRU->push_back(i);
				instrcutionAccess++;
				return ;
		}
	}

	//MISS
	int LRUIndex = ins.insCache[tIndex].LRU->front();
	strcpy(ins.insCache[tIndex].cacheNWay[LRUIndex].tag, tag1);
	ins.insCache[tIndex].LRU->remove(LRUIndex);
	ins.insCache[tIndex].LRU->push_back(LRUIndex);
	instrcutionMiss++;
}

void check2() {
	char tag1[32];
	char index[32];

	memcpy(tag1, &binary[0], cdata.tagBit);
	tag1[cdata.tagBit] = '\0';
	memcpy(index, &binary[cdata.tagBit], cdata.indexBit);
	index[cdata.indexBit] = '\0';

	int tIndex = twoConvertTen(atoll(index));

	for (int i = 0; i < cdata.associativity; i++) {
		//MISS
		if (!cdata.cdataCache[tIndex].cacheNWay[i].valid) {
			cdata.cdataCache[tIndex].cacheNWay[i].valid = 1;
			strcpy(cdata.cdataCache[tIndex].cacheNWay[i].tag, tag1);
			cdata.cdataCache[tIndex].LRU->push_back(i);
			dataMiss++;
			return;
		}

		//HIT
		if (!strcmp(cdata.cdataCache[tIndex].cacheNWay[i].tag, tag1)) {
			cdata.cdataCache[tIndex].LRU->remove(i);
			cdata.cdataCache[tIndex].LRU->push_back(i);
			dataAccess++;
			return;
		}
	}

	//MISS
	int LRUIndex = cdata.cdataCache[tIndex].LRU->front();
	strcpy(cdata.cdataCache[tIndex].cacheNWay[LRUIndex].tag, tag1);
	cdata.cdataCache[tIndex].LRU->remove(LRUIndex);
	cdata.cdataCache[tIndex].LRU->push_back(LRUIndex);
	dataMiss++;
}


void initialize() {
	long long index = (long long)pow(2, ins.indexBit);
	long long index2 = (long long)pow(2, cdata.indexBit);
	ins.insCache = (Cache*)malloc(sizeof(Cache)*index);
	cdata.cdataCache = (Cache*)malloc(sizeof(Cache)*index2);


	for (int i = 0; i < index; i++) {
		ins.insCache[i].cacheNWay = (CacheNway*)malloc(sizeof(CacheNway)*ins.associativity);
		ins.insCache[i].LRU = new list<int>;

	}
	for (int i = 0; i < index2; i++) {

		cdata.cdataCache[i].cacheNWay = (CacheNway*)malloc(sizeof(CacheNway)*cdata.associativity);
		cdata.cdataCache[i].LRU = new list<int>;
	}
	for (int i = 0; i < index; i++) {
		for (int j = 0; j < ins.associativity; j++) {
			ins.insCache[i].cacheNWay[j].valid = 0;
			ins.insCache[i].cacheNWay[j].cdata = 0;
		}
	}
	for (int i = 0; i < index2; i++) {
		for (int j = 0; j < cdata.associativity; j++) {
			cdata.cdataCache[i].cacheNWay[j].valid = 0;
			cdata.cdataCache[i].cacheNWay[j].cdata = 0;
		}
	}

}

const char *hextobin(long long x)
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

int twoConvertTen(long long num) {
	long long result = 0, mul = 1;
	while (num > 0)
	{
		if (num % 2)
			result += mul;
		mul *= 2;
		num /= 10;
	}
	return result;
}