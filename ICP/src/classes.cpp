/**
* @file classes.cpp
* @author Štefan Mračna
* @brief File contains headers of class classes which contains the backend of the application
**/

#include "classes.h"
#include <iostream>


using namespace std;

string In_out::get_name()
{
    return name;
}

string In_out::get_type()
{
    return type;
}

bool In_out::get_input()
{
    return input;
}

bool In_out::get_output()
{
    return output;
}

void In_out::set_name(string entr_name)
{
    name=entr_name;
}

void In_out::set_type(string entr_type)
{
    type=entr_type;
}

void In_out::set_input(bool new_input)
{
    input=new_input;
}

void In_out::set_output(bool new_output)
{
    output=new_output;
}

Block::Block()
{
    this->built_in = false;
    this->code = "";
    this->id = 0;
    this->name = "";
}

Block::Block(string name, int id)
{
    this->name = name;
    this->id = id;
    this->built_in = false;
    this->code = "";
}

void Block::add_input(std::vector<In_out> new_inputs)
{
    for (unsigned int i=0; i<new_inputs.size(); i++)
    {
        inputs.insert(inputs.end(), new_inputs[i]);
    }
}

void Block::add_output(std::vector<In_out> new_outputs)
{
    for (unsigned int i=0; i<new_outputs.size(); i++)
    {
        outputs.insert(outputs.end(), new_outputs[i]);
    }
}

int Block::can_output(string out)
{
    for(unsigned int i=0; i<outputs.size(); i++)
    {
        if(out == outputs[i].get_name())
        {
            return DUPLICATE_ID;
        }
    }
    return OK;
}

int Block::can_input(string in)
{
    for(unsigned int i=0; i<inputs.size(); i++)
    {
        if(in == inputs[i].get_name())
        {
            return DUPLICATE_ID;
        }
    }
    return OK;
}

void Block::remove_input(int index)
{
    inputs.erase(inputs.begin() + index);
}

void Block::remove_output(int index)
{
    outputs.erase(outputs.begin() + index);
}

void Block::remove_inputs()
{
    inputs.clear();
}
void Block::remove_outputs()
{
    outputs.clear();
}

bool Block::check_id(int entry_id)
{
    return entry_id==id;
}

int Block::get_id()
{
    return id;
}

string Block::get_name()
{
    return name;
}

string Block::get_code()
{
    return code;
}

std::vector<In_out> Block::get_inputs()
{
    return inputs;
}

std::vector<In_out> Block::get_outputs()
{
    return outputs;
}

bool Block::is_built_in()
{
    return built_in;
}

void Block::add_code(string new_code)
{
    code=new_code;
}

void Block::remove_code()
{
    code="";
}

void Block::set_id(int new_id)
{
    id=new_id;
}

void Block::set_name(string new_name)
{
    name=new_name;
}

void Block::set_built_in(bool new_built_in)
{
    built_in=new_built_in;
}

string Block::get_category()
{
    return category;
}

void Block::set_category(string new_category)
{
    category=new_category;
}

void Connection::change_input(Block new_input)
{
    is_in=1;
    input=&new_input;
}

void Connection::change_output(Block new_output)
{
    is_out=1;
    output=&new_output;
}

void Connection::change_input_ptr(Block *new_input)
{
    is_in=1;
    input=new_input;
}

void Connection::change_output_ptr(Block *new_output)
{
    is_out=1;
    output=new_output;
}

Block * Connection::get_input()
{
    return input;
}

Block * Connection::get_output()
{
    return output;
}

string Connection::get_in()
{
    return in;
}

string Connection::get_out()
{
    return out;
}

void Connection::set_in(string new_input)
{
    in=new_input;
}

void Connection::set_out(string new_output)
{
    out=new_output;
}

void Connection::set_in_bool_zero()
{
    is_in=0;
}

void Connection::set_out_bool_zero()
{
    is_out=0;
}

int Connection::check_input_output()
{
    if (is_in==0 && is_out==0)
    {
        return NONE;
    }
    else if (is_in==0)
    {
        return INPUT;
    }
    else if (is_out==0)
    {
        return OUTPUT;
    }
    else
    {
        return BOTH;
    }
}

