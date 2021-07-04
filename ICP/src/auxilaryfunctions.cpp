/**
@file auxilaryfunctions.cpp
@author David Hudak
@brief File contains implementation of utility class AuxilaryFunctions which serves as auxilary functions for working with files.
@brief added implementation of block separating functions.
**/


#include "auxilaryfunctions.h"

int AuxilaryFunctions::loadMap(QString FilePath, std::vector<Block> *blocks, std::vector<Connectives> *connectives)
{
    ifstream fr;
    fr.open(FilePath.toStdString());
    string line;
    while (getline(fr, line))
    {
        Block curr_block;
        if (line == "%Connections")
        {
            break;
        }
        if (line=="%Block")
        {
            if (getline(fr, line))
            {
                int entry_int=stoi(line);
                curr_block.set_id(entry_int);
            }
            else return WRONG_FORMAT;
            if (getline(fr, line))
            {
                curr_block.set_name(line);
            }
            else return WRONG_FORMAT;
            if (getline(fr, line))
            {
                curr_block.set_category(line);
            }
            else return WRONG_FORMAT;
            if (getline(fr, line))
            {
                if (line=="%Inputs")
                {
                    std::vector<In_out> new_inputs;
                    while (getline(fr, line) && line!="%End_Inputs")
                    {
                        In_out input;
                        input.set_name(line);
                        if (getline(fr, line))
                        {
                            input.set_type(line);
                        }
                        else return WRONG_FORMAT;
                        if (getline(fr, line))
                        {
                            int var=stoi(line);
                            bool var2=int(var);
                            input.set_input(var2);
                        }
                        else return WRONG_FORMAT;
                        if (getline(fr, line))
                        {
                            int var=stoi(line);
                            bool var2=int(var);
                            input.set_output(var2);
                        }
                        else return WRONG_FORMAT;
                        new_inputs.insert(new_inputs.end(), input);
                    }
                    if (line!="%End_Inputs")
                    {
                        return WRONG_FORMAT;
                    }
                    else
                    {
                        curr_block.add_input(new_inputs);
                        getline(fr, line);
                    }
                }
            }
                if (line=="%Outputs")
                {
                    std::vector<In_out> new_outputs;
                    while (getline(fr, line) && line!="%End_Outputs")
                    {
                        In_out output;
                        output.set_name(line);
                        if (getline(fr, line))
                        {
                            output.set_type(line);
                        }
                        else return WRONG_FORMAT;
                        if (getline(fr, line))
                        {
                            int var=stoi(line);
                            bool var2=int(var);
                            output.set_input(var2);
                        }
                        else return WRONG_FORMAT;
                        if (getline(fr, line))
                        {
                            int var=stoi(line);
                            bool var2=int(var);
                            output.set_output(var2);
                        }
                        new_outputs.insert(new_outputs.end(), output);
                    }
                    if (line!="%End_Outputs")
                    {
                        return WRONG_FORMAT;
                    }
                    else
                    {
                        curr_block.add_output(new_outputs);
                        if (getline(fr, line))
                        {
                                int var=stoi(line);
                                bool var2=int(var);
                                curr_block.set_built_in(var2);
                        }
                        else return WRONG_FORMAT;
                        getline(fr, line);
                    }
                }
            else return WRONG_FORMAT;
            if (line=="%Code")
            {
                string new_code;
                while (getline(fr, line) && line!="%End_code")
                {
                    new_code.append(line);
                }
                if (line!="%End_code")
                {
                    return WRONG_FORMAT;
                }
                else
                {
                    curr_block.add_code(new_code);
                }
            }
            else return WRONG_FORMAT;
        }
        if (getline(fr, line))
        {
            if (line!="%End_block")
            {
                return WRONG_FORMAT;
            }
            blocks->push_back(curr_block);
        }
        else return WRONG_FORMAT;
    }

    while(getline(fr, line)) // Reading connections from file
    {
        Connectives aux;
        string word = ""; // String of number of id, or string of name of input/output.
        while(line.size()>0) // Load characters to word
        {
            char x = line[0];
            line.erase(0, 1);
            if(x==',')
            {
                break;
            }
            word += x;
        }
        try // Illegal characters.
        {
            aux.idSrc = stoi(word);
        }
        catch(...)
        {
            return WRONG_FORMAT;
        }
        word = "";
        while(line.size()>0)
        {
            char x = line[0];
            line.erase(0, 1);
            if(x==',')
            {
                break;
            }
            word += x;
        }
        try {
            aux.idDst = stoi(word);
        }  catch (...) {
            return WRONG_FORMAT;
        }

        word = "";
        while(line.size()>0)
        {
            char x = line[0];
            line.erase(0, 1);
            if(x==',')
            {
                break;
            }
            word += x;
        }
        aux.nameSrc = word;
        word = "";
        while(line.size()>0)
        {
            char x = line[0];
            line.erase(0, 1);
            if(x==',')
            {
                break;
            }
            word += x;
        }
        aux.nameDst = word;
        word = "";
        connectives->push_back(aux);
    }
    return OK;
}

