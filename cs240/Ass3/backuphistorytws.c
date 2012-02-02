#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int readinput(char *input);
int shellcheck(char *input, char *PATH, int *pathsize, char alias[1000][100], char aliased_cmd[1000][100], int HISTORYCOUNT);
int runcmd(char **cmd, int pathspec);
int Parse(char **cmd, char *temp);
int pathcheck(char *input, char *PATH, int *pathsize);
int aliascheck(char *input, char alias[1000][100], char aliased_cmd[1000][100]);
int addalias(char *input, char alias[1000][100], char aliased_cmd[1000][100]);
int removealias(char *input, char alias[1000][100], char aliased_cmd[1000][100], int aliasindex);
int traverse_aliases(char *input, char alias[1000][100], int searchsection, int aliastotal);
int stringcmp(char *arr, char *input, int stringsize, int stringlocation);
int syntaxcheck(char *input, char operator);
int historyedit(char *input, int HISTORYCOUNT);
int loghistory(char *input, char history[1000][100]);


int main()
{
  char *cmd[100], input[100], alias[1000][100], aliased_cmd[1000][100], PATH[128], *currpath, history[1000][100];
  int m=0, pathspec=0, shellcmd=0, execute_sucess=0, pathsize=0, HISTORYCOUNT=100;
  FILE *twsrc=fopen(".twsrc","r");                               /*open twsrc file*/
  currpath=getenv("PATH");                                       /*grabbing the true PATH*/
 
  while(currpath[pathsize]!='\0')                                /*find end of PATH so it can append to it*/
    {
      PATH[pathsize]=currpath[pathsize];                        /*copy pointer to chars to an array*/
      pathsize++;
    }
  PATH[pathsize]='\0';                        /*NULL end of string*/
  

  memset(alias, '\0', sizeof(char) * 1000 * 100);
  memset(aliased_cmd, '\0', sizeof(char) *1000 *100);

  while(fgets(input, 80, twsrc) != NULL)                         /*read from twsrc file*/
    {
        shellcmd=aliascheck(input, alias, aliased_cmd);
    }
  while(1)
    {
      pathspec=readinput(input);                                                   /*take input. pathspec is a flag that determines if the user made the command path specified*/ 
      shellcmd=shellcheck(input, PATH, &pathsize, alias, aliased_cmd, HISTORYCOUNT);             /*is input a shellcommand?*/
      if(shellcmd==0)     /*skips execution step if input was just a shell command*/
	    {
	      Parse(cmd, input);                                                   /*parsing input into a 2 dimensional array so that execvp can use it*/	  
	      execute_sucess=runcmd(cmd, pathspec);                                /*executes child process to run shell command*/
	    }
      else                                                                         /*if it was a successful shellcommand then log history*/
	{
	  loghistory(input, history);
	}
      shellcmd=0;
      if(execute_sucess==1)
	{
	  loghistory(input, history);                                                       /*if command was executed sucessfully then log it in the history*/
	}
    }
}

int shellcheck(char *input, char *PATH, int *pathsize, char alias[1000][100], char aliased_cmd[1000][100], int HISTORYCOUNT)
{
  int shellcmd=0;
  shellcmd=pathcheck(input, PATH, pathsize); 
  if(shellcmd==1)
    {
      return 1;
    }
  shellcmd=aliascheck(input, alias, aliased_cmd);
  if(shellcmd==1)
    {
      return 1;
    }
  shellcmd=historyedit(input, HISTORYCOUNT);
    if(shellcmd==1)
      {
	return 1;
      }
  return 0;
}

int readinput(char *input)
{
  int i=-1, pathspec=0;
  memset(input, '\0', sizeof(char)* 100);
  printf("\n$");  /*command line symbol*/
      while(input[i]!='\n')
        {
          i++;
          input[i]=getchar();           /*grabing char by char from command line*/
          if(input[i]==EOF)
	    {
	      exit(0);                 /*if user typed control D then terminate shell*/
	    }
          else if(input[i]=='/')
            {
              pathspec=1;              /*path specified input*/
            }
        }     
      input[i+1]='\0';                 /*ending input with NULL for easier string parsing*/
      return pathspec;
}

int runcmd(char **cmd, int pathspec)
{
  int status;
  pid_t pid=fork();

 if(pid<0)   /* if it failed*/
  {
    printf("could not make child process\n");
  }
  else if(pid==0 && pathspec==0)   /*child process*/          /*if not path specific command*/
  {
    printf("no such command %d", wait(&status));
    
    execvp(*cmd, cmd);
    exit(0);
  }
  else if(pid==0 && pathspec==1)                    /*if path specific command*/
    {
      execv(*cmd, cmd);
      pathspec=0;
    }
  else if(pid!=wait(&status))                /*wait for child process to end*/
  {
    printf("parent did not wait for child to end");
  }
  else
  {
    /*do nothing*/
  }
 if(wait(&status)!=-1)
   {
     return 1;
   }
 else
   {
     return 0;
   }
}


