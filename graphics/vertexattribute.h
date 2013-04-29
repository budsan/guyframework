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
    static Attribute& get(int ID);
    static Attribute& get(const std::string &name);

    int ID();

    bool operator == (const Attribute& a) const;
    bool operator != (const Attribute& a) const;

    typedef std::set<std::string>::const_iterator iterator;
    iterator begin() { return m_names.begin();}
    iterator end()   { return m_names.end();  }

    Attribute& addName(const std::string &name);

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
    Attribute(int ID);
    int m_id;

    static void init();
    static bool s_attrsInit;
    static std::map<std::string, Attribute*> s_names;
    static std::vector<Attribute*> s_attrs;

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
