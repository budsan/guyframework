#include "vertexattribute.h"
#include "log.h"

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

bool Attribute::s_attrsInit = false;
std::map<std::string, Attribute*> Attribute::s_names;
std::vector<Attribute*> Attribute::s_attrs;

void Attribute::init()
{
    //Default names for default elements
    s_attrs.resize(Attribute::UserAttribute);

    s_attrs[Attribute::Position] = new Attribute(Attribute::Position);
    s_attrs[Attribute::Position]->addName(VERTEX_ATTRIBUTE_POSITION_NAME);

    s_attrs[Attribute::Normal] = new Attribute(Attribute::Normal);
    s_attrs[Attribute::Normal]->addName(VERTEX_ATTRIBUTE_NORMAL_NAME);

    s_attrs[Attribute::Color] = new Attribute(Attribute::Color);
    s_attrs[Attribute::Color]->addName(VERTEX_ATTRIBUTE_COLOR_NAME);

    s_attrs[Attribute::Tangent] = new Attribute(Attribute::Tangent);
    s_attrs[Attribute::Tangent]->addName(VERTEX_ATTRIBUTE_TANGENT_NAME);

    s_attrs[Attribute::Binormal] = new Attribute(Attribute::Binormal);
    s_attrs[Attribute::Binormal]->addName(VERTEX_ATTRIBUTE_BINORMAL_NAME);

    s_attrs[Attribute::BlendWeight] = new Attribute(Attribute::BlendWeight);
    s_attrs[Attribute::BlendWeight]->addName(VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME);

    s_attrs[Attribute::BlendIndices] = new Attribute(Attribute::BlendIndices);
    s_attrs[Attribute::BlendIndices]->addName(VERTEX_ATTRIBUTE_BLENDINDICES_NAME);

    s_attrs[Attribute::TexCoord0] = new Attribute(Attribute::TexCoord0);
    s_attrs[Attribute::TexCoord0]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME);
    s_attrs[Attribute::TexCoord0]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"0");

    s_attrs[Attribute::TexCoord1] = new Attribute(Attribute::TexCoord1);
    s_attrs[Attribute::TexCoord1]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"1");

    s_attrs[Attribute::TexCoord2] = new Attribute(Attribute::TexCoord2);
    s_attrs[Attribute::TexCoord2]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"2");

    s_attrs[Attribute::TexCoord3] = new Attribute(Attribute::TexCoord3);
    s_attrs[Attribute::TexCoord3]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"3");

    s_attrs[Attribute::TexCoord4] = new Attribute(Attribute::TexCoord4);
    s_attrs[Attribute::TexCoord4]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"4");

    s_attrs[Attribute::TexCoord5] = new Attribute(Attribute::TexCoord5);
    s_attrs[Attribute::TexCoord5]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"5");

    s_attrs[Attribute::TexCoord6] = new Attribute(Attribute::TexCoord6);
    s_attrs[Attribute::TexCoord6]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"6");

    s_attrs[Attribute::TexCoord7] = new Attribute(Attribute::TexCoord7);
    s_attrs[Attribute::TexCoord7]->addName(VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME"7");

    s_attrsInit = true;
}

Attribute::Attribute(int id) : m_id(id)
{

}

Attribute& Attribute::get(int id)
{
    if (!s_attrsInit) init();
    GUY_ASSERT(id >= 0 && id < s_attrs.size());
    return *s_attrs[id];
}

Attribute& Attribute::get(const std::string &name)
{
    if (!s_attrsInit) init();
    std::map<std::string, Attribute*>::iterator it = s_names.find(name);
    if (it == s_names.end()) {
        s_attrs.push_back(new Attribute(s_attrs.size()));
        it = s_names.insert(std::pair<std::string, Attribute*>(name,s_attrs.back())).first;
    }

    return *(it->second);
}

bool Attribute::operator == (const Attribute& a) const
{
    return m_id == a.m_id;
}

bool Attribute::operator != (const Attribute& a) const
{
    return m_id != a.m_id;
}

Attribute& Attribute::addName(const std::string &name)
{
    std::pair<std::map<std::string, Attribute*>::iterator, bool>
            result = s_names.insert(std::pair<std::string, Attribute*>(name, this));
    if (result.second) m_names.insert(name);

    return *this;
}

Element::Element() : attr(Attribute::get(Attribute::Position)), size(0)
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
