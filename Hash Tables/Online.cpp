#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <random>
#include <iomanip>
#include <cmath>

#define SeparateChaining 0
#define DoubleHashing 1 
#define CustomProbing 2

using namespace std;

struct Item {
    string key;
    int data;

	Item(const string& k, int val) : key(k), data(val) {}
};

class HashTable {

	vector<list<Item> > table;
	vector<int> Sizes;
	int resolutionMethod;

	bool chaining, doubhash, cusprob;
	int numOfItems;
	int tableSize, init_N;
	int maxChainLength, MCL;
	int NumOfCollisions, Prob_Count;
	int C1, C2;;


	// int hash1(const string& key) {
    //     unsigned int h = 0;
    //     for (int i = 0; i < key.size(); i++) { 
    //         h = (h << 5) | (h >> 27);
    //         h += (unsigned int) key[i];
    //     }
        
	// 	if (h < 0) {
	// 		return static_cast<int>((h + static_cast<unsigned int>(tableSize)) % static_cast<unsigned int>(tableSize));
	// 	}
    //     return static_cast<int>(h % static_cast<unsigned int>(tableSize));
    // }

    //DJB Hash
    int hash1(const string& str) {
   	    unsigned int hash = 5381; 
    	
    	for (char c : str) {
        hash = (hash << 5) + hash + c; 
    	}

    	int hashVal = static_cast<int>(hash % static_cast<unsigned int>(tableSize));

    	if (hashVal < 0) return (hashVal+tableSize) % tableSize;

    	return hashVal;
	}	


    // Jenkin's Hash Function
	int hash2(const string& key) {
	    unsigned int hash = 0; 

	    for (unsigned int i = 0; i < key.size(); ++i) {
	        hash += static_cast<unsigned int>(key[i]); 
	        hash += (hash << 10);
	        hash ^= (hash >> 6);
	    }

	    hash += (hash << 3);
	    hash ^= (hash >> 11);
	    hash += (hash << 15);

	    return static_cast<int>((hash) % static_cast<unsigned int>(tableSize));
	}


    int auxHash(const string & key) {

		int hashVal = 0;
		for (char ch : key)
		hashVal = 37 * (int)(ch - 'a') + hashVal;

		if (hashVal < 0) return (hashVal + tableSize) % tableSize;
		
		return hashVal % tableSize;
	}


	int doubleHash(const string& key, int i) { 
		int index = (hash1(key) + (i * auxHash(key)));

		if (index < 0) return (index + tableSize) % tableSize;
		
		return ( index % tableSize );
	}

	int customHash(const string& key, int i) {
		int index = (hash1(key) + (C1 * i * auxHash(key)) + (C2 * i * i)); 
		
		if (index < 0) {
			return (index + tableSize) % tableSize;
		}
		return (index % tableSize);
	}



