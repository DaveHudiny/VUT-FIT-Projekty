/**
  @file in.cpp
  @author David Hudak
  @brief File contains implementation of In and Out classes.
    *These classes are mostly used for dynamic_cast<> and for making difference between In and Out labels.
  */

#include "in.h"

In::In(QGraphicsItem *parent, int index, QString text)
{
    this->setParentItem(parent);
    this->index = index;
    this->setPlainText(text);

}


Out::Out(QGraphicsItem *parent, int index, QString text)
{
    this->setParentItem(parent);
    this->index = index;
    this->setPlainText(text);

}






