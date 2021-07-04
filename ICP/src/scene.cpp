/**
  @file scene.cpp
  @author David Hudak
  @brief File contains implementation of scene class methods.
  */

#include "scene.h"


scene::scene(QObject *parent) : QGraphicsScene(parent)
{
    setParent(parent);
    connections.clear();
}

void scene::clearAll()
{
    //qDebug() << "In 1";
    map.removeAll(); // Clears all vectors everywhere
    /*for(auto connect: connections) // Deletes all pointer from pointer vector.
    {
        delete connect;
    }*/
    //qDebug() << "In 2";
    connections.clear();
    //qDebug() << "In 3";
    line = nullptr;
    //qDebug() << "In 4";
}

scene::~scene()
{

    map.removeAll();
}

void scene::addConnections()
{ 
    for(auto &connection: connections) // Goes through all connections in vector and changes their pointer from BlockItem Block to map block.
    {
        Connection *newer = connection->getConnectionPtr();
        if(newer->get_input()!=nullptr)
        {
            newer->change_input_ptr(map.getBlockByIndex(newer->get_input()->get_id()));
        }
        if(newer->get_output()!=nullptr)
        {
            newer->change_output_ptr(map.getBlockByIndex(newer->get_output()->get_id()));
        }

        //qDebug() << "Input " << QString::fromStdString(newer->get_input()->get_name())
         //        << " output " << QString::fromStdString(newer->get_output()->get_name());
        map.create_connection(*newer); // Adds connection to map.
    }
}

Block_map *scene::getMap()
{
    return &this->map;
}

void scene::addBlock(BlockItem *block)
{
    std::vector<mapa> mapVec;
    for(__SIZE_TYPE__ i = 0; i<connections.size(); i++)
    {
        mapa aux;
        aux.idConnection=connections[i]->getId();
        if(connections[i]->getConnectionPtr()->get_input())
        {
            aux.idOriginInput=connections[i]->getConnectionPtr()->get_input()->get_id();
        }
        else
        {
            aux.idOriginInput=-1;
        }
        if(connections[i]->getConnectionPtr()->get_output())
        {
            aux.idOriginOutput=connections[i]->getConnectionPtr()->get_output()->get_id();
        }
        else
        {
            aux.idOriginOutput=-1;
        }
        mapVec.push_back(aux);
    }
    map.add_block(*block->getBlockPtr());
    for(__SIZE_TYPE__ i = 0; i<connections.size(); i++)
    {
        if(mapVec[i].idOriginInput!=-1)
        {
            connections[i]->getConnectionPtr()->change_input_ptr(
                        map.getBlockByIndex(mapVec[i].idOriginInput));
        }
        if(mapVec[i].idOriginOutput!=-1)
        {
            connections[i]->getConnectionPtr()->change_output_ptr(
                        map.getBlockByIndex(mapVec[i].idOriginOutput));
        }
    }
     // Reads block from BlockItem and adds it to map
}

void scene::leftClickIn(QGraphicsSceneMouseEvent *event, In *aux, QGraphicsItem *item)
{
    line = new LineConnection(item, lastId++); // Create new connection
    line->setPen(QPen({Qt::black}, 3));
    line->setLine(QLineF(QPoint(1, 3),  event->scenePos() - line->parentItem()->scenePos())); // Correction between object and scene position
    line->getConnectionPtr()->set_out(aux->toPlainText().toStdString()); // Setting name
    BlockItem *auxBlock;
    if((auxBlock = dynamic_cast<BlockItem*>(item->parentItem())) == nullptr) // Casting to BlockItem (expected not NULL)
    {
        line->getConnectionPtr()->change_output_ptr(nullptr);
    }
    else
    {
        line->getConnectionPtr()->change_output_ptr(map.getBlockByIndex(auxBlock->getBlockPtr()->get_id()));
        // Translating BlockItem pointer to map Block pointer (weird misunderstandment with backend)
    }
    stringLineStart = "In"; // For recognition of connection input output
    addedYet = false; // New connection, so we cannot expect it in vector of connections
}

void scene::leftClickOut(QGraphicsSceneMouseEvent *event, Out *aux, QGraphicsItem *item)
{
    line = new LineConnection(item, lastId++);
    line->setPen(QPen({Qt::black}, 3));
    line->setLine(QLineF(QPoint(1, 3), event->scenePos() - line->parentItem()->scenePos()));
    line->getConnectionPtr()->set_in(aux->toPlainText().toStdString());
    BlockItem *auxBlock;
    if((auxBlock = dynamic_cast<BlockItem*>(item->parentItem())) == nullptr)
    {
        line->getConnectionPtr()->change_input_ptr(nullptr);
    }
    else
    {
        line->getConnectionPtr()->change_input_ptr(map.getBlockByIndex(auxBlock->getBlockPtr()->get_id()));
    }
    stringLineStart = "Out";
    addedYet = false;
}

