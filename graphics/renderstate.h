#pragma once

#include "graphics.h"

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace Guy
{

class ShaderProgram;

class RenderState
{
	friend class Environment;

public:

	RenderState();
	~RenderState();

	typedef boost::function<void(ShaderProgram&)> AutoBinding;

	enum Blend
	{
		BLEND_ZERO = GL_ZERO,
		BLEND_ONE = GL_ONE,
		BLEND_SRC_COLOR = GL_SRC_COLOR,
		BLEN_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
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

	class StateBlock
	{
		friend class RenderState;

	public:
		boost::shared_ptr<StateBlock> create();

		void bind();
		void setBlend(bool enabled);
		void setBlendSrc(Blend blend);
		void setBlendDst(Blend blend);
		void setCullFace(bool enabled);
		void setDepthTest(bool enabled);
		void setDepthWrite(bool enabled);

	private:

		StateBlock();
		~StateBlock();

		void bindNoRestore();
		static void restore(long stateOverrideBits);
		static void enableDepthWrite();

		bool m_cullFaceEnabled;
		bool m_depthTestEnabled;
		bool m_depthWriteEnabled;
		bool m_blendEnabled;
		Blend m_blendSrc;
		Blend m_blendDst;
		long m_bits;

		static void initialize();
		static boost::shared_ptr<StateBlock> s_defaultState;
	};

	class UniformBinding
	{
		friend class RenderState;

	public:

		UniformBinding(const std::string& name);

	private:
		std::string m_name;
		bool m_valid;
	};


	void setStateBlock(boost::shared_ptr<StateBlock> state);
	boost::shared_ptr<StateBlock> stateBlock();
	boost::shared_ptr<UniformBinding> uniformBinding(const char* name);

private:

	void init();

	std::map<std::string, boost::shared_ptr<UniformBinding> > m_bindings;
	boost::shared_ptr<StateBlock> m_state;
};

} // namespace Guy
