#include "CSVimport.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;


int main()
{
    string dfafile;
    string tokenfile;
    string keywordfile;
    string sourcefile;
    cout << "Please input the file path for your DFA state table.\n";
    std::cin >> dfafile;
    cout << "Please input the file path for the token table.\n";
    std::cin >> tokenfile;
    cout << "Please input the file path for the keyword identifier.\n";
    std::cin >> keywordfile;
    cout << "Please input the file path for the source file to tokenize.\n";
    std::cin >> sourcefile;




    
	CSVimport import(dfafile);
    std::vector<std::vector<std::string> > dfatable = import.getData();
    CSVimport import1(tokenfile);
    vector<vector<string>> tokentable = import1.getData();
    CSVimport import2(keywordfile);
    vector<vector<string>> keywords = import2.getData();

   


 
    std::ifstream in(sourcefile);

    std::stringstream buff;
    buff << in.rdbuf();

    std::string sourcestring(buff.str());

  
    string buffer = "";
    string intbuffer;
    string state = "0";
    
    std::vector<std::vector<string>> output;
    int indexX = 0;
    int indexY = 0;
    std::string character;
    bool intToIdentSwitch = false; 
    bool doubleEqual = false;

    
      
    character.push_back(0x20);
    dfatable[0][82] = character;
    character = "";
    character.push_back(0x09);
    dfatable[0][83] = character;
    character = "";
    character.push_back(0x0D);
    dfatable[0][84] = character;
    character = "";
    character.push_back(0x0A);
    dfatable[0][85] = character;
    character = "";
    character.push_back(0x2C);
    dfatable[0][76] = character;
   
    character = "";
    character.push_back(0x27);
    dfatable[0][69] = character;
    character = "";
    character.push_back(0x22);
    dfatable[0][70] = character;
    
    
    
    
   

    for (long s = 0; s < sourcestring.size(); s++) {
        character = "";

     
        character.push_back(sourcestring[s]);
      
        
       



        Tokenize:
        for (int i = 1; i < dfatable[0].size() ; i++)
        {
            if (dfatable[0][i] == character)
            {
                indexX = i;
                break;
            }

        }
        
        for (int j = 1; j < dfatable.size() ; j++)
        {
            if (dfatable[j][0] == state)
            {
                indexY = j;
                
 
                if (dfatable[indexY][indexX] == "")
                {
                    for (int i = 0; i < tokentable.size(); i++)
                    {
                        if (tokentable[i][0] == state)
                        {
                            if (tokentable[i][1] != "")
                            {
                                //after switch is turned on use the token table identifier as a the clue to trigger the procedure

                               
                                

                                //std::cout << "Token: " << tokentable[i][1] << "               Lexeme: " << buffer << "\n";
                                vector<string> token;
                                token.push_back(tokentable[i][1]);
                                token.push_back(buffer);
                                output.push_back(token);




                                if (state == "38") // to generalize exceptions to make the lexer work withoutside CSV lists, just prompt the user to put their number for states int ---> identifier and the double equal 
                                {
                                    
                                    if ((sourcestring[s - 3] == '*') || (sourcestring[s - 3] == '-')|| (sourcestring[s - 3] == '/')|| (sourcestring[s - 3] == '+'))
                                    {
                                        output.pop_back();
                                        token[0] = "Error, Invalid lexeme";
                                        token[1].push_back(sourcestring[s - 2]);
                                        token[1].append("==");
                                        output.push_back(token);
                                        cout << "COCK]\n";

                                    }
                                }
                                
                               
                                if ((state=="1" || state == "2") && intToIdentSwitch)  //trigerring too early before follow up ident ca
                                {
                                    string errorstring;
                                    errorstring.append(output[output.size() - 2][1]);
                                    errorstring.append(output[output.size()-1][1]);
                                    output.pop_back();
                                    output.pop_back();
                                    token[0] = "Error, Invalid lexeme";
                                    token[1] = errorstring;
                                    output.push_back(token);
                                    intToIdentSwitch = false;
                                    

                                    
                                }
                                



                                buffer = "";
                                state = "0";
                                goto Tokenize;
                                break;

                            }
                        }
                    }

                }

                
                state = dfatable[indexY][indexX];
                
                

                if (state == "")
                {
                    state = "0";
                    break;
                }
             
                if (state == "3")
                {
                    char next = sourcestring[s + 1];
                    string nextstring = "";
                    nextstring.push_back(next);
                    int x = 0;
                    int y = 0;
                    string tempstate = "0";
                    for (int i = 1; i < dfatable[0].size() - 1; i++)
                    {
                        if (dfatable[0][i] == nextstring)
                        {
                            x = i;
                            break;
                        }

                    }
                    for (int j = 1; j < dfatable.size(); j++)
                    {
                        if (dfatable[j][0] == tempstate)
                        {
                            y = j;
                            break;

                        }
                    }
                    if (dfatable[y][x] == "1")
                    {
                        intToIdentSwitch = true; 
                    }

                    
                }








                buffer.append(character);
               
                break;
            }



        }

}

for (int i = 0; i < output.size(); i++)
{
    if (output[i][0] == "identifier")
    {
        for (int j = 0; j < keywords.size(); j++)
        {
            if (output[i][1] == keywords[j][0])
            {
                output[i][0] == "Reserved word";
            }



        }
    }






    for (int i = 0; i < output.size(); i++)
    {
        cout << "Token: " << output[i][0] << "      Lexeme: " << output[i][1] << endl;


    }
}
    


    

    std::fstream fout;
    string filename1 = "D:\\outputlist.csv ";

    // opens an existing csv file or creates a new file.
    fout.open(filename1, std::ios::out | std::ios::app);

    for (int i = 0; i < output.size(); i++)
    {
        for (int j = 0; j < output[i].size(); j++)
        {
            fout << output[i][j] << ",";
        }
        fout << endl;



    }


   
    
 
    
        return 0;
        

}