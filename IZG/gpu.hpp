/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once

#include <student/fwd.hpp>
#include <vector>

/**
 * @brief This class represent software GPU
 */
class GPU{
  public:

      struct frameBuffer {
          uint32_t width;
          uint32_t height;
          uint8_t *colorPixels;
          float *depthBuffer;
      };

      struct Buffer {
          uint64_t size = 0;
          void* data = NULL;
          BufferID ID=emptyID;
      };

      struct Heads {
          BufferID ID;
          uint64_t stride;
          uint64_t offset;
          AttributeType type;
          bool enabled;
      };

      struct VertexPuller {
          VertexPullerID vertex_ID;
          Heads *heads;
          bool Indexing;
          BufferID buffer_ID;
          IndexType Type;
      };

      struct Program {
          ProgramID ID;
          Uniforms attributes;
          VertexShader vertexShader;
          FragmentShader fragmentShader;
          AttributeType *type = new AttributeType[maxAttributes];
      };

      struct Triangle {
          OutVertex vertexes[3];
      };

    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

    /// \addtogroup gpu_init 00. proměnné, inicializace / deinicializace grafické karty
    /// @{
    /// \todo zde si můžete vytvořit proměnné grafické karty (buffery, programy, ...)
    /// @}
private:

    float countT(OutVertex first, OutVertex second);

    OutVertex countVertex(float t, OutVertex first, OutVertex second, AttributeType* types);

    float countDistance(float x1, float y1, float x2, float y2);

    double findMin(float val1, float val2, float val3);

    double findMax(float val1, float val2, float val3);

    float triangleObsah(float Ax, float Ay, float Bx, float By, float Cx, float Cy);

    frameBuffer* framebuffer;
    uint64_t BufferCount=0;
    std::vector<Buffer*> bufferArray;
    BufferID original = 0;
    VertexPullerID VertexOriginal = 0;
    std::vector<VertexPuller*> vertexArray;
    VertexPullerID bindedVertexPuller = emptyID;
    std::vector<Program*> programArray;
    ProgramID ProgramOriginal = 0;
    ProgramID activeProgram=emptyID;
};