bool Connection::operator==(Connection &other)
{
    return this->input==other.input && this->output==other.output;

}

bool Connection::is_input()
{
    return is_in;
}

bool Connection::is_output()
{
    return is_out;
}

Libs::Libs()
{
    Block AND, OR, ADDINT, SUBINT, MULINT, DIVINT;
    AND.set_id(0);
    AND.set_name("and_bool");
    AND.set_built_in(1);
    AND.add_code("c=a&&b;");
    AND.set_category("logical");
    In_out a, b, c;
    a.set_name("a");
    a.set_type("bool");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("bool");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("bool");
    c.set_input(0);
    c.set_output(1);
    std::vector<In_out>in;
    std::vector<In_out>out;
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    AND.add_input(in);
    AND.add_output(out);
    OR.set_id(1);
    OR.set_name("or_bool");
    OR.set_built_in(1);
    OR.add_code("c=a||b;");
    OR.set_category("logical");
    a.set_name("a");
    a.set_type("bool");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("bool");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("bool");
    c.set_input(0);
    c.set_output(1);
    in.clear();
    out.clear();
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    OR.add_input(in);
    OR.add_output(out);
    ADDINT.set_id(2);
    ADDINT.set_name("and_int");
    ADDINT.set_built_in(1);
    ADDINT.add_code("c=a+b;");
    ADDINT.set_category("arithmetic");
    a.set_name("a");
    a.set_type("int");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("int");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("int");
    c.set_input(0);
    c.set_output(1);
    in.clear();
    out.clear();
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    ADDINT.add_input(in);
    ADDINT.add_output(out);
    DIVINT.set_id(3);
    DIVINT.set_name("div_int");
    DIVINT.set_built_in(1);
    DIVINT.add_code("c=a/b;");
    DIVINT.set_category("arithmetic");
    a.set_name("a");
    a.set_type("int");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("int");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("int");
    c.set_input(0);
    c.set_output(1);
    in.clear();
    out.clear();
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    DIVINT.add_input(in);
    DIVINT.add_output(out);
    SUBINT.set_id(4);
    SUBINT.set_name("sub_int");
    SUBINT.set_built_in(1);
    SUBINT.add_code("c=a-b;");
    SUBINT.set_category("arithmetic");
    a.set_name("a");
    a.set_type("int");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("int");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("int");
    c.set_input(0);
    c.set_output(1);
    in.clear();
    out.clear();
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    SUBINT.add_input(in);
    SUBINT.add_output(out);
    MULINT.set_id(5);
    MULINT.set_name("mul_int");
    MULINT.set_built_in(1);
    MULINT.add_code("c=a*b;");
    MULINT.set_category("arithmetic");
    a.set_name("a");
    a.set_type("int");
    a.set_input(1);
    a.set_output(0);
    b.set_name("b");
    b.set_type("int");
    b.set_input(1);
    b.set_output(0);
    c.set_name("c");
    c.set_type("int");
    c.set_input(0);
    c.set_output(1);
    in.clear();
    out.clear();
    in.insert(in.end(), a);
    in.insert(in.end(), b);
    out.insert(out.end(), c);
    MULINT.add_input(in);
    MULINT.add_output(out);
    blocks.insert(blocks.end(), AND);
    blocks.insert(blocks.end(), OR);
    blocks.insert(blocks.end(), ADDINT);
    blocks.insert(blocks.end(), SUBINT);
    blocks.insert(blocks.end(), MULINT);
    blocks.insert(blocks.end(), DIVINT);

}

int Libs::add_block(Block block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        int current_id=blocks[i].get_id();
        if (block.check_id(current_id))
        {
            return DUPLICATE_ID;
        }
    }
    blocks.insert(blocks.end(), block);
    return OK;
}
void Libs::actualize_block(Block block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        if (blocks[i].check_id(block.get_id()))
        {
            blocks[i].set_name(block.get_name());
            blocks[i].remove_code();
            blocks[i].add_code(block.get_code());
            blocks[i].remove_inputs();
            blocks[i].remove_outputs();
            blocks[i].add_input(block.get_inputs());
            blocks[i].add_output(block.get_outputs());
            blocks[i].set_category(block.get_category());
            return;
        }
    }
}

