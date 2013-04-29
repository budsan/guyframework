#pragma once

#include "graphics.h"

namespace Guy
{

class RenderState
{

public:

    enum Blend
    {
        BLEND_ZERO = GL_ZERO,
        BLEND_ONE = GL_ONE,
        BLEND_SRC_COLOR = GL_SRC_COLOR,
        BLEND_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        BLEND_DST_COLOR = GL_DST_COLOR,
        BLEND_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        BLEND_SRC_ALPHA = GL_SRC_ALPHA,
        BLEND_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        BLEND_DST_ALPHA = GL_DST_ALPHA,
        BLEND_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        BLEND_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        BLEND_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        BLEND_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
    };

    enum DepthFunction
    {
        DEPTH_NEVER = GL_NEVER,
        DEPTH_LESS = GL_LESS,
        DEPTH_EQUAL = GL_EQUAL,
        DEPTH_LEQUAL = GL_LEQUAL,
        DEPTH_GREATER = GL_GREATER,
        DEPTH_NOTEQUAL = GL_NOTEQUAL,
        DEPTH_GEQUAL = GL_GEQUAL,
        DEPTH_ALWAYS = GL_ALWAYS
    };

    class StateBlock
    {
        friend class RenderState;

    public:

        StateBlock();
        StateBlock(const StateBlock& copy);
        ~StateBlock();

        void bind();

        void setBlend(bool enabled);
        void setBlendSrc(Blend blend);
        void setBlendDst(Blend blend);
        void setCullFace(bool enabled);
        void setDepthTest(bool enabled);
        void setDepthWrite(bool enabled);
        void setDepthFunction(DepthFunction func);

    private:

        void bindNoRestore();

        static void restore(long stateOverrideBits);
        static void enableDepthWrite();

        // States
        bool _cullFaceEnabled;
        bool _depthTestEnabled;
        bool _depthWriteEnabled;
        DepthFunction _depthFunction;
        bool _blendEnabled;
        Blend _blendSrc;
        Blend _blendDst;
        long _bits;

        static StateBlock _defaultState;
    };

    void setStateBlock(const StateBlock &state);
    StateBlock& getStateBlock();

protected:

    RenderState();
    virtual ~RenderState();


private:

    RenderState(const RenderState& copy);
    RenderState& operator=(const RenderState&);

protected:

    StateBlock _state;
};

}
