#include <cassert>
#include <cstdint>
#include <iostream>
using namespace std;

struct TypeID
{
	inline bool operator==(const TypeID& inOther) const
	{
		return Value == inOther.Value;
	}

	const void* Value;
};

class ITypeTrait
{
protected:
	ITypeTrait() = default;
	virtual ~ITypeTrait() = default;
};

template <
	typename Type,
	typename = std::is_class<Type>,
	typename = std::enable_if_t<!std::is_pointer_v<Type>>
>
class TypeTrait : public ITypeTrait
{
public:
	constexpr static TypeID ID()
	{
		return m_typeID;
	}

private:
	constexpr static char m_value{};
	constexpr static TypeID m_typeID{ &m_value };
};

//TypeID 해시 특수화
namespace std
{
	template<>
	struct hash<TypeID>
	{
		size_t operator()(const TypeID& typeId) const
		{
			return std::hash<const void*>{}(typeId.Value);
		}
	};
}

int main() {
	constexpr const void* id_int = TypeTrait<int>::ID().Value;
	constexpr const void* id_int_again = TypeTrait<int>::ID().Value;
	constexpr const void* id_float = TypeTrait<float>::ID().Value;
	constexpr const void* id_double = TypeTrait<double>::ID().Value;
	constexpr const void* id_char = TypeTrait<char>::ID().Value;

	static_assert(id_int == id_int_again);
	static_assert(id_int != id_float);

	std::cout << id_int << std::endl;
	std::cout << id_float << std::endl;
	std::cout << id_double << std::endl;
	std::cout << id_char << std::endl;

	assert(id_int == id_int_again);

	return 0;
}