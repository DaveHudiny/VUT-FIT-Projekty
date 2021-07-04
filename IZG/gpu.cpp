/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <cassert>
#include <iostream>
#include <cstdint>
#include <cstring>


using namespace std;



/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
    framebuffer = nullptr;


}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU() {
    /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
    deleteFramebuffer();
    while (bufferArray.size()>0)
    {
        deleteBuffer(bufferArray[0]->ID);
    }
    while (vertexArray.size()>0)
    {
        deleteVertexPuller(vertexArray[0]->vertex_ID);
    }
    while (programArray.size()>0)
    {
        deleteProgram(programArray[0]->ID);
    }
}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>
    Buffer *novy = new Buffer();
    if (novy == nullptr)
    {
        return emptyID;
    }
    BufferCount++;
    original++;
    novy->ID = original;
    novy->size = size;
    novy->data =::operator new(size);
    if (novy->data == nullptr)
    {
        delete novy;
        return emptyID;
    }
    bufferArray.push_back(novy);
    return novy->ID;
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.
    for (unsigned i = 0; i < bufferArray.size(); i++)
    {
        if (bufferArray[i]->ID == buffer)
        {
            Buffer *del = bufferArray[i];
            delete (char *)bufferArray[i]->data;

            bufferArray.erase(bufferArray.begin()+i);
            delete del;
            return;
        }
    }
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>
    unsigned int i=0;
    while (i < bufferArray.size() && bufferArray[i]->ID != buffer)
    {
        i++;
    }
    if (i < bufferArray.size())
    {
        if (bufferArray[i]->size < (size + offset))
        {
            return;
        }
        memcpy((char *)bufferArray[i]->data + offset, data, size);
    }
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>
    unsigned i = 0;
    while (i < BufferCount && bufferArray[i]->ID != buffer)
    {
        i++;
    }
    if (i < BufferCount)
    {
        if (bufferArray[i]->size < (size + offset))
        {
            return;
        }
        memcpy(data, (char*)bufferArray[i]->data + offset, size);
        return;
    }
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>
    if (buffer == emptyID)
    {
        return false;
    }
    int i = 0;
    while (i < bufferArray.size() && bufferArray[i]->ID != buffer)
    {
        i++;
    }
    if (i < bufferArray.size())
    {
        return true;
    }
    return false;
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.
    VertexPuller *novy = new VertexPuller();
    if (novy == nullptr)
    {
        return emptyID;
    }
    VertexOriginal++;
    novy->vertex_ID = VertexOriginal;
    novy->heads = new Heads[maxAttributes];
    novy->Indexing = false;
    for (int i = 0; i < maxAttributes; i++)
    {
        novy->heads[i].enabled = false;
    }
    vertexArray.push_back(novy);
    return novy->vertex_ID;
}

/**
 * @brief This function deletes vertex puller settings
 *
 * @param vao vertex puller identificator
 */
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        VertexPuller* del = vertexArray[i];
        delete[] vertexArray[i]->heads;
        vertexArray.erase(vertexArray.begin() + i);
        delete del;
        return;
    }
}

/**
 * @brief This function sets one vertex puller reading head.
 *
 * @param vao identificator of vertex puller
 * @param head id of vertex puller head
 * @param type type of attribute
 * @param stride stride in bytes
 * @param offset offset in bytes
 * @param buffer id of buffer
 */
void GPU::setVertexPullerHead(VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        vertexArray[i]->heads[head].type = type;
        vertexArray[i]->heads[head].stride=stride;
        vertexArray[i]->heads[head].offset=offset;
        vertexArray[i]->heads[head].ID = buffer;
    }
}

/**
 * @brief This function sets vertex puller indexing.
 *
 * @param vao vertex puller id
 * @param type type of index
 * @param buffer buffer with indices
 */
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        vertexArray[i]->buffer_ID = buffer;
        vertexArray[i]->Type = type;
        vertexArray[i]->Indexing = true;
    }
}

/**
 * @brief This function enables vertex puller's head.
 *
 * @param vao vertex puller 
 * @param head head id
 */
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        vertexArray[i]->heads[head].enabled = true;
    }
}

/**
 * @brief This function disables vertex puller's head
 *
 * @param vao vertex puller id
 * @param head head id
 */
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        vertexArray[i]->heads[head].enabled = false;
    }
}

