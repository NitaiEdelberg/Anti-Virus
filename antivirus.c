#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fileName = "signatures-L";

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

struct link {
    link *nextVirus;
    virus *vir;
} link;

struct linkedlist
{
    link *first;
    link *last;
    int size;
};

void SetSigFileName( )
{
    char buff[1024];
    printf("Please enter signiture file name:\n");
    if (fgets(buff, sizeof(buff), stdin) == NULL)
    {
        perror("Error parsing signiture name file.\n");
    }
    buff[strcspn(buff, "\n")] = 0; // remove the '\n' char
    fileName = buff;
}

virus* readVirus(FILE* file)
{
    virus *virus = malloc(sizeof(virus));
    fread(virus->SigSize, 18, 1, file); // handle error (reads sig and name)
    virus->sig = malloc(virus->SigSize);
    fread(virus->sig, virus->SigSize, 1, file); // handle error
    return virus;
}

void printVirus(virus* virus)
{
    printf("Virus name: %s\n", virus->virusName);
    printf("Virus size: %d\n", virus->SigSize);
    printf("Signature:\n");
    for (int i = 0; i < virus->SigSize; i++)
    {
        printf("%X ", virus->sig[i]);
        if (i % 20 == 0)
        {
            printf("\n");
        }
    }
}

/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE* file)
{
    link *curr = virus_list;
    while (curr != NULL)
    {
        fprintf(file, curr->vir);
        fprintf(file, "%s", "\n");
    }
    
    
}

link* list_append(link* virus_list, virus* data);
/* Add a new link with the given data at the beginning of the list and return a pointer to the list (i.e., the first link in the list). If the list is null - create a new entry and return a pointer to the entry. */
void list_free(link *virus_list);
/* Free the memory allocated by the list. */


int main(int argc, char **argv)
{
    FILE *suspectedFile = fopen(fileName, "rb+");
    char *magicNumber;
    fread(magicNumber, 4, 1, suspectedFile);
    if (strcmp(magicNumber, "VIRL") == 0)
    {
        //no need to change file;
    } else if (strcmp(magicNumber, "VIRB") == 0)
    {
        //change file type if VIRB
    } else
    {
        perror("Unknown file type.\n");
        exit(EXIT_FAILURE);
    }
    
    

    return (0);
}