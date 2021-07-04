/**
  @file mainwindow.cpp
  @author David Hudak
  @brief File contains implementation of mainwindow class methods, slots etc.
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Calling some constructors, downloading library etc. etc.
    ui->setupUi(this);
    setWindowTitle ("Block Editor");
    model = new QStringListModel(this);
    library = new Libs();
    ifstream file;
    file.open("library.txt");
    library->download_lib(file);
    file.close();
    loadData();

    myscene = new scene(ui->graphicsView);
    ui->graphicsView->setScene(myscene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);

    // Creating connections between signals and slots of UI. Nothing really special.
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(compileButton()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(cleanBoard()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(createNewBlock()));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(setName(QModelIndex)));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(deleted()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(edit()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(addInput()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(addOutput()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(removeOutputInput()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(zoomPlus()));
    connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(zoomMinus()));
    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(addBlock()));
    connect(ui->graphicsView->scene(), SIGNAL(selectionChanged()), this, SLOT(selectSlot()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(loadMap()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveMap()));
}

void MainWindow::printBlocks(std::vector<Block> start, std::vector<Block> middle,
                             std::vector<Block> end, std::vector<Block> otherBlocks, std::vector<Connectives> *conns)
{
    qreal moveX = 0;
    qreal moveY = 50.;
    int counter = 0;
    std::vector<Block> aux;
    for(auto &block: start) // Push all block to "ordered" vector
    {
        aux.push_back(block);
    }
    for(auto &block: middle)
    {
        aux.push_back(block);
    }
    for(auto &block: end)
    {
        aux.push_back(block);
    }
    for(auto &block: otherBlocks)
    {
        aux.push_back(block);
    }
    // Aux vector = start vectors, middle vectors, end vectors and other vectors (applications).
    int maxLim = (int) floor(sqrt((double)aux.size()));
    for(__SIZE_TYPE__ i = 0; i<aux.size(); i++)
    {
        Block block = aux[i];
        auto *polygon = new BlockItem(nullptr, &block, blockId++);
        for(__SIZE_TYPE__ i = 0; i<conns->size(); i++)
        {
            if(block.get_id() == (*conns)[i].idDst) // remapping old connections to new IDs
            {
                (*conns)[i].idDst = blockId - 1;
            }
            if(block.get_id() == (*conns)[i].idSrc)
            {
                (*conns)[i].idSrc = blockId - 1;
            }
        }
        myscene->addItem(polygon); // Important to add block to scene and to Block Map
        myscene->addBlock(polygon);
        polygon->moveIt(moveX, moveY);
        moveY += block.get_inputs().size() > block.get_outputs().size() ?
                    30.0*(block.get_inputs().size()+1) : 30.0*(1+block.get_outputs().size());
        polygons.push_back(polygon);
        counter++;
        if(counter>maxLim)
        {
            counter = 0;
            moveY = 50.;
            moveX += 200;
        }
    }
}


void MainWindow::loadMap()
{
    QString fileNames = QFileDialog::getOpenFileName(this, tr("Load Composite Block"),"/path/to/file/",tr("All Files (*)"));
    std::vector<Block> blocks;
    std::vector<Connectives> conns;
    int err = AuxilaryFunctions::loadMap(fileNames, &blocks, &conns);
    if(err != OK)
    {
        ui->label->setText("Loading failed -- file corrupted.");
        return;
    }
    std::vector<Block> startBlocks;
    std::vector<Block> endBlocks;
    std::vector<Block> otherBlocks;
    std::vector<Block> midBlocks;
    startBlocks = AuxilaryFunctions::getStartBlocks(blocks, conns); // Separate loaded blocks.
    endBlocks = AuxilaryFunctions::getEndBlocks(blocks, conns);
    midBlocks = AuxilaryFunctions::getMiddleBlocks(blocks, conns);
    otherBlocks = AuxilaryFunctions::getOtherBlocks(blocks, conns);
    /*qDebug() << "Before connections";
    for(auto conn: conns)
    {
        qDebug() << conn.idDst << " " << conn.idSrc;
    }
    myscene->debugConnections();*/
    printBlocks(startBlocks, midBlocks, endBlocks, otherBlocks, &conns);
    myscene->createConnections(conns);
    selectedBlock = nullptr;

}