	int getNextPrime(int i) {
		int n = i;
        while (!isPrime(n)) {
            ++n;
        }
        return n;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

public:

	HashTable(int N, int C, int method) {

        int n_prime = getNextPrime(N);
        tableSize = n_prime;
        numOfItems = 0;
        maxChainLength = C;
		MCL = 0;
		NumOfCollisions = 0;
		Prob_Count = 0;
        table.resize(tableSize);
        resolutionMethod = method;
        init_N = n_prime;

		Sizes.resize(tableSize, 0);

		C1 = 5; C2 = 7;

        chaining = false; 
        doubhash = false;
        cusprob = false;

        if (method == 1) {
      	  doubhash = true;
    	} else if (method == 2) {
       	  cusprob = true;
    	} else {
		  chaining = true;	
		}

    }


    ~HashTable() {
		
		for (auto & row: table) {
			row.clear();
		}
    }


    void rehashUp() {

		int oldTableSize = tableSize;
		int newTableSize = getNextPrime(tableSize * 2); 

		vector<list<Item> > newTable(newTableSize); 
		Sizes.resize(newTableSize);
		tableSize = newTableSize; 
		for (int i = 0; i < newTableSize; i++) Sizes[i] = 0;
		
		
		for (int i = 0; i < oldTableSize; ++i) {
			for (const auto& item : table[i]) {
				string newKey = item.key;
				int newIndex = hash1(newKey); 
				newTable[newIndex].push_back(item);
				Sizes[newIndex]++;
			}
		}

		MCL = 0;
		for (int i = 0; i < newTableSize; ++i) {
			if (MCL < Sizes[i]) {
				MCL = Sizes[i];
			}
		}

		table = std::move(newTable); 
		cout << "Rehashing Up completed. New table size: " << tableSize << endl;
	}



	void rehashDown() {
		if (getNextPrime(tableSize / 2) <= init_N) return;

		int oldTableSize = tableSize;
		int newTableSize = getNextPrime(tableSize / 2); 

		vector<list<Item> > newTable(newTableSize); 
		Sizes.resize(newTableSize);
		tableSize = newTableSize; 
		for (int i = 0; i < newTableSize; i++) Sizes[i] = 0;
		
		for (int i = 0; i < oldTableSize; ++i) {
			for (const auto item : table[i]) {
				string newKey = item.key;
				int newIndex = hash1(newKey); 
				newTable[newIndex].push_back(item);
				Sizes[newIndex]++;
			}
		}

		MCL = 0;
		for (int i = 0; i < newTableSize; ++i) {
			if (MCL < Sizes[i]) {
				MCL = Sizes[i];
			}
		}

		table = std::move(newTable); 
		cout << "Rehashing Down completed. New table size: " << tableSize << endl;

	}



	string generateRandomWord() {
    	const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

    	int length = rand() % 6 + 5; 
    	string randomWord = "";

    	for (int i = 0; i < length; ++i) {
        	randomWord += alphabet[rand() % (sizeof(alphabet) - 1)];
    	}
    	return randomWord;
	}

	int calculateMaxChain() {
		int mcl = -50;
		for (int i = 0; i < tableSize; ++i) {
			if (Sizes[i] > mcl) {
				mcl = Sizes[i];
			}
		}
		MCL = mcl;
		return MCL;
	}

    //Insert works perfectly
	void Insert(const string& key, int value) {

        int index = hash1(key);
		int i=0;

		if (doubhash) {		
			index = doubleHash(key,i);
				if (!table[index].empty()) {
					NumOfCollisions++;
				}
				while (!table[index].empty()) {
					i++;
					Prob_Count++;
					index = doubleHash(key, i);
					if (i > tableSize)	{
						return;
					}
				}
			table[index].push_back(Item(key,value));
			numOfItems++;
			return;			
		} else if (cusprob) {
			index   = customHash(key,i);
				if (!table[index].empty()) {
					NumOfCollisions++;
				}
				while (!table[index].empty()) {
					i++;
					Prob_Count++;
					index = customHash(key, i);
					if (i > tableSize)	{
						return;
					}
				}
			table[index].push_back(Item(key,value));
			numOfItems++;
			return;
		}

		if (!table[index].empty()) NumOfCollisions++;
		
		table[index].push_back(Item(key,value));
		Sizes[index]++;
		numOfItems++;
		return;
	}


	//Delete works perfectly
	void Delete(const string& key) {
    
    int index = hash1(key);

    if (doubhash) {
        int i = 0;
		index = doubleHash(key, i);
        while (!table[index].empty() && table[index].front().key != key && i <= tableSize) {
			i++;
			Prob_Count++;
			index = doubleHash(key, i);
		}
        if (table[index].front().key != key) return;
		table[index].pop_back();
		return;
    } else if (cusprob) {
        int i = 0;
		index = customHash(key, i);
        while (!table[index].empty() && table[index].front().key != key && i <= tableSize) {
			i++;
			Prob_Count++;
			index = customHash(key, i);
		}
        if (table[index].front().key != key) return;
		table[index].pop_back();
		return;
    }

    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->key == key) {
            table[index].erase(it);
            Sizes[index]--;
            return;
        	}
    	}
	}


	// Find works perfectly
	int Find(const string& key) {

		int index = hash1(key);

		if (doubhash) {
			int i = 0;
			index = doubleHash(key, i);
			
        	while (!table[index].empty() && table[index].front().key != key && i <= tableSize) {
				i++;
				Prob_Count++;
				
				index = doubleHash(key, i);
			}
        if (table[index].front().key != key) return -1;

		return table[index].front().data;
    } else if (cusprob) {
        int i = 0;
		index = customHash(key, i);
		
        while (!table[index].empty() && table[index].front().key != key && i <= tableSize) {
			i++;
			Prob_Count++;
			
			index = customHash(key, i);
		}
        if (table[index].front().key != key) return -1;

		return table[index].front().data;
	 }

	 	
	    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
	      
	        if (it->key == key) {
	            return it->data;
	        }
	    }
    	return -1;
	}	

	int getSize(const string& key) {
		int index = hash1(key);
		return Sizes[index];
	}
};


void online(int l, int k,const string& dna) {

	HashTable Table(pow(4,k), 10000, SeparateChaining);

	for (int i = 0; i <= dna.length() - k; i++) {
		string key = dna.substr(i,k);
		Table.Insert(key, i);
	}

	for (int i = 0; i <= dna.length() - k; i++) {
		string key = dna.substr(i,k);

		if (Table.Find(key) == -1) continue;
		cout << key << " - " << Table.getSize(key) << endl;
		
		for (int j=0;j <= Table.getSize(key); j++) Table.Delete(key);
	}
}


int main() {

	
	int l,k;
	string dna;
	cin >> l >> k >> dna;
	online(l, k, dna);

	return 0;
}