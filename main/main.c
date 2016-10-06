//
//  main.c
//  main
//
//  Created by Woody Briggs on 09/06/2016.
//  Copyright © 2016 Woody Briggs. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int headersize;
    unsigned int lastPlayed;
    unsigned int number_of_versions;
    unsigned long *bytes_per_version;
} HEADER;

int main(int argc, const char * argv[]) {
    
    if (argc == 1) {
        printf("Usage: version1.mp3 version2.mp3... versionN.mp3\n");
    }
    
    HEADER comp;
    
    // find out number of versions entered and assign
    comp.number_of_versions = argc - 1;
    comp.lastPlayed = 0;
    
    // tempory storage for array size
    unsigned long num_of_bytes[comp.number_of_versions];
    
    // for each version, get size in bytes
    for (int i = 1; i < comp.number_of_versions + 1; ++i) {
        
        FILE * vf;
        
        vf = fopen(argv[i], "rb");
        
        fseek(vf, 0L, SEEK_END);
        
        unsigned long sz = ftell(vf);
        
        fclose(vf);
        
        num_of_bytes[i-1] = sz;
        
    }
    
    comp.bytes_per_version = num_of_bytes;
    
    // find out total size of header in bytes
    comp.headersize = sizeof(comp.number_of_versions)
                    + sizeof(comp.lastPlayed)
                    + (sizeof(comp.bytes_per_version) * comp.number_of_versions)
                    + sizeof(comp.headersize);
    
    FILE* output = fopen("output.rsf", "wb");
    
    if (output != NULL) {
        
        // write header to file
        fwrite(&comp.headersize, sizeof(comp.headersize), 1, output);
        fwrite(&comp.lastPlayed, sizeof(comp.lastPlayed), 1, output);
        fwrite(&comp.number_of_versions, sizeof(comp.number_of_versions), 1, output);
        fwrite(comp.bytes_per_version, sizeof(comp.bytes_per_version), comp.number_of_versions, output);
        
        FILE *verFile = NULL;
        
        char *buffer = malloc(comp.bytes_per_version[0]);
        
        if (buffer != NULL) {
            
            printf("Allocated\n");
            
            memset(buffer, 0, comp.bytes_per_version[0]);
        
            // write data to file
            for (int i = 1; i < comp.number_of_versions + 1; ++i) {
                
                verFile = fopen(argv[i], "rb");
                
                if (verFile != NULL) {
                    
                    size_t sizeTo = comp.bytes_per_version[i-1];
                    
                    size_t written;
                    size_t read;
                    
                    realloc(buffer, comp.bytes_per_version[i-1]);
                    
                    if (buffer != NULL) {
                        read = fread(buffer, 1, sizeTo, verFile);
                    }
                    
                    written = fwrite(buffer, 1, sizeTo, output);
                    
                    if (written != sizeTo) {
                        printf("Something went wrong!\n");
                    }
                    
                } else printf("failed to open file");
                
            }
        }
        
        fclose(verFile);
        fclose(output);
        free(buffer);
        
    }else printf("cant open");
    
    return 0;
}