void MainWindow::saveMap()
{
    QString fileNames = QFileDialog::getSaveFileName(this, tr("Save Composite Block"),"/path/to/file/",tr("All Files (*)"));
    Block_map *map = myscene->getMap();
    std::vector<Block> blocks = map->getBlocks();
    std::vector<Connectives> conns = myscene->createConnectives();
    AuxilaryFunctions::saveMap(fileNames, blocks, conns);
}

MainWindow::~MainWindow()
{
    library->save_lib("library.txt");
    delete model;
    delete library;
    delete ui;
    delete selectedBlock;
    delete myscene;

}

void MainWindow::help()
{
    HelpWindow window;
    window.setModal(true);
    window.exec();
}

void MainWindow::addCategory()
{
    // Hello :)
}

void MainWindow::compositeBlock()
{
    int compositeIndex=0;
    while(list.indexOf("composite" + QString::fromStdString((to_string(compositeIndex))))!=-1)
    { // Looking for free composite name
        compositeIndex++;
    }
    std::vector<Block> blocks = library->return_blocks();
    int maxId = blocks[0].get_id();
    for(__SIZE_TYPE__ i = 1; i < blocks.size(); i++) // Looking for free id to insert.
    {
        if(maxId < blocks[i].get_id())
        {
            maxId = blocks[i].get_id();
        }
    }
    Block newBlock;
    if(OK == myscene->returnCompositeBlock(maxId+1, "composite" + to_string(compositeIndex), &newBlock))
    {
        library->add_block(newBlock);
        loadData();
    }
    else
    {
        ui->label->setText("Some internal error.");
    }
}

void MainWindow::selectSlot()
{
    /*for(auto *select: myscene->selectedItems())
    {
        qDebug() << "Selected\n";
        last = dynamic_cast<QGraphicsPolygonItem*>(select);
        if(last != nullptr)
            return;

        auto item = select->parentItem();
        qDebug() << select->parentItem();
        myscene->removeItem(item);
        for(auto *selection: item->childItems())
        {
            selection->setSelected(true);
        }
        for(auto *selection: select->childItems())
        {
            selection->setSelected(true);
        }
        item->setSelected(true);
    }*/
}

void MainWindow::zoomPlus()
{
    ui->graphicsView->scale(1.25, 1.25);
}

void MainWindow::zoomMinus()
{
    ui->graphicsView->scale(0.8, 0.8);
}

void MainWindow::addBlock()
{
    if(selectedBlock == nullptr)
    {
        ui->label->setText("You have to set block.");
        return;
    }
    auto *polygon = new BlockItem(nullptr, selectedBlock, blockId++);
    polygons.push_back(polygon);
    myscene->addItem(polygon); // Important to add block to scene and to Block Map
    myscene->addBlock(polygon);
}

void MainWindow::comboChange()
{
    ui->pushButton_8->setEnabled(false);
    Block block;
    int find = library->return_block(structs[currentSet].id, &block);
    if(find == OK) // Changing of block editor data due to combo box selected item
    {
        if(ui->comboBox->currentText() == "Name") // Selected name
        {
            ui->label->setText("All ok");
            ui->pushButton_8->setEnabled(false);
            ui->label_4->setText("Block code");
            ui->lineEdit->setText(QString::fromStdString(block.get_name()));
            ui->plainTextEdit->setPlainText(QString::fromStdString(block.get_code()));
        }
        else
        {
            ui->pushButton_8->setEnabled(true);
            for(__SIZE_TYPE__ i = 0; i<block.get_inputs().size(); i++) // Looking for input by name
            {
                if(block.get_inputs()[i].get_name() == ui->comboBox->currentText().toStdString())
                {
                    ui->pushButton_8->setText("Remove input");
                    ui->label_4->setText("Input type");
                    ui->label->setText("All ok");
                    ui->lineEdit->setText(QString::fromStdString(block.get_inputs()[i].get_name()));
                    ui->plainTextEdit->setPlainText(QString::fromStdString(block.get_inputs()[i].get_type()));
                    return;
                }
            }
            for(__SIZE_TYPE__ i = 0; i<block.get_outputs().size(); i++) // Looking for output by name (no by input found)
            {
                if(block.get_outputs()[i].get_name() == ui->comboBox->currentText().toStdString())
                {
                    ui->pushButton_8->setText("Remove output");
                    ui->label_4->setText("Output type");
                    ui->label->setText("All ok");
                    ui->lineEdit->setText(QString::fromStdString(block.get_outputs()[i].get_name()));
                    ui->plainTextEdit->setPlainText(QString::fromStdString(block.get_outputs()[i].get_type()));
                    return;
                }
            }
            ui->label->setText("Internal error -- value does not exist");
        }
    }
}

