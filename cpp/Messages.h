#pragma once

#include "serdes.h"

#include <list>
#include <vector>

#define M_ALLOCATELIST(ClassName, variableName)
#define M_DECLAREVARIABLE(ClassName, variableName) ClassName variableName
#define M_DECLARELIST(ClassName, variableName) ClassName variableName
#define M_DECLAREJSONLIST(variableName) json variableName
#define M_BASECLASS(baseClass,func)		baseClass::func
#define	M_LISTFORLOOPSTART(iterator, List) for(auto& iterator:List) {
#define	M_LISTLEN(list) (int)list.size()
#define	M_LISTPUSHBACK(List, item) List.push_back(item)
#define	M_CPPONLY(cppline) cppline
#define	M_SIZE(_name) (int)_name.size()
#define	M_ELEMENT(_vector, index) _vector[index]
#define M_JSON	json
#define M_JSONL	json
#define M_JSONREF	json&
#define M_JSONNULL 0
#define M_JSONPAIR(key, value)	{ key, value }
#define M_JSONPUSHBACK(jlist, value)	jlist.push_back(value)
#define M_ENUM(_enum, value) _enum::value
#define M_ENUMINDEX(value) value
#define M_FIND(base, word) (base.find(word)!=std::string::npos)
#define M_INTTODARTENUM(base, word) (base)word
#define M_BYREF(value, other) value& other
#define M_SERDES "serdes.h"
#define M_JSONORDYNAMIC json

template <typename T>
class List : public std::list<T> {
public:
	void add(T t) { return std::list<T>::push_back(t); }
};

typedef std::string String;
typedef SerDes& RSerDes;

// -----------------------------------------------------------------------
// Json is also used in messages include path uere
#include "json.hpp"
using json = nlohmann::json;

// -----------------------------------------------------------------------
// Messages are based on Common code used by both C++ and Dart
// The following include file has the message declarations.
// See the comments in the include file for further information

#include "MessagesCommon.h"
#include "HyperCubeMessagesCommon.h"

// -----------------------------------------------------------------------

