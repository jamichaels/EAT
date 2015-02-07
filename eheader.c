#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <elf.h>
#include "eat.h"

main(int argc, char **argv[]) {

    Elf64_Ehdr ehdr;
    Elf64_Shdr *shdr;
    Elf64_Phdr *phdr;

    int i;
    int fd;

    FILE* fp;

    if(argc < 2) { goto usage; }
    
    // Open the file and read the ELF header
    fp = fopen((char *)argv[1], "r");
    fread(&ehdr,sizeof(Elf64_Ehdr),1,fp);

    // Is it a valid ELF file?
    if( is_elf(ehdr) == -1) { not_elf(); } 

    printf("\nELF header for %s\n",(char *)argv[1]);

    printf(" File Class: ");
    if(ehdr.e_ident[4] == ELFCLASSNONE) { printf("invalid class\n");}
    if(ehdr.e_ident[4] == ELFCLASS32) { printf("32-bit onject\n");}
    if(ehdr.e_ident[4] == ELFCLASS64) { printf("64-bit object\n");}

    printf(" Data Encoding: ");
  //  if(ehdr.e_ident[5] == ELFDATANONE) { printf("Invalid data encoding\n");}
  //  if(ehdr.e_ident[5] == ELFDATALSB) { printf("Little endian (LSB)\n");}
  //  if(ehdr.e_ident[5] == ELFDATA2MSB) { printf("Big endian (MSB)\n"); }

    printf("  File Version %x\n", ehdr.e_ident[6]);

    printf("Object File Type: ");
    if(ehdr.e_type == ET_NONE) {printf("No file type\n");}
    if(ehdr.e_type == ET_REL) {printf("Relocatable file\n");}
    if(ehdr.e_type == ET_EXEC) {printf("Executable file\n");}
    if(ehdr.e_type == ET_DYN) {printf("Shared Object File\n");}
    if(ehdr.e_type == ET_CORE) {printf("Core file\n");}

    // edit
    printf("Required Architecture: ");
    printf("%d\n",ehdr.e_machine);

    //

    printf("Object File Version: ");
 //   if(ehdr.e_version == EV_NONE){printf("Invalid version\n");}
 //   if(ehdr.e_version == EVCURRENT){printf("Current version\n");}

    printf("Virtual Entry Address %li\n",ehdr.e_entry);
    printf("Program Header Offset: %li\n",ehdr.e_phoff);
    printf("Section Header Offset: %li\n",ehdr.e_shoff);
    printf("Processor Flags: %x\n",ehdr.e_flags);
    printf("ELF Header Size %x\n",ehdr.e_ehsize);
    printf("Program Header Entry Size: %x\n",ehdr.e_phentsize);
    printf("Number of Program Header Entries: %x\n",ehdr.e_phnum);
    printf("Section Header Entry Size: %x\n",ehdr.e_shentsize);
    printf("Number of Section Header Entries: %x\n",ehdr.e_shnum);
    printf("Index of string table in Section Header Table: %x\n",ehdr.e_shstrndx);
    exit(1);

usage:
    printf("\n64 bit ELF Header Reader v 0.01a - sblip - Metaphase\n");
    printf("usage: %s <file>\n",(char *)argv[0]);
    exit(-1);
}

not_elf()
{
    printf("\nThis file is not a valid elf object.\n");
    exit(-1);
}
