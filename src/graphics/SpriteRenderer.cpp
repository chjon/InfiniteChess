#include "SpriteRenderer.h"

using namespace ic;

void SpriteRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    // prepare transformations
    m_shader.use();
    glm::mat3 model = glm::mat3(1.0f);
    model = glm::translate(model, position);  
    model = glm::translate(model, 0.5f * size); 
    model = glm::rotate   (model, glm::radians(rotate)); 
    model = glm::translate(model, -0.5f * size);
    model = glm::scale    (model, size);
  
    m_shader.setMatrix4("model", model);
    m_shader.setVector3f("spriteColor", color);
  
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}  