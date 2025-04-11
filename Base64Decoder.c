#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
typedef struct LinkedList LinkedList;

struct Node{
    char c;
    Node* next;
};

struct LinkedList{
    Node* head;
    Node* tail;
    void (*insert)(LinkedList*, Node*);
    void (*insertB64)(LinkedList*, char);
    char (*remove)(LinkedList*);
    unsigned char (*removeByte)(LinkedList*);
    int (*isEmpty)(LinkedList*);
};

void listInsert(LinkedList* list, Node* n){
    if(list->head == NULL){
        list->head = n;
    }else{
        list->tail->next = n;
    }

    while(n->next != NULL){
        n = n->next;
    }

    list->tail = n;
}

void listInsertB64(LinkedList* list, char c){
    Node* toBeInserted = NULL;
    Node* tmp;

    if(c == '='){
        // Padding

        for(int i = 0; i < 2; i++){
            tmp = toBeInserted;
            toBeInserted = (Node*) malloc(sizeof(Node));
            toBeInserted->c = '0';
            toBeInserted->next = tmp;
        }
    }else{
        // Not padding

        if(c >= 'A' && c <= 'Z'){
            c -= 65;
        }else if(c >= 'a' && c <= 'z'){
            c -= 71;
        }else if(c >= '0' && c <= '9'){
            c += 4;
        }else if(c == '+'){
            c += 19;
        }else if(c == '/'){
            c += 16;
        }
        
        for(int i = 0; i < 6; i++){
            tmp = toBeInserted;
            toBeInserted = (Node*) malloc(sizeof(Node));
            toBeInserted->c = c % 2 + '0';
            toBeInserted->next = tmp;
            c = c / 2;
        }
    }

    list->insert(list, toBeInserted);
}

char listRemove(LinkedList* list){
    Node* removedNode = list->head;

    if(removedNode == NULL) return 0;

    char c = removedNode->c;

    list->head = list->head->next;

    free(removedNode);

    return c;
}

int power(int n, int power){
    if(power == 0) return 1;

    int origin_n = n;
    
    for(int i = 1; i < power; i++){
        n *= origin_n;
    }

    return n;
}

unsigned char listRemoveByte(LinkedList* list){
    unsigned char b = 0;

    for(int i = 7; i >= 0; i--){
        b += (list->remove(list) - '0') * power(2, i);
    }

    return b;
}

int listIsEmpty(LinkedList* list){
    return list->head == NULL;
}

LinkedList* LinkedListConstructor(){
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));

    list->head = NULL;
    list->tail = NULL;
    list->insert = &listInsert;
    list->insertB64 = &listInsertB64;
    list->remove = &listRemove;
    list->removeByte = &listRemoveByte;
    list->isEmpty = &listIsEmpty;

    return list;
}

void txtToMp3Extension(char* src, char* dst){
    char file_extension[6] = "";
    int dot_start = 0;
    char c;

    printf("Type the output file extension: ");

    for(int i = 0; (c = getchar()) != '\n' && i < 5; i++){
        file_extension[i] = c;
    }

    while(dot_start < strlen(src) && src[dot_start] != '.'){
        dst[dot_start] = src[dot_start];
        dot_start++;
    }

    for(int i = 0; file_extension[i] != '\0'; i++){
        dst[dot_start] = file_extension[i];
        dot_start++;
    }

    dst[dot_start] = '\0';
}

int main(int argc, char** argv){
    if(argc < 2) return 1;

    char* src_file = argv[1];

    char binary_file_name[strlen(src_file) + 2];  // (strlen + 2) considers a possible 4th character in the new extension + \0 character
    txtToMp3Extension(src_file, binary_file_name);

    FILE* text_file = fopen(src_file, "r");

    FILE* audio_file = fopen(binary_file_name, "wb");

    LinkedList* list = LinkedListConstructor();

    char c;

    while((c = fgetc(text_file)) != EOF){
        list->insertB64(list, c);
    }

    unsigned char b;

    while(!list->isEmpty(list)){
        b = list->removeByte(list);
        fwrite(&b, 1, 1, audio_file);
    }

    fclose(text_file);
    fclose(audio_file);

    return 0;
}