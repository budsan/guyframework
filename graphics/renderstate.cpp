#include "renderstate.h"

#define RS_BLEND 1
#define RS_BLEND_FUNC 2
#define RS_CULL_FACE 4
#define RS_DEPTH_TEST 8
#define RS_DEPTH_WRITE 16

namespace Guy
{

boost::shared_ptr<RenderState::StateBlock> RenderState::StateBlock::s_defaultState();

RenderState::RenderState()
{
}

RenderState::~RenderState()
{
}

boost::shared_ptr<RenderState::UniformBinding> RenderState::uniformBinding(const char* c_name)
{
	std::string name(c_name);
	std::map<std::string, boost::shared_ptr<UniformBinding> >::iterator it = m_bindings.find(name);
	if (it == m_bindings.end()) {
		it = m_bindings.insert(name, UniformBinding(name));
	}

	return it->second;
}

void RenderState::setStateBlock(boost::shared_ptr<RenderState::StateBlock> state)
{
	m_state = state;
}

boost::shared_ptr<RenderState::StateBlock> RenderState::stateBlock()
{
	if (m_state == NULL) {
		m_state = StateBlock::create();
	}
	return m_state;
}


//--RenderState::StateBlock--------------------------------------------------//

boost::shared_ptr<RenderState::StateBlock> RenderState::StateBlock::create()
{
	return boost::shared_ptr<StateBlock>(new StateBlock());
}

RenderState::StateBlock::StateBlock()
	: m_cullFaceEnabled(false), m_depthTestEnabled(false), m_depthWriteEnabled(false),
	  m_blendEnabled(false), m_blendSrc(RenderState::BLEND_ONE), m_blendDst(RenderState::BLEND_ZERO),
	  m_bits(0L)
{
}

RenderState::StateBlock::StateBlock(const StateBlock& copy)
{
	// Hidden
}

RenderState::StateBlock::~StateBlock()
{
}

void RenderState::StateBlock::bind()
{
	StateBlock::restore(m_bits);

	bindNoRestore();
}

void RenderState::StateBlock::bindNoRestore()
{
	GUY_ASSERT(s_defaultState);

	// Update any state that differs from _defaultState and flip _defaultState bits
	if ((m_bits & RS_BLEND) && (m_blendEnabled != s_defaultState->m_blendEnabled))
	{
		if (m_blendEnabled)
			GL_ASSERT( glEnable(GL_BLEND) );
		else
			GL_ASSERT( glDisable(GL_BLEND) );
		s_defaultState->m_blendEnabled = m_blendEnabled;
	}
	if ((m_bits & RS_BLEND_FUNC) && (m_blendSrc != s_defaultState->m_blendSrc || m_blendDst != s_defaultState->m_blendDst))
	{
		GL_ASSERT( glBlendFunc((GLenum)m_blendSrc, (GLenum)m_blendDst) );
		s_defaultState->m_blendSrc = m_blendSrc;
		s_defaultState->m_blendDst = m_blendDst;
	}
	if ((m_bits & RS_CULL_FACE) && (m_cullFaceEnabled != s_defaultState->m_cullFaceEnabled))
	{
		if (m_cullFaceEnabled)
			GL_ASSERT( glEnable(GL_CULL_FACE) );
		else
			GL_ASSERT( glDisable(GL_CULL_FACE) );
		s_defaultState->m_cullFaceEnabled = m_cullFaceEnabled;
	}
	if ((m_bits & RS_DEPTH_TEST) && (m_depthTestEnabled != s_defaultState->m_depthTestEnabled))
	{
		if (m_depthTestEnabled)
			GL_ASSERT( glEnable(GL_DEPTH_TEST) );
		else
			GL_ASSERT( glDisable(GL_DEPTH_TEST) );
		s_defaultState->m_depthTestEnabled = m_depthTestEnabled;
	}
	if ((m_bits & RS_DEPTH_WRITE) && (m_depthWriteEnabled != s_defaultState->m_depthWriteEnabled))
	{
		GL_ASSERT( glDepthMask(m_depthWriteEnabled ? GL_TRUE : GL_FALSE) );
		s_defaultState->m_depthWriteEnabled = m_depthWriteEnabled;
	}

	s_defaultState->m_bits |= m_bits;
}

void RenderState::StateBlock::restore(long stateOverrideBits)
{
	GUY_ASSERT(s_defaultState);

	// If there is no state to restore (i.e. no non-default state), do nothing.
	if (s_defaultState->m_bits == 0)
	{
		return;
	}

	// Restore any state that is not overridden and is not default
	if (!(stateOverrideBits & RS_BLEND) && (s_defaultState->m_bits & RS_BLEND))
	{
		GL_ASSERT( glDisable(GL_BLEND) );
		s_defaultState->m_bits &= ~RS_BLEND;
		s_defaultState->m_blendEnabled = false;
	}
	if (!(stateOverrideBits & RS_BLEND_FUNC) && (s_defaultState->m_bits & RS_BLEND_FUNC))
	{
		GL_ASSERT( glBlendFunc(GL_ONE, GL_ZERO) );
		s_defaultState->m_bits &= ~RS_BLEND_FUNC;
		s_defaultState->m_blendSrc = RenderState::BLEND_ONE;
		s_defaultState->m_blendDst = RenderState::BLEND_ZERO;
	}
	if (!(stateOverrideBits & RS_CULL_FACE) && (s_defaultState->m_bits & RS_CULL_FACE))
	{
		GL_ASSERT( glDisable(GL_CULL_FACE) );
		s_defaultState->m_bits &= ~RS_CULL_FACE;
		s_defaultState->m_cullFaceEnabled = false;
	}
	if (!(stateOverrideBits & RS_DEPTH_TEST) && (s_defaultState->m_bits & RS_DEPTH_TEST))
	{
		GL_ASSERT( glDisable(GL_DEPTH_TEST) );
		s_defaultState->m_bits &= ~RS_DEPTH_TEST;
		s_defaultState->m_depthTestEnabled = false;
	}
	if (!(stateOverrideBits & RS_DEPTH_WRITE) && (s_defaultState->m_bits & RS_DEPTH_WRITE))
	{
		GL_ASSERT( glDepthMask(GL_TRUE) );
		s_defaultState->m_bits &= ~RS_DEPTH_WRITE;
		s_defaultState->m_depthWriteEnabled = true;
	}
}

void RenderState::StateBlock::enableDepthWrite()
{
	GUY_ASSERT(s_defaultState);

	// Internal method used by Game::clear() to restore depth writing before a
	// clear operation. This is necessary if the last code to draw before the
	// next frame leaves depth writing disabled.
	if (!s_defaultState->m_depthWriteEnabled)
	{
		GL_ASSERT( glDepthMask(GL_TRUE) );
		s_defaultState->m_bits &= ~RS_DEPTH_WRITE;
		s_defaultState->m_depthWriteEnabled = true;
	}
}

void RenderState::StateBlock::setBlend(bool enabled)
{
	m_blendEnabled = enabled;
	if (!enabled)
	{
		m_bits &= ~RS_BLEND;
	}
	else
	{
		m_bits |= RS_BLEND;
	}
}

void RenderState::StateBlock::setBlendSrc(Blend blend)
{
	m_blendSrc = blend;
	if (m_blendSrc == BLEND_ONE && m_blendDst == BLEND_ZERO)
	{
		// Default blend func
		m_bits &= ~RS_BLEND_FUNC;
	}
	else
	{
		m_bits |= RS_BLEND_FUNC;
	}
}

void RenderState::StateBlock::setBlendDst(Blend blend)
{
	m_blendDst = blend;
	if (m_blendSrc == BLEND_ONE && m_blendDst == BLEND_ZERO)
	{
		// Default blend func
		m_bits &= ~RS_BLEND_FUNC;
	}
	else
	{
		m_bits |= RS_BLEND_FUNC;
	}
}

void RenderState::StateBlock::setCullFace(bool enabled)
{
	m_cullFaceEnabled = enabled;
	if (!enabled)
	{
		m_bits &= ~RS_CULL_FACE;
	}
	else
	{
		m_bits |= RS_CULL_FACE;
	}
}

void RenderState::StateBlock::setDepthTest(bool enabled)
{
	m_depthTestEnabled = enabled;
	if (!enabled)
	{
		m_bits &= ~RS_DEPTH_TEST;
	}
	else
	{
		m_bits |= RS_DEPTH_TEST;
	}
}

void RenderState::StateBlock::setDepthWrite(bool enabled)
{
	m_depthWriteEnabled = enabled;
	if (enabled)
	{
		m_bits &= ~RS_DEPTH_WRITE;
	}
	else
	{
		m_bits |= RS_DEPTH_WRITE;
	}
}

void RenderState::initialize()
{
	if (StateBlock::_defaultState == NULL)
	{
	    StateBlock::_defaultState = StateBlock::create();
	}
}

}
