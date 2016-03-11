# tcsh-shell

C++ Program to emulate a basic shell script. Commands are as follows:

 - **quit**               ends program
 - **date**               displays current date and time
 - **curr**               displays absolute pathname of current dir
 - **env**               display all of the environment variables
 - **cd <dir name>**      moves current directory to given dir
 - **hlist**              display recent commands (up to 10)
 - **dlist**            display recent directories (up to 10)
 - **!N**                displays and executes command at line N if in hlist
 - **#N**                move to directory at given index in dlist