void scene::mapRemover(int id)
{
    std::vector<mapa> mapVec;
    for(__SIZE_TYPE__ i = 0; i<connections.size(); i++)
    {
        mapa aux;
        aux.idConnection=connections[i]->getId();
        if(connections[i]->getConnectionPtr()->get_input())
        {
            aux.idOriginInput=connections[i]->getConnectionPtr()->get_input()->get_id();
        }
        else
        {
            aux.idOriginInput=-1;
        }
        if(connections[i]->getConnectionPtr()->get_output())
        {
            aux.idOriginOutput=connections[i]->getConnectionPtr()->get_output()->get_id();
        }
        else
        {
            aux.idOriginOutput=-1;
        }
        mapVec.push_back(aux);
    }
    map.remove_block(id);
    for(__SIZE_TYPE__ i = 0; i<connections.size(); i++)
    {
        if(mapVec[i].idOriginInput!=-1)
        {
            connections[i]->getConnectionPtr()->change_input_ptr(
                        map.getBlockByIndex(mapVec[i].idOriginInput));
        }
        if(mapVec[i].idOriginOutput!=-1)
        {
            connections[i]->getConnectionPtr()->change_output_ptr(
                        map.getBlockByIndex(mapVec[i].idOriginOutput));
        }
    }
}

