/**
  @file mainwindow.h
  @author David Hudak
  @brief File contains header for mainwindow, some stuff generated
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <classes.h>
#include <QDebug>
#include <QStringListModel>
#include <QString>
#include <QItemSelection>
#include <algorithm>
#include <vector>
#include "scene.h"
#include <QGraphicsLineItem>
#include "blockitem.h"
#include "helpwindow.h"
#include <QFileDialog>
#include <auxilaryfunctions.h>
#include <cmath>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
  * @brief The MainWindow class contains partly generated code for main window. Also contains all important slots and auxilary functions.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

    /**
      * Slots for buttons.
      */
private slots:
    /**
     * @brief compileButton calls functions of scene to compile file.
     */
    void compileButton();
    /**
     * @brief cleanBoard clears all stuff from scene.
     */
    void cleanBoard();
    /**
     * @brief createNewBlock
     */
    void createNewBlock();
    /**
     * @brief setName Works as slot for listView. When user changes selected block, all the stuff is done here.
     * @param index Variable from signal of listView
     */
    void setName(QModelIndex index);
    /**
     * @brief deleted Function deletes block from block library.
     */
    void deleted();
    /**
     * @brief edit Function edits block in block library.
     */
    void edit();
    /**
     * @brief addOutput function adds output to block in block library.
     */
    void addOutput();
    /**
     * @brief addInput function adds input to block in block library.
     */
    void addInput();
    /**
     * @brief removeOutputInput function removes output or input from block in block library.
     */
    void removeOutputInput();
    /**
     * @brief comboChange function serves combo box in block editor.
     */
    void comboChange();
    /**
     * @brief zoomPlus function zooms scene.
     */
    void zoomPlus();
    /**
     * @brief zoomMinus function zooms scene.
     */
    void zoomMinus();
    /**
     * @brief addBlock Function adds BlockItem to scene and Block to block map
     */
    void addBlock();
    /**
     * @brief selectSlot Function planned but deleted, because of different functionality.
     */
    void selectSlot();
    /**
     * @brief addCategory Function planned, but not used. Categorisation works differently.
     */
    void addCategory();
    /**
     * @brief compositeBlock Function creates composite block from map.
     */
    void compositeBlock();
    /**
     * @brief help function Function creates modal window of help.
     */
    void help();
    /**
     * @brief saveMap Slot saves composite block to file.
     */
    void saveMap();
    /**
     * @brief loadMap Slot loads composite block from file.
     */
    void loadMap();



public:
    /**
     * @brief MainWindow Constructor for main window
     * @param parent parent of MainWindow, expected nullptr
     */
    MainWindow(QWidget *parent = nullptr);
    /**
      * @brief destructor for MainWindow, deletes all objects.
      */
    ~MainWindow();

private:
    Ui::MainWindow *ui; /**< Generated*/
    bool really = false; /**< Variable for confirming choice of deleting block or cleaning board (with really2)**/
    bool really2 = false;
    Libs *library; /**< Contains block library from file library.txt**/
    QStringListModel *model; /**< Model containing strings of listView*/
    QStringList list; /**< List for model containing strings of listView*/
    int empty = 0; /**< Number for generating new blocks*/
    ifstream file; /**< Auxilary variable for reading library*/
    std::vector<string> categories; /**< String containing current categories */
    std::vector<Block_identity> structs; /**< */
    /**
     * @brief loadData Function loads data from library and changes listView
     */
    void loadData(); /**< Function loads data from library and changes listView */
    int id = 1; /**< Maximum id in library*/
    int currentSet = -1; /**< row of selected item in listView*/
    /**
     * @brief actualizeCombo Function actualizes combo box of names and inputs/outputs in GUI.
     */
    void actualizeCombo();
    scene *myscene; /**< Variable contains graphics scene */
    std::vector<BlockItem*> polygons; /**< Vector of current BlockItems on scene */
    std::vector<bool> isCategoryOnIndex; /**< Vector serves as informational for listView -> when selected some index and on this index in this
        vector is true value, program serves it as cattegory (instead of block from library)*/
    Block *selectedBlock = nullptr; /**< Variable for selected block => when expanding, this should be used for more stuff */
    //QGraphicsPolygonItem *last; // Deleted var, irrelevant
    int blockId = 0; /**< Maximal index of block at scene*/

    /**
     * @brief printBlocks Function prints blocks to map. (Partially functional - blocks arent in suspected order)
     * @param start Start blocks, should be printed first.
     * @param middle Middle blocks, should be printed in the middle.
     * @param end End blocks, should be printed last.
     * @param otherBlocks Application blocks, can be printed anywhere.
     * @param conns Pointer to connections -> will be changed after procedure - blocks are changing indexes on map due to removing duplicities.
     */
    void printBlocks(std::vector<Block> start, std::vector<Block> middle, std::vector<Block> end,
                     std::vector<Block> otherBlocks, std::vector<Connectives> *conns);

};
#endif // MAINWINDOW_H
