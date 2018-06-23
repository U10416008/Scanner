#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> 
#include "type.h"

struct node{
    int line_count_start;
    int line_count;
    char* type;
    char* name;
    struct node* next;
};
struct node* head;
struct node* tail;
//free(nodePtr);
struct node* createNode(int line_count,char* type, char* name){
    struct node* nodePtr = (struct node*)malloc(sizeof(struct node));
    nodePtr -> type = (char *) malloc(strlen(type));
    strcpy(nodePtr -> type , type);
    nodePtr->line_count = line_count;
    nodePtr->line_count_start = -1;
    nodePtr -> name = (char *) malloc(strlen(name));
    strcpy(nodePtr -> name , name);
    nodePtr->next = NULL;
    return nodePtr;
}

struct node* insertNodetoTail(struct node* tail,struct node* newNodePtr){
    if(tail ==NULL){
        return newNodePtr;
    }else{
        tail->next = newNodePtr;
        return newNodePtr;
    }

}
struct node* removeNode(struct node* head , struct node** nextRef){
    *nextRef = head;
    if(head!=NULL){
        
        head = head->next;
    }

    (*nextRef)->next = NULL;
    return head;
}
void printList(struct node* head){
    FILE *f = fopen("./output.txt", "w");
    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }
    if(head == NULL){
        fprintf(f,"List is empty\n");
        printf("List is empty\n");
    }else{
        int i = 0;
        struct node* curt = head;
        while(curt !=NULL){
            if(curt->line_count_start>=0){
                fprintf(f,"%d-",curt->line_count_start);
                printf("%d-",curt->line_count_start);
            }
            fprintf(f,"%d\t%s\t%s\n",curt->line_count,curt->type,curt->name);
            printf("%d\t%s\t%s\n",curt->line_count,curt->type,curt->name);

            curt = curt->next;
            i++;
        }
        
    }
    fclose(f);
}


const int half_STR = 99;
const int half_CHAR = 100;
int now_type = -1;
int isMC = 0;
int isSC = 0;
int line_count = 0;
int between_oper = 0;
int point_before_oper = 0;
int hasfloat = 0;
int isNeg = 0;
int line_count_start = 0;
char* save_STR;
char* save_CHAR;
int single_quo;
int double_quo;
int isINTEorFLOAT(char* number){
    int i = 0;
    int point_num=0;
    int e_num = 0;
    int neg_sign = 0;
    if(strcmp(number,"")==0){
        return -1;
    }
    for(i = 0 ; i < strlen(number); i++){
        if(number[0] == '-'&& neg_sign == 0){
            neg_sign ++;
            continue;
        }
        if(number[i]=='.'&& point_num == 0 && e_num == 0){
            point_num ++;
        }else if(number[i] == 'e' && e_num == 0 && (number[i+1] == '+' || number[i+1] == '-')){
            i++;
            e_num++;
        }else if(isdigit(number[i]) == 0 ){
            if(number[i] == 'e'){
                printf("%d\t%s\t%s\n",line_count,type[ERROR],number);
            }
            return -1;
        }
    }
    if (point_num == 0 && e_num == 0){
        now_type = INTE;
        struct node* newPtr = createNode(line_count, type[INTE],number);
        tail = insertNodetoTail(tail,newPtr);
        printf("%d\t%s\t%s\n",line_count,type[INTE],number);
    }else{
        now_type = FLOT;
        struct node* newPtr = createNode(line_count, type[FLOT],number);
        tail = insertNodetoTail(tail,newPtr);
        printf("%d\t%s\t%s\n",line_count,type[FLOT],number);

    }
    return 1;
}
int IDENTIFIER(char* pch){
    char* identifier = (char*)malloc(strlen(pch));
    strcpy(identifier,pch);
    int length = strlen(identifier);
    int i = 0;

    for( i = 0 ; i < length ; i++){
        if(i == 0){
            
            if(!(isalpha(identifier[i]) == 1 || identifier[i] == '_')){
                //printf("1 %s\n",identifier);
                return isINTEorFLOAT(pch) ;
                
            }
            
            
        }
        //printf("2 %c\n",identifier[i]);
        if(!(isalpha(identifier[i]) == 1 || identifier[i] == '_'|| isdigit(identifier[i])==1)){
            //printf("2 %c\n",identifier[i]);
            return 0;
        }
    }
    now_type = IDEN;
    printf("%s:%lu,%s:%lu\n",pch,strlen(pch),identifier,strlen(identifier));
    printf("%d\t%s\t%s\n",line_count,type[IDEN],identifier);
    struct node* newPtr = createNode(line_count, type[IDEN],identifier);
    tail = insertNodetoTail(tail,newPtr);
    return 1;
}

