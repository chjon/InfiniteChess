#include "../Common.h"
#include "Shader.h"
#include "Texture2D.h"

namespace ic {
    class SpriteRenderer {
    public:
        SpriteRenderer(Shader &shader);
        ~SpriteRenderer();

        void drawSprite(Texture2D &texture, glm::vec2 position, 
            glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, 
            glm::vec3 color = glm::vec3(1.0f));
    private:
        void initRenderData();
        Shader m_shader; 
        unsigned int m_quadVAO;
    };
}