/**
@file classes.h
@author Štefan Mračna
@brief File contains headers of class classes which contains the backend of the application
**/

#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

#include <QDebug>

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string.h>

/**
 * @brief Error values
 */
enum errors{OK, DUPLICATE_ID, ID_NOT_FOUND, CONNECTION_NOT_FOUND, VOID_CONNECTION, FILE_NOT_FOUND, FILE_NOT_OPENED, WRONG_FORMAT, BLOCK_NOT_FOUND};

/**
 * @brief Determines whether a connection is an input, output, input and output, or neither.
 */
enum input_output{INPUT, OUTPUT, BOTH, NONE};

using namespace std;

/**
 * @brief Structure determining the identity of the block
 */
typedef struct
{
    int id; /** <ID of the block**/
    string name; /** <Name of the block**/
    string category; /** <Category of the block (logical, arithmetic...)**/
} Block_identity;

/**
 * @brief Contains information about an input/output of a block
 */
class In_out{
    private:
        string name; /** <The name of an input/output**/
        bool input; /** <Indicates, that the input/output is an input**/
        bool output; /** <Indicates, that the input/output is an output**/
        string type; /** <The data type of a given input/output**/
    public:
        /**
         * @brief Returns the name of the input/output
         * @returns The name of the input/output
         */
        string get_name();

        /**
         * @brief Returns the type of the input/output
         * @returns The name of the type of input/output
         */
        string get_type();

        /**
         * @brief Returns, whether the input/output is an input
         * @returns Bool value determining whether the input/output is an input
         */
        bool get_input();

        /**
         * @brief Returns, whether the input/output is an output
         * @returns Bool value determining whether the input/output is an output
         */
        bool get_output();

        /**
         * @brief Sets the name of the input/output
         * @param new_name Name of the input/output to be set
         */
        void set_name(string new_name);

        /**
         * @brief Sets the name of the data type of the input/output
         * @param new_type Name of the data type of the input/output to be set
         */
        void set_type(string new_type);

        /**
         * @brief Sets the input/output with a value determining whether or not the input/output is an input
         * @param new_input Bool value determining whether or not the input/output is an input
         */
        void set_input(bool new_input);

        /**
         * @brief Sets the input/output with a value determining whether or not the input/output is an output
         * @param new_output Bool value determining whether or not the input/output is an output
         */
        void set_output(bool new_output);
};

/**
 * @brief Contains information about a block and operations with the block
 */
class Block{
    private:
        int id=-1; /** <ID of the block**/
        string name; /** <Name of the block**/
        std::vector<In_out> inputs; /** <Inputs of the block**/
        std::vector<In_out> outputs; /** <Outputs of the block**/
        bool built_in; /** <Determines, whether the block is built in or not**/
        string code; /** <C++ code of the block**/
        string category; /** <Category of the block (logical, arithmetic...)**/
    public:

        /**
         * @brief Constructor of the block, sets the block to default values (ID 0, name and code null, built-in false)
         */
        Block();

        /**
         * @brief Constructor of the block, sets the block to default values and name and id to parameter values
         * @param name Name of the block to be set
         * @param id ID of the block to be set
         */
        Block(string name, int id);

        /**
         * @brief Adds a set of inputs to a block
         * @param new_inputs Vector of inputs to be set
         */
        void add_input(std::vector<In_out> new_inputs);

        /**
         * @brief Adds a set of outputs to a block
         * @param new_outputs Vector of outputs to be set
         */
        void add_output(std::vector<In_out> new_outputs);

        /**
         * @brief Prevents redundancy of outputs, returns DUPLICATE_ID if values are redundant
         * @param out Name of the output to be checked - method returns error if the value is found within outputs
         * @returns error message
         */
        int can_output(string out);

        /**
         * @brief Prevents redundancy of inputs, returns DUPLICATE_ID if values are redundant
         * @param in Name of the input to be checked - method returns error if the value is found within inputs
         * @returns error message
         */
        int can_input(string in);

        /**
         * @brief Removes an input
         * @param index Index of the input in vector inputs to be removed
         */
        void remove_input(int index);

        /**
         * @brief Removes an output
         * @param index Index of the output in vector outputs to be removed
         */
        void remove_output(int index);

        /**
         * @brief Removes all inputs
         */
        void remove_inputs();

        /**
         * @brief Removes all outputs
         */
        void remove_outputs();

        /**
         * @brief Checks, whether the block has a certain ID
         * @param entry_id The ID to be checked
         * @returns True if the block ID is equal to the passed ID, false if not
         */
        bool check_id(int entry_id);

        /**
         * @brief Returns the ID of the block
         * @returns The ID of the block
         */
        int get_id();