int isOPER(char* oper_spec){
    //printf("in ISOPER:%s\n",oper_spec);
    int length = sizeof(oper) / sizeof(oper[0]);
    int length2 = sizeof(spec) / sizeof(spec[0]);
    int i = 0;
    int j = 0;
    if(oper_spec == NULL){
        return -1;
    }
    
    int oper_len = 0;
    int oper_i = -1;
    int curt = 0;
    point_before_oper = 0;
    hasfloat = 0;
    isNeg = 0;
    //printf("isOPER%s\n",oper_spec);
    for(i = 0 ; i < strlen(oper_spec) && curt == 0;i++){
        if(oper_spec[i] == '-'&& isdigit(oper_spec[i+1]) == 1 && isNeg ==0){
            i++;
            isNeg++;
            continue;
        }
        if((oper_spec[i] == '-'|| oper_spec[i] == '+')&& oper_spec[i-1] == 'e'&&hasfloat == 0 ){
            hasfloat = 1;
            continue;
        }
        if(oper_spec[i] == '.'){
            if(isINTEorFLOAT(oper_spec) == 1){
                        
                return -2;
            }
            if(( (i > 0 && isdigit(oper_spec[i-1]) == 1)|| isdigit(oper_spec[i+1]) == 1)&& point_before_oper == 0){
               point_before_oper = 1; 
               continue;
            }
        }
        
        for(j = 0; j < length;j++){
            /*if((strcmp(oper[j],"+")== 0|| strcmp(oper[j],"-")== 0)&& hasfloat ==1){
                continue;
            }
            if(strcmp(oper[j],".")== 0 && point_before_oper ==1){
                continue;
            }*/
            if(strlen(oper[j])>1){
                if(oper_spec[i] == oper[j][0]&& oper_spec[i+1] == oper[j][1]){
                    now_type =  OPER;
                    oper_len = 2;
                    oper_i = j;
                    
                }
            }else{
                if(oper_spec[i] == oper[j][0]&& oper_len < 1){
                    now_type =  OPER;
                    oper_len = 1;
                    oper_i = j;
                    
                }
            }
            
        }
        if(oper_len > 1){
            curt = 1;
        }
        for(j = 0 ; j < length2 ; j++){
            if(oper_spec[i] == spec[j][0]){
                if(oper_len  == 0){
                    now_type =  SPEC;
                    oper_i = j;
                    return j;
                }
            }
        }
        
    }
    if(curt == 1 || oper_len >0){
        now_type = OPER;
        return oper_i;
    }
    return -1;
    
}

