#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#define size 100

typedef struct
{
    char symbol[30];
    char value[30];
    int equation;
    float result;
} Data;

struct Node
{
    char arr[100];
    float val;
    struct Node* left;
    struct Node* right;
};

typedef union
{
    float fData;
    char  cData;
} Item;

typedef struct
{
    Item arr[size];
    int top;
} Stack;

int isnumber(char * token)
{

    int i;
    int flag=0;
    for(i=0; i<strlen(token); i++)
    {
        if(isdigit(token[i]))
            flag=1;

    }
    return flag;
}
struct Node * newNode(char data[],float result)
{
    struct Node* n=malloc(sizeof(struct Node));
    strcpy(n->arr,data);
    n->val=result;
    n->left=NULL;
    n->right=NULL;
    return n;
}

struct Node* put(struct Node* node,float result, char data[])
{
    if (node == NULL)
    {
        return newNode(data,result);
    }
    if (strcasecmp(node->arr,data)>0)
    {
        node->left = put(node->left, result,data);
    }
    else if (strcasecmp(node->arr,data)<0)
    {
        node->right = put(node->right, result,data);
    }
    return node;
}
void InOrdertraverse(struct Node* t)
{
    if(t!=NULL)
    {
        InOrdertraverse(t->left);
        printf("%s= %.1f\n",t->arr,t->val);
        InOrdertraverse(t->right);
    }

}

Stack * initialize()
{
    Stack *s=malloc(sizeof(Stack));
    s->top=0;
    return s;
}

int isEmpty(Stack *s)
{
    if(s->top==0)
    {
        return 1;
    }
    return -1;
}

Item top(Stack *s)
{
    Item m2;
    m2=s->arr[s->top-1];
    return m2;
}

Item pop(Stack *s)
{
    Item m1;
    s->top--;
    m1=s->arr[s->top];
    return m1;
}

void push(Stack *s, Item val)
{
    s->arr[s->top]=val;
    s->top++;
}

int precidence(char b)
{

    if(b=='^')
        return 6;
    else if(b=='*' || b=='/')
        return 5;
    else if(b=='+' || b=='-')
        return 2;
    else if(b=='(' || b==')')
        return 0;
    return 0;
}


void infixToPostfix(char* infix, char* postfix)
{

    postfix[0]='\0';
    Item m;
    Stack *s=initialize();
    char* token =strtok(infix,"  ");
    while(token!=NULL)
    {
        if(isnumber(token))
        {
            strcat(postfix,token);
            strcat(postfix," ");
        }
        else
        {
            if((isEmpty(s)==1||top(s).cData=='(') && *token!=')')
            {
                m.cData=*token;
                push(s,m);
            }
            else if (*token == '(')
            {
                m.cData=*token;
                push(s, m);
            }
            else if (*token == ')')
            {
                while (top(s).cData != '(' && isEmpty(s)!=1)
                {
                    char c = (pop(s).cData);
                    strncat(postfix,&c,1);
                    strcat(postfix," ");
                }
                pop(s);
            }
            else if(precidence(*token)>precidence(top(s).cData))
            {
                m.cData=*token;
                push(s,m);

            }

            else
            {
                while(isEmpty(s)== -1 && precidence(*token)<=precidence(top(s).cData))
                {
                    char c = (pop(s).cData);
                    if(c!='(')
                    {
                        strncat(postfix, &c, 1);
                        strcat(postfix," ");
                    }

                }
                m.cData=*token;
                push(s,m);
            }
        }
        token =strtok(NULL," ");
    }
    while(isEmpty(s)==-1)
    {
        char c = (pop(s).cData);
        if(c!='(')
        {
            strncat(postfix, &c, 1);
            strcat(postfix," ");
        }


    }
    strcat(postfix,"\0"); // extra space
    free(s);

}




float evaluatePostfix(char* postfix)
{
    Stack *s1=initialize();
    float op1,op2,result;
    Item m;
    char *token=strtok(postfix," ");
    while(token!=NULL)
    {
        if (isnumber(token))
        {
            float f=atof(token);
            m.fData=f;
            push(s1,m);
        }
        // If the scanned character is an operator, pop two
        // elements from stack apply the operator
        else
        {
            op2 = pop(s1).fData;
            op1= pop(s1).fData;
            switch (*token)
            {
            case '+':
                m.fData=op2+op1;
                push(s1,m);
                break;
            case '-':
                m.fData=op1-op2;
                push(s1,m);
                break;
            case '*':
                m.fData=op2*op1;
                push(s1,m);
                break;
            case '/':
                m.fData=op1/op2;
                push(s1,m);
                break;
            case '^':
                m.fData=pow(op1,op2);
                push(s1,m);
                break;
            }
        }
        token=strtok(NULL," ");

    }
    result=pop(s1).fData;
    if(token==NULL)
    {
        free(s1);
        return result;
    }
    else
        return -1;

}

