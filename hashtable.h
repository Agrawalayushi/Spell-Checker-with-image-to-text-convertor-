// HASHTABLE IMPLEMENTATION
#ifndef TEMPLATE_HASH_TABLE
#define TEMPLATE_HASH_TABLE
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

const int HASH_SIZE = 100;

template <class ItemType>
class HashTable
{
public:
	HashTable(int hashSze = HASH_SIZE);
		
	bool IsEmpty(int key);
	
	bool  IsFull();
		
	int Hash(ItemType newItem);
	
	void Insert(ItemType newItem);
		
	void Append(int key, ItemType newItem);
		
	bool Remove(ItemType deleteItem, int key = -1);
		
	void Sort(int key);
		
	int TableSize();
	
	int TotalElems();
		
	int BucketSize(int key);
	
	int Count(ItemType searchItem);
	
	void MakeEmpty();
		
	~HashTable();
	
private:
	struct node	
	{			
		ItemType currentItem;		
		node* next;	
	};
	node** head; 		 //front of each hash table cell which is an array
	int hashSize; 		 // size of the hash table 
	int totElems;		 // total number of elements in the entire table
	int* bucketSize;	 // holds the total number of elems in each specific hash table cell
};


template<class ItemType>
HashTable<ItemType>::HashTable(int hashSze)
{
	hashSize = hashSze;
	head = new node*[hashSize];
	bucketSize = new int[hashSize];
	for(int x=0; x < hashSize; ++x)
	{
		head[x] = NULL;
		bucketSize[x] = 0;
	}
	totElems = 0;
}

template<class ItemType>
bool HashTable<ItemType>::IsEmpty(int key)
{
	if(key >=0 && key < hashSize)
	{
		return head[key] == NULL;
	}
	return true;
}/* End of IsEmpty */

template<class ItemType>
bool HashTable<ItemType>::IsFull()
{
	try
	{
		node* location = new node;
		delete location;
		return false;
	}
	catch(std::bad_alloc&)
	{
		return true;
	}
}

template<class ItemType>
int HashTable<ItemType>::Hash(ItemType newItem)
{	
	long h = 19937;
	std::stringstream convert;

	// convert the parameter to a string using "stringstream" which is done
	// so we can hash multiple datatypes using only one function
	convert << newItem;
	std::string temp = convert.str();

	for(unsigned x=0; x < temp.length(); ++x)
	{
		h = (h << 6) ^ (h >> 26) ^ temp[x];
	}
	return abs(h % hashSize);
} 
template<class ItemType>
void HashTable<ItemType>::Insert(ItemType newItem)
{
	if(IsFull())
	{
        //std::cout<<"nINSERT ERROR - HASH TABLE FULLn";
	}
	else
	{
		int key = Hash(newItem);
		Append(key,newItem);
	}
}

template<class ItemType>
void HashTable<ItemType>::Append(int key, ItemType newItem)
{
	if(IsFull())
	{
        //std::cout<<"nAPPEND ERROR - HASH TABLE FULLn";
	}
	else
	{
		node* newNode = new node;  // adds new node
		newNode-> currentItem = newItem;
		newNode-> next = NULL;
	
		if(IsEmpty(key))
		{
			head[key] = newNode;
		}
		else
		{
			node* tempPtr = head[key];
			while(tempPtr-> next != NULL)
			{
				tempPtr = tempPtr-> next;
			}
			tempPtr-> next = newNode;
		}
		++bucketSize[key];
		++totElems;
	}
}

template<class ItemType>
bool HashTable<ItemType>::Remove(ItemType deleteItem, int key) 
{
	bool isFound = false;
	node* tempPtr;
	if(key == -1)
	{
		key = Hash(deleteItem);
	}	

    if(IsEmpty(key))
    {
        //std::cout<<"nREMOVE ERROR - HASH TABLE EMPTYn";
    }
    else if(head[key]->currentItem == deleteItem)
    {
        tempPtr = head[key];
        head[key] = head[key]-> next;
        delete tempPtr;
        --totElems;
        --bucketSize[key];
        isFound = true;
    }
    else
    {        		
        for(tempPtr = head[key];tempPtr->next!=NULL;tempPtr=tempPtr->next)
        {
            if(tempPtr->next->currentItem == deleteItem)
            {
                node* deleteNode = tempPtr->next;
                tempPtr-> next = tempPtr-> next-> next;
                delete deleteNode;
                isFound = true;
                --totElems;
                --bucketSize[key];
                break;
            }
        }        
    }
	return isFound;
}

template<class ItemType>
void HashTable<ItemType>::Sort(int key)
{		
    if(IsEmpty(key))
    {
        //std::cout<<"nSORT ERROR - HASH TABLE EMPTYn";
    }
    else
    { 
        int listSize = BucketSize(key);
        bool sorted = false;

        do{
            sorted = true;
            int x = 0;
            for(node* tempPtr = head[key];
                tempPtr->next!=NULL && x < listSize-1;
                tempPtr=tempPtr->next,++x)
            {
                if(tempPtr-> currentItem > tempPtr->next->currentItem)
                {
                    ItemType temp = tempPtr-> currentItem;
                    tempPtr-> currentItem = tempPtr->next->currentItem;
                    tempPtr->next->currentItem = temp;
                    sorted = false;
                }
            }
            --listSize;
        }while(!sorted);
    }
}

template<class ItemType>
int HashTable<ItemType>::TableSize()
{
    return hashSize;
}

template<class ItemType>
int HashTable<ItemType>::TotalElems()
{
    return totElems;
}

template<class ItemType>
int HashTable<ItemType>::BucketSize(int key)
{
	return(!IsEmpty(key)) ? bucketSize[key]:0;
}

template<class ItemType>
int HashTable<ItemType>::Count(ItemType searchItem) 
{
	int key = Hash(searchItem);
	int search = 0;	

    if(IsEmpty(key))
    {
        //std::cout<<"nCOUNT ERROR - HASH TABLE EMPTYn";
    }
    else
    {        		
        for(node* tempPtr = head[key];tempPtr!=NULL;tempPtr=tempPtr->next)
        {
            if(tempPtr->currentItem == searchItem)
            {
                ++search;
            }
        }        
    }
	return search;
}

template<class ItemType>
void HashTable<ItemType>::MakeEmpty()
{
    totElems = 0;
    for(int x=0; x < hashSize; ++x)
    {		
        if(!IsEmpty(x))
        {
            //std::cout << "Destroying nodes ...n";
            while(!IsEmpty(x))
            {
                node* temp = head[x];
                //std::cout << temp-> currentItem <<std::endl;
                head[x] = head[x]-> next;
                delete temp;
            }			
        }
        bucketSize[x] = 0;
    }
}

template<class ItemType>
HashTable<ItemType>::~HashTable() 
{
	MakeEmpty();
	delete[] head;
	delete[] bucketSize;
}/* End of ~HashTable */

//   END OF THE HASH TABLE CLASS


#endif