int isREWD(char* check_string){
    int i = 0;
    //printf("%lu:%lu\n",sizeof(rewd),sizeof(rewd[0]));
    
    for(i = 0 ; i < sizeof(rewd)/sizeof(rewd[0]); i ++){
        if(strcmp(check_string,rewd[i]) == 0){
            //if(i == 10||i==11||i==12||i==13){
            now_type = REWD;
            return i;
        }
    }
    
    return -1;
}
char* cat(char* front,char* back){
    char * after_cat = (char*)malloc(strlen(front)+ strlen(back));
    strcpy(after_cat,front);
    strcat(after_cat,back);
    return after_cat; 
}
char* split(char* pch ,char* str, char* oper_spec){
    int i = 0 ;
    int arrive = -1;
    
    char* pch_head = strstr(str,oper_spec);
    char* new_pch = (char *) malloc(strlen(pch_head));
    if(isNeg == 1&& strstr(pch_head+1,oper_spec)){
        pch_head = strstr(pch_head+1,oper_spec);
        isNeg = 0;
    }
    if(point_before_oper == 1 && strstr(pch_head+1,oper_spec)){
        pch_head = strstr(pch_head+1,oper_spec);
        point_before_oper = 0;
    }
    if(hasfloat == 1&& strstr(pch_head+1,oper_spec)){
        pch_head = strstr(pch_head+1,oper_spec);
        hasfloat = 0;
    }
    
    //printf("%s\n",pch);
    //printf("%s,%s:%lu,%lu\n",oper_spec,pch_head+strlen(oper_spec),strlen(oper_spec),strlen(pch_head+strlen(oper_spec)));
    strncpy(new_pch,(pch_head+strlen(oper_spec)),strlen(pch_head+strlen(oper_spec)));
    new_pch[strlen(pch_head+strlen(oper_spec))] = '\0';
    //printf("split,pch:%s,pch_cmp:%s:%s\n",pch,pch_head,new_pch);
    if(pch!=NULL){
        //printf("%lu,%lu:%lu\n",strlen(pch),strlen(str),strlen(str)- strlen(pch_head));
        strncpy(pch,str,strlen(str)- strlen(pch_head));
        //printf("%s,%s:%s\n",pch,pch_head,new_pch);
        pch[strlen(str)- strlen(pch_head)] = '\0';
    }
    //printf("%s,%s:%s\n",pch,pch_head,new_pch);
    
    return new_pch;
}
int hasStringOrChar(char* pch){
    int i = 0;
    int double_quote_num = 0;
    int single_quote_num = 0;
    
    for(i = 0 ; i < strlen(pch);i++){
        if(pch[i] == '/' && (pch[i+1] == '/' || pch[i+1] == '*')&& double_quo == 0 && single_quo == 0){
            if((single_quote_num == 0 && double_quote_num ==0)|| i == strlen(pch)-2){
                return -1;
            }
        }

        if(pch[i] == quote_char[0] && single_quo==0 && single_quote_num == 0){
            
            if(double_quote_num== 1){
                double_quote_num++;
                //printf("second\n");
                return STR;
            }else if(single_quote_num == 0){
                double_quote_num++;
                //printf("first\n");
            } 
        }else if(pch[i] == quote_char[1] &&double_quo==0 && double_quote_num ==0){
            
            
            if(single_quote_num== 1){
                single_quote_num++;
                return CH;
            }else if(double_quote_num == 0){
                single_quote_num++;
                
            }
        }
    }
    if(single_quote_num ==1){
        
        return half_CHAR;
    }
    if(double_quote_num == 1 ){
        return half_STR;
    }
    return -1;
}
int checkStringOrCharEnd(int type_in , char* pch,char* rest3,int quote){
    if(type_in == STR){
        //printf("%s:%d\n",rest3,quote);
        strcpy(rest3,split(pch,rest3,quote_str[0]));
        if(quote%2 ==0){
            strcpy(rest3,split(pch,rest3,quote_str[0]));
        }
        printf("%d\t%s\t%s\n",line_count,type[type_in],pch);
        struct node* newPtr = createNode(line_count,type[type_in],pch);
        tail = insertNodetoTail(tail,newPtr);
        //printf("%s\n",rest3);
    }
    if(type_in == CH){
        //printf("%s:%d\n",rest3,quote);
        strcpy(rest3,split(pch,rest3,quote_str[1]));
        if(quote%2 ==0){
            strcpy(rest3,split(pch,rest3,quote_str[1]));
        }
        printf("%d\t%s\t%s\n",line_count,type[type_in],pch);
        struct node* newPtr = createNode(line_count,type[type_in],pch);
        tail = insertNodetoTail(tail,newPtr);
        //printf("%s\n",rest3);
    }
    return hasStringOrChar(rest3);
}

