/*
 * MotifUtils.cpp
 *
 *  Created on: Dec 2, 2018
 *      Author: ori
 */

#include "../includes/MotifUtils.h"

vector<vector<unsigned int> *> *permutations(vector<unsigned int> &nbunch) {
    vector<vector<unsigned int> *> *all_perm = new vector<vector<unsigned int> *>();
    for (int i = 0; i < nbunch.size(); i++) {
        for (int j = 0; j < nbunch.size(); j++) {
            if (i != j) {
                vector<unsigned int> *curr_perm = new vector<unsigned int>{nbunch[i], nbunch[j]};
                all_perm->push_back(curr_perm);
            }

        }
    }
    return all_perm;
}

vector<vector<int> *> *permutations(vector<int> &nbunch) {
    vector<vector<int> *> *all_perm = new vector<vector<int> *>();
    for (int i = 0; i < nbunch.size(); i++) {
        for (int j = 0; j < nbunch.size(); j++) {
            if (i != j) {
                vector<int> *curr_perm = new vector<int>{nbunch[i], nbunch[j]};
                all_perm->push_back(curr_perm);
            }

        }
    }
    return all_perm;
}

vector<vector<unsigned int> *> *combinations(vector<unsigned int> &nbunch) {
    vector<vector<unsigned int> *> *all_perm = new vector<vector<unsigned int> *>();
    for (int i = 0; i < nbunch.size(); i++) {
        for (int j = i + 1; j < nbunch.size(); j++) {
            vector<unsigned int> *curr_perm = new vector<unsigned int>{i, j};
            all_perm->push_back(curr_perm);
        }
    }
    return all_perm;
}


vector<vector<unsigned int> *> *neighbors_combinations(const unsigned int* neighbors, int64 start, int64 end){
    vector<vector<unsigned int> *> *all_perm = new vector<vector<unsigned int> *>();
    for (int64 i = start; i < end; i++) {
        for (int64 j = i + 1; j < end; j++) {
            vector<unsigned int> *curr_perm = new vector<unsigned int>{neighbors[i], neighbors[j]};
            all_perm->push_back(curr_perm);
        }
    }
    return all_perm;
}


vector<vector<int> *> *combinations(vector<int> &nbunch) {
    vector<vector<int> *> *all_perm = new vector<vector<int> *>();
    for (int i = 0; i < nbunch.size(); i++) {
        for (int j = i + 1; j < nbunch.size(); j++) {
            vector<int> *curr_perm = new vector<int>{nbunch[i], nbunch[j]};
            all_perm->push_back(curr_perm);
        }
    }
    return all_perm;
}



int bool_vector_to_int(vector<bool> &edges) {
    int sum = 0;
    int power = 1;
    for (int i = 0; i < edges.size(); i++) {
        sum += edges[i] * power;
        power *= 2;
    }
    return sum;
}

