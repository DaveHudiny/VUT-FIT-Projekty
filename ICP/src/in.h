/**
  @file in.h
  @author David Hudák
  @brief File contains headers for classes In and Out which serves for input and output texts.
  */

#ifndef IN_H
#define IN_H


#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <classes.h>

/**
 * @brief The In class contains text item and index of item in block. Small missunderstanding in creating project was,
 * that In now means output from block and Out means input of block. It means actually source and destination.
 */
class In : public QGraphicsTextItem
{
private:
    int index;

public:
    /**
     * @brief In constructor for class In
     * @param parent contains parent of item
     * @param index value of order of input.
     * @param text contiains text for label.
     */
    explicit In(QGraphicsItem *parent = nullptr, int index = 0, QString text = "");

};

/**
 * @brief The Out class contains text item and index of item in block.
 */
class Out : public QGraphicsTextItem
{
private:
    int index;
    QGraphicsLineItem *line;

public:
    /**
     * @brief Out constructor for class In
     * @param parent contains parent of item
     * @param index value of order of input.
     * @param text contiains text for label.
     */
    explicit Out(QGraphicsItem *parent = nullptr, int index = 0, QString text = "");

};


#endif // IN_H