void MainWindow::actualizeCombo()
{
    Block editBlock;
    int err=library->return_block(structs[currentSet].id, &editBlock);
    if(err == OK) // Downloaded block from library, now add items to combobox
    {
        ui->comboBox->clear();
        ui->comboBox->insertItem(1, "Name");
        __SIZE_TYPE__ i;
        for(i = 0; i<editBlock.get_inputs().size(); i++)
        { // Inserting inputs to combo box
            ui->comboBox->insertItem(i+2, QString::fromStdString(editBlock.get_inputs()[i].get_name()));
        }
        int j = i;
        for(i = 0; i<editBlock.get_outputs().size(); i++)
        { // Inserting outputs to combo box
            ui->comboBox->insertItem(j+i+2, QString::fromStdString(editBlock.get_outputs()[i].get_name()));
        }
    }
}

void MainWindow::addOutput()
{
    std::vector<In_out> vec;
    Block editBlock;
    int err=library->return_block(structs[currentSet].id, &editBlock); // Download block by id
    if(err == OK)
    {
        string name = "out";
        int empty = 0;
        while(OK != editBlock.can_output(name + to_string(empty)) || OK != editBlock.can_input(name + to_string(empty)))
        { // Looking for name, which doesn't exist yet
            empty++;
        }
        name = name + to_string(empty);
        In_out pom;
        pom.set_name(name);
        pom.set_output(true);
        pom.set_input(false);
        pom.set_type("NOTYPE");
        vec.push_back(pom);
        editBlock.add_output(vec);
        selectedBlock->add_output(vec); // Adding new output to block
        library->actualize_block(editBlock);
        actualizeCombo();
    }
    else
    {
        ui->label->setText("Internal error");
    }
}

void MainWindow::addInput()
{ // Same as function before, but with input
    std::vector<In_out> vec;
    Block editBlock;
    int err=library->return_block(structs[currentSet].id, &editBlock);
    if(err == OK)
    {
        string name = "in";
        int empty = 0;
        while(OK != editBlock.can_output(name + to_string(empty)) || OK != editBlock.can_input(name + to_string(empty)))
        {
            empty++;
        }
        name = name + to_string(empty);
        In_out pom;
        pom.set_name(name);
        pom.set_output(false);
        pom.set_input(true);
        pom.set_type("NOTYPE");
        vec.push_back(pom);
        editBlock.add_input(vec);
        selectedBlock->add_input(vec);
        library->actualize_block(editBlock);
        actualizeCombo();
    }
    else
    {
        ui->label->setText("Internal error");
    }
}

void MainWindow::removeOutputInput()
{
    Block block;
    int find = library->return_block(structs[currentSet].id, &block);
    if(find != OK)
    {
        ui->label->setText("Removing input/output wasn't succesfull");
    }
    for(__SIZE_TYPE__ i = 0; i<block.get_inputs().size(); i++)
    { // Looking if the item is input or output.
        if(block.get_inputs()[i].get_name() == ui->comboBox->currentText().toStdString())
        { // input
            block.remove_input((int)i);
            selectedBlock->remove_input((int)i);
            ui->label->setText("Input removed");
            library->actualize_block(block);
            actualizeCombo();
            return;
        }
    }
    for(__SIZE_TYPE__ i = 0; i<block.get_outputs().size(); i++)
    { // output
        if(block.get_outputs()[i].get_name() == ui->comboBox->currentText().toStdString())
        {
            block.remove_output((int)i);
            selectedBlock->remove_output((int)i);
            ui->label->setText("Output removed");
            library->actualize_block(block);
            actualizeCombo();
            return;
        }
    }
    ui->label->setText("Internal error"); // Wrong situation.

}