char* checkMC_END(char* pch){
    if(strstr(pch,"*/") != NULL&& isMC ==1){
        struct node* newPtr = createNode(line_count, type[MC_END],"");
        tail = insertNodetoTail(tail,newPtr); 
        tail -> line_count_start = line_count_start;
        now_type = MC_END;
        isMC = 0;
        return split(NULL,pch,"*/");
        //printf("%s",line);
    }
    return pch;
}
int context(char* line){
    char *rest = line;
    //printf("%s\n",line);
    line = checkMC_END(line);
    char * pch = strtok_r (line," ",&rest);
    
    if(isMC == 1 ){
        now_type = MC_START;
        return MC_START;
    }
    
    
    while (pch != NULL)
    {
        //hasQuote = hasStringOrChar(pch);
        //printf("before %s:%d\n",pch,hasQuote);
        //printf("%s\n",pch);
        if(strstr(pch,"/*")){
            if(pch[0] == '/' && pch[1] == '*'){
                isMC = 1;
                line_count_start = line_count;
                pch = checkMC_END(pch);
                if(isMC == 1){
                    return MC_START;
                }
            }
            isMC = 1;
            pch = strtok_r (pch,"/*",&rest);
            
            
            
            if (strstr(pch,"//")){

                if(pch[0] == '/' && pch[1] == '/'){
                    isSC = 1;
                    struct node* newPtr = createNode(line_count, type[SC],"");
                    tail = insertNodetoTail(tail,newPtr); 
                    return SC;
                }
                pch = strtok_r (pch,"//",&rest);
                isSC = 1;
                isMC = 0;
            }else{
                line_count_start = line_count;
            }
            
            
            
        }else if (strstr(pch,"//")){
            if(pch[0] == '/' && pch[1] == '/'){
                isSC = 1;
                struct node* newPtr = createNode(line_count, type[SC],"");
                tail = insertNodetoTail(tail,newPtr);
                return SC;
            }
            
            pch = strtok_r (pch,"//",&rest);
            isSC = 1;
            
        }
        
        if(isMC == 1){
            pch = checkMC_END(pch);
        }
        int rew = isREWD(pch);
        if(rew == -1){
            char *rest3 = (char*)malloc(strlen(pch));
            strcpy(rest3,pch);
            int quote_num = 0;
            /*if(hasQuote == STR){
                rest3 = split(pch,rest3,quote_str[0]);
                quote_num++;
            }
            if(hasQuote == CH){
                rest3 = split(pch,rest3,quote_str[1]);
                quote_num++;
            }*/
            
            

            
            int check_oper = isOPER(pch);
            
            if(check_oper >= 0){
                while(check_oper>=0){
                    
                    char *rest2 = pch; 
                    char *str = (char *)malloc(strlen(pch));
                    strcpy(str,pch);
                    //printf("after%s%s\n",pch,type[now_type]);
                    if(now_type == OPER){
                        
                        if(strlen(str) == strlen(oper[check_oper])){
                            printf("%d\t%s\t%s\n",line_count,type[OPER],oper[check_oper]);
                            struct node* newPtr = createNode(line_count, type[OPER],oper[check_oper]);
                            tail = insertNodetoTail(tail,newPtr);
                            break;
                        }else{
                            //pch = strtok_r (pch, oper[check_oper],&rest2);
                            //printf("before%s\n",pch);
                            //printf ("before,pch:%s\n",pch);
                            char* pch_cmp = split(pch,str,oper[check_oper]);
                            //printf ("pch:%s,pch_cmp:%s\n",pch,pch_cmp);
                            //printf("after%s:%s\n",pch,pch_cmp);
                            //printf("OPER%s\n",pch_cmp);
                            //int isNum = isINTEorFLOAT(pch);
                            if(strcmp(pch,"") != 0){
                                IDENTIFIER(pch);
                            }
                            printf("%d\t%s\t%s\n",line_count,type[OPER],oper[check_oper]);
                            struct node* newPtr = createNode(line_count, type[OPER],oper[check_oper]);
                            tail = insertNodetoTail(tail,newPtr);
                            if(strcmp(pch_cmp,"") != 0){
                                IDENTIFIER(pch_cmp);
                            }
                            /*if(str[0] == oper[check_oper][0]&&  isNum< 0){
                                if(strcmp(pch,"") != 0&& isNum <0){
                                    IDENTIFIER(pch);
                                }
                                printf("%d\t%s\t%s\n",line_count,type[OPER],oper[check_oper]);
                                struct node* newPtr = createNode(line_count, type[OPER],oper[check_oper]);
                                tail = insertNodetoTail(tail,newPtr);
                                if(strcmp(pch_cmp,"") != 0){
                                    IDENTIFIER(pch_cmp);
                                }
                                //printf("OPER1:%s\n",pch);
                            }else{
                                if(strcmp(pch,"") != 0&& isNum <0){
                                    IDENTIFIER(pch);
                                }
                                printf("%d\t%s\t%s\n",line_count,type[OPER],oper[check_oper]);
                                struct node* newPtr = createNode(line_count, type[OPER],oper[check_oper]);
                                tail = insertNodetoTail(tail,newPtr);

                                if(strcmp(pch ,  pch_cmp)!= 0){
                                    between_oper = 1;
                                }
                                //printf("OPER2:%s\n",pch);
                            }*/
                            
                            pch = pch_cmp;
                            
                            //strcpy(rest3,pch_cmp);
                            
                            //printf("OPER:%s\n",pch);
                            
                        }
                        /*if(now_type == FLOT || now_type == INTE){
                            now_type = OPER;
                            break;
                        }*/
                        now_type = OPER;
                        //printf("%s:%s\n",pch,oper[check_oper]);
                        
                    }else if(now_type == SPEC){
                        if(strlen(str) == strlen(spec[check_oper])){
                            printf("%d\t%s\t%s\n",line_count,type[SPEC],spec[check_oper]);
                            struct node* newPtr = createNode(line_count, type[SPEC],spec[check_oper]);
                            tail = insertNodetoTail(tail,newPtr);
                            break;
                        }else{
                            //pch = strtok_r (pch, spec[check_oper],&rest2);
                            //printf("before spec%s\n",pch);
                            char* pch_cmp = split(pch,str,spec[check_oper]);
                            //printf("after spec%s\n",pch);
                            if(strcmp(pch,"") != 0){
                                IDENTIFIER(pch);
                            }
                            printf("%d\t%s\t%s\n",line_count,type[SPEC],spec[check_oper]);
                            struct node* newPtr = createNode(line_count, type[SPEC],spec[check_oper]);
                            tail = insertNodetoTail(tail,newPtr);     
                            if(strcmp(pch_cmp,"") != 0){
                                IDENTIFIER(pch_cmp);
                            }
                            /*if(str[0] == spec[check_oper][0]){
                                if(strcmp(pch,"") != 0){
                                    IDENTIFIER(pch);
                                }
                                printf("%d\t%s\t%s\n",line_count,type[SPEC],spec[check_oper]);
                                struct node* newPtr = createNode(line_count, type[SPEC],spec[check_oper]);
                                tail = insertNodetoTail(tail,newPtr);     
                                if(strcmp(pch_cmp,"") != 0){
                                    IDENTIFIER(pch_cmp);
                                }
                                //printf("%s:%s\n",pch,spec[check_oper]);
                            }else{
                                if(strcmp(pch,"") != 0){
                                    IDENTIFIER(pch);
                                }
                                //printf("%s:%s\n",pch,spec[check_oper]);
                                printf("%d\t%s\t%s\n",line_count,type[SPEC],spec[check_oper]);
                                struct node* newPtr = createNode(line_count, type[SPEC],spec[check_oper]);
                                tail = insertNodetoTail(tail,newPtr);
                                if(strcmp(pch , pch_cmp) != 0){
                                    between_oper = 1;
                                }
                            }*/
                            
                            pch = pch_cmp;
                        }
                        if(now_type == FLOT || now_type == INTE){
                            now_type = SPEC;
                            break;
                        }
                        now_type = SPEC;
                        //printf("%s:%s\n",pch,spec[check_oper]);
                    }
                    //printf("%s\n",pch);
                    /*if(hasQuote == STR){
                        strcpy(pch,rest3);
                        hasQuote = checkStringOrCharEnd(hasQuote,pch,rest3,quote_num++);
                        if(quote_num%2 ==1){
                            quote_num++;
                        }
                        //printf("after%s\n",rest3);
                        strcpy(pch,rest3);
                    }
                    else if(hasQuote == CH){
                        strcpy(pch,rest3);
                        hasQuote = checkStringOrCharEnd(hasQuote,pch,rest3,quote_num++);
                        if(quote_num%2 ==1){
                            quote_num++;
                        }
                        //printf("after%s\n",rest3);
                        strcpy(pch,rest3);
                    }*/
                    check_oper = isOPER(pch);
                    
                    //printf("check:%s\n",pch);
                    //printf("checkpoint:%d,%s\n",check_oper,pch);
                    free(str);
                }
                /*if( check_oper!= -2){
                    if(strcmp(pch,"") != 0){
                        //printf("%d %s:%s\n",check_oper,"between",pch);
                        IDENTIFIER(pch);
                    }
                        //between_oper = 0;
                }*/
                /*if(between_oper == 1 && check_oper!= -2){
                    if(strcmp(pch,"") != 0){
                        //printf("%d %s:%s\n",check_oper,"between",pch);
                        IDENTIFIER(pch);
                    }
                    between_oper = 0;
                }*/
            }
            else if(now_type != FLOT && now_type != INTE &&check_oper != -2){
                /*if(hasQuote == STR){
                    strcpy(pch,rest3);
                    hasQuote = checkStringOrCharEnd(hasQuote,pch,rest3,quote_num++);
        
                    strcpy(pch,rest3);
                }
                else if(hasQuote == CH){
                    strcpy(pch,rest3);
                    hasQuote = checkStringOrCharEnd(hasQuote,pch,rest3,quote_num++);
    
                    strcpy(pch,rest3);
                }*/
                //printf("%d %s:%s\n",check_oper,"between",pch);
                IDENTIFIER(pch);
            }
            free(rest3);
        }else{
            printf("%d\t%s\t%s\n",line_count,type[REWD],rewd[rew]); 
            struct node* newPtr = createNode(line_count, type[REWD],rewd[rew]);
            tail = insertNodetoTail(tail,newPtr); 
        }

        
        if(isMC == 1){
            
            return MC_START;
        }
        if(isSC == 1){
            isSC = 0;
            return SC;
        }
        pch = strtok_r (NULL, " ",&rest);
        
        //printf ("after_while:%s\n",pch);
    }
    //free(rest);

    return -1;
}
int main(){
    //REWD IDEN EXPR
    //REWD ( CONDITION ){}
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    file = fopen("./sample.c","r");
    if(file == NULL){
        printf("file does not exist.");
        return 0;
    }
    int i = 0;
    head =createNode(0,"START","sample.c");
    tail = head;
    save_STR = "";
    save_CHAR = "";
    
    int hasQuote;
    while((read = getline(&line,&len,file)) != EOF ){

        //printf("Retrieved line of length %zu :\n", read);
        printf("Line %d : %s", ++line_count,line);
        //printf("%c\n",line[0]);
        if(line[0] == '#' && single_quo ==0 && double_quo == 0){
            struct node* newPtr = createNode(line_count, type[PREP],line);
            tail = insertNodetoTail(tail,newPtr);
            continue;
        }
        if(isMC == 1){
            context(line);
            continue;
        }
        hasQuote = hasStringOrChar(line);
        char * remainline = (char*) malloc(strlen(line));
        strcpy(remainline,line);
        while(hasQuote == STR || hasQuote == CH){
            //need strcat
            printf("single %d,double %d\n",single_quo,double_quo);
            if(double_quo == 1){
                
                remainline = split(line,remainline,quote_str[0]);

                save_STR = cat(save_STR,"\t\t");
                save_STR = cat(save_STR,line);
                printf("%d\t%s\t%s\n",line_count,type[STR],save_STR);
                //printf("%d\t%s\t%s\n",line_count,type[STR],line);
                strcpy(line,remainline);
                struct node* newPtr = createNode(line_count, type[STR],save_STR);
                tail = insertNodetoTail(tail,newPtr); 
                tail -> line_count_start = line_count_start;
                save_STR = "";
                
                double_quo = 0;
                hasQuote = hasStringOrChar(line);
            }else if(single_quo ==1){
                
                remainline = split(line,remainline,quote_str[1]);
                save_CHAR = cat(save_CHAR,"\t\t");
                save_CHAR = cat(save_CHAR,line);
                printf("%d\t%s\t%s\n",line_count,type[CH],save_CHAR);
                //printf("%d\t%s\t%s\n",line_count,type[CH],line);
                strcpy(line,remainline);
                single_quo = 0;
                struct node* newPtr = createNode(line_count, type[CH],save_CHAR);
                tail = insertNodetoTail(tail,newPtr); 
                tail -> line_count_start = line_count_start;
                save_CHAR = "";
                hasQuote = hasStringOrChar(line);
            }
            while(hasQuote == STR){
                
                line_count_start = line_count;
                remainline = split(line,remainline,quote_str[0]);
                context(line);
                
                remainline = split(line,remainline,quote_str[0]);
                
                //printf("%s,%s",line,remainline);
                printf("%d\t%s\t%s\n",line_count,type[STR],line);
                struct node* newPtr = createNode(line_count,type[STR],line);
                tail = insertNodetoTail(tail,newPtr);
                strcpy(line,remainline);
                hasQuote = hasStringOrChar(remainline);
            }
            while(hasQuote == CH){
                
                line_count_start = line_count;
                remainline = split(line,remainline,quote_str[1]);
                context(line);
                remainline = split(line,remainline,quote_str[1]);
                printf("%d\t%s\t%s\n",line_count,type[CH],line);
                struct node* newPtr = createNode(line_count,type[CH],line);
                tail = insertNodetoTail(tail,newPtr);
                //printf("%s,%s",line,remainline);
                strcpy(line,remainline);
                hasQuote = hasStringOrChar(remainline);
            }
        }
        //need strcat
        if(!(hasQuote == STR || hasQuote == CH)){
            
            if(hasQuote == half_STR && single_quo == 0  ){
                if(double_quo == 0){
                    line_count_start = line_count;
                    remainline = split(line,remainline,quote_str[0]);
                    //printf("%d\t%s\t%s\n",line_count,type[STR],remainline);
                    save_STR = cat(save_STR,remainline);
                    printf("%d\t%s\t%s\n",line_count,type[STR],save_STR);
                    double_quo++;
                }else{
                    
                    remainline = split(line,remainline,quote_str[0]);
                    //printf("%d\t%s\t%s\n",line_count,type[STR],line);
                    save_STR = cat(save_STR,"\t\t");
                    save_STR = cat(save_STR,line);
                    printf("%d\t%s\t%s\n",line_count,type[STR],save_STR);
                    strcpy(line,remainline);
                    struct node* newPtr = createNode(line_count, type[STR],save_STR);
                    tail = insertNodetoTail(tail,newPtr); 
                    tail -> line_count_start = line_count_start;
                    save_STR = "";
                    //printf("save:%s\n",save_STR);
                    double_quo = 0;
                }
            }else if(hasQuote == half_CHAR && double_quo == 0){
                if(single_quo == 0){
                    line_count_start = line_count;
                    remainline = split(line,remainline,quote_str[1]);
                    save_CHAR = cat(save_CHAR,remainline);
                    //printf("%d\t%s\t%s\n",line_count,type[CH],remainline);
                    printf("%d\t%s\t%s\n",line_count,type[CH],save_CHAR);

                    single_quo++;
                }else{
                    
                    remainline = split(line,remainline,quote_str[1]);
                    //printf("%d\t%s\t%s\n",line_count,type[CH],line);
                    save_CHAR = cat(save_CHAR,"\t\t");
                    save_CHAR = cat(save_CHAR,line);
                    printf("%d\t%s\t%s\n",line_count,type[CH],save_CHAR);
                    strcpy(line,remainline);
                    struct node* newPtr = createNode(line_count, type[CH],save_CHAR);
                    tail = insertNodetoTail(tail,newPtr); 
                    tail -> line_count_start = line_count_start;
                    save_CHAR = "";
                    single_quo = 0;
                }
            }else if(single_quo == 1){
                save_CHAR = cat(save_CHAR,"\t\t");
                save_CHAR = cat(save_CHAR,line);
                printf("%d\t%s\t%s\n",line_count,type[CH],save_CHAR);
                //printf("%d\t%s\t%s\n",line_count,type[CH],line);
                
                continue;
                
            }else if(double_quo == 1){
                save_STR = cat(save_STR,"\t\t");
                save_STR = cat(save_STR,line);
                printf("%d\t%s\t%s\n",line_count,type[STR],save_STR);
                //printf("%d\t%s\t%s\n",line_count,type[STR],line);
                
                continue;
            }
            
            
            context(line);
            
            
            
        }
            //REWD_IDEN(line);
        
        
        
        

    }
    if(single_quo == 1){
        save_CHAR = cat(save_CHAR,line);
        printf("%d\t%s\t%s\n",line_count,type[ERROR],save_CHAR);
        //printf("%d\t%s\t%s\n",line_count,type[CH],line);
        struct node* newPtr = createNode(line_count, type[ERROR],save_CHAR);
        tail = insertNodetoTail(tail,newPtr); 
        tail->line_count_start = line_count_start;
        
    }else if(double_quo == 1){
        save_STR = cat(save_STR,line);
        printf("%d\t%s\t%s\n",line_count,type[ERROR],save_STR);
        //printf("%d\t%s\t%s\n",line_count,type[STR],line);
        struct node* newPtr = createNode(line_count, type[ERROR],save_STR);
        tail = insertNodetoTail(tail,newPtr); 
        tail->line_count_start = line_count_start;

    }
    printList(head);
    /*printf("%d\n",isREWD("int"));
    printf("%d\n",IDENTIFIER("0Hello"));*/
}