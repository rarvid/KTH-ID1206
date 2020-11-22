#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

// 20% of pages will have 80% of the refrences
#define HIGH 20
#define FREQ 80
#define SAMPLES 20

typedef struct pte {
    int id;
    int present;
    int referenced;
    struct pte *next;
} pte;


void init(int *sequence, int refs, int pages) {

    int high = (int)(pages * ((float)HIGH/100));

    for(int i = 0; i < refs; i++) {
        if(rand() % 100 < FREQ) {
            sequence[i] = rand() % high;
        } else {
            sequence[i] = high + rand() % (pages - high);
        }
    }
}

void clear_page_table(pte * page_table, int pages) {
    for(int i = 0; i < pages; i++) {
        page_table[i].present = 0;
    }
}

int simulate(int *seq, pte *table, int refs, int frames, int pages) {

    int hits = 0;
    int allocated = 0;
    pte *last = NULL;

    for(int i = 0; i < refs; i++) {
        int next = seq[i];                          // get ref number from sequence array
        pte *entry = &table[next];                  // save pte at that refrence number

        if(entry->present == 1) {
            entry->referenced = 1;
            hits++;                                 // if the entry exists in page table (hit)
        } else {
            if (allocated < frames) {
                allocated++;                        // if not in page table and there is space
                entry->present = 1;                 // insert the entry in page table

                if (last == NULL) {
                    last = entry;
                    last->next = last;
                } else {
                    entry->next = last->next->next;
                    last->next = entry;
                    last = entry;
                }

            } else {
                pte *cand = last->next;

                while(cand->referenced !=0) {
                    cand->referenced = 0;
                    last = cand;
                    cand = cand->next;
                }
                cand->present = 0;
                cand->referenced = 0;

                entry->present = 1;
                entry->referenced = 0;

                entry->next = cand->next;
                last->next = entry;
                last = entry;
                cand->next = NULL;
            }
        }
    }
    return hits;
}

int main(int argc, char *argv[]) {
    // command line arguments
    int refs = atoi(argv[1]);   // 10
    int pages= atoi(argv[2]);   // 100

    pte *table = (pte *)malloc(pages * sizeof(pte));

    printf("# This is a benchmark of random replacemnt\n");
    printf("# %d page refernces\n", refs);
    printf("# %d pages\n", pages);
    printf("#\n#\n# frames\tratio\n");

    int *sequence = (int *)malloc(refs*sizeof(int));
    init(sequence, refs, pages);

    int frames; // frames is the size of the memroy in frames

    int incr = pages/SAMPLES;

    for(frames = incr; frames <= pages; frames += incr) {
        clear_page_table(table, pages); // clears page table entries
        int hits = simulate(sequence, table ,refs, frames, pages);

        float ratio = (float)hits/refs;

        printf("%d\t\t%.2f\n", frames, ratio);
    }



    return 0;
}