int Libs::remove_block(int id_block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        if (blocks[i].check_id(id_block))
        {
            blocks.erase(blocks.begin()+i);
            return OK;
        }
    }
    return ID_NOT_FOUND;
}

int Libs::download_lib(ifstream& fr)
{
    string line;
    while (getline(fr, line))
    {
        Block curr_block;
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
            add_block(curr_block);
        }
        else return WRONG_FORMAT;

    }
    return OK;
}

int Libs::save_lib(string filename)
{
    ofstream fw (filename);
    if (fw.is_open())
    {
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
    }
    else
    {
        return FILE_NOT_OPENED;
    }
    return OK;
}

int Libs::return_block(int id_block, Block *entr_block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        if (blocks[i].check_id(id_block))
        {
            string name=blocks[i].get_name();
            *entr_block=blocks[i];
            return OK;
        }
    }
    entr_block=nullptr;
    return BLOCK_NOT_FOUND;
}

std::vector<Block> Libs::return_blocks()
{
    return blocks;
}

std::vector<Block_identity> Libs::return_identity(std::vector<Block> input_blocks)
{
    std::vector<Block_identity> ret_identity;
    for (unsigned int i=0; i<input_blocks.size(); i++)
    {
        Block_identity aux_identity;
        aux_identity.id=input_blocks[i].get_id();
        aux_identity.name=input_blocks[i].get_name();
        aux_identity.category=input_blocks[i].get_category();
        ret_identity.push_back(aux_identity);
    }
    return ret_identity;
}

int Block_map::add_block(Block block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        int current_id=blocks[i].get_id();
        if (block.check_id(current_id))
        {
            return DUPLICATE_ID;
        }
    }
    blocks.insert(blocks.end(), block);
    return OK;
}

int Block_map::remove_block(int id_block)
{
    for (unsigned int i=0; i<blocks.size(); i++)
    {
        if (blocks[i].check_id(id_block))
        {
            blocks.erase(blocks.begin()+i);
            return OK;
        }
    }
    return ID_NOT_FOUND;
}

int Block_map::create_connection(Connection connection)
{
    int type=connection.check_input_output();
    if (type==INPUT)
    {
        input_connections.insert(input_connections.end(), connection);
    }
    else if (type==OUTPUT)
    {
        output_connections.insert(output_connections.end(), connection);
    }
    else if (type==NONE)
    {
        return VOID_CONNECTION;
    }
    connections.insert(connections.end(), connection);
    return OK;
}

int Block_map::remove_connection(Connection connection)
{
    int type=connection.check_input_output();
    if (type==INPUT)
    {
        for (unsigned int i=0; i<input_connections.size(); i++)
        {
            if (connection==input_connections[i])
            {
                input_connections.erase(input_connections.begin()+i);
                return OK;
            }
        }
    }
    if (type==OUTPUT)
    {
        for (unsigned int i=0; i<output_connections.size(); i++)
        {
            if (connection==output_connections[i])
            {
                output_connections.erase(output_connections.begin()+i);
                return OK;
            }
        }
    }
    for (unsigned int i=0; i<connections.size(); i++)
    {
        if (connection==connections[i])
        {
            connections.erase(connections.begin()+i);
            return OK;
        }
    }
    return CONNECTION_NOT_FOUND;
}

int Block_map::add_const_connection(Connection connection)
{
    const_connections.insert(const_connections.end(), connection);
    return 0;
}

int Block_map::remove_const_connection(Connection connection)
{
    for (unsigned int i=0; i<const_connections.size(); i++)
    {
        if (connection==const_connections[i])
        {
            const_connections.erase(const_connections.begin()+i);
            return 0;
        }
    }
    return CONNECTION_NOT_FOUND;
}

