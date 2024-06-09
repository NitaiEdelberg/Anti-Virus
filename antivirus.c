#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_MAX_SIZE 10000

/*================================================================================================================================*/

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
struct link{
    link *nextVirus;
    virus *vir;
};

/*================================================================================================================================*/

char* fileName = "/home/devcontainers/ESPL/labB/signatures-L";
FILE* suspectedFile = NULL;
link* virus_list = NULL;

/*================================================================================================================================*/

/* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE* file)
{
    link *curr = virus_list;
    while (curr != NULL && file != NULL)
    {
        fprintf(file, "\nVirus name: %s\n",curr->vir->virusName);
        fprintf(file, "Virus size: %d\n",curr->vir->SigSize);
        fprintf(file, "Signature:\n");
        for (int i = 0; i < curr->vir->SigSize; i++)
        {
            fprintf(file, "%X ", curr->vir->sig[i]);
            if ((i + 1)% 20 == 0)
            {
                fprintf(file, "\n");
            }
        }
        fprintf(file, "\n");
        curr = curr->nextVirus;
    }
}

/* Add a new link with the given data at the beginning of the list and return a pointer to the list (i.e., the first link in the list). If the list is null - create a new entry and return a pointer to the entry. */
link* list_append(link* virus_list, virus* data)
{
    link *newLink = malloc(sizeof(link));
    newLink->nextVirus = virus_list; //if list is empty, virus_list is NULL
    newLink->vir = data;
    return newLink;
}

/* Free the memory allocated by the list. */
void list_free(link *virus_list)
{
    if (virus_list == NULL)
    {
        return;
    }

    free(virus_list->vir->sig);
    free(virus_list->vir);
    if (virus_list->nextVirus != NULL)
    {
        link *tmp = virus_list->nextVirus;
        list_free(tmp);
    }
    virus_list = NULL;
}

/*================================================================================================================================*/

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
    virus* virs = malloc(sizeof(virus));
    if (fread(virs, 1, 18, file) == 0) // ERROR/EOF
    {
        free(virs);
        return NULL;
    }
    virs->sig = malloc(virs->SigSize);
    fread(virs->sig, 1, virs->SigSize, file); // handle error
    return virs;
}

void printVirus(virus* virus)
{
    printf("\nVirus name: %s\n", virus->virusName);
    printf("Virus size: %d\n", virus->SigSize);
    printf("Signature:\n");
    for (int i = 0; i < virus->SigSize; i++)
    {
        printf("%X ", virus->sig[i]);
        if ((i + 1)% 20 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

void load_sig()
{
    if (suspectedFile != NULL)
    {
        fclose(suspectedFile);
    }
    
    suspectedFile = fopen(fileName, "rb");
    if(suspectedFile == NULL)
    {
        perror("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    char magicNumber[4];
    fread(magicNumber, 4, 1, suspectedFile);
    if (memcmp(magicNumber, "VIRL", 4) == 0)
    {
        //no need to change file
    } else if (memcmp(magicNumber, "VIRB", 4) == 0)
    {
        //change file type if VIRB
    } else
    {
        perror("Unknown file type.\n");
        exit(EXIT_FAILURE);
    }

    if (virus_list != NULL)
    {
        list_free(virus_list);
    }
    
    virus* virus;
    while ((virus = readVirus(suspectedFile)) != NULL)
    {
        virus_list = list_append(virus_list, virus);
    }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    link *curr_virus = virus_list;
    while (curr_virus != NULL)
    {
        char *sig = curr_virus->vir->sig;
        int sigSize = curr_virus->vir->SigSize;
        for (int i = 0; i < size - sigSize; i++)
        {
            if (memcmp(&buffer[i], sig, sigSize) == 0)
            {
                printf("\nStarting location of virus: %d\n", i);
                printf("Virus name: %s\n", curr_virus->vir->virusName);
                printf("Virus signature size: %d\n", sigSize);
                i = i + sigSize - 1; //can a virus apear once more?
            }
        }
        curr_virus = curr_virus->nextVirus;
    }
}

void detect_viruses(link *virus_list)
{
    suspectedFile = fopen(fileName, "r");
    char fileBuffer[FILE_MAX_SIZE];
    fread(fileBuffer, 1, FILE_MAX_SIZE, suspectedFile);
    fclose(suspectedFile);
    detect_virus(fileBuffer, FILE_MAX_SIZE, virus_list);
}

//TODO
void neutralize_virus(char *fileName, int signatureOffset)
{
    printf("not implemented");
}

void quit(link *viruslist) {
    list_free(viruslist);
    exit(EXIT_SUCCESS);
}


struct fun_desc
{
    char *name;
    void (*fun)();
};

void menu(void){
    char s[5];

    struct fun_desc menu[] = {
        { "Set signatures file name", SetSigFileName }, { "Load signatures", load_sig }, { "Print signatures", list_print},
        { "Detect viruses", detect_viruses}, { "Fix file", neutralize_virus }, { "Quit", quit},
        { NULL, NULL }
    };

    while (1){
        printf("\nSelect operation from the following menu (ctrl^D for exit):\n");
        size_t size = sizeof(menu) / sizeof(menu[0]) - 1;
        for (int i = 0; i < size; i++){
            printf("%d) %s\n", i, menu[i].name);
        }
        printf("Option: ");

        if (fgets(s, sizeof(s), stdin) == NULL){
            if (!feof(stdin)){
                perror("Error reading");
            }
            printf("\n");
            break;
        }
        if (s[0] < '0' || s[0] > '9')
        {
            printf("Input not legal\n");
            break;
        }
        int userChoise = atoi(s);
        if (userChoise < 0 || userChoise >= size){
            printf("\nNot within bounds\n");
            break;
        }
        printf("\nWithin bounds\n");
        if (userChoise == 0 || userChoise == 1)
        {
            menu[userChoise].fun();
        } else if (userChoise == 2)
        {
            char namebuff[50];
            printf("Enter file name: \n");
            if (fgets(namebuff, sizeof(namebuff), stdin) != NULL)
            {
                namebuff[strcspn(namebuff, "\n")] = '\0';
                FILE *output = fopen(namebuff, "w");
                menu[userChoise].fun(virus_list, suspectedFile);
                if (output != stdout)
                {
                    fclose(output);
                }
                
            }
        } else if (userChoise == 3 || userChoise == 5)
        {
            menu[userChoise].fun(virus_list);
        } else // userChoise == 4
        {
            menu[userChoise].fun(fileName, 0); //TODO
        }
        
        printf("DONE.\n");
    }
    list_free(virus_list);
}



int main(int argc, char **argv)
{
    // while (1)
    // {
    //     virus *virus = readVirus(suspectedFile);
    //     if(virus == NULL)
    //         break;
    //     printVirus(virus);
        
    // }
    menu();
    

    return (0);
}