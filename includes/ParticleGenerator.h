#pragma once

#include "linmath/float3.h"
#include <vector>
#include "IRenderComponent.h"

#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f

class Camera;
class Particle;
class ParticleConf;

class ParticleGenerator : public IRenderComponent
{
public:
    ParticleGenerator(Texture *texture, int amount,
                      Camera *camera, float4x4 modelMatrix,
                      ParticleConf *conf);

    ~ParticleGenerator();

    /**
     * Update the position of all spawned Particles.
     * @param dt The time in seconds since last update call.
     */
    void update(float dt);

    /**
     * Render shadows from the spawned Particles.
     * @param shaderProgram The ShaderProgram used to render the shadows.
     */
    void renderShadow(ShaderProgram* shaderProgram) {};

    /**
     * Render all the spawned Particles.
     */
    void render();

    void setScaleLod(bool value);

    void setLooping(bool value);
private:
    std::vector<Particle*> m_particles;
    GLuint m_vaob;
    Texture *texture;
    int m_amount = 0;
    Camera *m_camera;

    ParticleConf *conf;
    bool doScale = true;

    chag::float3x3 getModelMatrix3x3();
};