void scene::mousePressEvent(QGraphicsSceneMouseEvent *event) // Overwritten method of mousePressEvent
{
    for(auto *item: items(event->scenePos())) // Cycle goes through all selected items.
    {

        if(dynamic_cast<In*>(item) != nullptr && event->button() == Qt::LeftButton) // If selected item is In, add variable is output
        {
            In *aux = dynamic_cast<In*>(item); // Casting item to In class
            leftClickIn(event, aux, item);
        }
        else if(dynamic_cast<Out*>(item) != nullptr && event->button() == Qt::LeftButton) // Same as above, but with Out
        {
            Out *aux = dynamic_cast<Out*>(item);
            leftClickOut(event, aux, item);
        } // End of adding connection commands.
        else if(event->button() == Qt::RightButton && dynamic_cast<BlockItem*>(item) != nullptr) //deleting block from scene
        {
            for(__SIZE_TYPE__ i=0; i<connections.size(); i++) // Looking for connections with selected block -- these needed to be deleted.
            {
                auto &connect = connections[i];
                if(connect == nullptr) continue;
                bool removed = false;
                if(!removed && connect->getConnectionPtr()->get_input())
                {
                    if(dynamic_cast<BlockItem*>(item)->getBlockPtr()->get_id() == connect->getConnectionPtr()->get_input()->get_id())
                    {
                        int idRem = connect->getId();
                        connections.erase(remove_if(connections.begin(), connections.end(), [idRem](auto const& pi)
                                                    {return pi->getId() == idRem; }), connections.end());
                        removed = true;
                        i--;
                    }
                }
                if(!removed &&connect->getConnectionPtr()->get_output())
                {
                    if(dynamic_cast<BlockItem*>(item)->getBlockPtr()->get_id() == connect->getConnectionPtr()->get_output()->get_id())
                    {
                        int idRem = connect->getId();
                        connections.erase(remove_if(connections.begin(), connections.end(), [idRem](auto const& pi)
                                                    {return pi->getId() == idRem; }), connections.end());
                        i--;
                    }
                }

            }
            mapRemover(dynamic_cast<BlockItem*>(item)->getBlockPtr()->get_id());
            //map.remove_block(dynamic_cast<BlockItem*>(item)->getBlockPtr()->get_id());
            delete item;
        } // end of deleting block
        else if(event->button() == Qt::LeftButton && dynamic_cast<BlockItem*>(item) != nullptr)
        {
            QGraphicsScene::mousePressEvent(event); // Just propagate mousePressEvent // Now maybe redundant
        }
        else if(event->button() == Qt::RightButton && dynamic_cast<LineConnection*>(item) != nullptr) // Deleting connection
        {
            int idRem = dynamic_cast<LineConnection*>(item)->getId();
            connections.erase(remove_if(connections.begin(), connections.end(), [idRem](auto const& line)
            {return line->getId() == idRem;}),connections.end());
        }
        else if(event->button() == Qt::LeftButton && dynamic_cast<LineConnection*>(item) != nullptr) // Moving with existing connection
        {
            line = dynamic_cast<LineConnection*>(item);
            stringLineStart = dynamic_cast<In*>(line->parentItem()) ? "In" : "Out";
            line->setLine(QLineF(line->line().p1(),  event->scenePos()-line->parentItem()->scenePos()));
            addedYet = true;
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(line) // Connection taken, move it.
    {
        line->setPen(QPen({Qt::black}, 3));
        line->setLine(QLineF(line->line().p1(), event->scenePos()-line->parentItem()->scenePos()));
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(line)
    {
        if(stringLineStart == "Out") // Connection starts as Out
        {
            for(auto *itemEnd: items(event->scenePos())) // Loop goes through all items, where was line released.
            {
                if(dynamic_cast<In*>(itemEnd) != nullptr) // In object was found, connection should be finished
                {
                    auto aux = dynamic_cast<In*>(itemEnd);
                    line->setLine(QLineF(line->line().p1(), event->scenePos()-line->scenePos()));
                    line->getConnectionPtr()->set_out(aux->toPlainText().toStdString());
                    BlockItem *auxBlock;
                    if((auxBlock = dynamic_cast<BlockItem*>(itemEnd->parentItem())) == nullptr)
                    { // Weird error, In and Out object should have parent item BlockItem
                        line->getConnectionPtr()->change_output_ptr(nullptr);
                        line->getConnectionPtr()->set_out_bool_zero();
                    }
                    else
                    { // Get object pointer from map by translation by id.
                        line->getConnectionPtr()->change_output_ptr(map.getBlockByIndex(auxBlock->getBlockPtr()->get_id()));
                    }
                    if(!addedYet) connections.push_back(std::unique_ptr<LineConnection>(line)); // If not added yet, add new connection.
                    line = nullptr;
                    QGraphicsScene::mouseReleaseEvent(event);
                    return; // End this function here, maybe not so good solution, but still working
                }
            } // end of for loop
            line->getConnectionPtr()->change_output_ptr(nullptr); // No In object found, set nulls everywhere
            line->getConnectionPtr()->set_out_bool_zero();
            if(!addedYet)
            {
                connections.push_back(std::unique_ptr<LineConnection>(line));
            }
        } // end of "Out" condition
        else if(stringLineStart == "In") // Connection starts as input, then similar to Out, but with complementary functions.
        {
            for(auto *itemEnd: items(event->scenePos()))
            {
                if(dynamic_cast<Out*>(itemEnd) != nullptr) // Found Out at scenePosition.
                {
                    auto aux = dynamic_cast<Out*>(itemEnd);
                    line->setLine(QLineF(line->line().p1(), event->scenePos()-line->scenePos()));
                    line->getConnectionPtr()->set_in(aux->toPlainText().toStdString());
                    BlockItem *auxBlock;
                    if((auxBlock = dynamic_cast<BlockItem*>(itemEnd->parentItem())) == nullptr)
                    {
                        line->getConnectionPtr()->change_input_ptr(nullptr);
                    }
                    else
                    {
                        line->getConnectionPtr()->change_input_ptr(map.getBlockByIndex(auxBlock->getBlockPtr()->get_id()));
                    }
                    if(!addedYet)
                    {
                        connections.push_back(std::unique_ptr<LineConnection>(line));
                    }
                    QGraphicsScene::mouseReleaseEvent(event);
                    line = nullptr;
                    return; // Same as before
                } // end of if(dyn_cast)
            } // end of for
            line->getConnectionPtr()->change_input_ptr(nullptr);
            line->getConnectionPtr()->set_in_bool_zero();
            if(!addedYet)
            {
                connections.push_back(std::unique_ptr<LineConnection>(line));
            }
        }
        line->setLine(QLineF(line->line().p1(), event->scenePos()-line->scenePos())); // This option is universal to not found situation
        line = nullptr;
    } // end of if(line)
    QGraphicsScene::mouseReleaseEvent(event);
}

int scene::returnCompositeBlock(int index, string name, Block *block)
{
    addConnections(); // Adds connection to map
    int errcode = map.combine_blocks(map.getBlocks(), index, name, "composite block");
    map.clearConnections(); // Clears connection from map -- unwanted there
    if(errcode == OK && !map.getBlocks().empty()) // Composing blocks was OK and vector of blocks in library is not empty.
    {
        *block = map.getBlocks()[map.getBlocks().size()-1];
        map.remove_block(map.getBlocks()[map.getBlocks().size()-1].get_id());
        return OK;
    }
    else
        return errcode;
}

std::vector<Connectives> scene::createConnectives()
{
    std::vector<Connectives> conns;
    //qDebug()
    for(auto &connection: connections)
    {
        Connectives aux;
        if(connection->getConnectionPtr()->is_input())
        {
            aux.idSrc = connection->getConnectionPtr()->get_input()->get_id();
            aux.nameSrc = connection->getConnectionPtr()->get_in();
        }
        else
        {
            aux.idSrc = -1;
            aux.nameSrc = "%";
        }
        if(connection->getConnectionPtr()->is_output())
        {
            aux.idDst = connection->getConnectionPtr()->get_output()->get_id();
            aux.nameDst = connection->getConnectionPtr()->get_out();
        }
        else
        {
            aux.idDst = -1;
            aux.nameDst = "%";
        }
        conns.push_back(aux);
    }
    return conns;
}

In *scene::lookForIn(BlockItem *block, string name)
{
    for(auto item: block->childItems())
    {
        if(dynamic_cast<In*>(item))
        {
            if(dynamic_cast<In*>(item)->toPlainText().toStdString() == name)
            {
                return dynamic_cast<In*>(item);
            }
        }
    }
    return nullptr;
}

Out *scene::lookForOut(BlockItem *block, string name)
{
    for(auto item: block->childItems())
    {
        if(dynamic_cast<Out*>(item))
        {
            if(dynamic_cast<Out*>(item)->toPlainText().toStdString() == name)
            {
                return dynamic_cast<Out*>(item);
            }
        }
    }
    return nullptr;
}

void scene::addConnection(BlockItem *auxDst, BlockItem *auxSrc, In *end, Out *start, LineConnection *line)
{
    /*qDebug() << "Adding";
    debugConnections();*/
    line->getConnectionPtr()->set_in(start->toPlainText().toStdString());
    line->getConnectionPtr()->set_out(end->toPlainText().toStdString());
    line->getConnectionPtr()->change_input_ptr(map.getBlockByIndex(auxSrc->getBlockPtr()->get_id()));
    line->getConnectionPtr()->change_output_ptr(map.getBlockByIndex(auxDst->getBlockPtr()->get_id()));
    line->setParentItem(start);
    line->setLine(QLineF(line->line().p1()+QPoint(3,3), end->scenePos() - start->scenePos()));
    line->setPen(QPen({Qt::black}, 3));
    connections.push_back(std::unique_ptr<LineConnection>(line));
    line = nullptr;
    /*qDebug() << "Added";
    debugConnections();*/
}

void scene::createConnections(std::vector<Connectives> conns)
{
    /*qDebug() << "First state";
    debugConnections();*/
    for(auto &conn: conns)
    {
        if(conn.idDst!=-1 && conn.idSrc!=-1)
        {
            bool endB=false, startB=false;
            BlockItem *aux, *auxDst=nullptr, *auxSrc=nullptr;
            In *end=nullptr;
            Out *start=nullptr;
            LineConnection *line=nullptr;
            for(auto item: this->items())
            {
                if(dynamic_cast<BlockItem*>(item))
                {
                    aux = dynamic_cast<BlockItem*>(item);
                    if(aux->getBlockPtr()->get_id()==conn.idDst)
                    {
                        auxDst = aux;
                        end = lookForIn(aux, conn.nameDst);
                        if(end)
                        {
                           if(line)
                           {
                               endB = true;
                               break;
                           }
                           line = new LineConnection(end, lastId++);
                        }
                    }
                    if(aux->getBlockPtr()->get_id()==conn.idSrc)
                    {
                        auxSrc = aux;
                        start = lookForOut(aux, conn.nameSrc);
                        if(start)
                        {
                            if(line)
                            {
                                startB = true;
                                break;
                            }
                            line = new LineConnection(start, lastId++);
                        }
                    }
                }
            }
            if(startB || endB)
            {
                addConnection(auxDst, auxSrc, end, start, line);
            }
            else
            {
                delete line;
            }
        }
    }
}

void scene::debugConnections()
{
    int i = 0;
    for(auto &conn: connections)
    {
        qDebug() << "Connection: " << i;
        if(conn->getConnectionPtr()->get_input())
        {
            qDebug() << "Vstup: " << conn->getConnectionPtr()->get_input()->get_id();
        }
        else
        {
            qDebug() << "Vstup: -1";
        }
        if(conn->getConnectionPtr()->get_output())
        {
            qDebug() << "Vystup: " << conn->getConnectionPtr()->get_output()->get_id();
        }
        else
        {
            qDebug() << "Vystup: " << -1;
        }
        qDebug();
        i++;
    }
}
