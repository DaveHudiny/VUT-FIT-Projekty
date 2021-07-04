/**
@file blockitem.h
@author David Hudak
@brief File contains headers of class BlockItem which serves as graphical and informational class for block editor.
**/

#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <QGraphicsPolygonItem>
#include "classes.h"
#include <QString>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <in.h>

/**
 * @brief The BlockItem class Contains block for map editor (graphical) and information about block (from class.h)
 */
class BlockItem : public QGraphicsPolygonItem
{
private:

    Block *block;/** <block contains block from library with new index. Can be edited in future.**/
    QGraphicsTextItem *name;/** <Name contains name of this created block.**/
    std::vector<In*> inputs;/** <inputs contains vector of text items with information about input.**/
    std::vector<Out*> outputs;/** <outputs contains vector of text items with information about output.**/

public:
    /**
     * Explicit constructor to class Block item
     * @param parent contains pointer to parent GraphicsItem object. By default nullptr.
     * @param defaultBlock pointer to block, which should describe this BlockItem object.
     * @param index value of new block index.
     * @param moveX Move object on axis X.
     * @param moveY Move object on axis Y.
     */
    explicit BlockItem(QGraphicsItem *parent = nullptr, Block *defaultBlock = nullptr, int index = 0, qreal moveX = 0, qreal moveY = 0);

    /**
     * Destructor to BlockItem. Frees allocated memory.
     */
    ~BlockItem();
    /**
     * function returns block contained in BlockItem
     * @return pointer to Block contained in BlockItem
     */
    Block *getBlockPtr();

    void moveIt(qreal X, qreal Y);

};

#endif // BLOCKITEM_H
