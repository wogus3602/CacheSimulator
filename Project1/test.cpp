//#define _CRT_SECURE_NO_WARNINGS
//#include <bitset>
//#include <iostream>
//#include <list>
//#include <math.h>
//#include <string.h>
//
//using namespace std;
//
//constexpr int ADDRESS_BITS = 32;	// 32비트 메모리 주소
//
//// 캐시 슬롯 구조체
//typedef struct CacheSlot_t
//{
//	bitset<ADDRESS_BITS> tagBits;		// 슬롯의 Tag 비트
//	bitset<1> validBit;					// valid 비트
//} CacheSlot;
//
//// 캐시 인덱스 슬롯 구조체
//typedef struct CacheIndexSlot_t
//{
//	bitset<ADDRESS_BITS> indexBits;	// 인덱스 슬롯의 Index 비트
//
//	list<int> LRU_list;		// LRU 슬롯을 찾기 위한 리스트
//	int slotCount;			// 캐시 슬롯 개수
//	CacheSlot *slotList;	// 캐시 슬롯 리스트
//} CacheIndexSlot;
//
//// 캐시 Hit 시뮬레이터 전용 캐시 구조체
//typedef struct Cache_t
//{
//	int size;			// 캐시 크기
//	int blockSize;		// 블럭 크기
//	int associativity;	// associativity
//
//	int tagBits;		// Tag 비트 수
//	int indexBits;		// Index 비트 수
//	int offsetBits;		// Offset 비트 수
//
//	int accessCount;	// 캐시 접근 횟수
//	int hitCount;		// Hit count
//
//	CacheIndexSlot *indexSlotList;	// 캐시 세트 리스트
//} Cache;
//
//void initializeCache(Cache *cache, int size, int blockSize, int associativity);		// 캐시 초기 설정
//void access(Cache *cache, unsigned int address);	// 캐시에 address 주소로 접근
//
//int main(int argc, char *argv[])
//{
//	//// 입력 인자 개수 검사
//	//const int REQUIRE_ARGS = 4;
//	//if (argc != REQUIRE_ARGS + 1)
//	//{
//	//	printf("입력 인자가 '%d'개가 아닙니다\n", REQUIRE_ARGS);
//	//	return 0;
//	//}
//
//	// 인자 받아오기
//	//char *traceFileName = "trace_3.txt";
//	int cacheSize = 128;
//	int cacheBlockSize = 8;
//	int associativity = 4;
//
//	// 트레이스 파일 열기
//	FILE *file;
//	file = fopen("trace_1.txt", "r");
//
//	// 잘못된 인자 예외 처리
//	if (file == NULL)
//	{
//		printf("파일을 열 수 없습니다, file: '%s'\n", "trace_3.txt");
//		return 0;
//	}
//	if (cacheBlockSize & cacheBlockSize - 1)
//	{
//		printf("Cache block의 크기가 2의 지수승이 아닙니다, 입력: '%d'\n", cacheBlockSize);
//		return 0;
//	}
//	if (associativity != 1 && associativity != 2 && associativity != 4 && associativity != 8)
//	{
//		printf("Associativity가 1, 2, 4, 8 이외의 값이 입력되었습니다, 입력: '%d'\n", associativity);
//		return 0;
//	}
//	if (cacheSize % (cacheBlockSize * associativity) != 0)
//	{
//		printf("Cache 전체의 크기가 (CacheBlockSize) x (Associativity)의 배수가 아닙니다, 입력: '%d'\n", cacheSize);
//		return 0;
//	}
//
//	// 캐시 초기 설정
//	Cache cache;
//	initializeCache(&cache, cacheSize, cacheBlockSize, associativity);
//
//	// 캐시의 tag, index, offset의 비트 수를 출력
//	printf("tag: %d bits\nindex: %d bits\noffset: %d bits\n",
//		cache.tagBits, cache.indexBits, cache.offsetBits);
//
//	// 트레이스파일에서 읽어올 필드들
//	char mode[2];
//	unsigned int address;	// 메모리 주소
//	int value;
//
//	char buffer[128];
//	while (fgets(buffer, sizeof(buffer), file) != NULL)
//	{
//	
//			// 트레이스 파일에서 한 줄을 읽어서 각 필드를 분리
//			sscanf(buffer, "%d %x", &value, &address);
//		if (value == 2) {
//			// 읽어온 메모리 주소를 캐시에서 접근
//			access(&cache, address);
//		}
//	}
//
//	// 트레이스 파일에 있는 메모리 접근이 끝나고 Hit 결과를 출력
//	printf("Result: total access %d, hit %d, hit rate %.2lf\n",
//		cache.accessCount, cache.hitCount, (float)cache.hitCount / cache.accessCount);
//
//	return 0;
//}
//
//// 캐시의 크기, 블럭크기, associativity를 매개변수로 받아 캐시 세팅
//// [매개변수] cache: 대상, size: 캐시크기, blockSize: 블럭크기, associativity: associativity
//void initializeCache(Cache *cache, int size, int blockSize, int associativity)
//{
//	// 캐시 설정
//	cache->size = size;
//	cache->blockSize = blockSize;
//	cache->associativity = associativity;
//
//	// Tag, Index, Offset 비트 수 계산
//	int set = (size<<10) / blockSize / associativity;
//	cache->indexBits = (int)log2(set);
//	cache->offsetBits = (int)log2(blockSize);
//	cache->tagBits = ADDRESS_BITS - cache->indexBits - cache->offsetBits;
//
//	// 메모리 접근 횟수, hit count 초기화
//	cache->accessCount = 0;
//	cache->hitCount = 0;
//
//	// 캐시 블럭들을 만들고 초기화
//	cache->indexSlotList = new CacheIndexSlot[set];
//	for (int index = 0; index < set; index++)
//	{
//		// 캐시 인덱스 슬롯 값 설정
//		cache->indexSlotList[index].indexBits = index << cache->offsetBits;
//		cache->indexSlotList[index].slotCount = associativity;
//		cache->indexSlotList[index].slotList = new CacheSlot[associativity];
//
//		// 캐시 슬롯 초기화
//		for (int slotIndex = 0; slotIndex < associativity; slotIndex++)
//		{
//			cache->indexSlotList[index].slotList[slotIndex].tagBits = slotIndex;
//			cache->indexSlotList[index].slotList[slotIndex].validBit = 0;
//		}
//	}
//}
//
//// 매개변수로 전달 받은 주소에 접근 시도
//// [매개변수] cache: 대상, address: 접근 할 주소 값
//void access(Cache *cache, unsigned int address)
//{
//	// 캐시 접근 횟수를 증가
//	cache->accessCount++;
//
//	// 매개변수로 받은 주소값에서 Index 비트를 추출
//	bitset<ADDRESS_BITS> address_index = address;
//	address_index = (address_index >> cache->offsetBits) << cache->offsetBits;
//	address_index = (address_index << cache->tagBits) >> cache->tagBits;
//
//	// 매개변수로 받은 주소값에서 Tag 비트를 추출
//	bitset<ADDRESS_BITS> address_tag = address;
//	address_tag = (address_tag >> cache->offsetBits + cache->indexBits) << cache->offsetBits + cache->indexBits;
//
//	// 캐시에서 매개변수로 받은 주소값의 Index 비트에 해당하는 인덱스 슬롯의 포인터를 가져옴
//	CacheIndexSlot *targetSet = &cache->indexSlotList[(address_index >> cache->offsetBits).to_ulong()];
//	
//	int i = (address_index >> cache->offsetBits).to_ulong();
//	// 해당 인덱스 슬롯에 Tag 비트가 일치하는 캐시 슬롯이 있는지 검사
//	for (int index = 0; index < targetSet->slotCount; index++)
//	{
//		// Compulsory Miss 
//		// 캐시 슬롯의 valid bit가 0
//		if (targetSet->slotList[index].validBit == 0)
//		{
//			// validBit 1로 변경, 슬롯의 Tag 비트 설정
//			targetSet->slotList[index].tagBits = address_tag;
//			targetSet->slotList[index].validBit = 1;
//
//			// 해당 슬롯을 LRU 리스트 맨 앞에 추가
//			targetSet->LRU_list.push_front(index);
//			return;
//		}
//
//		// Hit 
//		// 캐시 슬롯의 Tag 비트와 매개변수로 받은 주소의 Tag 비트가 일치
//		if (targetSet->slotList[index].tagBits == address_tag)
//		{
//			// 해당 슬롯을 LRU 리스트 맨 앞으로 이동
//			targetSet->LRU_list.remove(index);
//			targetSet->LRU_list.push_front(index);
//
//			// Hit 회수 증가
//			cache->hitCount++;
//			return;
//		}
//	}
//	// Tag 비트가 일치하는 캐시 슬롯이 없으면
//
//	// LRU 리스트에서 가장 오래 전에 쓰인 슬롯을 가져옴
//	int targetIndex = targetSet->LRU_list.back();
//
//	// Conflict Miss 
//	// 해당 슬롯의 Tag 비트를 새로운 주소의 Tag로 변경
//	targetSet->slotList[targetIndex].tagBits = address_tag;
//
//	// 해당 슬롯을 LRU 리스트 맨 앞으로 이동
//	targetSet->LRU_list.remove(targetIndex);
//	targetSet->LRU_list.push_front(targetIndex);
//}
