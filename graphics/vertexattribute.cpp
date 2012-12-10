#include "vertexattribute.h"

#include <string>
#include <set>
#include <map>
#include <vector>

namespace Guy {
namespace Vertex {

#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR_NAME                 "a_color"
#define VERTEX_ATTRIBUTE_TANGENT_NAME               "a_tangent"
#define VERTEX_ATTRIBUTE_BINORMAL_NAME              "a_binormal"
#define VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME          "a_blendWeights"
#define VERTEX_ATTRIBUTE_BLENDINDICES_NAME          "a_blendIndices"
#define VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME       "a_texCoord"

void AttributeInit()
{
	//Default names for default elements
	Attribute::get(Element::Position).addName(VERTEX_ATTRIBUTE_POSITION_NAME);
	Attribute::get(Element::Normal).addName(VERTEX_ATTRIBUTE_NORMAL_NAME);
	Attribute::get(Element::Color).addName(VERTEX_ATTRIBUTE_COLOR_NAME);
	Attribute::get(Element::Tangent).addName(VERTEX_ATTRIBUTE_TANGENT_NAME);
	Attribute::get(Element::Binormal).addName(VERTEX_ATTRIBUTE_BINORMAL_NAME);
	Attribute::get(Element::BlendWeight).addName(VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME);
	Attribute::get(Element::BlendIndices).addName(VERTEX_ATTRIBUTE_BLENDINDICES_NAME);
	Attribute::get(Element::TexCoord0).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME)
		.addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"0");
	Attribute::get(Element::TexCoord1).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"1");
	Attribute::get(Element::TexCoord2).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"2");
	Attribute::get(Element::TexCoord3).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"3");
	Attribute::get(Element::TexCoord4).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"4");
	Attribute::get(Element::TexCoord5).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"5");
	Attribute::get(Element::TexCoord6).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"6");
	Attribute::get(Element::TexCoord7).addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"7");
}

bool s_attrsInit = false;
std::map<std::string, Attribute*> s_attrs;

Attribute::Attribute() {}
Attribute::Attribute(const Attribute &a) : m_names(a.m_names) {}
Attribute& Attribute::operator=(const Attribute &a)
{
	m_names = a.m_names;
	return *this;
}

Attribute& Attribute::get(const std::string &name)
{
	if (!s_attrsInit)
	{
		s_attrsInit = true;
		AttributeInit();
	}

	std::map<std::string, Attribute*>::iterator it = s_attrs.find(name);
	if (it == s_attrs.end())
		s_attrs.insert(std::pair<std::string, Attribute*>(name,new Attribute())) ;
	return *s_attrs[name];
}

bool Attribute::operator == (const Attribute& a) const
{
	return this == &a;
}

bool Attribute::operator != (const Attribute& a) const
{
	return this != &a;
}

Attribute& Attribute::addName(const std::string &name)
{
	m_names.insert(name);
	return *this;
}

Attribute& Attribute::delName(const std::string &name)
{
	m_names.erase(name);
	return *this;
}

unsigned int Attribute::nameCount()
{
	return m_names.size();
}

Element::Element() : attr(Attribute::get(Position)), size(0)
{
}

Element::Element(Attribute &attr, unsigned int size) : attr(attr), size(size)
{
}

bool Element::operator == (const Element& e) const
{
	return attr == e.attr && size == e.size;
}

bool Element::operator != (const Element& e) const
{
	return attr != e.attr && size != e.size;
}

const std::string Element::Position     = "POSITION";
const std::string Element::Normal       = "NORMAL";
const std::string Element::Color        = "COLOR";
const std::string Element::Tangent      = "TANGENT";
const std::string Element::Binormal     = "BINORMAL";
const std::string Element::BlendWeight = "BLENDWEIGHTS";
const std::string Element::BlendIndices = "BLENDINDICES";
const std::string Element::TexCoord0    = "TEXCOORD0";
const std::string Element::TexCoord1    = "TEXCOORD1";
const std::string Element::TexCoord2    = "TEXCOORD2";
const std::string Element::TexCoord3    = "TEXCOORD3";
const std::string Element::TexCoord4    = "TEXCOORD4";
const std::string Element::TexCoord5    = "TEXCOORD5";
const std::string Element::TexCoord6    = "TEXCOORD6";
const std::string Element::TexCoord7    = "TEXCOORD7";

Format::Format(const std::vector<Element> &elements)
	: m_elements(elements), m_vertexSize(0)
{
	for (unsigned int i = 0, count = m_elements.size(); i < count; ++i)
	{
		m_vertexSize += m_elements[i].size * sizeof(float);
	}
}

Format::~Format()
{
}

const Element& Format::element(unsigned int index) const
{
	return m_elements[index];
}

unsigned int Format::elementCount() const
{
	return m_elements.size();
}

unsigned int Format::vertexSize() const
{
	return m_vertexSize;
}
bool Format::operator == (const Format& f) const
{
	if (m_elements.size() != f.m_elements.size())
	    return false;

	for (unsigned int i = 0, count = m_elements.size(); i < count; ++i)
	{
	    if (m_elements[i] != f.m_elements[i])
		return false;
	}

	return true;
}

bool Format::operator != (const Format& f) const
{
	return !(this->operator == (f));
}

} // namespace Vertex
} // namespace Guy
