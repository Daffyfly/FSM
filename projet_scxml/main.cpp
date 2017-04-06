#include <iostream>
#include "Xmlparser.h"
#include "codeGenerator.h"

using namespace std;

int main() {
    Xmlparser x("resources/.scxml");
    AST ast = x.getAST();
    std::string path = "result/";
    /*cout<<"Choose the path where you want to copy the files (with /)"<<endl;
    cin>>path;*/;
    std::string filename = "machineState";
    /*cout<<"Choose a name for your machine state"<<endl;
    cin>>filename;*/;
    if(filename.compare("")){
        codeGenerator c(path,filename,ast);
        c.generateFile();
    }
    else{
        cout<<"Invalid file name, exiting"<<endl;
        return -1;
    }
    return 0;
}