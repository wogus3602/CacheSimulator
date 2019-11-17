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

/* N개 associativity할당을 위한 cache*/
typedef struct CacheNway {
	int valid;
	char tag[32];
	int cdata;
}CacheNway;

/* cache의 LRU와 associativity의 갯수의 cacheNway */
typedef struct cache {
	list<int> *LRU;
	CacheNway *cacheNWay;
}Cache;

/* Instruction의 주소(Tag, Index, BlockOffset)와 Index만큼의 Cache size*/
typedef struct instruction {
	int tagBit;
	int indexBit;
	int cachesize;
	int blockOffset;
	int cacheBlockSize;
	int associativity;
	Cache *insCache;
}Instruction;

/* Data의 주소(Tag, Index, BlockOffset)와 Index만큼의 Cache size*/

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
FILE *file;    // 파일을 저장하기 위한 변수
char fileName[12];   // 파일 이름을 받기 위한 배열
int modeSelect;      // LRU, FIFO 선택 변수
char bin[9];       // address를 저장하기 위한 변수
Instruction ins;   
cData cdata;

/* cache output*/
/* Instruction, Data의 access, miss ,ratio값을 저장하기 위한 변수*/
int instrcutionAccess = 0;         
int instrcutionMiss = 0;
double instrcutionRatio = 0.0;
int dataAccess = 0;
int dataMiss = 0;
double dataRatio = 0.0;


char binary[32] = { 0 };   //16진수 -> 2진수 변환값 저장할 배열

/* --- 함수 --- */
void check();           // Instruction의 Access를 확인을 위한 함수
void check2();          // Data의 Access를 확인을 위한 함수
void initialize();      // Instrction, Data의 데이터 초기화 함수
const char *hextobin(long long x);   // 16진수 -> 2진수 변환 함수
int twoConvertTen(long long num) ;   // 2진수 -> 10진수 변환 함수(2진수 index값을 받았을때 10진수로 바꾸기 위한)
void free();
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

	/* test */
	//ins.cachesize = 128;
	//ins.cacheBlockSize = 8;
	//ins.associativity = 4;

	//cdata.cachesize = 64;
	//cdata.cacheBlockSize = 4;
	//cdata.associativity = 2;

	// AddressBit 길이
	int addressBit = 32;

	/*Index, BlockOffset, TagBit의 길이를 구함*/
	ins.indexBit = (int)log2((ins.cachesize<<10) / ins.cacheBlockSize / ins.associativity); // cache line
	ins.blockOffset = (int)log2(ins.cacheBlockSize); //offset
	ins.tagBit = addressBit - ins.indexBit - ins.blockOffset; //tag size

	cdata.indexBit = (int)log2((cdata.cachesize<<10) / cdata.cacheBlockSize / cdata.associativity);
	cdata.blockOffset = (int)log2(cdata.cacheBlockSize);
	cdata.tagBit = addressBit - cdata.indexBit - cdata.blockOffset;

	
	initialize();   //데이터 초기화

	file = fopen(fileName, "r");
	if (file == NULL) {
		printf("파일이 없습니다.");
		return -1;
	}

	int label;   // File내 Instruction인지 ,Data인지 구분하는 변수
	int address; // File내 label에 해당하는 주소
	int i;

	while (!feof(file)) {
		fscanf(file, "%d %x", &label, &address);

		sprintf(bin, "%08x", address);
		binary[0] = '\0';
		for (i = 0; i < 8; i++) {
			strcat(binary, hextobin(bin[i]));   // 16진수 -> 2진수 변환 후 binary에 저장
		}
		if (label == 2) {
			check();    // Instruction의 Access확인
		}
		else {
			check2();   // Data의 Access확인
		}
	}

	/* Access, Miss ,Ratio 값 출력 */
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


	free();
	return 0;
}

/*동적할당 해제*/
void free() {
	long long index = (long long)pow(2, ins.indexBit);
	long long index2 = (long long)pow(2, cdata.indexBit);

	for (int i = 0; i < index; i++) {
		free(ins.insCache[i].cacheNWay);
	}

	for (int i = 0; i < index2; i++) {
		free(cdata.cdataCache[i].cacheNWay);
	}

	free(ins.insCache);
	free(cdata.cdataCache);
}

/* Instruction Tag Check 나머지 위와 동일*/
void check() {
	char tag1[32];
	char index[32];

	memcpy(tag1, &binary[0], ins.tagBit);     //  0번째 부터 ins.tagBit까지가 Tag길이
	tag1[ins.tagBit] = '\0';		
	memcpy(index, &binary[ins.tagBit], ins.indexBit);   // ins.tagBit번째 부터 ins.indexBit까지가 Index길이
	index[ins.indexBit] = '\0';

	int tIndex = twoConvertTen(atoll(index));  //2진수 값을 10진수로 변환하는 과정

	for (int i = 0; i < ins.associativity; i++) {
		//MISS
		
		if (!ins.insCache[tIndex].cacheNWay[i].valid) {    //valid가 0일때 
				ins.insCache[tIndex].cacheNWay[i].valid = 1;
				strcpy(ins.insCache[tIndex].cacheNWay[i].tag, tag1);   //해당 tag를 cache에 저장해준다.
				ins.insCache[tIndex].LRU->push_back(i);		//LRU에 index값을 push
				instrcutionMiss++;
				return ;
		}
		//HIT
		if (!strcmp(ins.insCache[tIndex].cacheNWay[i].tag, tag1)) {  //Index위치에 tag가 tag1가 같을때
				ins.insCache[tIndex].LRU->remove(i);     //LRU에 저장되어있던 값 삭제 후
				ins.insCache[tIndex].LRU->push_back(i);  //다시 제일 첫번째에 넣어준다.
				instrcutionAccess++;
				return ;
		}
	}

	//MISS
	/* tag가 일치하는게 없을때*/
	int LRUIndex = ins.insCache[tIndex].LRU->front();      //제일 마지막번째 위치 저장 후
	strcpy(ins.insCache[tIndex].cacheNWay[LRUIndex].tag, tag1); // 확인하려고 했던 Tag를 이미 들어있던 위치의 Tag에 저장시켜준다.
	ins.insCache[tIndex].LRU->remove(LRUIndex);   //해당 위치를 제일 최근으로 옮겨준다.
	ins.insCache[tIndex].LRU->push_back(LRUIndex);
	instrcutionMiss++;
}

/* Data Tag Check 나머지 위와 동일*/
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


/*Instruction, Data의 값 초기화*/
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

/*16진수 -> 2진수*/
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

/*2진수 -> 10진수*/
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