void MainWindow::edit()
{
    Block editBlock;
    int find=library->return_block(structs[currentSet].id, &editBlock);
    if(find != OK) // No block found
    {
        ui->label->setText("Cannot edit block, some internal error.");
        return;
    }
    editBlock.set_category(ui->lineEdit_2->text().toStdString());
    if(ui->comboBox->currentText() == "Name")
    { // Edited part is code and name
        string newName = ui->lineEdit->text().toStdString();
        string newCode = ui->plainTextEdit->toPlainText().toStdString();
        if(find == OK)
        {
            editBlock.set_name(newName); // Some weird stuff to use Stefan's interface.
            editBlock.remove_code();
            editBlock.add_code(newCode);
            library->actualize_block(editBlock);
            ui->label->setText("Block edited");
            emit ui->listView->clicked(ui->listView->currentIndex());
            loadData();
        }
    } // End of name if
    else
    { // Selected from combo box is not name, but some input/output
        bool output = true;
        bool found = false;
        int index = -1;

        for(__SIZE_TYPE__ i = 0; i<editBlock.get_inputs().size(); i++)
        { // Go through inputs and look for the name
            if(editBlock.get_inputs()[i].get_name() == ui->comboBox->currentText().toStdString())
            {
                index = (int) i;
                output = false;
                found = true;
            }
            if(ui->lineEdit->text().toStdString() != ui->comboBox->currentText().toStdString() &&
                    editBlock.get_inputs()[i].get_name() == ui->lineEdit->text().toStdString())
            { // Cannot have two same names
                ui->label->setText("Name already exists");
                return;
            }
        }

        for(__SIZE_TYPE__ i = 0; i<editBlock.get_outputs().size(); i++)
        { //  Go through outputs and look for the name
            if(editBlock.get_outputs()[i].get_name() == ui->comboBox->currentText().toStdString())
            {
                index = (int) i;
                output = true;
                found = true;
            }
            if(ui->lineEdit->text().toStdString() != ui->comboBox->currentText().toStdString() &&
                    editBlock.get_outputs()[i].get_name() == ui->lineEdit->text().toStdString())
            { // Cannot have two same names
                ui->label->setText("Name already exists");
                return;
            }
        }

        std::vector<In_out> vec;
        if(found) // Input or output found
        {

            if(output) // Found output
            {
                In_out out = editBlock.get_outputs()[index];
                out.set_name(ui->lineEdit->text().toStdString());
                out.set_type(ui->plainTextEdit->toPlainText().toStdString());
                out.set_input(false);
                out.set_output(true);
                editBlock.remove_output(index);
                vec.push_back(out);
                editBlock.add_output(vec);
            }
            else // Found input
            {
                In_out in = editBlock.get_inputs()[index];
                in.set_name(ui->lineEdit->text().toStdString());
                in.set_type(ui->plainTextEdit->toPlainText().toStdString());
                in.set_input(true);
                in.set_output(false);
                editBlock.remove_input(index);
                vec.push_back(in);
                editBlock.add_input(vec);
            }
            library->actualize_block(editBlock);
            actualizeCombo();
            ui->label->setText("Successfully edited");
        }
        else // Not found something what should be found
        {
            ui->label->setText("Internal error");
        }
        emit ui->listView->clicked(ui->listView->currentIndex());
        loadData();
    }

    // Disable all buttons etc.
    ui->plainTextEdit->setPlainText("This is category name.");
    ui->lineEdit->setText("");
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_2->setText("Change name in cattegories.");
    ui->comboBox->setEnabled(false);
    ui->plainTextEdit->setReadOnly(true);
    ui->lineEdit->setReadOnly(true);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    selectedBlock = nullptr;

}

void MainWindow::deleted()
{
    if(really2)
    { // double clicked on delete button
        if(currentSet != -1)
        {
            if(library->remove_block(structs[currentSet].id) == OK)
            {
                currentSet = -1;
                ui->label->setText("Block deleted");
            }
            {
                ui->label->setText("Internal error, block wasn't deleted.");
            }

            loadData();
        }
        else
        {
            ui->label->setText("You have to choose some block.");
        }
        this->really2 = false;
    }
    else
    {
        this->really2 = true;
        ui->label->setText("Do you really want to delete this block?");
    }
}

