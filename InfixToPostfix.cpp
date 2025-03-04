#include <stdio.h>
#include <stdlib.h>
#define   TOTAL_SYMBOL   8

char  symbol[TOTAL_SYMBOL] = {'(', '^', '*', '/', '%', '+', '-', ')'};
int  priority[TOTAL_SYMBOL] = {1, 2, 3, 3, 3, 4, 4, 5};

int   check_priority(char symb);
void  push(char stack[], int *p, char symb);
char  pop(char stack[], int *p);
int   calculate(char postfix_exp[]);
   
int main()
{
   char  exp[100];         // 輸入之原始中置式 
   char  sym_stack[40];    // 在中置式轉後置式中存放運算子的堆疊 
   char  ans_exp[150];     // 存放最後輸出的後置式（用空白作為間隔） 
   char  temp;
   int  top = 0, x, pos, cur, ans;
   
   while (true)
   { 
      printf("請輸入一個中置式算式(算式中間不加空白，直接按Enter表示結束)：\n");
      gets(exp);      
      fflush(stdin); 
      if (exp[0] == '\0')
         break;
      
      top = 0;    // 記錄堆疊元素個數(頂端將存放之位置)
      pos = 0;    // 記錄ans_exp[]的存放位置 
      for (x=0; exp[x]!='\0'; x++)
      {
         if (exp[x] >= '0' && exp[x] <= '9')
         {
            ans_exp[pos] = exp[x];
            pos++;
         }
         else   // symbol
         {
            if (exp[x] != '(')
            {    
               ans_exp[pos] = ' ';    // 以空白隔開每個元素(之前已經有東西)
               pos++;   
            }
            
            if (top == 0  ||          // 第1個符號 或者 優先權較高者 或者 ^ 或者 剛放入（ 
                check_priority(exp[x]) < check_priority(sym_stack[top-1])  ||
                exp[x] == '^'  ||  sym_stack[top-1] == '(')
            {
               // 直接將讀到的符號 PUSH 進堆疊 
               push(sym_stack, &top, exp[x]);
            }
            else if (exp[x] == ')')   // 碰到右括號，直接POP出堆疊元素直到 '(' 
            {
               while (true)
               {
                  temp = pop(sym_stack, &top);
                  if (temp != '(')
                  {
                     ans_exp[pos] = temp;
                     ans_exp[pos+1] = ' ';
                     pos += 2;      
                  }
                  else
                  {
                     pos--;      // 純粹美觀用 
                     break; 
                  }   
               }     
            }
            else     // 讀到的運算符號優先權較低 
            {
               // 將堆疊中運算符號優先權不低於讀到符號者悉數POP出，最後放入讀到的      
               cur = check_priority(exp[x]);
               while (true)
               {
                  if (top > 0)   
                  {
                     temp = pop(sym_stack, &top);   
                     if (check_priority(temp) <= cur)
                     {
                        ans_exp[pos] = temp;
                        ans_exp[pos+1] = ' ';
                        pos += 2;                      
                     }
                     else
                     {
                        push(sym_stack, &top, temp);
                        break;
                     }   
                  }
                  else
                     break;   
               }
               push(sym_stack, &top, exp[x]);
            }    
         } // else xymbol
      } // for(x)
      
      // 最後把堆疊全部 POP 清空
      if (ans_exp[pos-1] != ' ')
      {
         ans_exp[pos] = ' ';
         pos++;
      }   
      while (top > 0)
      {
         temp = pop(sym_stack, &top);
         ans_exp[pos] = temp;
         ans_exp[pos+1] = ' ';
         pos += 2;   
      }       
      
      ans_exp[pos] = '\0';
      puts(ans_exp);
      ans = calculate(ans_exp);
      printf("計算結果(整數)：%d\n", ans); 
          
   } // while(true)
   
   system("PAUSE");
   return  0;
}

int  check_priority(char symb)
{
   int  x;
   for (x=0; x<TOTAL_SYMBOL; x++)
      if (symbol[x] == symb)
         break;
   return  priority[x];           
}     

void  push(char stack[], int *p, char symb)
{
   stack[*p] = symb;
   (*p)++;   
}

char  pop(char stack[], int *p)
{
   (*p)--;
   return  stack[*p];      
}

int calculate(char postfix_exp[])    // 此處暫訂為整數間的運算 
{
   int  stack[50], top, x, num, num1, num2, num3;
   bool Is_num;
   
   top = 0;
   num = 0;
   Is_num = true;
   for (x=0; postfix_exp[x]!='\0'; x++)
   {
      if (postfix_exp[x] >= '0' && postfix_exp[x] <= '9')
      {
         num = num * 10 + (postfix_exp[x] - '0');
         Is_num = true;
      }   
      else if (postfix_exp[x] == ' '  &&  Is_num)
      {
         stack[top] = num;
         top++;
         num = 0;
         Is_num = false;
      }        
      else if (postfix_exp[x] != ' ')
      {
         num2 = stack[top-1];
         num1 = stack[top-2];  
         switch (postfix_exp[x])
         {
            case '+' : num3 = num1 + num2;     break;
            case '-' : num3 = num1 - num2;     break;
            case '*' : num3 = num1 * num2;     break;
            case '/' : num3 = num1 / num2;     break;
            case '%' : num3 = num1 % num2;     break;
            case '^' : num3 = 1;
                       for (int k=1; k<=num2; k++)
                          num3 = num3 * num1; 
                       break;    
         }
         stack[top-2] = num3;
         top--;
      }
   }  // for(x)
   
   return  stack[0];
}
    
