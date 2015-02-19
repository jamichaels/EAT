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
    Elf64_Shdr sectHdr;
    int i;
    int fd;

    char* SectNames = NULL;
    FILE* fp;
    const char* name = "";
    int idx;

    if(argc < 2) 
    {
        printf("\n64 bit ELF Section Header Reader v 0.01a - sblip");
        printf("usage: %s <file>\n",(char *)argv[0]);
        exit(-1);
    }
    // Open the file and read the ELF header
    fp = fopen((char *)argv[1], "r");
    fread(&ehdr,sizeof(Elf64_Ehdr),1,fp);

    // Is it a valid ELF file?
    if( is_elf(ehdr) == -1) { not_elf(); } 

    fseek(fp,ehdr.e_shoff + ehdr.e_shstrndx * sizeof(sectHdr),SEEK_SET);
    fread(&sectHdr,sizeof(sectHdr),1,fp);
    
    SectNames = malloc(sectHdr.sh_size);
    fseek(fp,sectHdr.sh_offset,SEEK_SET);
    fread(SectNames,sectHdr.sh_size,1,fp);

  //  lseek(fp,ehdr.e_shoff,SEEK_SET);
    for(i=0;i<ehdr.e_shnum;i++)
    {
        name = "";
        fseek(fp, ehdr.e_shoff + i * sizeof(sectHdr), SEEK_SET);
        fread(&shdr,sizeof(shdr),1,fp);
        if(shdr.sh_name) {
            name = SectNames + shdr.sh_name;
        }
        else {
            name = " ";
        }
        printf("\nSection Name: %s\n",name);
        printf("Section Type: %d\n",shdr.sh_type);
        printf("Section Flags: %lx\n",shdr.sh_flags);
        printf("Section Virtual Address at Execution: %lx\n",shdr.sh_addr);
        printf("Section File Offset: %lx\n",shdr.sh_offset);
        printf("Section Size in Bytes: %lx\n",shdr.sh_size);
        printf("Link to Another Section: %x\n",shdr.sh_link);
        printf("Additional Section Information: %x\n",shdr.sh_info);
        printf("Section Alignment: %lx\n",shdr.sh_addralign);
        printf("Entry size if section holds table: %lx\n",shdr.sh_entsize);
    }
   // printf("Section Header Offset: %li\n",ehdr.e_shoff);
   // printf("Section Header Entry Size: %x\n",ehdr.e_shentsize);
   // printf("Number of Section Header Entries: %x\n",ehdr.e_shnum);
   // printf("Index of string table in Section Header Table: %x\n",ehdr.e_shstrndx);
   // exit(1);

}

not_elf()
{
    printf("\nThis file is not a valid elf object.\n");
    exit(-1);
}