void MainWindow::loadData()
{
    library->save_lib("library.txt");
    list.clear();
    structs = library->return_identity(library->return_blocks());
    categories.clear();
    isCategoryOnIndex.clear();
    for(__SIZE_TYPE__ i = 0; i<structs.size(); i++)
    { //// Looking for all unique categories in blocks
        if(std::find(categories.begin(), categories.end(), structs[i].category) == categories.end())
        {
            categories.push_back(structs[i].category);
        }
    }
    QStringList pom;
    for(__SIZE_TYPE__ i = 0; i<categories.size(); i++)
    { //// Looking for blocks by categories (divides vector to lists)
        isCategoryOnIndex.push_back(true);
        list << QString::fromStdString("--- "+categories[i]+" ---"); // add category name
        pom.clear();
        for(__SIZE_TYPE__ j = 0; j<structs.size(); j++)
        { //// Goes through vector with names and categories connected
            if(categories[i] == structs[j].category)
            {
                isCategoryOnIndex.push_back(false);
                pom << QString::fromStdString(structs[j].name);
            }
        }
        pom.sort(); // sort sublist
        list << pom;
    }
    model->setStringList(list);
    ui->listView->setModel(model);
}
void MainWindow::compileButton()
{
    int err;
    myscene->addConnections(); // At first, it is needed to add connections to map
    if((err = myscene->getMap()->compile()) == OK)
    {
        ui->label->setText("Program compiled to file code.cpp");
    }
    else
    {
        ui->label->setText("Something failed, err code: " + QString::fromStdString(to_string(err)));
    }
    myscene->getMap()->clearConnections(); // Implementation detail - I delete all connections from map because of difficult of changes.

}

void MainWindow::cleanBoard()
{
    if(really)
    { // double click on clear button
        this->really = false;
        myscene->clearAll();
        myscene->clear();
        ui->label->setText("Cleared");
    }
    else
    {
        this->really = true;
        ui->label->setText("Do you really want to delete all your progress?");
    }
}

void MainWindow::createNewBlock()
{
    ui->label->setText("New empty block created.");
    while(list.indexOf("empty" + QString::fromStdString((to_string(empty))))!=-1)
    { // Looking for empty name
        empty++;
    }
    Block *newBlock = new Block("empty" + to_string(empty), 1);
    while(DUPLICATE_ID == library->add_block(*newBlock))
    { // Looking for empty ID
        id++;
        newBlock->set_id(id);
    }
    delete newBlock;
    loadData();
}

void MainWindow::setName(QModelIndex index)
{
    QString itemText = index.data(Qt::DisplayRole).toString();
    ui->label_3->setText(itemText);
    __SIZE_TYPE__ i;
    if(isCategoryOnIndex[index.row()] == true)
    { // Clicked on category, turn it all off
        ui->plainTextEdit->setPlainText("This is category name.");
        ui->lineEdit->setText(itemText);
        ui->lineEdit_2->setReadOnly(true);
        ui->lineEdit_2->setText("Change name in cattegories.");
        ui->comboBox->setEnabled(false);
        ui->plainTextEdit->setReadOnly(true);
        ui->lineEdit->setReadOnly(true);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
        selectedBlock = nullptr;
        return;
    }
    ui->comboBox->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    for(i = 0; i<structs.size(); i++)
    { // Looks where is the object in vector
        if(structs[i].name == itemText.toStdString())
        {
            break;
        }
    }
    if(i<structs.size())
    { // If found object
        currentSet = i;
        Block block;
        int find = library->return_block(structs[i].id, &block); // Find block from library
        if(find == OK)
        {
            if(selectedBlock != nullptr)
            { // Block was selected, but I create copy, so it's important to delete it.
                delete selectedBlock;
            }
            selectedBlock = new Block(block);
            actualizeCombo();
            ui->comboBox->setCurrentIndex(0);
            ui->lineEdit->setText(QString::fromStdString(block.get_name()));
            ui->plainTextEdit->setPlainText(QString::fromStdString(block.get_code()));
            ui->lineEdit_2->setText(QString::fromStdString(block.get_category()));
            if(block.is_built_in())
            { // Block is built-in -- ban editing
                ui->plainTextEdit->setReadOnly(true);
                ui->lineEdit->setReadOnly(true);
                ui->lineEdit_2->setReadOnly(true);
                ui->pushButton_4->setEnabled(false);
                ui->pushButton_5->setEnabled(false);
                ui->pushButton_6->setEnabled(false);
                ui->pushButton_7->setEnabled(false);
            }
            else
            { // Block is not built-in
                ui->plainTextEdit->setReadOnly(false);
                ui->lineEdit->setReadOnly(false);
                ui->lineEdit_2->setReadOnly(false);
                ui->pushButton_4->setEnabled(true);
                ui->pushButton_5->setEnabled(true);
                ui->pushButton_6->setEnabled(true);
                ui->pushButton_7->setEnabled(true);
            }
        } // End if

    } // End if

}

