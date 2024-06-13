#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

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

char* SigFileName;
int bigEndian = 0;
char* userInputFile = NULL;
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
            fprintf(file, "%02X ", curr->vir->sig[i]);
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
void list_free(link *virus_list) {
    while (virus_list != NULL) {
        link *tmp = virus_list;
        virus_list = virus_list->nextVirus;
        if (tmp->vir != NULL) {
            if (tmp->vir->sig != NULL) {
                free(tmp->vir->sig);
                tmp->vir->sig = NULL;
            }
            free(tmp->vir);
            tmp->vir = NULL;
        }
        free(tmp);
    }
    virus_list = NULL;
}

/*================================================================================================================================*/

void quit(link *viruslist) {
    list_free(viruslist);
    free(SigFileName);
    if (userInputFile != NULL)
    {
        free(userInputFile);
    }
    exit(EXIT_SUCCESS);
}

void SetSigFileName( )
{
    char nameBuff[1024];
    printf("Please enter signature file name:\n");
    if (fgets(nameBuff, sizeof(nameBuff), stdin) == NULL)
    {
        perror("Error parsing signature name file.\n");
    }
    nameBuff[strcspn(nameBuff, "\n")] = '\0'; // remove the '\n' char
    free(SigFileName);
    if ((SigFileName = strdup(nameBuff)) == NULL)
    {
        printf("Error reading signature file name. Setting to default.");
        SigFileName = strdup("signatures-L");
    }
    
    
}

virus* readVirus(FILE* file)
{
    virus* virs = malloc(sizeof(virus));
    if (fread(virs, 1, 18, file) == 0) // ERROR/EOF
    {
        free(virs);
        return NULL;
    }
    if (bigEndian)
    {
        virs->SigSize = (virs->SigSize >> 8) | (virs->SigSize << 8);
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
    FILE* virusSignatures;
    
    virusSignatures = fopen(SigFileName, "rb");
    if(virusSignatures == NULL)
    {
        perror("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    char magicNumber[4];
    fread(magicNumber, 4, 1, virusSignatures);
    if (memcmp(magicNumber, "VIRL", 4) == 0)
    {
        bigEndian = 0;
    } else if (memcmp(magicNumber, "VIRB", 4) == 0)
    {
        bigEndian = 1;
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
    while ((virus = readVirus(virusSignatures)) != NULL)
    {
        virus_list = list_append(virus_list, virus);
    }
    fclose(virusSignatures);
}

int virus_offset(char *buffer, int starting_offset, unsigned char *sig, int sigSize)
{
    for (int i = starting_offset; i < FILE_MAX_SIZE - sigSize; i++)
    {
        if (memcmp(&buffer[i], sig, sigSize) == 0)
        {
            return i;
        }
    }
    return -1;
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    link *curr_virus = virus_list;
    while (curr_virus != NULL)
    {
        unsigned char *sig = curr_virus->vir->sig;
        int sigSize = curr_virus->vir->SigSize;
        int offset = 0;
        while (offset < size)
        {
            offset = virus_offset(buffer, offset, sig,sigSize);
            if (offset >= 0)
            {
                printf("\nStarting location of virus: %d\n", offset);
                printf("Virus name: %s\n", curr_virus->vir->virusName);
                printf("Virus signature size: %d\n", sigSize);
                offset += 1;
            } else
            {
                break;
            }
            
            
        }
        curr_virus = curr_virus->nextVirus;
    }
}

void detect_viruses(link *virus_list)
{
    FILE *suspectedFile;
    if((suspectedFile = fopen(userInputFile, "rb")) == NULL ) {
        perror("fail opening user input file"); 
        quit(virus_list);
    }
    char fileBuffer[FILE_MAX_SIZE];
    memset(fileBuffer, 0, FILE_MAX_SIZE);
    fread(fileBuffer, 1, FILE_MAX_SIZE, suspectedFile);
    fclose(suspectedFile);
    detect_virus(fileBuffer, FILE_MAX_SIZE, virus_list);
}

void neutralize_virus(char *fileName, int signatureOffset)
{
    FILE *fileToFix;
    if((fileToFix = fopen(fileName, "rb+")) == NULL) {
        perror("\nfail opening user input file\n"); 
        return;
    }
    fseek(fileToFix,signatureOffset,0);
    unsigned char ret = 0xC3;
    fwrite(&ret,sizeof(ret),1,fileToFix);
    fclose(fileToFix);
    printf("neutralized virus in offset: %d\n", signatureOffset);
}

void fix_file(link *virus_list)
{
    FILE *fileToFix;
    if((fileToFix = fopen(userInputFile, "r")) == NULL)
    {
        perror("\nfail opening user input file\n"); 
        quit(virus_list);
    }
    char fileBuffer[FILE_MAX_SIZE];
    memset(fileBuffer, 0, FILE_MAX_SIZE);
    fread(fileBuffer, 1, FILE_MAX_SIZE, fileToFix);
    fclose(fileToFix);
    link *curr_virus = virus_list;
    while (curr_virus != NULL)
    {
        unsigned char *sig = curr_virus->vir->sig;
        int sigSize = curr_virus->vir->SigSize;
        int offset = 0;
        while (offset < FILE_MAX_SIZE)
        {
            offset = virus_offset(fileBuffer, offset, sig,sigSize);
            if (offset >= 0)
            {
                neutralize_virus(userInputFile, offset);
                offset += 1;
            }else
            {
                break;
            }
        }
        curr_virus = curr_virus->nextVirus;
    }
}


struct fun_desc
{
    char *name;
    void (*fun)();
};


int main(int argc, char **argv)
{
    SigFileName = strdup("signatures-L");
    
    if (argc == 2)
    {
        char *filename = argv[1];
        filename[strcspn(filename, "\n")] = '\0';
        userInputFile = strdup(filename);
    }
    

    char s[5];

    struct fun_desc menu[] = {
        { "Set signatures file name", SetSigFileName }, { "Load signatures", load_sig }, { "Print signatures", list_print},
        { "Detect viruses", detect_viruses}, { "Fix file", fix_file }, { "Quit", quit},
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
                menu[userChoise].fun(virus_list, output);
                fclose(output);
            }
        } else if (userChoise == 3 || userChoise == 5) //detect viruses or quit
        {
            menu[userChoise].fun(virus_list);
        } else // userChoise == 4
        {
            if(userInputFile != NULL)
                menu[userChoise].fun(virus_list);
        }
        
        printf("DONE.\n");
    }
    list_free(virus_list);

    return (0);
}