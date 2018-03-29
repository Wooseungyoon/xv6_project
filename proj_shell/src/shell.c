#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int ExecuteCmd(char *argvp);

int main(int argc, char *argv[]){
    
    char input[1024];
    char **argvp;      //parsing cmd
    char tmp_cmd[1024];     //save cmd
    int input_len;
    int num_semicolon;
    int i;

    //batch mode
    if (argv[1] != NULL) {
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL)
            printf("fopen Error!\n");

        while (fgets(input, 1024, fp) != NULL) {
                num_semicolon = 0;  //initialize number of semicolon
                argvp = NULL;

                input_len = strlen(input);
                input[input_len - 1] = '\0';
                strcpy(tmp_cmd, input);

                printf("%s\n", tmp_cmd);

                //check number of semicolon and assign cmd
                if (strtok(input, ";") != NULL)
                        for (num_semicolon = 0 ; strtok(NULL, ";") != NULL; num_semicolon++);

                argvp = (char**) malloc(sizeof(char*)*(num_semicolon + 1));
                *argvp = strtok(tmp_cmd, ";");
                for (i = 1; i < num_semicolon + 1; i++)
                        *(argvp + i) = strtok(NULL, ";");
 
                for (i = 0; i < num_semicolon + 1; i++) {
                        switch (fork()) {
                                case -1 : {
                                        printf("fork() Error!\n"); 
                                        break;
                                        }
                                case 0 : {
                                        ExecuteCmd(argvp[i]);
                                        break;
                                        }
                                default : {
                                        wait(NULL);
                                        break;
                                        }
                        }
               }
        memset(input, '\0', sizeof(input));     //initialize input
        memset(tmp_cmd, '\0', sizeof(input));     //initialize input
        }
    }


    while (1) {
        num_semicolon = 0;  //initialize number of semicolon
        argvp = NULL; 

        memset(input, '\0', sizeof(input));     //initialize input
        memset(tmp_cmd, '\0', sizeof(input));     //initialize input
        printf("prompt> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input Error \n");
            return -1;
        }
        if (!strcmp(input, "quit\n"))
            exit(1);

        input_len = strlen(input);
        input[input_len - 1] = '\0';
        strcpy(tmp_cmd, input);
        fflush(stdin);

        //check number of semicolon and assign cmd
        if (strtok(input, ";") != NULL)
            for (num_semicolon = 0 ; strtok(NULL, ";") != NULL; num_semicolon++);

        argvp = (char**) malloc(sizeof(char*)*(num_semicolon + 1));
        *argvp = strtok(tmp_cmd, ";");
        for (i = 1; i < num_semicolon + 1; i++)
                *(argvp + i) = strtok(NULL, ";");
 
        for (i = 0; i < num_semicolon + 1; i++) {
            switch (fork()) {
                case -1 : {
                              printf("fork() Error!\n"); 
                              break;
                          }
                case 0 : {
                             ExecuteCmd(argvp[i]);
                             break;
                         }
                default : {
                              wait(NULL);
                              break;
                          }
            }
        }
    }
    return 0;
}

int ExecuteCmd(char *argvp) {

    char cmd[1024];
    char *pch;  //temp string
    char *exec_cmd[4];    //cmd
    int i = 0;

	strcpy(cmd, argvp);
	//just execute cmd
	cmd[strlen(cmd)] = '\0';
	//put command into first_cmd 
	pch = strtok(cmd, " ");
	while (pch != NULL && i < 3) {
		exec_cmd[i] = pch;
		pch = strtok(NULL, " ");
		i++;
	}
	exec_cmd[i] = (char*)0;

	if (exec_cmd[0] != NULL) {
		//again check
		if (!strcmp(exec_cmd[0], "quit\n"))
			exit(1);
		execvp(exec_cmd[0], exec_cmd);
		exit(1);
	}
	return 0;
}

