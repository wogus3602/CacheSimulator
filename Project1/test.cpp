//#define _CRT_SECURE_NO_WARNINGS
//#include <bitset>
//#include <iostream>
//#include <list>
//#include <math.h>
//#include <string.h>
//
//using namespace std;
//
//constexpr int ADDRESS_BITS = 32;	// 32��Ʈ �޸� �ּ�
//
//// ĳ�� ���� ����ü
//typedef struct CacheSlot_t
//{
//	bitset<ADDRESS_BITS> tagBits;		// ������ Tag ��Ʈ
//	bitset<1> validBit;					// valid ��Ʈ
//} CacheSlot;
//
//// ĳ�� �ε��� ���� ����ü
//typedef struct CacheIndexSlot_t
//{
//	bitset<ADDRESS_BITS> indexBits;	// �ε��� ������ Index ��Ʈ
//
//	list<int> LRU_list;		// LRU ������ ã�� ���� ����Ʈ
//	int slotCount;			// ĳ�� ���� ����
//	CacheSlot *slotList;	// ĳ�� ���� ����Ʈ
//} CacheIndexSlot;
//
//// ĳ�� Hit �ùķ����� ���� ĳ�� ����ü
//typedef struct Cache_t
//{
//	int size;			// ĳ�� ũ��
//	int blockSize;		// �� ũ��
//	int associativity;	// associativity
//
//	int tagBits;		// Tag ��Ʈ ��
//	int indexBits;		// Index ��Ʈ ��
//	int offsetBits;		// Offset ��Ʈ ��
//
//	int accessCount;	// ĳ�� ���� Ƚ��
//	int hitCount;		// Hit count
//
//	CacheIndexSlot *indexSlotList;	// ĳ�� ��Ʈ ����Ʈ
//} Cache;
//
//void initializeCache(Cache *cache, int size, int blockSize, int associativity);		// ĳ�� �ʱ� ����
//void access(Cache *cache, unsigned int address);	// ĳ�ÿ� address �ּҷ� ����
//
//int main(int argc, char *argv[])
//{
//	//// �Է� ���� ���� �˻�
//	//const int REQUIRE_ARGS = 4;
//	//if (argc != REQUIRE_ARGS + 1)
//	//{
//	//	printf("�Է� ���ڰ� '%d'���� �ƴմϴ�\n", REQUIRE_ARGS);
//	//	return 0;
//	//}
//
//	// ���� �޾ƿ���
//	//char *traceFileName = "trace_3.txt";
//	int cacheSize = 128;
//	int cacheBlockSize = 8;
//	int associativity = 4;
//
//	// Ʈ���̽� ���� ����
//	FILE *file;
//	file = fopen("trace_1.txt", "r");
//
//	// �߸��� ���� ���� ó��
//	if (file == NULL)
//	{
//		printf("������ �� �� �����ϴ�, file: '%s'\n", "trace_3.txt");
//		return 0;
//	}
//	if (cacheBlockSize & cacheBlockSize - 1)
//	{
//		printf("Cache block�� ũ�Ⱑ 2�� �������� �ƴմϴ�, �Է�: '%d'\n", cacheBlockSize);
//		return 0;
//	}
//	if (associativity != 1 && associativity != 2 && associativity != 4 && associativity != 8)
//	{
//		printf("Associativity�� 1, 2, 4, 8 �̿��� ���� �ԷµǾ����ϴ�, �Է�: '%d'\n", associativity);
//		return 0;
//	}
//	if (cacheSize % (cacheBlockSize * associativity) != 0)
//	{
//		printf("Cache ��ü�� ũ�Ⱑ (CacheBlockSize) x (Associativity)�� ����� �ƴմϴ�, �Է�: '%d'\n", cacheSize);
//		return 0;
//	}
//
//	// ĳ�� �ʱ� ����
//	Cache cache;
//	initializeCache(&cache, cacheSize, cacheBlockSize, associativity);
//
//	// ĳ���� tag, index, offset�� ��Ʈ ���� ���
//	printf("tag: %d bits\nindex: %d bits\noffset: %d bits\n",
//		cache.tagBits, cache.indexBits, cache.offsetBits);
//
//	// Ʈ���̽����Ͽ��� �о�� �ʵ��
//	char mode[2];
//	unsigned int address;	// �޸� �ּ�
//	int value;
//
//	char buffer[128];
//	while (fgets(buffer, sizeof(buffer), file) != NULL)
//	{
//	
//			// Ʈ���̽� ���Ͽ��� �� ���� �о �� �ʵ带 �и�
//			sscanf(buffer, "%d %x", &value, &address);
//		if (value == 2) {
//			// �о�� �޸� �ּҸ� ĳ�ÿ��� ����
//			access(&cache, address);
//		}
//	}
//
//	// Ʈ���̽� ���Ͽ� �ִ� �޸� ������ ������ Hit ����� ���
//	printf("Result: total access %d, hit %d, hit rate %.2lf\n",
//		cache.accessCount, cache.hitCount, (float)cache.hitCount / cache.accessCount);
//
//	return 0;
//}
//
//// ĳ���� ũ��, ��ũ��, associativity�� �Ű������� �޾� ĳ�� ����
//// [�Ű�����] cache: ���, size: ĳ��ũ��, blockSize: ��ũ��, associativity: associativity
//void initializeCache(Cache *cache, int size, int blockSize, int associativity)
//{
//	// ĳ�� ����
//	cache->size = size;
//	cache->blockSize = blockSize;
//	cache->associativity = associativity;
//
//	// Tag, Index, Offset ��Ʈ �� ���
//	int set = (size<<10) / blockSize / associativity;
//	cache->indexBits = (int)log2(set);
//	cache->offsetBits = (int)log2(blockSize);
//	cache->tagBits = ADDRESS_BITS - cache->indexBits - cache->offsetBits;
//
//	// �޸� ���� Ƚ��, hit count �ʱ�ȭ
//	cache->accessCount = 0;
//	cache->hitCount = 0;
//
//	// ĳ�� ������ ����� �ʱ�ȭ
//	cache->indexSlotList = new CacheIndexSlot[set];
//	for (int index = 0; index < set; index++)
//	{
//		// ĳ�� �ε��� ���� �� ����
//		cache->indexSlotList[index].indexBits = index << cache->offsetBits;
//		cache->indexSlotList[index].slotCount = associativity;
//		cache->indexSlotList[index].slotList = new CacheSlot[associativity];
//
//		// ĳ�� ���� �ʱ�ȭ
//		for (int slotIndex = 0; slotIndex < associativity; slotIndex++)
//		{
//			cache->indexSlotList[index].slotList[slotIndex].tagBits = slotIndex;
//			cache->indexSlotList[index].slotList[slotIndex].validBit = 0;
//		}
//	}
//}
//
//// �Ű������� ���� ���� �ּҿ� ���� �õ�
//// [�Ű�����] cache: ���, address: ���� �� �ּ� ��
//void access(Cache *cache, unsigned int address)
//{
//	// ĳ�� ���� Ƚ���� ����
//	cache->accessCount++;
//
//	// �Ű������� ���� �ּҰ����� Index ��Ʈ�� ����
//	bitset<ADDRESS_BITS> address_index = address;
//	address_index = (address_index >> cache->offsetBits) << cache->offsetBits;
//	address_index = (address_index << cache->tagBits) >> cache->tagBits;
//
//	// �Ű������� ���� �ּҰ����� Tag ��Ʈ�� ����
//	bitset<ADDRESS_BITS> address_tag = address;
//	address_tag = (address_tag >> cache->offsetBits + cache->indexBits) << cache->offsetBits + cache->indexBits;
//
//	// ĳ�ÿ��� �Ű������� ���� �ּҰ��� Index ��Ʈ�� �ش��ϴ� �ε��� ������ �����͸� ������
//	CacheIndexSlot *targetSet = &cache->indexSlotList[(address_index >> cache->offsetBits).to_ulong()];
//	
//	int i = (address_index >> cache->offsetBits).to_ulong();
//	// �ش� �ε��� ���Կ� Tag ��Ʈ�� ��ġ�ϴ� ĳ�� ������ �ִ��� �˻�
//	for (int index = 0; index < targetSet->slotCount; index++)
//	{
//		// Compulsory Miss 
//		// ĳ�� ������ valid bit�� 0
//		if (targetSet->slotList[index].validBit == 0)
//		{
//			// validBit 1�� ����, ������ Tag ��Ʈ ����
//			targetSet->slotList[index].tagBits = address_tag;
//			targetSet->slotList[index].validBit = 1;
//
//			// �ش� ������ LRU ����Ʈ �� �տ� �߰�
//			targetSet->LRU_list.push_front(index);
//			return;
//		}
//
//		// Hit 
//		// ĳ�� ������ Tag ��Ʈ�� �Ű������� ���� �ּ��� Tag ��Ʈ�� ��ġ
//		if (targetSet->slotList[index].tagBits == address_tag)
//		{
//			// �ش� ������ LRU ����Ʈ �� ������ �̵�
//			targetSet->LRU_list.remove(index);
//			targetSet->LRU_list.push_front(index);
//
//			// Hit ȸ�� ����
//			cache->hitCount++;
//			return;
//		}
//	}
//	// Tag ��Ʈ�� ��ġ�ϴ� ĳ�� ������ ������
//
//	// LRU ����Ʈ���� ���� ���� ���� ���� ������ ������
//	int targetIndex = targetSet->LRU_list.back();
//
//	// Conflict Miss 
//	// �ش� ������ Tag ��Ʈ�� ���ο� �ּ��� Tag�� ����
//	targetSet->slotList[targetIndex].tagBits = address_tag;
//
//	// �ش� ������ LRU ����Ʈ �� ������ �̵�
//	targetSet->LRU_list.remove(targetIndex);
//	targetSet->LRU_list.push_front(targetIndex);
//}
