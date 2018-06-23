typedef enum{SC=0,MC_START,MC_END,IDEN,OPER,REWD,SPEC,INTE,FLOT,CH,STR,PREP,ORIGINAL,ERROR} TYPE;
char* type[] = {"SC","","MC","IDEN","OPER","REWD","SPEC","INTE","FLOAT","CHAR","STR","PREP","ORIGINAL","ERROR"};
typedef enum{ERROR_TYPE=40,ERROR_SEMANTIC,ERROR_SYNTAX,NO_SEMICOLON,GOOD} error;
typedef enum{IF=10,ELSE,WHILE,FOR,DO,SWITCH,CASE,DEFAULT,CONTINUE,INT,FLOAT,DOUBLE,CHAR,BREAK,STATIC,EXTERN,AUTO,REGISTER,SIZEOF,UNION,STRUCT,ENUM,RETURN,GOTO,CONST} REWD_ENUM;
char* rewd[] = {"if","else","while","for","do","switch","case","default","continue","int","float","double","char","break","static","extern","auto","register","sizeof","union","struct","enum","return","goto","const"};
char* oper[]={"+","-","*","/","=",",","%",">>","<<","++","--","+=","-=","*=","/=","%=","!","&&","||","&","[","]","|","^",".","->",">","<","==",">=","<=","!=",":"};
char* spec[]={"{","}","(",")",";"};
char quote_char[] = {'"','\''};
char* quote_str[] = {"\"","'"};