        /**
         * @brief Returns the name of the block
         * @returns The name of the block
         */
        string get_name();

        /**
         * @brief Returns the code of the block
         * @returns The code of the block
         */
        string get_code();

        /**
         * @brief Returns the inputs of the block
         * @returns The inputs of the block
         */
        std::vector<In_out> get_inputs();

        /**
         * @brief Returns the outputs of the block
         * @returns The outputs of the block
         */
        std::vector<In_out> get_outputs();

        /**
         * @brief Returns, whether the block is built-in
         * @returns Bool value, determining whether or not the block is built-in
         */
        bool is_built_in();

        /**
         * @brief Adds the C++ code of the block
         * @param new_code String representing the C++ code of the block
         */
        void add_code(string new_code);

        /**
         * @brief Removes the C++ code of the block
         */
        void remove_code();

        /**
         * @brief Sets the ID of the block
         * @param new_id the ID to be set for the block
         */
        void set_id(int new_id);

        /**
         * @brief Sets the name of the block
         * @param new_name the name to be set for the block
         */
        void set_name(string new_name);

        /**
         * @brief Sets, whether the block is built in or not
         * @param new_built_in Bool value - determines, whether the block will be set as built-in or not
         */
        void set_built_in(bool new_built_in);

        /**
         * @brief Returns the category of the block
         * @returns The category of the block
         */
        string get_category();

        /**
         * @brief Sets the category of the block
         * @param new_category the category to be set for the block
         */
        void set_category(string new_category);
};

/**
 * @brief Establishes a connection between two blocks
 */
class Connection{
    private:
        Block *input=nullptr; /** <Pointer to the block that serves as the source of the connection**/
        Block *output=nullptr; /** <Pointer to the block that serves as the destination of the connection**/
        string in; /** <Name of the input/output, that serves as the source of the connection**/
        string out; /** <Name of the input/output, that serves as the destination of the connection**/
        bool is_in; /** <Value determining, whether or not the connection has a source block**/
        bool is_out; /** <Value determining, whether or not the connection has a destination block**/
    public:
        /**
         * @brief is_input Function returns true, if connection has in and false, if connection doesnt.
         * @return True, or false.
         */
        bool is_input();
        /**
         * @brief is_output Function returns true, if connection has out and false, if connection doesnt.
         * @return True, or false.
         */
        bool is_output();
        /**
         * @brief Changes the source block of the connection
         * @param new_input the new source block
         */
        void change_input(Block new_input);

        /**
         * @brief Changes the destination block of the connection
         * @param new_output the new destination block
         */
        void change_output(Block new_output);

        /**
         * @brief Alternative version of the change_input function with a pointer to the source block
         * @see change_input
         * @param *new_input the pointer to the new source block
         */
        void change_input_ptr(Block *new_input);

        /**
         * @brief Alternative version of the change_output function with a pointer to the destination block
         * @see change_input
         * @param *new_output the pointer to the new destination block
         */
        void change_output_ptr(Block *new_output);


        /**
         * @brief Returns the pointer of the source block
         * @returns The pointer to the source block
         */
        Block * get_input();

        /**
         * @brief Returns the pointer of the destination block
         * @returns The pointer to the destination block
         */
        Block * get_output();

        /**
         * @brief Returns the source input/output
         * @returns The source input/output
         */
        string get_in();

        /**
         * @brief Returns the destination input/output
         * @returns The destination input/output
         */
        string get_out();

        /**
         * @brief Sets the source input/output
         * @param new_input The source input/output
         */
        void set_in(string new_input);

        /**
         * @brief Sets the destination input/output
         * @param new_output The destination input/output
         */
        void set_out(string new_output);

        /**
         * @brief Turns the connection to a source-less connection (connection has no source block)
         */
        void set_in_bool_zero();

        /**
         * @brief Turns the connection to a destination-less connection (connection has no destination block)
         */
        void set_out_bool_zero();

        /**
         * @brief Checks whether the connection is an input (has no source block), output (has no destination block), both or neither
         * @returns INPUT if the connection has no source block, OUTPUT if the connection has no destination block, BOTH if it has both and NEITHER if it has neither
         */
        int  check_input_output();

        /**
         * @brief Operator determining, whether or not the connection are equal
         * @param other The other connection that the current connection will be compared to
         * @returns Truth value depending on whether or not the connections are equal
         */
        bool operator==(Connection & other);
};

/**
 * @brief Structure containing the library of blocks
 */
class Libs{
    private:
        std::vector<Block> blocks; /** <The blocks saved in the library**/
        FILE *fr; /** <The file, from which the library will be downloaded**/
    public:

        /**
         * @brief Constructor, which initializes built-in libraries
         */
        Libs();

        /**
         * @brief Adds a block to the library
         * @param block The block to be added
         * @returns DUPLICATE_ID if the ID of the block is already saved, OK otherwise
         */
        int add_block(Block block);

        /**
         * @brief Removes a block from the library
         * @param id_block The block to be removed from the library
         * @returns ID_NOT_FOUND if the ID of the block is not found, OK otherwise
         */
        int remove_block(int id_block);

        /**
         * @brief Updates information about a block
         * @param block The block to be updated
         */
        void actualize_block(Block block);

        /**
         * @brief Downloads a library from a given file
         * @param fr The file from which the library will be downloaded
         * @returns WRONG_FORMAT if the text in the file is of the wrong format, OK otherwise
         */
        int download_lib(ifstream& fr);

        /**
         * @brief Saves a library to a given file
         * @param filename The name of the file to which the library will be saved
         * @returns FILE_NOT_OPENED if the file cannot be opened, OK otherwise
         */
        int save_lib(string filename);

        /**
         * @brief Fetches a block based on the ID of the block
         * @param id The ID of the block to be fetched
         * @param entr_block The block to be returned
         * @returns BLOCK_NOT_FOUND if the block is not found, OK otherwise
         */
        int return_block(int id, Block *entr_block);

        /**
         * @brief Returns all blocks saved within the library
         * @returns Blocks saved in the library
         */
        std::vector<Block> return_blocks();

        /**
         * @brief Returns the identities of given blocks
         * @see block_identity
         * @param input_blocks The blocks to fetch identities from
         * @returns The identities of the given blocks
         */
        std::vector<Block_identity> return_identity(std::vector<Block> input_blocks);
        //void write_block(Block block);
};

/**
 * @brief Structure containing the current map of the circuit
 */
class Block_map{
    private:
        std::vector<Block> blocks; /** <The blocks on the block map**/
        std::vector<Connection> connections; /** <Connections on the block map**/
        std::vector<Connection> input_connections; /** <Input connections on the block map (connections without a source block)**/
        std::vector<Connection> output_connections; /** <Output connections on the block map (connections without a destination block)**/
        std::vector<Connection> const_connections; /** <Constant connections on the block map**/
    public:

        /**
         * @brief Adds a block to the block map
         * @param block The block to be added
         * @returns DUPLICATE_ID if the ID of the block is already saved, OK otherwise
         */
        int add_block(Block block);

        /**
         * @brief Removes a block from the block map
         * @param id_block The ID of the block to be removed
         * @returns ID_NOT_FOUND if the ID of the block is not found, OK otherwise
         */
        int remove_block(int id_block);

        /**
         * @brief Adds a connection to the block map and categorizes it to input_connections and output_connections
         * @param connection The connection to be added
         * @returns VOID_CONNECTION if the connection does not have a source or a destination block, OK otherwise
         */
        int create_connection(Connection connection);

        /**
         * @brief Removes a connection from the block map
         * @param connection The connection to be removed
         * @returns CONNECTION_NOT_FOUND if the connection does not exist, OK otherwise
         */
        int remove_connection(Connection connection);

        /**
         * @brief Adds a constant connection to the block map
         * @param connection The connection to be added
         * @returns Error message
         */
        int add_const_connection(Connection connection);

        /**
         * @brief Removes a constant connection from the block map
         * @param connection The connection to be removed
         * @returns CONNECTION_NOT_FOUND if the connection does not exist, OK otherwise
         */
        int remove_const_connection(Connection connection);

        /**
         * @brief Compiles the block map to C++ code
         * @returns Error message
         */
        int compile();

        /**
         * @brief Creates one composite block out of multiple blocks
         * @param old_blocks The series of blocks to be combined
         * @param new_id The new ID of the composite block
         * @param new_name The new name of the composite block
         * @param type The new type of the composite block
         * @returns Error message
         */
        int combine_blocks(std::vector<Block> old_blocks, int new_id, string new_name, string type);

        /**
         * @brief Clears the block map
         */
        void removeAll();

        /**
         * @brief Returns all connections of the block map
         * @returns All connections of the block map
         */
        std::vector<Connection> returnConnections();

        /**
         * @brief Clears the connections of the block map
         */
        void clearConnections();

        /**
         * @brief Returns all blocks of the block map
         * @returns All blocks of the block map
         */
        std::vector<Block> getBlocks();

        /**
         * @brief Returns a pointer of the block of a given ID
         * @param index The ID of a block to be fetched
         * @returns Pointer to the block with a given ID, nullptr if a block does not exist
         */
        Block *getBlockByIndex(int index);


};


#endif // CLASSES_H