/**
 * @brief This function selects active vertex puller.
 *
 * @param vao id of vertex puller
 */
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        bindedVertexPuller = vao;
    }
}

/**
 * @brief This function deactivates vertex puller.
 */
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.
    bindedVertexPuller = emptyID;
}

/**
 * @brief This function tests if vertex puller exists.
 *
 * @param vao vertex puller
 *
 * @return true, if vertex puller "vao" exists
 */
bool     GPU::isVertexPuller         (VertexPullerID vao){
  /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
  /// Pokud ano, funkce vrací true.
    if (emptyID == vao)
    {
        return false;
    }
    unsigned int i = 0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != vao)
    {
        i++;
    }
    if (i < vertexArray.size())
    {
        return true;
    }
    return false;
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>
    Program *novy = new Program();
    if (novy == nullptr)
    {
        return emptyID;
    }
    novy->ID = ProgramOriginal;
    ProgramOriginal++;
    novy->fragmentShader = nullptr;
    novy->vertexShader = nullptr;
    for (unsigned i = 0; i < maxAttributes; i++)
    {
        novy->type[i] = AttributeType::EMPTY;
    }
    programArray.push_back(novy);
    return novy->ID;
}

/**
 * @brief This function deletes shader program
 *
 * @param prg shader program id
 */
void             GPU::deleteProgram         (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        Program *del = programArray[i];
        delete[] del->type;
        programArray.erase(programArray.begin() + i);
        delete del;
    }
}

/**
 * @brief This function attaches vertex and frament shader to shader program.
 *
 * @param prg shader program
 * @param vs vertex shader 
 * @param fs fragment shader
 */
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->vertexShader = vs;
        programArray[i]->fragmentShader = fs;
    }
}

/**
 * @brief This function selects which vertex attributes should be interpolated during rasterization into fragment attributes.
 *
 * @param prg shader program
 * @param attrib id of attribute
 * @param type type of attribute
 */
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->type[attrib] = type;
    }
}

/**
 * @brief This function actives selected shader program
 *
 * @param prg shader program id
 */
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        activeProgram = prg;
    }
}

/**
 * @brief This function tests if selected shader program exists.
 *
 * @param prg shader program
 *
 * @return true, if shader program "prg" exists.
 */
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>
    if (prg == emptyID)
    {
        return false;
    }
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        return true;
    }
  return false;
}

/**
 * @brief This function sets uniform value (1 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void GPU::programUniform1f(ProgramID prg, uint32_t uniformId, float const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->attributes.uniform[uniformId].v1 = d;
    }
}

/**
 * @brief This function sets uniform value (2 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->attributes.uniform[uniformId].v2 = d;
    }
}

/**
 * @brief This function sets uniform value (3 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->attributes.uniform[uniformId].v3 = d;
    }
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->attributes.uniform[uniformId].v4 = d;
    }
}

/**
 * @brief This function sets uniform value (4 float).
 *
 * @param prg shader program
 * @param uniformId id of uniform value (number of uniform values is stored in maxUniforms variable)
 * @param d value of uniform variable
 */
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).
    unsigned int i = 0;
    while (i < programArray.size() && programArray[i]->ID != prg)
    {
        i++;
    }
    if (i < programArray.size())
    {
        programArray[i]->attributes.uniform[uniformId].m4= d;
    }
}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>
    framebuffer = new frameBuffer();
    framebuffer->height = height;
    framebuffer->width = width;
    framebuffer->colorPixels = new uint8_t[(width * height * 4)+10];
    framebuffer->depthBuffer = new float[width * height+1];
    clear(0, 0, 0, 1);
}

/**
 * @brief This function deletes framebuffer.
 */
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.
    if (framebuffer != nullptr)
    {
        if (framebuffer->colorPixels != nullptr)
        {
            uint8_t* pomu;
            pomu = framebuffer->colorPixels;
            delete[] pomu;
            framebuffer->colorPixels = nullptr;
        }
        if (framebuffer->depthBuffer != nullptr)
        {
            float* pom;
            pom = framebuffer->depthBuffer;
            delete[] pom;
            framebuffer->depthBuffer = nullptr;
        }
        frameBuffer *temp;
        temp = framebuffer;
        delete temp;
        framebuffer = nullptr;
    }
}

