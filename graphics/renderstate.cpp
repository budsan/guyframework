#include "renderstate.h"
#include "log.h"
#include "graphics.h"

// Render state override bits
#define RS_BLEND 1
#define RS_BLEND_FUNC 2
#define RS_CULL_FACE 4
#define RS_DEPTH_TEST 8
#define RS_DEPTH_WRITE 16
#define RS_DEPTH_FUNC 32

namespace Guy
{

RenderState::StateBlock RenderState::StateBlock::_defaultState;

RenderState::RenderState()
{
}

RenderState::~RenderState()
{
}

void RenderState::setStateBlock(const StateBlock &state)
{
    _state = state;
}

RenderState::StateBlock &RenderState::getStateBlock()
{
    return _state;
}

RenderState::StateBlock::StateBlock()
    : _cullFaceEnabled(false), _depthTestEnabled(false), _depthWriteEnabled(true), _depthFunction(RenderState::DEPTH_LESS),
      _blendEnabled(false), _blendSrc(RenderState::BLEND_ONE), _blendDst(RenderState::BLEND_ZERO),
      _bits(0L)
{
}

RenderState::StateBlock::StateBlock(const StateBlock& copy)
    : _cullFaceEnabled(copy._cullFaceEnabled),
      _depthTestEnabled(copy._depthTestEnabled),
      _depthWriteEnabled(copy._depthWriteEnabled),
      _depthFunction(copy._depthFunction),
      _blendEnabled(copy._blendEnabled),
      _blendSrc(copy._blendSrc),
      _blendDst(copy._blendDst),
      _bits(copy._bits)
{

}

RenderState::StateBlock::~StateBlock()
{
}

void RenderState::StateBlock::bind()
{
    // When the public bind() is called with no RenderState object passed in,
    // we assume we are being called to bind the state of a single StateBlock,
    // irrespective of whether it belongs to a hierarchy of RenderStates.
    // Therefore, we call restore() here with only this StateBlock's override
    // bits to restore state before applying the new state.
    StateBlock::restore(_bits);

    bindNoRestore();
}

void RenderState::StateBlock::bindNoRestore()
{
    // Update any state that differs from _defaultState and flip _defaultState bits
    if ((_bits & RS_BLEND) && (_blendEnabled != _defaultState._blendEnabled))
    {
        if (_blendEnabled)
            GL_ASSERT( glEnable(GL_BLEND) );
        else
            GL_ASSERT( glDisable(GL_BLEND) );
        _defaultState._blendEnabled = _blendEnabled;
    }
    if ((_bits & RS_BLEND_FUNC) && (_blendSrc != _defaultState._blendSrc || _blendDst != _defaultState._blendDst))
    {
        GL_ASSERT( glBlendFunc((GLenum)_blendSrc, (GLenum)_blendDst) );
        _defaultState._blendSrc = _blendSrc;
        _defaultState._blendDst = _blendDst;
    }
    if ((_bits & RS_CULL_FACE) && (_cullFaceEnabled != _defaultState._cullFaceEnabled))
    {
        if (_cullFaceEnabled)
            GL_ASSERT( glEnable(GL_CULL_FACE) );
        else
            GL_ASSERT( glDisable(GL_CULL_FACE) );
        _defaultState._cullFaceEnabled = _cullFaceEnabled;
    }
    if ((_bits & RS_DEPTH_TEST) && (_depthTestEnabled != _defaultState._depthTestEnabled))
    {
        if (_depthTestEnabled)
            GL_ASSERT( glEnable(GL_DEPTH_TEST) );
        else
            GL_ASSERT( glDisable(GL_DEPTH_TEST) );
        _defaultState._depthTestEnabled = _depthTestEnabled;
    }
    if ((_bits & RS_DEPTH_WRITE) && (_depthWriteEnabled != _defaultState._depthWriteEnabled))
    {
        GL_ASSERT( glDepthMask(_depthWriteEnabled ? GL_TRUE : GL_FALSE) );
        _defaultState._depthWriteEnabled = _depthWriteEnabled;
    }
    if ((_bits & RS_DEPTH_FUNC) && (_depthFunction != _defaultState._depthFunction))
    {
        GL_ASSERT( glDepthFunc((GLenum)_depthFunction) );
        _defaultState._depthFunction = _depthFunction;
    }

    _defaultState._bits |= _bits;
}

void RenderState::StateBlock::restore(long stateOverrideBits)
{
    // If there is no state to restore (i.e. no non-default state), do nothing.
    if (_defaultState._bits == 0)
    {
        return;
    }

    // Restore any state that is not overridden and is not default
    if (!(stateOverrideBits & RS_BLEND) && (_defaultState._bits & RS_BLEND))
    {
        GL_ASSERT( glDisable(GL_BLEND) );
        _defaultState._bits &= ~RS_BLEND;
        _defaultState._blendEnabled = false;
    }
    if (!(stateOverrideBits & RS_BLEND_FUNC) && (_defaultState._bits & RS_BLEND_FUNC))
    {
        GL_ASSERT( glBlendFunc(GL_ONE, GL_ZERO) );
        _defaultState._bits &= ~RS_BLEND_FUNC;
        _defaultState._blendSrc = RenderState::BLEND_ONE;
        _defaultState._blendDst = RenderState::BLEND_ZERO;
    }
    if (!(stateOverrideBits & RS_CULL_FACE) && (_defaultState._bits & RS_CULL_FACE))
    {
        GL_ASSERT( glDisable(GL_CULL_FACE) );
        _defaultState._bits &= ~RS_CULL_FACE;
        _defaultState._cullFaceEnabled = false;
    }
    if (!(stateOverrideBits & RS_DEPTH_TEST) && (_defaultState._bits & RS_DEPTH_TEST))
    {
        GL_ASSERT( glDisable(GL_DEPTH_TEST) );
        _defaultState._bits &= ~RS_DEPTH_TEST;
        _defaultState._depthTestEnabled = false;
    }
    if (!(stateOverrideBits & RS_DEPTH_WRITE) && (_defaultState._bits & RS_DEPTH_WRITE))
    {
        GL_ASSERT( glDepthMask(GL_TRUE) );
        _defaultState._bits &= ~RS_DEPTH_WRITE;
        _defaultState._depthWriteEnabled = true;
    }
    if (!(stateOverrideBits & RS_DEPTH_FUNC) && (_defaultState._bits & RS_DEPTH_FUNC))
    {
        GL_ASSERT( glDepthFunc((GLenum)GL_LESS) );
        _defaultState._bits &= ~RS_DEPTH_FUNC;
        _defaultState._depthFunction = RenderState::DEPTH_LESS;
    }
}

void RenderState::StateBlock::enableDepthWrite()
{
    // Internal method used by Game::clear() to restore depth writing before a
    // clear operation. This is necessary if the last code to draw before the
    // next frame leaves depth writing disabled.
    if (!_defaultState._depthWriteEnabled)
    {
        GL_ASSERT( glDepthMask(GL_TRUE) );
        _defaultState._bits &= ~RS_DEPTH_WRITE;
        _defaultState._depthWriteEnabled = true;
    }
}

void RenderState::StateBlock::setBlend(bool enabled)
{
    _blendEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_BLEND;
    }
    else
    {
        _bits |= RS_BLEND;
    }
}