int Block_map::compile()
{
    ofstream fw ("code.cpp");
    if (fw.is_open())
    {
        fw << "#include <vector>\n#include <iostream>\n#include <fstream>\n#include <string.h>\n#include <vector>\n#include <algorithm>\n";
        std::vector<string>names;
        for (unsigned int i=0; i<blocks.size(); i++)
        {
            int found=0;
            string name=blocks[i].get_name();
            string code=blocks[i].get_code();
            std::vector<In_out> curr_block_inp=blocks[i].get_inputs();
            std::vector<In_out> curr_block_out=blocks[i].get_outputs();
            for (unsigned int j=0; j<names.size(); j++)
            {
                if (name==names[j])
                {
                    found=1;
                }
            }
            if (found==0)
            {
                names.insert(names.end(), name);
                fw << "void " << name << "(";
                for (unsigned int j=0; j<curr_block_inp.size(); j++)
                {
                    string type=curr_block_inp[j].get_type();
                    string name_variable=curr_block_inp[j].get_name();
                    fw << type << " " << name_variable;
                    if (j<curr_block_inp.size()-1 || !curr_block_out.empty())
                    {
                        fw << ", ";
                    }
                }

                for (unsigned int j=0; j<curr_block_out.size(); j++)
                {
                    string type=curr_block_out[j].get_type();
                    string name_variable=curr_block_out[j].get_name();
                    fw << type << " *" << name_variable;
                    if (j<curr_block_out.size()-1)
                    {
                        fw << ", ";
                    }
                }

                fw << ")\n{\n";
                fw << code << "\n}\n";
            }

        }
        fw << "\n" << "int main()\n{";
        std::vector<string> declared;
        for (unsigned int i=0; i<blocks.size(); i++)
        {
            std::vector<In_out> curr_block_inp=blocks[i].get_inputs();
            std::vector<In_out> curr_block_out=blocks[i].get_outputs();
            bool found=0;
            for (unsigned int j=0; j<curr_block_inp.size(); j++)
            {
                found=0;
                for (unsigned int k=0; k<declared.size(); k++)
                {
                    if (curr_block_inp[j].get_name()==declared[k])
                    {
                        found=1;
                    }
                }
                if (found==0)
                {
                    declared.insert(declared.end(), curr_block_inp[j].get_name());
                    string var=curr_block_inp[j].get_name();
                    fw << curr_block_inp[j].get_type() << " " << var << ";\n";
                }
            }
            for (unsigned int j=0; j<curr_block_out.size(); j++)
            {
                found=0;
                for (unsigned int k=0; k<declared.size(); k++)
                {
                    if (curr_block_out[j].get_name()==declared[k])
                    {
                        found=1;
                    }
                }
                if (found==0)
                {
                    declared.insert(declared.end(), curr_block_out[j].get_name());
                    string var=curr_block_out[j].get_name();
                    fw << curr_block_out[j].get_type() << " " << var << ";\n";
                }
            }
        }
        std::vector<Block> blocks_clone=blocks;
        std::vector<Connection> connections_clone=connections;
        std::vector<Connection> in_connections_clone=input_connections;
        while (blocks_clone.empty()!=true)
        {
            std::vector<string> name_output, name_input;
            for (unsigned int i=0; i<blocks_clone.size(); i++)
            {
                bool not_in=0;
                std::vector<Connection> block_input;
                std::vector<Connection> block_output;
                for (unsigned int j=0; j<connections_clone.size(); j++)
                {
                    int id1, id2;
                    Block *new_block=connections_clone[j].get_output();
                    Block *new_block2=connections_clone[j].get_input();
                    if(!new_block2)
                    {
                        id2=-1;
                    }
                    else id2=new_block2->get_id();
                    if (!new_block)
                    {
                        id1=-1;
                    }
                    else id1=new_block->get_id();
                    if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()!=INPUT)
                    {
                        block_input.clear();
                        block_output.clear();
                        not_in=1;
                        break;
                    }
                    else if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()==INPUT)
                    {

                        block_input.insert(block_input.end(), connections_clone[j]);
                    }
                    else if (id2==blocks_clone[i].get_id())
                    {
                        block_output.insert(block_output.end(), connections_clone[j]);
                    }
                }
                if (not_in==0)
                {
                    fw << blocks_clone[i].get_name() << "(";
                    std::vector<In_out>block_inputs, block_outputs;
                    block_inputs=blocks_clone[i].get_inputs();
                    block_outputs=blocks_clone[i].get_outputs();
                    for (unsigned int j=0; j<block_inputs.size(); j++)
                    {
                        if (j>0)
                        {
                            fw << ", ";
                        }
                        fw << block_inputs[j].get_name();
                    }
                    std::vector<string>outputs;
                    for (unsigned int j=0; j<block_outputs.size(); j++)
                    {
                        int duplicate=0;
                        for (unsigned int k=0; k<outputs.size(); k++)
                        {
                            if (block_outputs[j].get_name()==outputs[k])
                            {
                                duplicate=1;
                            }
                        }
                        if (duplicate==0)
                        {
                            if (j>0 || block_inputs.size()>0)
                            {
                                fw << ", ";
                            }
                            outputs.insert(outputs.end(), block_outputs[j].get_name());
                            fw << "&" << block_outputs[j].get_name();
                        }
                    }
                    fw << ");\n";
                    for (unsigned int j=0; j<block_output.size(); j++)
                    {
                        string in=block_output[j].get_in(), out=block_output[j].get_out();
                        if (block_output[j].check_input_output()==BOTH)
                        {
                            fw << out << "=" << in << ";\n";
                        }
                    }
                    for (unsigned int j=0; j<connections_clone.size(); j++)
                    {
                        int id1, id2;
                        Block *new_block=connections_clone[j].get_output();
                        Block *new_block2=connections_clone[j].get_input();
                        if (new_block==nullptr)
                        {
                            id1=-1;
                        }
                        else id1=new_block->get_id();
                        if (new_block2==nullptr)
                        {
                            id2=-1;
                        }
                        else id2=new_block2->get_id();
                        if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()==INPUT)
                        {
                            connections_clone.erase(connections_clone.begin()+j);
                            j--;
                        }
                        if (id2==blocks_clone[i].get_id())
                        {
                            connections_clone[j].set_in_bool_zero();
                        }
                    }
                    blocks_clone.erase(blocks_clone.begin()+i);
                    i--;
                }
            }
        }
        fw << "\n}";
        fw.close();


    }
    else
    {
        return FILE_NOT_OPENED;
    }
    return OK;
}

