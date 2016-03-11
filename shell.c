/*
 *Author: Steven Jorgensen
 *Date: 2/18/16
 *
 *
 *Program inplements a shell for users to interact with. Included commands are
 *
 *quit               ends program
 *date               displays current date and time
 *curr               displays absolute pathname of current dir
 *env                display all of the environment variables
 *cd <dir name>      moves current directory to given dir
 *hlist              display recent commands (up to 10)
 *dlist              display recent directories (up to 10)
 *!N                 displays and executes command at line N if in hlist
 *#N                 move to directory at given index in dlist
 *
 * */

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <list>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
using namespace std;


int main(){
    char curdir[1024];
    map <long,string> m;
    list <string> d;
    string input;
    long count = 1;
    string user = getenv("USER");
    struct stat st;
    bool repeat = 0;
    bool call = 0;
    string s2 = "";

    getcwd(curdir,sizeof(curdir));
    d.push_front(curdir);

    while (true){
        cout<<"<"<<count<<" "<<user<<"> ";
        
        getline(cin,input);
        cout<<endl; 
        istringstream iss(input); 
        string s;

        iss >> s;
        if(s == ""){
            continue;
        }
        
        // keep hlist at size 10
        if(m.size() == 10){
            m.erase(m.begin());
        }
        if(d.size() == 11){
            d.pop_back();
        }
        // add command to hlist
        m[count] = input;

        // !N implementation
        if(s[0] =='!'){
            repeat = 1;
            string ss = s.substr(1);
            int num = atoi(ss.c_str());
            if (m.find(num) != m.end()){
                 cout<<"Command at "<<num<<": "<<m[num]<<endl<<endl;
                 s = m[num];
                 m[count] = s;
                 if(s[0] == '!'){
                     cout<<"Bad command number"<<endl;
                     count += 1;
                     continue;
                 }
            }
            else{
                cout<<"Command not in hlist"<<endl<<endl;
                
            }
        }

        // #N implementation
        if (s[0] == '#'){
            call = 1;
            string str = s.substr(1);
            int index = atoi(str.c_str());

            if (index > d.size() || index < 1){
                cout << "Bad index number" <<endl<<endl;
                continue;
            }
            else{
                auto j = d.begin();
                advance(j, index - 1);
                s = "cd";
                s2 = *j;

            }
            
        }

        //quit implementation
        if(s == "quit"){
            break;
        }
        // date implementation
        else if(s == "date"){
            time_t t;
            time(&t);
            cout<<ctime(&t)<<endl;

        }
        // curr implementation
        else if(s == "curr"){
            char dir[1024];
            if(getcwd(dir, sizeof(dir)) != NULL){
                getcwd(dir,sizeof(dir));
                cout<<dir<<endl<<endl;
            }
            else{
                cout<<"Current Directory Error"<<endl<<endl;
            }

        }
        // cd <dir name> implementation
        else if (s.substr(0,2) == "cd"){
            if (repeat == 0){
                iss >> s;

                
            }
            else{
                repeat = 0;
                s.erase(s.begin());  // remove "cd"
                s.erase(s.begin());
                if(s[0] == ' '){
                    s.erase(s.begin());
                }
                else{
                    s = "";
                }
            }
            
            if (s[0] == '#'){
                call = 1;
                string str = s.substr(1);
                int index = atoi(str.c_str());

                if (index > d.size() || index < 1){
                    cout << "Bad index number" <<endl<<endl;
                    continue;
                }
                else{
                    auto j = d.begin();
                    advance(j, index - 1);
                    s = *j;

                }
        
            }


            if(s != "" && s != "cd"){
                if(s[0] == '~'){
                    s = "/user/" + s.substr(1);
                    if (s == "/user/"){
                        s = s + user;
                    }
                }
                
                if(stat(s.c_str(),&st) != -1){ // check if dir exists
                    
                    if(S_ISDIR(st.st_mode)){     // check if file
                       int success =   chdir(s.c_str());
                       if(success == 0){
                           getcwd(curdir,sizeof(curdir));
                           d.push_front(curdir);
                            cout<<"Done"<<endl<<endl;
                       }
                       else{
                           cout<<"Access Denied"<<endl<<endl;
                       }
                    }
                    else{
                        cout<<"Not a directory"<<endl<<endl;
                    }
                }
                else{
                    cout<<"Directory doesn't exist"<<endl<<endl;
                }
            }
            else{
                cout<<"INVALID ARGUMENT... Usage: cd <directory name>"<<endl<<endl;
            }
        }
        // hlist implementation
        else if (s == "hlist"){
            for(auto i = m.begin(); i != m.end(); i++){
                cout<<i->first<<" "<<i->second<<endl;
            }
            cout<<endl;
        }
        // dlist implementation
        else if ( s == "dlist"){
            int ind = 0;
            for (auto i : d){
                ind += 1;
                cout<< ind <<"  "<< i <<endl;
            }
            cout<<endl;
        }
        // env implementation
        else if (s == "env"){
            extern char **environ;
            char *env = *environ;
            int i = 1;

            for(;env;i++){
                cout<<env<<endl;
                env = *(environ + i);
            }
            cout<<endl;

        }
        else{
            if(s[0] != '!'){
                cout<<"INVALID INPUT"<<endl<<endl;
            }
        }

        count += 1;
    }
    
    return 0;

}