void RenderState::StateBlock::setBlendSrc(Blend blend)
{
    _blendSrc = blend;
    if (_blendSrc == BLEND_ONE && _blendDst == BLEND_ZERO)
    {
        // Default blend func
        _bits &= ~RS_BLEND_FUNC;
    }
    else
    {
        _bits |= RS_BLEND_FUNC;
    }
}

void RenderState::StateBlock::setBlendDst(Blend blend)
{
    _blendDst = blend;
    if (_blendSrc == BLEND_ONE && _blendDst == BLEND_ZERO)
    {
        // Default blend func
        _bits &= ~RS_BLEND_FUNC;
    }
    else
    {
        _bits |= RS_BLEND_FUNC;
    }
}

void RenderState::StateBlock::setCullFace(bool enabled)
{
    _cullFaceEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_CULL_FACE;
    }
    else
    {
        _bits |= RS_CULL_FACE;
    }
}

void RenderState::StateBlock::setDepthTest(bool enabled)
{
    _depthTestEnabled = enabled;
    if (!enabled)
    {
        _bits &= ~RS_DEPTH_TEST;
    }
    else
    {
        _bits |= RS_DEPTH_TEST;
    }
}

void RenderState::StateBlock::setDepthWrite(bool enabled)
{
    _depthWriteEnabled = enabled;
    if (enabled)
    {
        _bits &= ~RS_DEPTH_WRITE;
    }
    else
    {
        _bits |= RS_DEPTH_WRITE;
    }
}

void RenderState::StateBlock::setDepthFunction(DepthFunction func)
{
    _depthFunction = func;
    if (_depthFunction == DEPTH_LESS)
    {
        // Default depth function
        _bits &= ~RS_DEPTH_FUNC;
    }
    else
    {
        _bits |= RS_DEPTH_FUNC;
    }
}

}
