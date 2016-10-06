//
//  main.c
//  player
//
//  Created by Woody Briggs on 09/06/2016.
//  Copyright © 2016 Woody Briggs. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    unsigned int headersize;
    unsigned int lastPlayed;
    unsigned int number_of_versions;
    unsigned long *bytes_per_version;
} HEADER;

unsigned int randChoice(unsigned int num_of_versions, unsigned int last_played);

int main(int argc, const char * argv[]) {
    
    if (argc == 1) {
        printf("Usage: input.rsf\n");
    }
    
    const char *path = argv[1];
    
    HEADER head;
    FILE   *filein = fopen(path, "r+b");
    
    fread(&head.headersize, sizeof(head.headersize), 1, filein);
    fread(&head.lastPlayed, sizeof(head.lastPlayed), 1, filein);
    fread(&head.number_of_versions, sizeof(head.number_of_versions), 1, filein);
    
    unsigned long num_of_bytes[head.number_of_versions];
    
    for (int i = 0; i < head.number_of_versions; ++i) {
        
        fread(&num_of_bytes[i], sizeof(unsigned long), 1, filein);
        
    }
    
    head.bytes_per_version = num_of_bytes;
    
    FILE *fileout = fopen("temp.mp3", "wb");
    
    unsigned int choice = randChoice(head.number_of_versions, head.lastPlayed);
    
    printf("%d\n", choice);

    fseek(filein, sizeof(head.headersize), SEEK_SET);
    fwrite(&choice, sizeof(choice), 1, filein);
    
    char *buffer = malloc(head.bytes_per_version[choice-1]);
    
    unsigned long seek_pos = 0;
    
    for (int i = 0; i < choice - 1; ++i) {
        
        seek_pos = head.bytes_per_version[i] + seek_pos;
        
    }
    
    seek_pos = seek_pos + head.headersize;
    
    fseek(filein, seek_pos, SEEK_SET);
    
    fread(buffer, 1, head.bytes_per_version[choice-1], filein);
    fwrite(buffer, 1, head.bytes_per_version[choice-1], fileout);
    
    free(buffer);
    
    system("afplay temp.mp3");
    
    return 0;
}


unsigned int randChoice(unsigned int num_of_versions, unsigned int last_played) {
    
    unsigned int choice;
    
    srand(time(NULL));
    
    while (choice == 0 || choice == last_played) {
        
        choice = rand() % ((num_of_versions + 1) * 10);
        
        choice = choice / 10;
    }
    
    return choice;
}