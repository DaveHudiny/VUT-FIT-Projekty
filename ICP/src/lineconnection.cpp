/**
  @file lineconnection.cpp
  @author David Hudak
  @brief File contains implementation for class LineConnection
  */

#include "lineconnection.h"


LineConnection::LineConnection(QGraphicsItem *parent, int index)
{
    setParentItem(parent);
    this->id = index;
}

Connection *LineConnection::getConnectionPtr()
{
    return &connection;
}

int LineConnection::getId()
{
    return this->id;
}