int Parse(char **cmd, char *input)
{
  int i=0, flag=0, col=0;
  while(input[i]!='\n')
    {
      if(input[i]=='\0')
	{
	  break;
	}
      if((input[i]==' ' || input[i]=='\t') && flag==1)                   /*NULLing spaces in input*/
        {
          input[i]='\0';   
	  flag=0;
	}
      else if(flag==0)      
	{
	  cmd[col]=&input[i];
	  flag=1;
	  col++;
	}
      i++;
    }
  input[i]='\0';                         /* nullifying the \n character from input*/
}

int pathcheck(char *input, char *PATH, int *pathsize)
{
  int i=4, k=0, stringcmd=0, pathlength;
  char pathstring[10]={'P', 'A', 'T', 'H'};
  stringcmd=stringcmp(pathstring, input, 4,0); 

    if(stringcmd==1)
    {
      if(input[i]=='=')                  /*is this an assignment of a new PATH?*/
	{
	  stringcmd=0;                   /*clearing variable for future use*/
	  while(input[i]!='\0')
	    {
	      if(input[i]=='=')                  
		{
		  i++;                           /*parse out equal sign*/
       		  stringcmd=stringcmp(pathstring, input, 4, i);
		  if(stringcmd==1)                         /*if it equals "PATH" then give it the env var "PATH"*/
		    {
		      while(PATH[k]!='\0')                 /*find end of PATH so it can append to it*/
			{
			  k++;
			}
		      i=i+4;   /*this allows for it to skip over the literal "PATH" in the input string after we already extracted what PATH means*/
		    }
		}
		 
	      if(input[i]=='+')
                {
                  PATH[k]=':';
                  i++;
		  k++;
                }
	      if(input[i]==' ')
		{
		  printf("%s", "please dont use spaces when specifying path");
		  return 1;
		}
	      PATH[k]=input[i];
	      i++;
	      k++;
	    }
	  PATH[k]='\0';
	  printf("your path is now %s\n", PATH);        
          return 1;
	}
      else                                               /*if the user justs types "PATH" then simply display PATH*/
	{
	  printf("your path is now %s\n", PATH);
	  return 1;
	} 
   }
   else
     {
       return 0;
     } 
}


int aliascheck(char *input, char alias[1000][100], char aliased_cmd[1000][100])
{
  int i, aliascmd=0, aliasindex, unaliascmd=0, q=0,m=0, p=0, correct_syntax=0; 
  static int aliastotal;
  char aliasstring[10]={'a', 'l', 'i','a', 's'};                 /*the string "alias" to compare to user input*/
  char unaliasstring[10]={'u', 'n', 'a', 'l', 'i', 'a', 's'};    /*the string "unalias" to compare to user input*/

  aliascmd=stringcmp(aliasstring, input, 5, 0);                 /*did the user type alias?*/
  unaliascmd=stringcmp(unaliasstring, input, 7, 0);    

  if(aliascmd==1)                      /*****************if input is defining an alias***********************/
    {
      correct_syntax=syntaxcheck(input, '=');                  /*checks if correct syntax was used for alias*/
      
        if(correct_syntax!=1)                                  /*if they used incorrect syntax, ignore command and restart input*/
	  {
	    printf("\n%s\n", "Incorrect usage of alias. example: use alias ls=l");
	  }
        else
	  {
	    aliastotal=addalias(input, alias, aliased_cmd);                   /*if they typed alias, then add the new alias*/
	  }  
	return 1;                                                  /*skip exec command*/
    }
  else if(unaliascmd==1)
    {  
      aliasindex=traverse_aliases(input, alias, 8, aliastotal);
      printf("it turned out to be %d", aliasindex);
      if(aliasindex==-1)
	{
	  printf("%s\n", "no such alias defined");
	  return 1;
	}
      else
	{
          removealias(input, alias, aliased_cmd, aliasindex);
	  return 1;
	}
    }
  else                                 /**************check if input is a pre-existing alias*********************/
    {
       m=traverse_aliases(input,alias,0,aliastotal);	     
       if(m!=-1)                /*if this is a pre defined alias then use it's real command*/ 
	 {
      	     while(aliased_cmd[m][p]!='\0')
	     {
               input[p]=aliased_cmd[m][p];
	       p++;
	     }
	     input[p]='\0';
       	 }
    }
  return 0;
}

