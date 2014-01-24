#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>

namespace Guy
{

namespace Vertex
{

class Attribute {
public:
	static Attribute& get(std::size_t _id);
	static Attribute& get(const std::string &_name);

	std::size_t ID();

	bool operator == (const Attribute& _a) const;
	bool operator != (const Attribute& _a) const;

	typedef std::set<std::string>::const_iterator iterator;
	iterator begin() { return m_names.begin();}
	iterator end()   { return m_names.end();  }

	Attribute& addName(const std::string &_name);

	enum  { //Build-in default attributes
		Position = 0,
		Normal,
		Color,
		Tangent,
		Binormal,
		BlendWeight,
		BlendIndices,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,
		UserAttribute
	};

private:
	Attribute(std::size_t  _id);
	std::size_t m_id;

	static void init();
	static bool s_attrsInit;
	static std::map<std::string, Attribute*> s_names;
	static std::vector<Attribute*> s_attrs;

	std::set<std::string> m_names;
};

class Element {
public:
	Attribute &attr;
	std::size_t size;

	Element();
	Element(Attribute &_attr, unsigned int _size);
	bool operator == (const Element& e) const;
	bool operator != (const Element& e) const;
};

class Format
{
public:

	Format(const std::vector<Element> &elements);
	~Format();

	const Element& element(std::size_t index) const;
	std::size_t elementCount() const;
	std::size_t vertexSize() const;
	bool operator == (const Format& f) const;
	bool operator != (const Format& f) const;

private:

	std::vector<Element> m_elements;
	std::size_t m_vertexSize;
};

} // namespace Vertex

} // namespace Guy
