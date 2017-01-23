#include "store.hpp"

namespace ve
{
	extern OwnPtr<Store> store;

	Ptr<Store> getStore()
	{
		return store;
	}
}