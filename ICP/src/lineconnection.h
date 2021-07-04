/**
  @file lineconnection.h
  @author David Hudak
  @brief File contains header for class LineConnection
  */

#ifndef LINECONNECTION_H
#define LINECONNECTION_H

#include<QGraphicsLineItem>
#include<classes.h>

/**
 * @brief The LineConnection class contains line object (inherited)
 */
class LineConnection : public QGraphicsLineItem
{
public:
    explicit LineConnection(QGraphicsItem *parent = nullptr, int index=0);
    /**
     * @brief getConnectionPtr return pointer to connection in LineConnection object.
     * @return pointer to connection contained in object
     */
    Connection *getConnectionPtr();
    int getId();
private:
    Connection connection;
    int id;
};

#endif // LINECONNECTION_H
