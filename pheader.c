#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>
#include "eat.h"

main(int argc, char **argv[]) {

    Elf64_Ehdr ehdr;
    Elf64_Shdr shdr;
    Elf64_Phdr phdr;

    int i;
    int fd;

    FILE* fp;

    if(argc < 2) { goto usage; }
    
    // Open the file and read the ELF header
    fp = fopen((char *)argv[1], "r");
    fread(&ehdr,sizeof(Elf64_Ehdr),1,fp);

    // Is it a valid ELF file?
    if( is_elf(ehdr) == -1) { not_elf(); } 

    printf("\nProgram header info for %s\n",(char *)argv[1]);

    if(ehdr.e_ident[4] == ELFCLASS64) { printf("64-bit object\n");}

    printf("Program Header Offset: %li\n",ehdr.e_phoff);
    printf("Number of Program Header Entries: %x\n",ehdr.e_phnum);
    printf("Program Header Entry Size: %x\n",ehdr.e_phentsize);
    
    lseek(fp,ehdr.e_phoff,0);
    for(i=0;i<ehdr.e_phnum;i++) {

        fread(&phdr,sizeof(Elf64_Phdr),1,fp);
            printf("Entry:\n");
            printf("Segment type: 0x%x\n",(Elf64_Word)phdr.p_type);
            printf("Segment flags: %x\n",phdr.p_flags);
            printf("Segment file offset: %li\n",phdr.p_offset);
            printf("Segment virtual address: %li\n",phdr.p_vaddr);
            printf("Segment physical address: %li\n",phdr.p_paddr);
            printf("Segment size in file: %li\n",phdr.p_filesz);
            printf("Segment size in memory: %li\n",phdr.p_memsz);
            printf("Segment alignment: %li\n",phdr.p_align);

            printf("\n");

    }


usage:
    printf("\n64 bit ELF Program Header Reader v 0.01a - sblip - Metaphase\n");
    printf("usage: %s <file>\n",(char *)argv[0]);
    exit(-1);
}

not_elf()
{
    printf("\nThis file is not a valid elf object.\n");
    exit(-1);
}
