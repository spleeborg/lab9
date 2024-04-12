#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType data;
	struct HashType *next; //next so it is a chain structure
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	for (i=0;i<hashSz;++i)
	{
		struct HashType *current = &pHashArray[i];
        if (current->next != NULL)
        {
            printf("\nIndex %d -> ", i);

            while (current->next != NULL)
            {
				current = current->next;
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                
            }
			printf(NULL);
        }
    }
}


int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	int hashSz = 31;
	struct HashType* hashTable = calloc(hashSz, sizeof(struct HashType));

    // Insert based on index
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        struct HashType* newEntry = (struct HashType*) malloc(sizeof(struct HashType));

        if (newEntry == NULL)
        {
            printf("Memory allocation failed\n");
            exit(-1);
        }

        // Sets up the new Entrys values
        newEntry->data = pRecords[i];
        newEntry->next = hashTable[index].next;
        hashTable[index].next = newEntry;
    }

    // Display records in hash table
    displayRecordsInHash(hashTable, hashSz);

    // Free Memory
    for (int i = 0; i < hashSz; ++i)
    {
        struct HashType* curr = hashTable[i].next;
        while (curr != NULL)
        {
            struct HashType* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(hashTable);
    free(pRecords);
    
    return 0;
}