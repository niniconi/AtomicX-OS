#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

struct font{
    unsigned char font[16];
    unsigned char code;
};

int main(int count,char *args[]){
    char *fin_name,*fout_name;
    int opt;
    char *string = "i:o:";
    while((opt = getopt(count,args,string)) != -1){
        if(opt == 'i'){
            fin_name = optarg;
        }else if(opt == 'o'){
            fout_name = optarg;
        }
    }
    FILE* fin = fopen(fin_name,"r");
    FILE* fout = fopen(fout_name,"w");
    if(!fin) {
            perror("Intpu File opening failed");
            return EXIT_FAILURE;
    }
    if(!fout){
            perror("Output File opening failed");
            return EXIT_FAILURE;
    }


    fprintf(fout,"unsigned char font_ascii[256][16] = {\n");

    int c;
    struct font f;

    char    temp[8],
        f_code[4],       //save f.code string data
        flag=0,          //size of f_code
        flag_s=0,        //^0x00     sign
        flag_content=0,  //content   sign
        index=0,         //size of temp
        index0=0;        //size of f.font

    while ((c = fgetc(fin)) != EOF) {
        if(flag == 4){
            #define LOW   f_code[3]
            #define HIGH  f_code[2]
            if(LOW<='9' && LOW >= '0'){
                f.code = LOW - 48;
            }else if(LOW<='z' && LOW >='a'){
                f.code = LOW - 97 + 0x0a;
            }else if(LOW<='Z' && LOW >='A'){
                f.code = LOW - 65 + 0x0a; 
            }
            if(HIGH<='9' && HIGH >= '0'){
                f.code |= (HIGH - 48) << 4;
            }else if(HIGH<='z' && HIGH >='a'){
                f.code |= (HIGH - 97 + 0x0a) << 4;
            }else if(HIGH<='Z' && HIGH >='A'){
                f.code |= (HIGH - 65 + 0x0a) << 4;
            }
            flag=0;
            flag_s=0;
        }
        if(flag_s){
            f_code[flag] = c;
            flag++;
        }
        switch(c){
            case '^':
                flag_s = 1;
                flag_content = 0;
                break;
            case '.':
                flag_content = 1;
                break;
            case '*':
                flag_content = 1;
                break;
            default:
                flag_content = 0;
                break;
        }
        if(flag_content){
            temp[index] = c;
            index++;
        }
        if(index == 8  && index0 < 16){
            unsigned char data = 0;
            if(temp[0] == '*')data |= 0x80;
            if(temp[1] == '*')data |= 0x40;
            if(temp[2] == '*')data |= 0x20;
            if(temp[3] == '*')data |= 0x10;
            if(temp[4] == '*')data |= 0x08;
            if(temp[5] == '*')data |= 0x04;
            if(temp[6] == '*')data |= 0x02;
            if(temp[7] == '*')data |= 0x01;
            f.font[index0] = data;
            index0++;
            index = 0;
        }else if(index0 == 16){
            int i;
            fprintf(fout,"\t//%X\n",f.code);
            fprintf(fout,"\t{");
            for(i=0;i<index0;i++){
                if(i == index0-1){
                    fprintf(fout,"%d",f.font[i]);
                    continue;
                }
                fprintf(fout,"%d,",f.font[i]);
            }
            if(f.code==255){
                fprintf(fout,"}\n");
            }else{
                fprintf(fout,"},\n");
            }
            index0 = 0;
            flag_content = 0;
        }
    }
    fprintf(fout,"};\n");
    return EXIT_SUCCESS;
}
