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
	static Attribute& get(const std::string &name);

	bool operator == (const Attribute& a) const;
	bool operator != (const Attribute& a) const;

	typedef std::set<std::string>::const_iterator iterator;
	iterator begin() { return m_names.begin();}
	iterator end()   { return m_names.end();  }

	Attribute& addName(const std::string &name);
	Attribute& delName(const std::string &name);
	unsigned int nameCount();
private:
	Attribute();
	Attribute(const Attribute &a);
	Attribute& operator=(const Attribute &a);

	std::set<std::string> m_names;
};

class Element {
public:
	Attribute &attr;
	unsigned int size;

	Element();
	Element(Attribute &attr, unsigned int size);
	bool operator == (const Element& e) const;
	bool operator != (const Element& e) const;

	static const std::string Position;
	static const std::string Normal;
	static const std::string Color;
	static const std::string Tangent;
	static const std::string Binormal;
	static const std::string BlendWeight;
	static const std::string BlendIndices;
	static const std::string TexCoord0;
	static const std::string TexCoord1;
	static const std::string TexCoord2;
	static const std::string TexCoord3;
	static const std::string TexCoord4;
	static const std::string TexCoord5;
	static const std::string TexCoord6;
	static const std::string TexCoord7;
};

class Format
{
public:

	Format(const std::vector<Element> &elements);
	~Format();

	const Element& element(unsigned int index) const;
	unsigned int elementCount() const;
	unsigned int vertexSize() const;
	bool operator == (const Format& f) const;
	bool operator != (const Format& f) const;

private:

	std::vector<Element> m_elements;
	unsigned int m_vertexSize;
};

} // namespace Vertex

} // namespace Guy