void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}

int RemoveSpaces(char* token,int* len,char* temp)
{
    int i,k=0,j;
    for(i=0; i<*len; i++)
    {
        if(token[i]==' ' || token[i]=='\n')
        {
            for(j=i; j<*len; j++)
            {
                token[j]=token[j+1];

            }
            *len=(*len)-1;
            k++;
        }
    }
    strcpy(temp,token);
    return k;
}
int checkEquation(char *equation){
    int i=0;
    int flag=0;
    int index;
    int x=0;
    int count,flag2;
    for(i=0;i<strlen(equation);i++){
            //count equal in expression
        if(*(equation+i)=='='){
            index=i;
            ++flag;
        }
        //check invalid symbols
        if(*(equation+i)=='\\'||*(equation+i)=='!'||*(equation+i)=='@'||*(equation+i)=='#'||*(equation+i)=='$'||*(equation+i)=='&'||*(equation+i)=='?'||*(equation+i)=='"'||*(equation+i)=='{'||*(equation+i)=='}'||*(equation+i)=='_'||*(equation+i)=='~'
           ||*(equation+i)==','||*(equation+i)==';'||*(equation+i)==':'||*(equation+i)=='<'||*(equation+i)=='>'){
               printf("\nINVALID EXPRESSION  ( Check this symbol (%c) )\n",*(equation+i));
                return 0;
        }
          //check operators
            if(*(equation+i)=='+'||*(equation+i)=='-'||*(equation+i)=='*'||*(equation+i)=='/'||*(equation+i)=='^'||*(equation+i)=='%'){
            count =i+1;
            flag2=1;
            while(flag2){
                    if(*(equation+count)==' '){
                        count=count+1;
                    }
                    else{
                        flag2=0;
                        break;
                    }
            }
            if(*(equation+count)=='\0'||*(equation+count)=='\n'||*(equation+count)=='+'||*(equation+count)=='*'||*(equation+count)=='/'||*(equation+count)=='^'||*(equation+count)=='%'){
                printf("\nINVALID EXPRESSION ( Check operators )\n");
            return 0;
            }
            }

    }
    if(flag==1){
            for(i=0;i<index;i++){
                    if(*(equation+i)=='+'||*(equation+i)=='-'||*(equation+i)=='*'||*(equation+i)=='/'||*(equation+i)=='^'||*(equation+i)=='('||*(equation+i)==')'||*(equation+i)=='%'){
                    printf("\nINVALID EXPRESSION \n");
                    return 0;
                    }
            }
            //check balanced parentheses
            for(i=0;i<strlen(equation);i++){
                if(*(equation+i)=='('){
                    x++;
                   }else if(*(equation+i)==')'){
                       x--;
                       if(x<0){
                            printf("\nINVALID EXPRESSION  (Unbalanced parentheses)\n");
                        return 0;
                       }
                   }
            }
            if(x!=0){
                   printf("\nINVALID EXPRESSION   (Unbalanced parentheses)\n");
                return 0;
            }


    }else{
        printf("\nINVALID EXPRESSION \n");
        return 0;
    }
    return 1;

}

void insertCharacter(char * a,char ch,int p)
{
    int i=strlen(a);
    if(p>=0&&p<=1000)
    {
        for(; i>=p; i--)
            a[i+1]=a[i];
        a[p]=ch;
    }
    else
        puts("Invalid Index/Position");
}