int addalias(char *input, char alias[1000][100], char aliased_cmd[1000][100])
{
  static int aliastotal=0;
  int i=6,m=0;                                       /*i starts as 6 to skip over the "alias" command in string*/
  while(input[i]!='=')
    {
      alias[aliastotal][m]=input[i];
      i++;
      m++;
    }
  m=0;
  i++;                                               /*skip '=' sign*/
   while(input[i]!='\0')
    {
      aliased_cmd[aliastotal][m]=input[i];          /*grab right side of equal sign and store real cmd in aliased_cmd*/
      i++;
      m++;
    }
    aliastotal++;
    return aliastotal;
}


int removealias(char *input, char alias[1000][100], char aliased_cmd[1000][100],int aliasindex)
{
 
  int p;
  if(alias[aliasindex+1][0]!='\0')
    {
      while(alias[aliasindex+1][0]!='\0')
      {
        p=0;
        while(alias[aliasindex+1][p]!='\0')
          {
            printf("%c : %c\n", alias[aliasindex][p], alias[aliasindex+1][p]);
            alias[aliasindex][p]=alias[aliasindex+1][p];
            aliased_cmd[aliasindex][p]=aliased_cmd[aliasindex+1][p];
            p++;
          }
        alias[aliasindex][p]='\0';
        aliased_cmd[aliasindex][p]='\0';
        printf("%s: %s\n", alias[aliasindex], aliased_cmd[aliasindex]);
        aliasindex++;
      }
    }
  else
    {
      memset(alias[aliasindex], '\0', sizeof(char)* 100);
      memset(aliased_cmd[aliasindex], '\0', sizeof(char) * 100);
    }
  aliased_cmd[aliasindex][0]='\0';
  alias[aliasindex][0]='\0';
  
}

int traverse_aliases(char *input, char alias[1000][100], int searchsection, int aliastotal)
{
  int n=0, p=0, k=0, i=searchsection;
  while(alias[n][p]!='\0')
    {
        if(alias[n][p]==input[i])    /*traverse aliases in search of portion of input specified by searchsection*/
	{
	  k=1;
	  p++;
	  i++;
	}
      else
	{
	  k=0;
	  n++;
	  p=0;
	  i=searchsection;
	}
    }
 if(k==1)
   {
     return n;          /*returns which block in the alias array that the alias is stored*/
   }
 else
   {
     return -1;
   }
}

int stringcmp(char *arr, char *input, int stringsize, int stringlocation)
{
  int i=0, g=0, k=0;
  for(i;i<stringsize;i++)
    {
      if(arr[k]==input[stringlocation])     
        {
          g=1;
        }
      else
        {
          g=0;
          return 0;
        }
      k++;
      stringlocation++;
    }
  return 1;
}

int syntaxcheck(char *input, char operator)
{
  int correct_syntax=0, q=0;
  while(input[q]!='\0')
    {
      if(input[q]==operator)
	{
	  correct_syntax=1;                              /*checks for correct usage of alias (i.e. alias ls=l)*/
	  return 1;
	}
      q++;
    }
  if(correct_syntax!=1)                                  /*if they used incorrect syntax, ignore command and restart input*/
    {
      return 0;
    }
}

int historyedit(char *input, int HISTORYCOUNT)
{
  int match=0, i=12;          /*i is 13 to skip "HISTORYCOUNT" in input*/
  char hist[16]={'H','I','S','T','O','R','Y','C','O','U','N','T'}, *numconvert;
  match=stringcmp(hist, input, 12,0);            /*did the user type HISTORYCOUNT?*/
  if(match==1)
    {
      printf("match");
      if(input[i]!='=')               /*if they are not assigning a new value to HISTORYCOUNT then just print out HISTORYCOUNT*/
	{
	  printf("HISTORYCOUNT is %d", HISTORYCOUNT);
	  return 1;
	}
      else                           /*the user is re initializing HISTORYCOUNT*/
	{
	  i++;
	  while(input[i]!='\0');
	  {
	    if(input[i]<'0' || input[i]>'9')
	      {
		printf("%s", "cannot assign character to HISTORYCOUNT");
		return 1;
	      }
	    else
	      {
		numconvert=&input[i];             /*putting number to be assigned in a new string for type casting*/
	      }
	  }
	  HISTORYCOUNT=atoi(numconvert);         /*assigns new value to HISTORYCOUNT*/
	}
	    return 1;
    }
	  return 0;
}

int loghistory(char *input, char history[1000][100])
{

}  
