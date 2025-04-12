#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

unsigned char b64ToB(unsigned char c){
    if(c >= 'A' && c <= 'Z'){
        c -= 'A';
    }else if(c >= 'a' && c <= 'z'){
        c -= 71;
    }else if(c >= '0' && c <= '9'){
        c += 4;
    }else if(c == '+'){
        c += 19;
    }else if(c == '/'){
        c += 16;
    }else{
        // padding
        c = 0;
    }

    return c;
}

int main(int argc, char** argv){
    if(argc < 2) return 1;

    char* src_file = argv[1];

    char binary_file_name[strlen(src_file) + 2];  // (strlen + 2) considers a possible 4th character in the new extension + \0 character
    txtToMp3Extension(src_file, binary_file_name);

    FILE* text_file = fopen(src_file, "r");

    FILE* output_file = fopen(binary_file_name, "wb");

    char c;
    unsigned char byte, last_byte, toWrite;

    for(int i = 0; (c = fgetc(text_file)) != EOF; i++){
        byte = b64ToB(c);
        
        switch(i % 4){
            case 0:
                last_byte = byte << 2;
                break;
            case 1:
                toWrite = last_byte | (byte >> 4);
                fwrite(&toWrite, 1, 1, output_file);
                last_byte = byte << 4;
                break;
            case 2:
                toWrite = last_byte | (byte >> 2);
                fwrite(&toWrite, 1, 1, output_file);
                last_byte = byte << 6;
                break;
            case 3:
                toWrite = last_byte | byte;
                fwrite(&toWrite, 1, 1, output_file);
                break;
        }
        
    }

    fclose(text_file);
    fclose(output_file);

    return 0;
}