void AuxilaryFunctions::saveMap(QString filePath, std::vector<Block> blocks, std::vector<Connectives> connectives)
{ // Copy paste from classes.cpp
    ofstream fw (filePath.toStdString());
    if(!fw.is_open())
    {
        return;
    }
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        fw << "%Block\n";
        fw << blocks[i].get_id() << "\n";
        fw << blocks[i].get_name() << "\n";
        fw << blocks[i].get_category() << "\n";
        fw << "%Inputs\n";
        std::vector<In_out> inputs_of_block=blocks[i].get_inputs();
        for (unsigned int j=0; j<inputs_of_block.size(); j++)
        {
            fw << inputs_of_block[j].get_name() << "\n" << inputs_of_block[j].get_type() << "\n" << inputs_of_block[j].get_input() << "\n" << inputs_of_block[j].get_output() << "\n";
        }
        fw << "%End_Inputs\n";
        fw << "%Outputs\n";
        std::vector<In_out> outputs_of_block=blocks[i].get_outputs();
        for (unsigned int j=0; j<outputs_of_block.size(); j++)
        {
            fw << outputs_of_block[j].get_name() << "\n" << outputs_of_block[j].get_type() << "\n" << outputs_of_block[j].get_input() << "\n" << outputs_of_block[j].get_output() << "\n";
        }
        fw << "%End_Outputs\n";
        if (blocks[i].is_built_in()==true)
        {
            fw << "1\n";
        }
        else fw << "0\n";
        fw << "%Code\n";
        fw << blocks[i].get_code() << "\n";
        fw << "%End_code\n";
        fw << "%End_block\n";
    }

    // Part of getting connections
    fw << "%Connections\n";
    for(unsigned int i = 0; i<connectives.size(); i++)
    {
        auto conn = connectives[i];
        //std::cout << conn.idSrc << "," << conn.idDst << "," << conn.nameSrc << "," << conn.nameDst << std::endl;
        fw << conn.idSrc << "," << conn.idDst << "," << conn.nameSrc << "," << conn.nameDst << std::endl;
    }
    fw.close();
}

std::vector<Block> AuxilaryFunctions::getStartBlocks(std::vector<Block> blocks, std::vector<Connectives> conns)
{
    std::vector<Block> aux;
    bool startFound = false;
    bool endFound = false;
    for(__SIZE_TYPE__ i = 0; i<blocks.size(); i++)
    {
        for(__SIZE_TYPE__ j = 0; j<conns.size(); j++)
        {
            if(blocks[i].get_id()==conns[j].idDst)
            {
                startFound = true;
            }
            if(blocks[i].get_id()==conns[j].idSrc)
            {
                endFound = true;
            }
            if(startFound && endFound)
            {
                break;
            }
        }
        if(!startFound && endFound)
        {
            aux.push_back(blocks[i]);
        }
    }
    return aux;
}

std::vector<Block> AuxilaryFunctions::getEndBlocks(std::vector<Block> blocks, std::vector<Connectives> conns)
{
    std::vector<Block> aux;
    bool startFound = false;
    bool endFound = false;
    for(__SIZE_TYPE__ i = 0; i<blocks.size(); i++)
    {
        for(__SIZE_TYPE__ j = 0; j<conns.size(); j++)
        {
            if(blocks[i].get_id()==conns[j].idDst)
            {
                startFound = true;
            }
            if(blocks[i].get_id()==conns[j].idSrc)
            {
                endFound = true;
            }
            if(startFound && endFound)
            {
                break;
            }
        }
        if(startFound && !endFound)
        {
            aux.push_back(blocks[i]);
        }
    }
    return aux;
}

std::vector<Block> AuxilaryFunctions::getMiddleBlocks(std::vector<Block> blocks, std::vector<Connectives> conns)
{
    std::vector<Block> aux;
    bool startFound = false;
    bool endFound = false;
    for(__SIZE_TYPE__ i = 0; i<blocks.size(); i++)
    {
        for(__SIZE_TYPE__ j = 0; j<conns.size(); j++)
        {
            if(blocks[i].get_id()==conns[j].idDst)
            {
                startFound = true;
            }
            if(blocks[i].get_id()==conns[j].idSrc)
            {
                endFound = true;
            }
            if(startFound && endFound)
            {
                break;
            }
        }
        if(startFound && endFound)
        {
            aux.push_back(blocks[i]);
        }
    }
    return aux;
}

std::vector<Block> AuxilaryFunctions::getOtherBlocks(std::vector<Block> blocks, std::vector<Connectives> conns)
{
    std::vector<Block> aux;
    bool startFound = false;
    bool endFound = false;
    for(__SIZE_TYPE__ i = 0; i<blocks.size(); i++)
    {
        for(__SIZE_TYPE__ j = 0; j<conns.size(); j++)
        {
            if(blocks[i].get_id()==conns[j].idDst)
            {
                startFound = true;
                break;
            }
            if(blocks[i].get_id()==conns[j].idSrc)
            {
                endFound = true;
                break;
            }
        }
        if(!startFound && !endFound)
        {
            aux.push_back(blocks[i]);
        }
    }
    return aux;
}
