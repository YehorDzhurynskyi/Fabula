#include "pch.h"
#include "Renderer.h"
#include "Camera.h"

#include "SDL_image.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

Renderer::Renderer()
    : m_windowResizedListener(this)
    , m_currentSpriteCount(0)
{
    m_windowResizedListener.bind(EventType::WindowResized, [this](const Event& event)
    {
        assert(event.type() == EventType::WindowResized);
        const WindowResizedEvent& windowResizedEvent = AS(const WindowResizedEvent&, event);

        glBindTexture(GL_TEXTURE_2D, m_target_Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowResizedEvent.Width, windowResizedEvent.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    });
}

bool Renderer::init()
{
    const bool staticPassSucceed = m_staticPass.init();
    const bool motionPassSucceed = m_motionBlurPass.init();
    const bool trailPassSucceed = m_trailPass.init();

    assert(staticPassSucceed);
    assert(motionPassSucceed);
    assert(trailPassSucceed);

    bool initialized = staticPassSucceed && motionPassSucceed && trailPassSucceed;
    if (!initialized)
    {
        return false;
    }

    { // Texture Atlas
        glGenTextures(1, &m_atlas_Texture);
        glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_Surface* atlasSurface = IMG_Load("Assets/atlas.png");
        if (atlasSurface == nullptr)
        {
            REVEAL_SDL_ERROR("Failed to load sprite atlas")
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasSurface->w, atlasSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        SDL_FreeSurface(atlasSurface);
    }

    { // IBO
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_MaxIndicesCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);
    }

    { // FBO & Target Texture
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures(1, &m_target_Texture);
        glBindTexture(GL_TEXTURE_2D, m_target_Texture);

        const vec2f screenSize = Camera::get().getScreenSize();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_target_Texture, 0);

        initialized = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return initialized;
}

void Renderer::shutdown()
{
    glDeleteFramebuffers(1, &m_FBO);

    glDeleteBuffers(1, &m_IBO);

    glDeleteTextures(1, &m_target_Texture);
    glDeleteTextures(1, &m_atlas_Texture);

    m_staticPass.shutdown();
    m_motionBlurPass.shutdown();
    m_trailPass.shutdown();
}

u32 Renderer::compile_shader(i32 shaderType, const char* sourceCode)
{
    const ShaderID shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCode, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        char log[4096];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Renderer::render(const SpriteURI uri, const Transform& transform, const u32 colorTint)
{
    render(SpriteAtlas::at(uri).Offset, SpriteAtlas::at(uri).Size, transform, colorTint);
}

void Renderer::render(const AnimatedSpriteURI uri, const int frame, const Transform& transform, const u32 colorTint)
{
    const AnimatedSprite& sprite = SpriteAtlas::at(uri);

    assert(frame == clamp<i32>(frame, 0, sprite.NOfFrames - 1));
    assert(sprite.Pitch != 0);
    assert(sprite.NOfFrames / sprite.Pitch != 0);

    const float dU = sprite.Size.x / sprite.Pitch;
    const float dV = sprite.Size.y / (sprite.NOfFrames / sprite.Pitch);

    const float uOffset = sprite.Offset.x + dU * (frame % sprite.Pitch);
    const float vOffset = sprite.Offset.y + dV * (frame / sprite.Pitch);

    render(vec2f(uOffset, vOffset), vec2f(dU, dV), transform, colorTint);
}

void Renderer::render(const vec2f uvOffset, const vec2f uvSize, const Transform& transform, const u32 colorTint)
{
    const float sx = transform.Size.x;
    const float sy = transform.Size.y;

    Position_VBO.push(vec2f(-sx, sy) + transform.Position);
    Position_VBO.push(vec2f(-sx, -sy) + transform.Position);
    Position_VBO.push(vec2f(sx, sy) + transform.Position);
    Position_VBO.push(vec2f(sx, -sy) + transform.Position);

    {
        Color_UV_Data& color_uv = Color_UV_VBO.push();
        color_uv.UV = uvSize * vec2f(0.0f, 0.0f) + uvOffset;
        color_uv.ColorTint = colorTint;
    }

    {
        Color_UV_Data& color_uv = Color_UV_VBO.push();
        color_uv.UV = uvSize * vec2f(0.0f, 1.0f) + uvOffset;
        color_uv.ColorTint = colorTint;
    }

    {
        Color_UV_Data& color_uv = Color_UV_VBO.push();
        color_uv.UV = uvSize * vec2f(1.0f, 0.0f) + uvOffset;
        color_uv.ColorTint = colorTint;
    }

    {
        Color_UV_Data& color_uv = Color_UV_VBO.push();
        color_uv.UV = uvSize * vec2f(1.0f, 1.0f) + uvOffset;
        color_uv.ColorTint = colorTint;
    }

    const i32 offset = m_currentSpriteCount * 4;
    u16* indices = &m_clientIndexBuffer[m_currentSpriteCount * 6];
    indices[0] = 0 + offset;
    indices[1] = 1 + offset;
    indices[2] = 2 + offset;
    indices[3] = 2 + offset;
    indices[4] = 1 + offset;
    indices[5] = 3 + offset;

    ++m_currentSpriteCount;
}

void Renderer::present_MotionBlured()
{
    present_Before();

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClear(GL_COLOR_BUFFER_BIT);

    {
        glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);
        m_staticPass.bind();

        glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

        m_staticPass.unbind();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    {
        glBindTexture(GL_TEXTURE_2D, m_target_Texture);
        m_motionBlurPass.bind();

        glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

        m_motionBlurPass.unbind();
    }

    present_After();
}

void Renderer::present_Static()
{
    present_Before();

    glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);
    m_staticPass.bind();
    glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);
    m_staticPass.unbind();

    present_After();
}

void Renderer::present_Before()
{
    assert(m_currentSpriteCount <= g_MaxVerticesCount / 4);

    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_currentSpriteCount * 6 * sizeof(u16), (void*)m_clientIndexBuffer);

        Position_VBO.flush();
        Color_UV_VBO.flush();
    }
}

void Renderer::present_After()
{
    m_currentSpriteCount = 0;
}