int Block_map::combine_blocks(std::vector<Block> old_blocks, int new_id, string new_name, string type)
{
    std::vector<Connection>connections_copy;
    Block new_block;
    std::vector<In_out> inputs, outputs;
    connections_copy=connections;
    for (unsigned int i=0; i<old_blocks.size(); i++)
    {
        std::vector<In_out>blocks_inputs, blocks_outputs;
        blocks_inputs=old_blocks[i].get_inputs();
        blocks_outputs=old_blocks[i].get_outputs();
        int id;
        string name_inpout;
        In_out new_in, new_out;
        for (unsigned int j=0; j<blocks_inputs.size(); j++)
        {
            bool exists=0;
            for (unsigned int k=0; k<connections_copy.size(); k++)
            {
                Block *block_ptr=connections_copy[k].get_output();
                if (block_ptr==nullptr)
                {
                    id=-1;
                }
                else id=block_ptr->get_id();
                if (id==old_blocks[i].get_id())
                {
                    if (connections_copy[k].get_out()==blocks_inputs[j].get_name())
                    {
                        exists=1;
                    }
                }
            }
            if (exists==0)
            {
                type=blocks_inputs[j].get_type();
                new_in.set_name(blocks_inputs[j].get_name());
                new_in.set_type(type);
                new_in.set_input(1);
                new_in.set_output(0);
                inputs.insert(inputs.end(), new_in);
            }
        }
        for (unsigned int j=0; j<blocks_outputs.size(); j++)
        {
            bool exists=0;
            for (unsigned int k=0; k<connections_copy.size(); k++)
            {
                Block *block_ptr=connections_copy[k].get_input();
                if (block_ptr==nullptr)
                {
                    id=-1;
                }
                else id=block_ptr->get_id();
                if (id==old_blocks[i].get_id())
                {
                    if (connections_copy[k].get_in()==blocks_outputs[j].get_name())
                    {
                        exists=1;
                    }
                }
            }
            if (exists==0)
            {
                type=blocks_outputs[j].get_type();
                new_out.set_name(blocks_outputs[j].get_name());
                new_out.set_type(type);
                new_out.set_input(1);
                new_out.set_output(0);
                outputs.insert(outputs.end(), new_out);
            }
        }
    }
    for (unsigned int i=0; i<connections_copy.size(); i++)
    {
        bool input=0, output=0;
        string type;
        int block_index_out=0, block_index_in=0;
        for (unsigned int j=0; j<old_blocks.size(); j++)
        {

            int id=old_blocks[j].get_id();
            Block *block_pointer;
            block_pointer=connections_copy[i].get_input();
            int inp_id, out_id;
            if (!block_pointer)
            {
                inp_id=-1;
            }
            else inp_id=block_pointer->get_id();
            block_pointer=connections_copy[i].get_output();
            if (!block_pointer)
            {
                out_id=-1;
            }
            else out_id=block_pointer->get_id();
            if (connections_copy[i].get_input()!=nullptr && id==inp_id)
            {
                output=1;
                block_index_out=j;
            }
            if (connections_copy[i].get_output()!=nullptr && id==out_id)
            {
                input=1;
                block_index_in=j;
            }
        }
        if (input==0 && output==0)
        {
            connections_copy.erase(connections_copy.begin()+i);
            i--;
        }
        else if (input==1 && output==0)
        {
            In_out new_in;
            std::vector<In_out>inputs2;
            inputs2=old_blocks[block_index_in].get_inputs();
            unsigned int k;
            for (k=0; k<inputs2.size(); k++)
            {
                if (connections_copy[i].get_out()==inputs2[k].get_name() && inputs2[k].get_input()==true)
                {
                    type=inputs2[k].get_type();
                    break;
                }
            }
            if (k<inputs2.size())
            {
                new_in.set_name(connections_copy[i].get_out());
                new_in.set_type(type);
                new_in.set_input(1);
                new_in.set_output(0);
                inputs.insert(inputs.end(), new_in);
            }
            connections_copy[i].set_in_bool_zero();
        }
        else if (output==1 && input==0)
        {
            In_out new_out;
            std::vector<In_out>outputs2;
            outputs2=old_blocks[block_index_out].get_outputs();
            unsigned int k;
            for (k=0; k<outputs2.size(); k++)
            {
                if (connections_copy[i].get_in()==outputs2[k].get_name() && outputs2[k].get_output()==true)
                {
                    type=outputs2[k].get_type();
                    break;
                }
            }
            if (k<outputs2.size())
            {
                new_out.set_name(connections_copy[i].get_in());
                new_out.set_type(type);
                new_out.set_input(0);
                new_out.set_output(1);
                outputs.insert(outputs.end(), new_out);
            }
            connections_copy[i].set_out_bool_zero();
        }
    }
    std::vector<Block>blocks_clone=old_blocks;
    std::vector<Connection>connections_clone=connections_copy;
    string code="";
    while (blocks_clone.empty()!=true)
    {
        for (unsigned int i=0; i<blocks_clone.size(); i++)
        {
            bool not_in=0;
            std::vector<Connection> block_input;
            std::vector<Connection> block_output;
            for (unsigned int j=0; j<connections_clone.size(); j++)
            {
                int id1, id2;
                Block *new_block=connections_clone[j].get_output();
                Block *new_block2=connections_clone[j].get_input();
                if(!new_block2)
                {
                    id2=-1;
                }
                else id2=new_block2->get_id();
                if (!new_block)
                {
                    id1=-1;
                }
                else id1=new_block->get_id();
                if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()!=INPUT)
                {

                    block_input.clear();
                    block_output.clear();
                    not_in=1;
                    break;
                }
                else if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()==INPUT)
                {

                    block_input.insert(block_input.end(), connections_clone[j]);
                }
                else if (id2==blocks_clone[i].get_id())
                {
                    block_output.insert(block_output.end(), connections_clone[j]);
                }
            }
            if (not_in==0)
            {
                code=code+blocks_clone[i].get_code()+"\n";
                for (unsigned int j=0; j<block_output.size(); j++)
                {
                    string in=block_output[j].get_in(), out=block_output[j].get_out();
                    if (block_output[j].check_input_output()!=OUTPUT)
                    {
                        code=code+out+"="+in+";\n";
                    }
                }
                for (unsigned int j=0; j<connections_clone.size(); j++)
                {
                    int id1, id2;
                    Block *new_block=connections_clone[j].get_output();
                    Block *new_block2=connections_clone[j].get_input();
                    if (new_block==nullptr)
                    {
                        id1=-1;
                    }
                    else id1=new_block->get_id();
                    if (new_block2==nullptr)
                    {
                        id2=-1;
                    }
                    else id2=new_block2->get_id();
                    if (id1==blocks_clone[i].get_id() && connections_clone[j].check_input_output()==INPUT)
                    {
                        connections_clone.erase(connections_clone.begin()+j);
                        j--;
                    }
                    if (id2==blocks_clone[i].get_id())
                    {

                        connections_clone[j].set_in_bool_zero();
                    }
                }
                blocks_clone.erase(blocks_clone.begin()+i);
                i--;
            }
        }
    }
    new_block.set_built_in(0);
    new_block.set_name(new_name);
    new_block.set_category(type);
    new_block.set_id(new_id);
    new_block.add_code(code);
    new_block.add_input(inputs);
    new_block.add_output(outputs);
    blocks.insert(blocks.end(), new_block);
    /*for (unsigned int i=0; i<connections.size(); i++)
    {
        bool input=0, output=0;
        for (unsigned int j=0; j<old_blocks.size(); j++)
        {
            int id=old_blocks[j].get_id();
            Block *block_pointer;
            block_pointer=connections[i].get_input();
            int inp_id, out_id;
            if (!block_pointer)
            {
                inp_id=-1;
            }
            else inp_id=block_pointer->get_id();
            block_pointer=connections[i].get_output();
            if (!block_pointer)
            {
                out_id=-1;
            }
            else out_id=block_pointer->get_id();
            if (connections[i].get_input()!=nullptr && id==inp_id)
            {
                output=1;
            }
            if (connections[i].get_output()!=nullptr && id==out_id)
            {
                input=1;
            }
        }
        if (input==1 && output==1)
        {
            connections.erase(connections.begin()+i);
            i--;
        }
        else if (input==1 && output==0)
        {
            Block *new_block;
            new_block=&blocks[blocks.size()-1];
            connections[i].change_output_ptr(new_block);
        }
        else if (output==1 && input==0)
        {
            Block *new_block;
            new_block=&blocks[blocks.size()-1];
            connections[i].change_input_ptr(new_block);
        }
    }
    for (unsigned int i=0; i<old_blocks.size(); i++)
    {
        for (unsigned int j=0; j<blocks.size(); j++)
        {
            int id1=old_blocks[i].get_id();
            int id2=blocks[j].get_id();
            if (id1==id2)
            {
                blocks.erase(blocks.begin()+j);
                j--;
                break;
            }
        }
    }*/

    return OK;

}

void Block_map::removeAll()
{
    this->blocks.clear();
    this->connections.clear();
    this->const_connections.clear();
    this->output_connections.clear();
    this->input_connections.clear();
}

void Block_map::clearConnections()
{
    this->connections.clear();
    this->const_connections.clear();
    this->output_connections.clear();
    this->input_connections.clear();
}

std::vector<Connection> Block_map::returnConnections()
{
    return connections;
}

std::vector<Block> Block_map::getBlocks()
{
    return blocks;
}

Block *Block_map::getBlockByIndex(int index)
{
    for(__SIZE_TYPE__ i = 0; i<blocks.size(); i++)
    {
        if(blocks[i].get_id() == index)
        {
            return &blocks[i];
        }
    }
    return nullptr;
}
