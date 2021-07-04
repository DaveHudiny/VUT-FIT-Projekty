/**
  @file scene.h
  @author David Hudak
  @brief File contains header for class scene for drawing block editor in mainwindow.
  */
#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <blockitem.h>
#include <QGraphicsLineItem>
#include <classes.h>
#include <lineconnection.h>
#include <auxilaryfunctions.h>
#include <memory>

/**
 * @brief The scene class contains map of blocks and the stuff around. Class inherits QGraphicsScene
 * and inherits QGraphicsScene, which add functionality of scene and makes possible to add graphical objects.
 */
class scene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief scene function is constructor for scene
     * @param parent contains object item, which is parent of scene. No parent expected.
     */
    explicit scene(QObject *parent = nullptr);
    /**
      * @brief destructor for scene, deletes all contained objects.
      */
    ~scene();
    /**
     * @brief getMap returns pointer to map contained in scene. Important for adding objects to scene from mainwindow.
     * @return Pointer to Block_map
     */
    Block_map *getMap();
    /**
     * @brief addBlock adds Block from BlockItem to map.
     * @param block pointer to BlockItem from editor.
     */
    void addBlock(BlockItem *block);
    /**
     * @brief clearAll clears all stuff from scene and makes it empty.
     */
    void clearAll();
    /**
     * @brief addConnections class scene contains all connections between blocks. This function adds them to map. Important for compilation.
     */
    void addConnections();
    /**
     * @brief returnCompositeBlock function returns by pointer new block generated from map.
     * @param index value of new id of generated block
     * @param name string value of new generated block
     * @param block pointer to block, which will be overwritten. Have to be not null.
     * @return error value of function generating composite block
     */
    int returnCompositeBlock(int index, string name, Block *block);
    /**
     * @brief createConnectives Function translates connections from map to structure connectives.
     * @return
     */
    std::vector<Connectives> createConnectives();
    /**
     * @brief createConnections Function creates connections on map from composite block.
     */
    void createConnections(std::vector<Connectives>);
    /**
     * @brief debugConnections Debugging function which prints all existing connections.
     */
    void debugConnections();

signals:

protected:
    /**
     * @brief mousePressEvent overwritten method for mouse click, creates new connection.
     * @param event Some Qt stuff.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent overwritten method for mouse move, works with connection.
     * @param event Some Qt stuff
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent overwritten method for mouse release, finishes connection.
     * @param event Some Qt stuff
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    LineConnection *line = nullptr; /**< if null, no line is painting, else painting and then saving to connections */
    string stringLineStart = "Out"; /**< Information about written object (Out -- painting starts from output, In -- painting starts from input) */
    Block_map map; /**< contains map without connections, added only when compiling */
    std::vector<std::unique_ptr<LineConnection>> connections; /**< Vector of connections */
    bool addedYet = false; /**< Indicates that connection isn't new*/
    /**
     * @brief lookForIn Function looks for text of Input in block. nullptr if not found.
     * @param block Pointer to block where is searched text.
     * @param name Name of input
     * @return Pointer to correct In
     */
    In *lookForIn(BlockItem *block, string name);
    /**
     * @brief lookForOut Function looks for text of Output in block. nullptr if not found.
     * @param block Pointer to block where is searched text
     * @param name name of Output
     * @return Pointer to correct Out
     */
    Out *lookForOut(BlockItem *block, string name);
    /**
     * @brief addConnection Function adds connection to map.
     * @param auxDst pointer to destination block
     * @param auxSrc pointer to source block
     * @param end pointer to text destination item
     * @param start pointer to text source item
     * @param line pointer to creating connection
     */
    void addConnection(BlockItem *auxDst, BlockItem *auxSrc, In *end, Out *start, LineConnection *line);
    /**
     * @brief leftClickIn Function serves left click on input text
     * @param event Some Qt stuff
     * @param aux Pointer to clicked text
     * @param item Pointer to parent BlockItem
     */
    void leftClickIn(QGraphicsSceneMouseEvent *event, In *aux, QGraphicsItem *item);
    /**
     * @brief leftClickIn Function serves left click on output text
     * @param event Some Qt stuff
     * @param aux Pointer to clicked text
     * @param item Pointer to parent BlockItem
     */
    void leftClickOut(QGraphicsSceneMouseEvent *event, Out *aux, QGraphicsItem *item);
    int lastId=0; /**< Last id of added block.*/

    /**
     * @brief The mapa struct contains information to remap connections when removing or adding block.
     */
    struct mapa
    {
        int idConnection; /**< Id of connection.*/
        int idOriginInput; /**< Id of original input.*/
        int idOriginOutput; /**< Id of original output.*/
    };
    /**
     * @brief mapRemover Function removes block from map. Important for bug fix for connections.
     * @param id Id of block which has to be removed.
     */
    void mapRemover(int id);
};

#endif // SCENE_H
