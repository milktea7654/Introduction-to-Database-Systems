#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "hash.h"
#include "utils.h"
#include <sstream>
#include <cstring>

using namespace std;

hash_entry::hash_entry(int k, int v) : key(k), value(v), next(nullptr) {}

hash_bucket::hash_bucket(int h_key, int depth) : hash_key(h_key), local_depth(depth), num_entries(0), first(nullptr) {}

/* 清除分配給 this->first 的內存 */
void hash_bucket::clear() {
    hash_entry* current = first;
    while (current) {
        hash_entry* to_delete = current;
        current = current->next;
        delete to_delete;
    }
    first = nullptr;
    num_entries = 0;
}

hash_table::hash_table(int size, int b_size, int rows, vector<int> keys, vector<int> values) 
    : table_size(size), bucket_size(b_size), global_depth(1) {
    bucket_table.resize(table_size);
    for (int i = 0; i < table_size; i++) {
        bucket_table[i] = new hash_bucket(i, 1);
    }
    for (int i = 0; i < rows; i++) {
        insert(keys[i], values[i]);
    }
}

/* 當插入發生衝突時，需要重新哈希並分配桶中的條目 */
void hash_table::extend(hash_bucket* bkt) {
    bkt->local_depth++;

    if (bkt->local_depth > global_depth) {
        global_depth++;
        int old_size = table_size;
        table_size *= 2;
        bucket_table.resize(table_size);
        for (int i = 0; i < old_size; i++) {
            bucket_table[i + old_size] = bucket_table[i];
        }
    }

    int new_key = bkt->hash_key | (1 << (bkt->local_depth - 1));
    hash_bucket* new_bkt = new hash_bucket(new_key, bkt->local_depth);
    
    int base_index = bkt->hash_key & ((1 << (bkt->local_depth - 1)) - 1);
    int step = 1 << (bkt->local_depth - 1);
    for (int i = base_index; i < table_size; i += step) {
        if (bucket_table[i] == bkt) {
            bucket_table[i + step] = new_bkt;
        }
    }

    hash_entry* current = bkt->first;
    bkt->first = nullptr;
    bkt->num_entries = 0;

    while (current != nullptr) {
        hash_entry* next = current->next;
        int index = current->key & ((1 << bkt->local_depth) - 1);
        hash_bucket* target_bkt = bucket_table[index];
        
        current->next = target_bkt->first;
        target_bkt->first = current;
        target_bkt->num_entries++;
        
        current = next;
    }
}

void hash_table::insert(int key, int value) {
    int index = key & ((1 << global_depth) - 1);
    hash_bucket* bucket = bucket_table[index];

    while (bucket->num_entries >= bucket_size) {
        extend(bucket);
        index = key & ((1 << global_depth) - 1);
        bucket = bucket_table[index];
    }

    hash_entry* new_entry = new hash_entry(key, value);
    new_entry->next = bucket->first;
    bucket->first = new_entry;
    bucket->num_entries++;
}

/* 檢查是否需要將表的大小減半 */
void hash_table::half_table() {
    global_depth--;
    table_size /= 2;
    bucket_table.resize(table_size);
}

/* 如果一個桶沒有條目，檢查是否需要合併 */
void hash_table::shrink(hash_bucket* bkt) {
    if (bkt->local_depth <= 1) return;

    int pair_index = bkt->hash_key ^ (1 << (bkt->local_depth - 1));
    hash_bucket* pair_bkt = bucket_table[pair_index];

    if (pair_bkt->local_depth == bkt->local_depth) {
        if (bkt->num_entries == 0 || pair_bkt->num_entries == 0) {
            hash_bucket* empty = bkt->num_entries == 0 ? bkt : pair_bkt;
            hash_bucket* full = bkt->num_entries == 0 ? pair_bkt : bkt;

            int base = empty->hash_key & ((1 << empty->local_depth) - 1);
            int step = 1 << empty->local_depth;
            for (int i = base; i < table_size; i += step) {
                bucket_table[i] = full;
            }

            full->local_depth--;
            delete empty;
            shrink(full);
        }
    }
}

void hash_table::remove(int key) {
    int index = key & ((1 << global_depth) - 1);
    hash_bucket* bucket = bucket_table[index];
    
    hash_entry* current = bucket->first;
    hash_entry* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                bucket->first = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            bucket->num_entries--;
            shrink(bucket);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void hash_table::key_query(vector<int> query_keys, string file_name) {
    stringstream buffer;
    buffer << global_depth << endl;

    for (int key : query_keys) {
        int index = key & ((1 << global_depth) - 1);
        hash_bucket* bucket = bucket_table[index];
        int value = -1;
        
        hash_entry* current = bucket->first;
        while (current != nullptr) {
            if (current->key == key) {
                value = current->value;
                break;
            }
            current = current->next;
        }
        
        buffer << value << "," << bucket->local_depth << endl;
    }

    ofstream out(file_name);
    out << buffer.str();
    out.close();
}

void hash_table::remove_query(vector<int> query_keys) {
    for (int key : query_keys) {
        remove(key);
    }

    bool can_half = true;
    while (can_half && global_depth > 1) {
        can_half = true;
        for (int i = 0; i < table_size; i++) {
            if (bucket_table[i]->local_depth == global_depth) {
                can_half = false;
                break;
            }
        }
        if (can_half) {
            half_table();
        }
    }
}

void hash_table::clear() {
    for (int i = 0; i < table_size; i++) {
        if (bucket_table[i]) {
            bucket_table[i]->clear();
            delete bucket_table[i];
            bucket_table[i] = nullptr;
        }
    }
    bucket_table.clear();
    table_size = global_depth = bucket_size = 0;
}