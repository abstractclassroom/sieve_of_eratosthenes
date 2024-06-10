#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define BIT_SIZE 32
#define BIT_TYPE uint32_t
#define FULL_MASK 0xFFFFFFFF

struct PrimeData {
    int value;
    int array_size;
    BIT_TYPE* data;
};

int count_primes(struct PrimeData* PD) {
    int count = 0;
    int n = PD->array_size;
    BIT_TYPE* bit_array = PD->data;
    for (int i = 0; i < n; i++) {
        BIT_TYPE block = bit_array[i];
        BIT_TYPE mask = 1;
        for (int j = 0; j < BIT_SIZE; j++) {
            if (block & mask) {
                ++count;
            }
            mask <<= 1;
        }
    }
    return count;
}

void print_primes(struct PrimeData* PD) {
    int n = PD->array_size;
    BIT_TYPE* bit_array = PD->data;
    int p =0;
    for (int i = 0; i < n; i++) {
        BIT_TYPE block = bit_array[i];
        BIT_TYPE mask = 1;
        for (int j = 0; j < BIT_SIZE; j++) {
            if (block & mask) {
                printf("%d ", p);
            }
            ++p;
            mask <<= 1;
        }
    }
    printf("\n");
}

static inline void cancel_multiples(struct PrimeData* PD, int array_index, int bit_index) {
    int p = array_index * BIT_SIZE + bit_index;
    int start = p * p;
    int p_block = start / BIT_SIZE;
    int p_offset = start % BIT_SIZE;
    int bb = p_block;
    int bo = p_offset;
    for (int k = start; k <= PD->value; k += p) {
        bo -= (bo >= BIT_SIZE) ? (++bb,BIT_SIZE) : 0;
        PD->data[bb] &= ~((BIT_TYPE)1 << bo);
        bb += array_index;
        bo += bit_index;
    }
}

void sieve_of_eratosthenes(struct PrimeData* PD) {
    int n = PD->value;
    BIT_TYPE* bit_array = PD->data;
    int size = (n / BIT_SIZE) + 1;
    int upper_bound = (int)sqrt(n);
    int upper_bound_index = upper_bound / BIT_SIZE;
    int upper_bound_offset = upper_bound % BIT_SIZE;
    for (int i = 0; i < size-1; i++) bit_array[i] = FULL_MASK; 
    bit_array[size-1] = ((BIT_TYPE)1 << (n % BIT_SIZE)) - (BIT_TYPE)1;
    bit_array[0] &= ~((BIT_TYPE)1);
    bit_array[0] &= ~((BIT_TYPE)1 << 1);

    for (int i = 0; i<upper_bound_index; i++) {
        BIT_TYPE block = bit_array[i];
        BIT_TYPE mask = 1;
        for (int j = 0; j < BIT_SIZE; j++) {
            if (block & mask) cancel_multiples(PD, i, j);
            mask <<= 1;
        }
    }
    if (upper_bound_offset) {
        BIT_TYPE block = bit_array[upper_bound_index];
        BIT_TYPE mask = 1;
        for (int j = 0; j < upper_bound_offset; j++) {
            if (block & mask) cancel_multiples(PD, upper_bound_index, j);
            mask <<= 1;
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    struct PrimeData PD;
    PD.value = n;
    PD.array_size = (n / BIT_SIZE) + 1;
    PD.data = (BIT_TYPE*)malloc(PD.array_size * sizeof(BIT_TYPE));
    sieve_of_eratosthenes(&PD);
    int count = count_primes(&PD);
    printf("There are %d primes less than or equal to %d\n", count, n);
    // print_primes(&PD);
    free(PD.data);
    return 0;
}
