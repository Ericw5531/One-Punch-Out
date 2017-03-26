#pragma once

#include "utils/general.h"

#include "entity/entity.h"

#include "gfx/particle.h"

class Projectile : public Entity
{
public:
    Projectile();
    ~Projectile();

	void reload(
		Vector2f pos,
		Vector2f vel,
		float a,
		float s);

	void render() const;
	void update(float deltaTime);

    inline void multiplyVelocity(float speed) { m_velocity = m_velocity * speed; }

	inline void setActive(bool a) { m_active = a; }
	inline bool isActive() const { return m_active; }

	void loadTexture(char* filename) { m_texture.loadFromFile(filename, 64, 64, false); }

	void setScale(GLfloat xscale, GLfloat yscale);
	GLfloat getXScale() { return m_xScale; }
	GLfloat getYScale() { return m_yScale; }

    void setCollisionBox(GLuint width, GLuint height);
	inline void setCenteredBox(bool centered) { m_centeredBox = centered;  }

	inline void setColor(SDL_Color color) { m_color = color; }

	void setParticleEmitter(ParticleEmitter* particleEmitter);
private:
    int m_projectileSpeed;

	ParticleEmitter* m_particleEmitter;

    //whether projectile is on screen or not
    bool m_active;

    bool m_centeredBox;

    SDL_Color m_color;

    GLuint m_baseWidth;
    GLuint m_baseHeight;
    GLfloat m_xScale;
    GLfloat m_yScale;
};
