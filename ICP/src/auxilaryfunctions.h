/**
@file auxilaryfunctions.h
@author David Hudak
@brief File contains headers of utility class AuxilaryFunctions which serves as auxilary functions for working with files.
@brief File also contains Connectives structure which serves for connections.
@brief Added headers for block separating functions.
**/

#ifndef AUXILARYFUNCTIONS_H
#define AUXILARYFUNCTIONS_H
#include<classes.h>

/**
 * @brief The Connectives struct serves for saving informations about connections.
 */
struct Connectives{
    int idSrc;
    int idDst;
    string nameSrc;
    string nameDst;
};

/**
  * @brief The AuxilaryFunctions class contains utility functions for working with files of composite blocks and functions for separating blocks.
  */
class AuxilaryFunctions
{
public:
    /**
     * @brief loadMap Function loads composite block from file.
     * @param filePath Name of file.
     * @param blocks Pointer to vector of blocks which will be filled after operation.
     * @param connectives Pointer to vector which will be filled after operation.
     * @return OK if success, something else if not.
     */
    static int loadMap(QString filePath, std::vector<Block> *blocks, std::vector<Connectives> *connectives);
    /**
     * @brief saveMap Function saves composite block to file.
     * @param filePath Name of file.
     * @param blocks Blocks to be saved.
     * @param connectives Connectives to be saved.
     */
    static void saveMap(QString filePath, std::vector<Block> blocks, std::vector<Connectives> connectives);

    /**
     * @brief getStartBlocks Function returns blocks which should be first (no connections to it's inputs)
     * @param blocks Block where to search.
     * @param conns Connectives in map.
     * @return Vector of starting blocks.
     */
    static std::vector<Block> getStartBlocks(std::vector<Block> blocks, std::vector<Connectives> conns);
    /**
     * @brief getEndBlocks Function returns blocks which should be last (no connections to it's outputs)
     * @param blocks Block where to search.
     * @param conns Connectives in map.
     * @return Vector of ending blocks.
     */
    static std::vector<Block> getEndBlocks(std::vector<Block> blocks, std::vector<Connectives> conns);
    /**
     * @brief getMiddleBlocks Function returns blocks which should be in the middle (connection to it's input and to output)
     * @param blocks Block where to search.
     * @param conns Connectives in map.
     * @return Vector of middle blocks.
     */
    static std::vector<Block> getMiddleBlocks(std::vector<Block> blocks, std::vector<Connectives> conns);

    /**
     * @brief getOtherBlocks Function returns blocks which are applications (no connections to it)
     * @param blocks Block where to search.
     * @param conns Connectives in map.
     * @return Vector of other blocks.
     */
    static std::vector<Block> getOtherBlocks(std::vector<Block> blocks, std::vector<Connectives> conns);
};

#endif // AUXILARYFUNCTIONS_H