/**
 * @brief This function resizes framebuffer.
 *
 * @param width new width of framebuffer
 * @param height new heght of framebuffer
 */
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla změnit velikost framebuffer.
    framebuffer->height = height;
    framebuffer->width = width;
    uint8_t* colorPixel = new uint8_t[10+((uint32_t)width * (uint32_t)height * 4)];
    delete[] framebuffer->colorPixels;
    framebuffer->colorPixels = colorPixel;
    float* depthBuffer = new float[width * height+1];
    delete[] framebuffer->depthBuffer;
    framebuffer->depthBuffer = depthBuffer;

}

/**
 * @brief This function returns pointer to color buffer.
 *
 * @return pointer to color buffer
 */
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
    return framebuffer->colorPixels;
}

/**
 * @brief This function returns pointer to depth buffer.
 *
 * @return pointer to dept buffer.
 */
float* GPU::getFramebufferDepth    (){
  /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
    return framebuffer->depthBuffer;
}

/**
 * @brief This function returns width of framebuffer
 *
 * @return width of framebuffer
 */
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.
  return framebuffer->width;
}

/**
 * @brief This function returns height of framebuffer.
 *
 * @return height of framebuffer
 */
uint32_t GPU::getFramebufferHeight(){
  /// \todo Tato funkce by měla vrátit výšku framebufferu.
  return framebuffer->height;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void            GPU::clear                 (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br>
    if (framebuffer == nullptr)
    {
        return;
    }
    for (uint32_t i = 0; i < framebuffer->width * framebuffer->height * 4; i++)
    {
        framebuffer->colorPixels[i] = (r * 255);
        framebuffer->colorPixels[i + 1] = (g * 255);
        framebuffer->colorPixels[i + 2] = (b * 255);
        framebuffer->colorPixels[i + 3] = (a * 255);
        if (i % 4 == 0)
        {
            getFramebufferDepth()[i / 4] = INFINITY;
        }
    }
}

float GPU::countT(OutVertex first, OutVertex second)
{
    float t = -first.gl_Position[3] - first.gl_Position[2];
    t = t / (second.gl_Position[3] + second.gl_Position[2] - first.gl_Position[3] - first.gl_Position[2]);
    return t;
}

OutVertex GPU::countVertex(float t, OutVertex first, OutVertex second, AttributeType* types)
{
    OutVertex novy;
    for (unsigned i = 0; i < 4; i++)
    {
        novy.gl_Position[i] = first.gl_Position[i] + (t * (second.gl_Position[i] - first.gl_Position[i]));
    }
    for (unsigned i = 0; i < maxAttributes; i++)
    {
        switch (types[i])
        {
            case AttributeType::EMPTY:
                break;
            case AttributeType::FLOAT:
                novy.attributes->v1 = first.attributes->v1 + (t * (second.attributes->v1-first.attributes->v1));
                break;
            case AttributeType::VEC2:
                novy.attributes->v2[0] = first.attributes->v2[0] + (t * (second.attributes->v2[0] - first.attributes->v2[0]));
                novy.attributes->v2[1] = first.attributes->v2[1] + (t * (second.attributes->v2[1] - first.attributes->v2[1]));
                break;
            case AttributeType::VEC3:
                novy.attributes->v3[0] = first.attributes->v3[0] + (t * (second.attributes->v3[0] - first.attributes->v3[0]));
                novy.attributes->v3[1] = first.attributes->v3[1] + (t * (second.attributes->v3[1] - first.attributes->v3[1]));
                novy.attributes->v3[2] = first.attributes->v3[2] + (t * (second.attributes->v3[2] - first.attributes->v3[2]));
                break;
            case AttributeType::VEC4:
                novy.attributes->v4[0] = first.attributes->v4[0] + (t * (second.attributes->v4[0] - first.attributes->v4[0]));
                novy.attributes->v4[1] = first.attributes->v4[1] + (t * (second.attributes->v4[1] - first.attributes->v4[1]));
                novy.attributes->v4[2] = first.attributes->v4[2] + (t * (second.attributes->v4[2] - first.attributes->v4[2]));
                novy.attributes->v4[3] = first.attributes->v4[3] + (t * (second.attributes->v4[3] - first.attributes->v4[3]));
                break;
        }
    }
    return novy;
}

float GPU::countDistance(float x1, float y1, float x2, float y2)
{
    return sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

float GPU::triangleObsah(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
{
    float stranaA = countDistance(Bx, By, Cx, Cy);
    float stranaB = countDistance(Ax, Ay, Cx, Cy);
    float stranaC = countDistance(Ax, Ay, Bx, By);
    float s = (stranaA+stranaB+stranaC)/2;
    return sqrt(s * (s - stranaA) * (s - stranaB) * (s - stranaC));
}

double GPU::findMin(float val1, float val2, float val3)
{
    if (val1 <= val2)
    {
        if (val1 <= val3)
        {
            return val1;
        }
        else
        {
            return val3;
        }
    }
    else
    {
        if (val2 <= val3)
        {
            return val2;
        }
        else
        {
            return val3;
        }
    }
}

double GPU::findMax(float val1, float val2, float val3)
{
    if (val1 >= val2)
    {
        if (val1 >= val3)
        {
            return (double)val1;
        }
        else
        {
            return (double)val3;
        }
    }
    else
    {
        if (val2 >= val3)
        {
            return (double)val2;
        }
        else
        {
            return (double)val3;
        }
    }
}

void            GPU::drawTriangles         (uint32_t  nofVertices){
  /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
  /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
  /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
  /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
    if (nofVertices % 3 != 0 || !isProgram(activeProgram) || framebuffer==nullptr || !isVertexPuller(bindedVertexPuller))
    {
        return;
    }
    
    unsigned i=0;
    while (i < vertexArray.size() && vertexArray[i]->vertex_ID != bindedVertexPuller);
    i = 0;
    VertexPuller *puller = vertexArray[i];
    while (i < programArray.size() && programArray[i]->ID != activeProgram);
    Program* program = programArray[i];

    std::vector<Triangle> primitives;
    primitives.resize(0);
    OutVertex prvni;
    prvni.gl_Position = { 0, 0, 0, 0 };
    OutVertex druhy;
    druhy.gl_Position = { 0, 0, 0, 0 };
    OutVertex treti;
    treti.gl_Position = { 0, 0, 0, 0 };

    for (i = 0; i < nofVertices; i++)
    {
        InVertex novy;
        if (puller->Indexing) // Vybrání typu indexování
        {
            switch (puller->Type)
            {
            case IndexType::UINT8:
                uint8_t nacti8;
                getBufferData(puller->buffer_ID, (uint32_t)i, 1, &nacti8);
                novy.gl_VertexID = (uint32_t)nacti8;
                break;
            case IndexType::UINT16:
                uint16_t nacti16;
                getBufferData(puller->buffer_ID, (uint32_t)(i*2), 2, &nacti16);
                novy.gl_VertexID = (uint32_t)nacti16;
                break;
            case IndexType::UINT32:
                getBufferData(puller->buffer_ID, (uint32_t)(i*4), 4, &novy.gl_VertexID);
                break;
            }
        }
        else
        {
            novy.gl_VertexID = i;
        }

        for (int j=0; j < maxAttributes; j++) // Načtení atributů pro invertexy
        {
            if (!puller->heads[j].enabled)
            {
                continue;
            }
            switch (puller->heads[j].type)
            {
            case AttributeType::EMPTY:
                break;
            case AttributeType::FLOAT:
                getBufferData(puller->heads[j].ID, puller->heads[j].offset + puller->heads[j].stride * novy.gl_VertexID, sizeof(Attribute::v1), &novy.attributes[j].v1);
                break;
            case AttributeType::VEC2:
                getBufferData(puller->heads[j].ID, puller->heads[j].offset + puller->heads[j].stride * novy.gl_VertexID, sizeof(Attribute::v2), &novy.attributes[j].v2);
                break;
            case AttributeType::VEC3:
                getBufferData(puller->heads[j].ID, puller->heads[j].offset + puller->heads[j].stride * novy.gl_VertexID, sizeof(Attribute::v3), &novy.attributes[j].v3);
                break;
            case AttributeType::VEC4:
                getBufferData(puller->heads[j].ID, puller->heads[j].offset + puller->heads[j].stride * novy.gl_VertexID, sizeof(Attribute::v4), &novy.attributes[j].v4);
                break;
            }
        } // Konec načítání atributů ze čtecích hlav
        
        switch (i % 3) // Tvorba vrcholů trojúhelníku
        {
        case 0:
            program->vertexShader(prvni, novy, program->attributes);
            break;
        case 1:
            program->vertexShader(druhy, novy, program->attributes);
            break;
        case 2:
            program->vertexShader(treti, novy, program->attributes);
            break;
        }

        if (i % 3 == 2) // Tvorba trojúhelniku - primitivního
        {
            Triangle trojuhelnik;
            trojuhelnik.vertexes[0] = prvni;
            trojuhelnik.vertexes[1] = druhy;
            trojuhelnik.vertexes[2] = treti;
            primitives.push_back(trojuhelnik);
            //cout << primitives[i / 3].vertexes[0].gl_Position[0] << " " << primitives[i / 3].vertexes[0].gl_Position[1] << " " << primitives[i / 3].vertexes[0].gl_Position[2] << endl;
        }
    }


    std::vector<Triangle> clipTrojuhelniky;

    bool mimoA, mimoB, mimoC;
    int celkemMimo;


    for (i = 0; i < primitives.size(); i++) // clipping
    {
        mimoA = false;
        mimoB = false;
        mimoC = false;
        celkemMimo = 0;
        if (primitives[i].vertexes[0].gl_Position[2] < -primitives[i].vertexes[0].gl_Position[3])
        {
            mimoA = true;
            celkemMimo++;
        }
        if (primitives[i].vertexes[1].gl_Position[2] < -primitives[i].vertexes[1].gl_Position[3])
        {
            mimoB = true;
            celkemMimo++;
        }
        if (primitives[i].vertexes[2].gl_Position[2] < -primitives[i].vertexes[2].gl_Position[3])
        {
            mimoC = true;
            celkemMimo++; 
        }
        if (celkemMimo == 3)
        {
            continue;
        }
        if (celkemMimo == 0)
        {
            clipTrojuhelniky.push_back(primitives[i]);
            continue;
        }
        if (celkemMimo == 1)
        {
            Triangle jeden, dva;
            float t;
            if(mimoA)
            {
                jeden.vertexes[0]= primitives[i].vertexes[1];
                jeden.vertexes[1] = primitives[i].vertexes[2];
                t = countT(primitives[i].vertexes[1], primitives[i].vertexes[0]);
                jeden.vertexes[2] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[0], program->type);
                dva.vertexes[0] = primitives[i].vertexes[2];
                t = countT(primitives[i].vertexes[2], primitives[i].vertexes[0]);
                dva.vertexes[1] = countVertex(t, primitives[i].vertexes[2], primitives[i].vertexes[0], program->type);
                t = countT(primitives[i].vertexes[1], primitives[i].vertexes[0]);
                dva.vertexes[2] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[0], program->type);
            }
            if (mimoB)
            {
                jeden.vertexes[0] = primitives[i].vertexes[0];
                jeden.vertexes[1] = primitives[i].vertexes[2];
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[1]);
                jeden.vertexes[2] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[0], program->type);
                dva.vertexes[0] = primitives[i].vertexes[0];
                t = countT(primitives[i].vertexes[2], primitives[i].vertexes[1]);
                dva.vertexes[1] = countVertex(t, primitives[i].vertexes[2], primitives[i].vertexes[1], program->type);
                t = countT(primitives[i].vertexes[1], primitives[i].vertexes[0]);
                dva.vertexes[2] = countVertex(t, primitives[i].vertexes[0], primitives[i].vertexes[1], program->type);
            }
            if (mimoC)
            {
                jeden.vertexes[0] = primitives[i].vertexes[0];
                jeden.vertexes[1] = primitives[i].vertexes[1];
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[2]);
                jeden.vertexes[2] = countVertex(t, primitives[i].vertexes[0], primitives[i].vertexes[2], program->type);
                dva.vertexes[0] = primitives[i].vertexes[1];
                t = countT(primitives[i].vertexes[1], primitives[i].vertexes[2]);
                dva.vertexes[1] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[2], program->type);
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[2]);
                dva.vertexes[2] = countVertex(t, primitives[i].vertexes[0], primitives[i].vertexes[2], program->type);
            }
            clipTrojuhelniky.push_back(jeden);
            clipTrojuhelniky.push_back(dva);

        }
        if (celkemMimo == 2)
        {
            Triangle oriznuty;
            float t;
            if (!mimoA)
            {
                oriznuty.vertexes[0] = primitives[i].vertexes[0];
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[1]);
                oriznuty.vertexes[1] = countVertex(t, primitives[i].vertexes[2], primitives[i].vertexes[0], program->type);
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[2]);
                oriznuty.vertexes[2] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[0], program->type);
            }
            if(!mimoB)
            {
                oriznuty.vertexes[0] = primitives[i].vertexes[1];
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[1]);
                oriznuty.vertexes[1] = countVertex(t, primitives[i].vertexes[0], primitives[i].vertexes[1], program->type);
                t = countT(primitives[i].vertexes[2], primitives[i].vertexes[1]);
                oriznuty.vertexes[2] = countVertex(t, primitives[i].vertexes[2], primitives[i].vertexes[1], program->type);
            }
            if (!mimoC)
            {
                oriznuty.vertexes[0] = primitives[i].vertexes[2];
                t = countT(primitives[i].vertexes[0], primitives[i].vertexes[2]);
                oriznuty.vertexes[1] = countVertex(t, primitives[i].vertexes[0], primitives[i].vertexes[2], program->type);
                t = countT(primitives[i].vertexes[1], primitives[i].vertexes[2]);
                oriznuty.vertexes[2] = countVertex(t, primitives[i].vertexes[1], primitives[i].vertexes[2], program->type);
            }
            clipTrojuhelniky.push_back(oriznuty);
        }
    }// Konec clippingu
    

    //std::cout << clipTrojuhelniky.size() << std::endl;


    for (i = 0; i < clipTrojuhelniky.size(); i++) // Perspektivní dělení
    {
        for (unsigned j = 0; j < 3; j++)
        {
            clipTrojuhelniky[i].vertexes[0].gl_Position[j] /= clipTrojuhelniky[i].vertexes[0].gl_Position[3];
            clipTrojuhelniky[i].vertexes[1].gl_Position[j] /= clipTrojuhelniky[i].vertexes[1].gl_Position[3];
            clipTrojuhelniky[i].vertexes[2].gl_Position[j] /= clipTrojuhelniky[i].vertexes[2].gl_Position[3];
        }
    } // Konec perspektivního dělení

    for (i = 0; i < clipTrojuhelniky.size(); i++) // Viewport transformace
    {
        for (unsigned j = 0; j < 3; j++)
        {
            clipTrojuhelniky[i].vertexes[j].gl_Position[0] = getFramebufferWidth() * (clipTrojuhelniky[i].vertexes[j].gl_Position[0]+1)/2;
            clipTrojuhelniky[i].vertexes[j].gl_Position[1] = getFramebufferHeight() * (clipTrojuhelniky[i].vertexes[j].gl_Position[1]+1)/2;
        }
    } // Konec viewport transformace
    
    std::vector<InFragment> pixels;


    for (i = 0; i < clipTrojuhelniky.size(); i++) // Milovaná rasterizace
    {

        float bodA[3] = { clipTrojuhelniky[i].vertexes[0].gl_Position[0], 
            clipTrojuhelniky[i].vertexes[0].gl_Position[1],
            clipTrojuhelniky[i].vertexes[0].gl_Position[2]};

        float bodB[3] = { clipTrojuhelniky[i].vertexes[1].gl_Position[0],
            clipTrojuhelniky[i].vertexes[1].gl_Position[1],
            clipTrojuhelniky[i].vertexes[1].gl_Position[2]};

        float bodC[3] = { clipTrojuhelniky[i].vertexes[2].gl_Position[0],
            clipTrojuhelniky[i].vertexes[2].gl_Position[1],
            clipTrojuhelniky[i].vertexes[2].gl_Position[2]};

        int xmin, xmax, ymin, ymax;
        xmin = floor(findMin(bodA[0], bodB[0], bodC[0]));
        xmax = ceil(findMax(bodA[0], bodB[0], bodC[0]));
        ymin = floor(findMin(bodA[1], bodB[1], bodC[1]));
        ymax = ceil(findMax(bodA[1], bodB[1], bodC[1]));
        //xmin = 0;
        //xmax = getFramebufferHeight();
        //ymin = 0;
        //ymax = getFramebufferWidth();
        /*std::cout << "Zaciname: \n";
        std::cout << bodA[0] << std::endl;
        std::cout << bodA[1] << std::endl;
        std::cout << bodB[0] << std::endl;
        std::cout << bodB[1] << std::endl;
        std::cout << bodC[0] << std::endl;
        std::cout << bodC[1] << std::endl;*/


        double Ea, Eb, Ec; //  (x − xi0)∆yi − (y − yi0)∆xi
        Ea = (xmin + 0.5 - (double)bodA[0]) * ((double)bodB[1] - (double)bodA[1]);
        Ea -= (ymin + 0.5 - (double)bodA[1]) * ((double)bodB[0] - (double)bodA[0]);
        Eb = (xmin + 0.5 - (double)bodB[0]) * ((double)bodC[1] - (double)bodB[1]);
        Eb -= (ymin + 0.5 - (double)bodB[1]) * ((double)bodC[0] - (double)bodB[0]);
        Ec = (xmin + 0.5 - (double)bodC[0]) * ((double)bodA[1] - (double)bodC[1]);
        Ec -= (ymin + 0.5 - (double)bodC[1]) * ((double)bodA[0] - (double)bodC[0]);


        float h0, h1, h2;
        h0 = clipTrojuhelniky[i].vertexes[0].gl_Position[3];
        h1 = clipTrojuhelniky[i].vertexes[1].gl_Position[3];
        h2 = clipTrojuhelniky[i].vertexes[2].gl_Position[3];


        //std::cout << "Chci vykrestlit trojuhelnik s body: " << bodA[0] << " " << bodB[0] << " " << bodC[0] << " " << bodA[1] << " " << bodB[1] << " " << bodC[1] <<std::endl;


        for (int radky = (int)ymin; radky < (int)ymax; radky++)
        {
            if (radky > getFramebufferHeight())
            {
                break;
            }
            //std::cout << "Ea, Eb, Ec: " << Ea << " " << Eb << " " << Ec << std::endl;
            for (int sloupce = (int)xmin; sloupce < (int)xmax; sloupce++)
            {
                if (sloupce > getFramebufferWidth())
                {
                    break;
                }
                if ((Ea >= 0 && Eb >= 0 && Ec >= 0) || (Ea <= 0 && Eb <= 0 && Ec <= 0))
                {
                    InFragment novy;
                    float lambda1, lambda2, lambda3;
                    float obsahCelku = triangleObsah(bodA[0], bodA[1], bodB[0], bodB[1], bodC[0], bodC[1]);
                    lambda1 = triangleObsah(sloupce+0.5, radky+0.5, bodB[0], bodB[1], bodC[0], bodC[1]) / obsahCelku;
                    lambda2 = triangleObsah(bodA[0], bodA[1], sloupce + 0.5, radky + 0.5, bodC[0], bodC[1]) / obsahCelku;
                    lambda3 = triangleObsah(bodA[0], bodA[1], bodB[0], bodB[1], sloupce + 0.5, radky + 0.5) / obsahCelku;

                    novy.gl_FragCoord[0] = (float)sloupce+0.5;
                    novy.gl_FragCoord[1] = (float)radky+0.5;


                    novy.gl_FragCoord[2] = (clipTrojuhelniky[i].vertexes[0].gl_Position[2] * lambda1 / h0) + (clipTrojuhelniky[i].vertexes[1].gl_Position[2] * lambda2 / h1) + (clipTrojuhelniky[i].vertexes[2].gl_Position[2] * lambda3 / h2);
                    novy.gl_FragCoord[2] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);

                    for (int atributy=0; atributy < maxAttributes; atributy++)
                    {
                        if (program->type[atributy] == AttributeType::FLOAT)
                        {
                            float A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v1;
                            float A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v1;
                            float A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v1;
                            novy.attributes[atributy].v1 = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v1 /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                        }
                        else if (program->type[atributy] == AttributeType::VEC2)
                        {
                            float A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v2[0];
                            float A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v2[0];
                            float A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v2[0];
                            novy.attributes[atributy].v2[0] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v2[0] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v2[1];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v2[1];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v2[1];
                            novy.attributes[atributy].v2[1] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v2[1] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                        }
                        else if (program->type[atributy] == AttributeType::VEC3)
                        {
                            float A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v3[0];
                            float A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v3[0];
                            float A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v3[0];
                            novy.attributes[atributy].v3[0] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v3[0] /= ((lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2));
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v3[1];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v3[1];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v3[1];
                            novy.attributes[atributy].v3[1] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v3[1] /= ((lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2));
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v3[2];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v3[2];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v3[2];
                            novy.attributes[atributy].v3[2] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v3[2] /= ((lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2));
                        }
                        else if (program->type[atributy] == AttributeType::VEC4)
                        {
                            float A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v4[0];
                            float A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v4[0];
                            float A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v4[0];
                            novy.attributes[atributy].v4[0] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v4[0] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v4[1];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v4[1];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v4[1];
                            novy.attributes[atributy].v4[1] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v4[1] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v4[2];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v4[2];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v4[2];
                            novy.attributes[atributy].v4[2] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v4[2] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                            A0 = clipTrojuhelniky[i].vertexes[0].attributes[atributy].v4[3];
                            A1 = clipTrojuhelniky[i].vertexes[1].attributes[atributy].v4[3];
                            A2 = clipTrojuhelniky[i].vertexes[2].attributes[atributy].v4[3];
                            novy.attributes[atributy].v4[3] = (A0 * lambda1 / h0) + (A1 * lambda2 / h1) + (A2 * lambda3 / h2);
                            novy.attributes[atributy].v4[3] /= (lambda1 / h0) + (lambda2 / h1) + (lambda3 / h2);
                        }
                       
                    }
                    
                    pixels.push_back(novy);
                    
                }


                Ea += ((double)bodB[1] - (double)bodA[1]);
                Eb += ((double)bodC[1] - (double)bodB[1]);
                Ec += ((double)bodA[1] - (double)bodC[1]);


            }

            Ea -= ((double)xmax - (double)xmin) * ((double)bodB[1] - (double)bodA[1]);
            Eb -= ((double)xmax - (double)xmin) * ((double)bodC[1] - (double)bodB[1]);
            Ec -= ((double)xmax - (double)xmin) * ((double)bodA[1] - (double)bodC[1]);
            Ea -= ((double)bodB[0] - (double)bodA[0]);
            Eb -= ((double)bodC[0] - (double)bodB[0]);
            Ec -= ((double)bodA[0] - (double)bodC[0]);

        }

    }


    //for (i = 0; i < pixels.size(); i++)
    //{
    //    if (fragmxmax < pixels[i].gl_FragCoord[0])
    //    {
    //        fragmxmax = pixels[i].gl_FragCoord[0];
    //    }
    //    if (fragmymax < pixels[i].gl_FragCoord[1])
    //    {
    //        fragmymax = pixels[i].gl_FragCoord[1];
    //    }
    //}
    //std::cout << "Max x je: " << fragmxmax << std::endl;
    //std::cout << "Max y je: " << fragmymax << std::endl;

    //clear(0, 0, 0, 0);

    for (i = 0; i < pixels.size(); i++)
    {
        OutFragment out;
        program->fragmentShader(out, pixels[i], program->attributes);
        //cout << "Pozice pro fragment shader je: " << (unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() + (unsigned)pixels[i].gl_FragCoord[0] << endl;
        //cout << "Barvy jsou: " << out.gl_FragColor[0] << " " << out.gl_FragColor[1] << " " << out.gl_FragColor[2] << endl;
        if (framebuffer->depthBuffer[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() + (unsigned)pixels[i].gl_FragCoord[0]] > pixels[i].gl_FragCoord[2])
        {
            framebuffer->depthBuffer[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() + (unsigned)pixels[i].gl_FragCoord[0]] = pixels[i].gl_FragCoord[2];
            framebuffer->colorPixels[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() * 4 + (unsigned)pixels[i].gl_FragCoord[0] * 4] = (out.gl_FragColor.r*255);
            framebuffer->colorPixels[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() * 4 + (unsigned)pixels[i].gl_FragCoord[0] * 4 + 1] = (out.gl_FragColor.g * 255);
            framebuffer->colorPixels[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() * 4 + (unsigned)pixels[i].gl_FragCoord[0] * 4 + 2] = (out.gl_FragColor.b * 255);
            framebuffer->colorPixels[(unsigned)pixels[i].gl_FragCoord[1] * getFramebufferWidth() * 4 + (unsigned)pixels[i].gl_FragCoord[0] * 4 + 3] = (out.gl_FragColor.a * 255);
            
        }
    }



}

/// @}
