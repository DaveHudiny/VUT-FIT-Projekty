/**
 * @file blockitem.cpp
 * @author David Hudak
 * @brief File contains BlockItem methods implementation
 **/

#include "blockitem.h"
#include <QDebug>


BlockItem::BlockItem(QGraphicsItem *parent, Block *defaultBlock, int index, qreal moveX, qreal moveY)
    : QGraphicsPolygonItem(parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable); // Makes block selectable -- does not work.
    if(defaultBlock!=nullptr) // If called with block (expected), creates new block
    {
        this->block = new Block(*defaultBlock); // creating new block with copying values.
        this->block->set_id(index); // Blocks from library can be repeated so map block should contain map index.
    }
    else return; // It doesn't make sense to do more.
    name = new QGraphicsTextItem(QString::fromStdString(block->get_name()), this); // Creates item for name label of block
    name->moveBy(5, 0);
    name->setFlags(QGraphicsItem::ItemIsSelectable);

    qreal diffIn=0; /**<Variable used for moving with input texts.**/
    int i = 0; // index
    qreal textSize = 0; /**<Variable used for getting size of text. Used when creating geometry of block.**/
    for(auto input: block->get_inputs()) // cycle for printing inputs to BlockItem
    {
        auto *in = new In(this, i++,QString::fromStdString(input.get_name())); // Creating new input
        if(input.get_name().size() > textSize/6) // Found new maximum of text length
        {
            textSize = 6*static_cast<qreal>(input.get_name().size()); // Retyping size of text to qreal
        }
        diffIn+=20; // Adds indentation of text
        in->moveBy(0, diffIn); //Moves with text
        in->setFlags(QGraphicsItem::ItemIsSelectable);
        inputs.push_back(in); // Adds input to vector of inputs. Serves for expansions and deleting objects.
    }
    qreal diffOut = 0; // Same as for inputs.
    qreal textSize2 = 0; // Same
    i = 0;
    for(auto output: block->get_outputs()) // Cycle for printing outputs. Similar to code before.
    {
        auto *out = new Out(this, i++,QString::fromStdString(output.get_name()));
        if(output.get_name().size() > textSize2/6)
        {
            textSize2 = 6*static_cast<qreal>(output.get_name().size());
        }
        diffOut+=20;
        out->moveBy(70+textSize, diffOut);
        out->setFlags(QGraphicsItem::ItemIsSelectable);
        outputs.push_back(out);
    }

    setFlags(QGraphicsItem::ItemIsMovable);
    QPolygonF set; // Contains geometry of polygon (rectangle)
    qreal diff = diffIn > diffOut ? diffIn : diffOut; // Chooses between intendations of outputs and inputs and chooses bigger.
    set << QPoint(0 + moveX, 0 + moveY) << QPoint(90 + textSize + textSize2 + moveX, 0 + moveY)
        << QPoint(90 + textSize + textSize2 + moveX, diff + 30 + moveY) << QPoint(0 + moveX, diff + 30 + moveY) << QPoint(0 + moveX, 0 + moveY);
    setPolygon(set);
}

Block *BlockItem::getBlockPtr()
{
    return block;
}


BlockItem::~BlockItem()
{
    delete name;
    delete block;
    for(auto input: inputs) // Goes through vector of inputs and outputs and deletes them.
    {
        delete input;
    }
    for(auto output: outputs)
    {
        delete output;
    }
}

void BlockItem::moveIt(qreal X, qreal Y)
{
    moveBy(X, Y);
}
