#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>
char cw[100]/*, host[100]*/;
int fgbg[100]={0};
int pid1[100], line=1 , hist=0;
char l1[100][100], pid[100][100];

main(int n, char *arg[])
{
    int p, q, i, k, j, f, no_tokens, pf[10][2], m, i1, pipec=0, i2, in, out,hf, hf1;

    char cwd[100], l[100], **t, cwd1[100], ch, **t1, **temptok, **t2, lt[100];
    pid1[0]=getpid();
    strcpy(pid[0], arg[0]);
   
    t=malloc(sizeof(char *)*10);
    t[0]=malloc(sizeof(char)*1000);
    temptok=malloc(sizeof(char *)*10);
    temptok[0]=malloc(sizeof(char)*1000);
    t1=t;
    t2=temptok;
    t1[0]=t[0];
    t2[0]=temptok[0];
    cw[0]='~';
    cw[1]='\0';
    getcwd(cwd, 100); // cwd is initial starting directory path and cw is for setting current dir as     ~
    l[0]='\0';
    m=1;
    hf=1;
    while(1)
    {
        t=t1;
        t[0]=t1[0];
        temptok=t2;
        temptok[0]=t2[0];
        if(hf!=0)
        l[0]='\0';
        for(i=1; i<10; i++)
        {
            t[i]=t[i-1]+100;
            temptok[i]=temptok[i-1]+100;
        }
        if(hf!=0)
        {
            printf("Avan Shell@%s: ", cwd);
            while(scanf("%[^\n]", l)==EOF); // l is input
            scanf("%c", &ch); // ch is enter
        }
        if(l[0]=='\0')
        continue;
        // printf("--%s---\n", l);
        k=0;
        j=0;
        hf=1;
        pipec=0;
        int n=strlen(l);
        for(i=0; i<n; i++) // traversing along l (input string)
        {
            if(l[i]=='|') // detecting pipe
            pipec=1;
            if((l[i]==' '||l[i]=='\n')&& j!=0)
            {
                t[k][j]='\0';
                k++; // storing input command in t
                j=0; // ls -al stored as 01(l) 02(s) 10( ) 11(-) 12(a) 13(l)
            }
            else if(l[i]!=' ' && l[i]!='\n')
            {
                t[k][j++]=l[i];
            }
            //printf("\n--%d %d--\n",k,j);
        }
        for(i2=0; i2<pipec; i2++)
        {
            pipe(pf[i2]);
        }
        if(j!=0)
        {
            t[k][j]='\0';
            k++;
        }
        no_tokens=k;
        if(k!=0)
        t[k]=(char *)NULL;
        if(k==0)
        continue;
        // for(i=0; i<k; i++)
        // {
        // printf("%s\n", t[i]);
        // }
        if(!strcmp(t[0], "chdir"))
        {
            m=chdir(t[1]);
            if(m==-1)
            printf("Error!! Directory does not exist!!\n");
            getcwd(cwd, 100);
           
        }
        else if(!strcmp(t[0], "quit"))
        {
            exit(0);
        }
        else
        {
               if(!strcmp(t[no_tokens-1], "&"))//background process
            {
                t[no_tokens-1]=(char *)NULL;
                no_tokens--;
                strcpy(pid[line], t[0]);
                k=fork();
                fgbg[line]=1;
                if(k==0)
                {
                    
                    execvp(t[0], t);
                    printf("ERROR! Command Not Found!!\n");
                    exit(0);
                }
                    
                    pid1[line]=k; // pid of child process
             printf("MY parent is-%d\n", i);
                     //i=wait(NULL);
                    
                    
                    line++;
            }
            else
            {
            j=0;
            k=0;
            hf1=0;
            for(i=0; i<no_tokens; i++)
            {
                if(!strcmp(t[i], "|")||i==no_tokens-1)//pipping
                {
                    if(i!=no_tokens-1)
                    {
                        // pipe(pf[j]);
                        temptok[k]=(char *)NULL;
                    }
                    else
                    {
                        strcpy(temptok[k],t[i]);
                        temptok[k+1]=(char *)NULL;
                        k++;
                    }
                    m=1;
                    m=fork();
                    if(m==0)
                    {
                        if(j!=0)
                        {
                            dup2(pf[j-1][0], 0);
                        }
                        if(j!=pipec)
                        {
                            dup2(pf[j][1], 1);
                        }
                        for(i2=0; i2<pipec; i2++)
                        {
                            close(pf[i2][0]);
                            close(pf[i2][1]);
                        }
                        f=0;
                        in=0;
                        out=1;
                        for(i2=0; i2<k; i2++)
                        {
                            if(!strcmp(temptok[i2], "<"))
                            {
                                in=open(temptok[i2+1], O_RDONLY);
                                if(f==0)
                                f=i2;
                            }
                            if(!strcmp(temptok[i2], ">"))
                            {
                                out=open(temptok[i2+1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                                if(f==0)
                                f=i2;
                            }
             if(!strcmp(temptok[i2], ">>"))
                            {
                printf("%s\n",temptok[i2-1]);
                int pid = fork ();
                 if (pid == 0)
                   {
                     out =open (temptok[i2+1], O_APPEND | O_WRONLY | O_CREAT,S_IRUSR | S_IRGRP | S_IROTH);
                                dup2 (out, 1);
                     if (execvp(temptok[i2-1], arg) < 0)
                {
                 printf ("%s: Command not found\n", arg [0]);
                }
                close(in);
                   }
                /*printf("%s\n",temptok[i2-1]);
                                out=open(temptok[i2+1], O_WRONLY | O_APPEND | O_TRUNC | O_CREAT ,O_APPEND | S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
                dup2(out,1);
                                if(f==0)
                                f=i2;*/
                            }



                        }
                        if(f!=0)
                        temptok[f]=(char *)NULL;
                        if(in==-1)
                        {
                            printf("Error!! File doesn't exist!!\n");
                            exit(0);
                        }
                        if(out==-1)
                        {
                            printf("Error!! File can't be created!!\n");
                            exit(0);
                        }
                        dup2(in, 0);
                        dup2(out, 1);
                        if(in!=0)
                        close(in);
                        if(out!=1)
                        close(out);
                            execvp(temptok[0], temptok);
                        printf("ERROR! Command Not Found!!\n");
                        exit(0);
                    }
                   
                    j++;
                        k=0;
                    temptok=t2;
                    temptok[0]=t2[0];
                    for(i1=1; i1<10; i1++)
                    {
                        temptok[i1]=temptok[i1-1]+100;
                    }
                    continue;
                }
                strcpy(temptok[k],t[i]);
                k++;
            }
            // if(m==0)
            // continue;
            if(hf1==1)
            {
                hf=1;
                continue;
            }
            for(i2=0; i2<pipec; i2++)
            {
                close(pf[i2][0]);
                close(pf[i2][1]);
            }
            for(i2=0; i2<pipec+1; i2++)
            {
                wait(NULL);
            }
            if(getpid()!=pid1[0])
            exit(0);
           
            }
        }
        l[0]='\0';
        for(i=0; i<no_tokens; i++)
        {
            
            strcat(l, t[i]);
            strcat(l, " ");
        }
        if(hf!=0)
        {
            strcpy(l1[hist], l);
            l[0]='\0';
            hist++;
        }
       
    }
}