char *replaceWord(char *s,char *symbol,char *value)
{
    char *result;
    int i, counter = 0;
    int valuelen = strlen(value);
    int symbollen = strlen(symbol);

    // Counting the number of times symbol found in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], symbol) == &s[i])
        {
            counter++;
            // Jumping to index after the symbol.
            i += symbollen - 1;
        }
    }

    // Making new string of enough length
    result = (char *)malloc(i + counter * (valuelen - symbollen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
        if (strstr(s, symbol) == s)
        {
            strcpy(&result[i], value);
            i += valuelen;
            s += symbollen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

void PrintData(Data lines[],int counter)
{
    int r;

        for(r=0; r<counter; r++)
        {
            printf("%s = %s\n",lines[r].symbol,lines[r].value);
        }
}

void create(float heap[])
{
	int i,n;
	n=heap[0]; //no. of elements

	for(i=n/2;i>=1;i--)
		down_adjust(heap,i);
}

void down_adjust(float heap[],int i)
{
	int j,n,flag=1;
	float temp;
	n=heap[0];

	while(2*i<=n && flag==1)
	{
		j=2*i;    //j points to left child
		if(j+1<=n && heap[j+1] > heap[j])
			j=j+1;
		if(heap[i] > heap[j])
			flag=0;
		else
		{
			temp=heap[i];
			heap[i]=heap[j];
			heap[j]=temp;
			i=j;
		}
	}
}

    void Load()
    {
        FILE* f;
        float heap[100];
        int u,last;
	    float temp2;
        f=fopen("Src.txt","r");
        if(f==NULL)
        {
            printf("can not open file\n");
            exit(0);
        }
        struct Node* root;
        struct Node* predecessor;
        struct Node* successor;
        Data lines[100];
        int counter=0,count=0;
        int flag=0,flagg=0;
        char line[size];
        char* token;
        int len;
        char temp[3000];
        int check=1;
        int i=0;
        char valuePostFix[100],FinalValue[10];
        float result;
        printf("Contents of file:\n");
        while(fgets(line,size,f))
        {
            printf("%s",line);
            if(checkEquation(line)==1)
            {
                token=strtok(line,"=");
                while(token)
                {
                    flag++;
                    i=0;
                    len=strlen(token);
                    count=RemoveSpaces(token,&len,temp);

                    if(flag==1)
                    {
                        strcpy(lines[counter].symbol,temp);
                    }
                    else
                    {
                        for(int k=0; k<strlen(temp); k++)
                        {
                            if(temp[k]=='+' || temp[k]=='/' || temp[k]=='-' || temp[k]=='^' || temp[k]=='*' || temp[k]=='(' || temp[k]==')' || temp[k]=='%')
                            {
                                lines[counter].equation=1;
                                break;
                            }
                            else
                            {
                                lines[counter].equation=0;
                            }
                        }
                        while(*(temp+i)!='\0')
                        {
                            if(i==0&&*(temp)=='-')
                            {
                                i++;
                            }
                            else if(i==0&&*(temp)=='(')
                            {
                                insertCharacter(temp,' ',i+1);
                                i=i+1;
                            }
                            else if(*(temp+i)=='+'||*(temp+i)=='-'||*(temp+i)=='*'||*(temp+i)=='/'||*(temp+i)=='('||*(temp+i)==')'||*(temp+i)=='%'||*(temp+i)=='^')
                            {
                                if(*(temp+(i-1))==' ')
                                {
                                    if(*(temp+i)=='-')  //for negative numbers
                                    {
                                        i++;
                                    }
                                    else
                                    {
                                        insertCharacter(temp,' ',i+1);
                                        i=i+2;
                                    }
                                }
                                else
                                {
                                    insertCharacter(temp,' ',i);
                                    insertCharacter(temp,' ',i+2);
                                    i=i+3;
                                }
                            }
                            else
                            {
                                i++;
                            }
                        }
                        for(int j=counter-1; j>=0; j--)
                        {
                            strcpy(temp,replaceWord(temp,lines[j].symbol,lines[j].value));
                        }
                        strcpy(lines[counter].value,temp);
                    }
                    token=strtok(NULL,"=");

                }
                flag=0;
                counter++;

            }
            else
            {
                check=0;
                break;
            }

        }
        if(check==1)
        {
            //check if the variable found more than once and overwrite the old value
            for(int b=0;b<counter;b++){
                    for(int a=b+1;a<counter;a++){
                        if(strcmp(lines[b].symbol,lines[a].symbol)==0){
                            for(int c=b; c<counter; c++)
                             {
                             lines[c] = lines[c + 1];
                             }
                             counter--;
                            b--;
                        }
                    }
            }
            for(int r=0; r<counter; r++)
            {
                if(lines[r].equation)
                {
                    replaceNewLineBySpace(lines[r].value);
                    infixToPostfix(lines[r].value,valuePostFix);
                    strcpy(lines[r].value,valuePostFix);
                    lines[r].result= evaluatePostfix(lines[r].value);
                    strcpy(lines[r].value,FinalValue);
                }else{
                    lines[r].result=atof(lines[r].value);
                }
                 if(flagg==0)
            {
                root=newNode(lines[r].symbol,lines[r].result);
                flagg=1;
            }
            else {
                root=put(root,lines[r].result,lines[r].symbol);
            }
            }

            printf("\n\nContents after sorting with variable name:\n");
            InOrdertraverse(root);
                    for(int j=1;j<=counter;j++){
            heap[j]=lines[j-1].result;
        }
        heap[0]=counter;
        create(heap);

	//sorting
	while(heap[0] > 1)
	{
		//swap heap[1] and heap[last]
		last=heap[0];
		temp2=heap[1];
		heap[1]=heap[last];
		heap[last]=temp2;
		heap[0]--;
		down_adjust(heap,1);
	}

	//print sorted data
	printf("\nContent after sorting with variable values:\n");
	 int m=0;
	 for(int k=1;k<=counter;k++){
	 for(m=0;heap[k]!=lines[m].result;m++){
	 }
	 printf("%s = %.2f\n",lines[m].symbol,lines[m].result);
	    }

        }
        fclose(f);
        }
    int main()
    {
        Load();
        return 0;
    }
