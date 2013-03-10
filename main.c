#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION_NUMBER "0.0.1"
#define MAX_LEN 255

#define PROMPT ">"

struct node
{
  char item[MAX_LEN];
  struct node* next;
  struct node* prev;
};

struct stack
{
  struct node* head;
  struct node* tail;
  int count;
};

struct stack* thestack = NULL;

struct stack* stack_new( void )
{

  struct stack* p = malloc(sizeof *p);

  if( p == NULL )
    {
      // could not malloc stack
      exit( EXIT_FAILURE );
    }
 
  p->head = NULL;
  p->tail = NULL;
  p->count = 0;
  return p;
}

struct stack* push(struct stack* s, const char* c )
{
  struct node* p = malloc(sizeof *p);

  if( p == NULL )
    {
      // could not malloc node
      exit( EXIT_FAILURE );
    }

  // truncate over-length items
  if( strlen(c) < MAX_LEN ) {
    strcpy(p->item, c);
  }

  p->next = NULL;
  p->prev = NULL;

  if( s == NULL )
    {
      // stack not initialized
      exit( EXIT_FAILURE );
    }
  else if( s->head == NULL )
    {
      // add first element to stack
      s->head = p;
      s->tail = p;
      s->count ++;
      return s;
    }
  else 
    {
      // push element
      p->prev = s->head;

      // top node
      s->head = p;

      // prev node points to this
      p->prev->next = p;

      s->count ++;
    }

  return s;
}

char* peek(struct stack* s)
{
  if( s == NULL )
    {
      // no stack initialized
      exit( EXIT_FAILURE );
    }
  else if( s->head == NULL )
    {
      return "";
    }
  else
    {
      return s->head->item;
    }
}

char* pop( struct stack* s )
{
  struct node* p = NULL;

  if( NULL == s )
    {
      // no stack initialized
      exit( EXIT_FAILURE );
    }
  else if(s->head == NULL)
    {
      printf("<stack underflow>");
      return "";
    }
  else
    {
      p = s->head;
      s->head = s->head->prev;
      if(s->head != NULL){
        s->head->next = NULL;
      }        
      free(p);
      s->count --;
    }

  return p->item;
}


void stack_print_element(const struct node* s)
{
  if( s ) {
    printf("%s ", s->item);
  }
}

void stack_print( const struct stack* s )
{
  struct node* p = NULL;

  if( s->count == 0 ) 
    {
      printf("<empty stack> ");
    }
  else
    {
      for( p = s->tail; p != NULL; p = p->next ) {
        stack_print_element(p);
      }
    }
}

char* prim_add(){

  char *a = pop(thestack);
  char *b = pop(thestack);

  int x = atoi( a );
  int y = atoi( b );

  int z = y + x;

  char res[MAX_LEN];

  sprintf(res, "%d", z);
  push(thestack, res);

  return 0;

}


char* prim_sub(){

  char *a = pop(thestack);
  char *b = pop(thestack);

  int x = atoi( a );
  int y = atoi( b );

  int z = y - x;

  char res[MAX_LEN];

  sprintf(res, "%d", z);
  push(thestack, res);

  return 0;

}

char* prim_mul(){

  char *a = pop(thestack);
  char *b = pop(thestack);

  int x = atoi( a );
  int y = atoi( b );

  int z = y * x;

  char res[MAX_LEN];

  sprintf(res, "%d", z);
  push(thestack, res);

  return 0;

}

char* prim_div(){

  char *a = pop(thestack);
  char *b = pop(thestack);

  int x = atoi( a );
  int y = atoi( b );

  float z = y / x;

  char res[MAX_LEN];

  sprintf(res, "%d", (int)z);
  push(thestack, res);

  return 0;

}

int do_primitive(char *word){

  char *arg;

  //add 
  if (strcmp(word, "+") == 0) {
    prim_add();
    return 0;
  }

  if (strcmp(word, "-") == 0) {
    prim_sub();
    return 0;
  }

  if (strcmp(word, "*") == 0) {
    prim_mul();
    return 0;
  }

  if (strcmp(word, "/") == 0) {
    prim_div();
    return 0;
  }

  // pop
  if (strcmp(word, ".") == 0) {    
    if(thestack->count == 0){
      printf("<empty stack> ");
    } else {
      arg = pop(thestack);
      printf("%s ", arg);
    }
    return 0;
  }

  // drop
  if (strcmp(word, "drop") == 0) {    
    pop(thestack);
    return 0;
  }

  // dup
  if (strcmp(word, "dup") == 0) {    
    push(thestack, peek(thestack));
    return 0;
  }

  return 1;
}

int do_word(char *word){

  return 1;
}

void process(char *word){

  //printf("= %s\n", word);
  
  if (strcmp(word, ".s") == 0) {
    printf("%s <%d> ","stack", thestack->count);
    stack_print(thestack);    
    return;
  }

  // find a word? execute it
  if (do_primitive(word) == 0) {
    return;
  }

  // find a word? execute it
  if (do_word(word) == 0) {
    printf("%s %s\n","word", word);
    return;
  }

  if (strcmp(word, ":") == 0) {
    printf("%s\n","start word");
    return;
  }

  if (strcmp(word, ";") == 0) {
    printf("%s\n","end word");
    return;
  }

  // not word? put on stack
  push(thestack, word);
  
}

void parse(char *s){
  
  char word[MAX_LEN];
  int len = strlen(s);
  int ix = 0;
  int ln = 0;

  for(ix = 0; ix < len; ix ++) {
      
    if( s[ix] == ' ' && ln == 0) {
      // repeated space, ignore
    } else if(s[ix] == ' ' || ix == len - 1) {
      // space or last char of string
      word[ln] = '\0';
      // process word here
      process(word);
      printf("%s", word);
      printf(" ");
      // reset for next word
      ln = 0;
    } else {
      // part of word
      word[ln] = s[ix];
      ln ++;
    }
  }
  
}

void init(){
  thestack = stack_new();
}

void repl(){

  size_t len = 0;
  ssize_t read;

  char *in;
  in = (char *) malloc (MAX_LEN + 1);

  while(1)
    {

      if(feof(stdin) != 0)
        { 
          printf("\n");
          break;
        }

      printf("%s ", PROMPT);
      read = getline(&in, &len, stdin);
      in[read] = '\0';      
      parse(in);
      printf("ok\n");

    }
}


int main(int argc, char **argv) {
  
  printf("pForth %s\n", VERSION_NUMBER);

  init();
  repl();

  return 0;

}
