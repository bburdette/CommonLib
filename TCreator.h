#ifndef TCreator_h
#define TCreator_h

// -------------------------------------------------------------------------------------
// Generic factory object.  Contains a map of type names to archetype objects.
// Class T must implement:
// Duplicate()
// operator<<()
// operator>>()
// GetType()

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <Exception.h>
//#include <iconv.h>

#include <locale>


template <class T>
class TCreator
{
public:
	typedef std::map<std::string, T*> TMap;
	void Add(T *aTArchetype)
	{
		// mTm.insert(typename TMap::value_type(aTArchetype->GetType(), aTArchetype));
		Insert(aTArchetype->GetType(), aTArchetype);
	}

	void Insert(const wchar_t *aWcType, T *aTArchetype)
	{
//	    std::locale lLocale;



//	    std::string lS(std::use_facet< std::ctype<wchar_t> >(lLocale).narrow ((wchar_t*)(aWcType), '?'));

        std::wstring lWs(aWcType);

	    std::string lS(lWs.begin(), lWs.end());

		mTm.insert(typename TMap::value_type(lS, aTArchetype));
	}

	void Insert(const char *aCType, T *aTArchetype)
	{
		mTm.insert(typename TMap::value_type(aCType, aTArchetype));
	}

	// Write the NPA together with its type.
	void Write(const T &aT, std::ostream &aOs)
	{
		aOs << aT.GetType() << std::endl << aT << std::endl;
	}

	void GetTypeStrings(std::set<std::string> &aS)
	{
		typename TMap::iterator lIter = mTm.begin();
		while (lIter != mTm.end())
		{
			aS.insert(lIter->first);
			++lIter;
		}
	}

	T* Create(const std::string &aSTypeName)
	{
		typename TMap::iterator lIter = mTm.find(aSTypeName);
		if (lIter == mTm.end())
			throw Common::MessageException("object type not found: ") << aSTypeName.c_str();

		return lIter->second->Duplicate();
	}

	T* Read(std::istream &aIs)
	{
		// Read in the T type, create the appropriate T, read in the T and
		// return same.
		std::string lSType;
		aIs >> lSType;
		T *lT = Create(lSType);
		aIs >> *lT;
		return lT;
	}

	// for if the Duplicate requires an arg.
	template <class U>
	T* Create(const std::string &aSTypeName, U &aU)
	{
		typename TMap::iterator lIter = mTm.find(aSTypeName);
		if (lIter == mTm.end())
			throw Common::MessageException("object type not found!");

		return lIter->second->Duplicate(aU);
	}

	template <class U>
	T* Read(std::istream &aIs, U &aU)
	{
		// Read in the T type, create the appropriate T, read in the T and
		// return same.
		std::string lSType;
		aIs >> lSType;
		T *lT = Create(lSType, aU);
		aIs >> *lT;
		return lT;
	}

private:
	TMap mTm;
};


// Create things just by their type, or by type and an arg.
// Object requirements to be in this object:
//
//
template <class T>
class TACreator
{
public:
	// Create with no default arg.
	template <class U>
	void Add()
	{
		UDuper<U> *lUd = new UDuper<U>;
		mTm.insert(TMap::value_type(U::sCType, lUd));
	}

	// Create with default arg, U.
	template <class U, class A>
	void Add(A *aADefaultArg)
	{
		UADuper<U, A> *lUad = new UADuper<U, A>;
		lUad->mA = aADefaultArg;
		mTm.insert(typename TMap::value_type(U::sCType, lUad));
	}

	T* Create(const std::string &aSTypeName)
	{
		typename TMap::iterator lIter = mTm.find(aSTypeName);
		if (lIter == mTm.end())
			throw Common::MessageException("object type not found: ") << aSTypeName.c_str();

		return lIter->second->Create();
	}

	// for if the Duplicate requires an arg.
	template <class A>
	T* Create(const std::string &aSTypeName, A &aA)
	{
		typename TMap::iterator lIter = mTm.find(aSTypeName);
		if (lIter == mTm.end())
			throw Common::MessageException("object type not found!");

		return lIter->second->Create(aA);
	}

	void GetTypeStrings(std::set<std::string> &aS)
	{
		typename TMap::iterator lIter = mTm.begin();
		while (lIter != mTm.end())
		{
			aS.insert(lIter->first);
			++lIter;
		}
	}

	template <class U>
	T* Read(std::istream &aIs, U &aU)
	{
		// Read in the T type, create the appropriate T, read in the T and
		// return same.
		std::string lSType;
		aIs >> lSType;
		T *lT = Create(lSType, aU);
		aIs >> *lT;
		return lT;
	}

	// Write the NPA together with its type.
	void Write(const T &aT, std::ostream &aOs)
	{
		aOs << aT.GetType() << std::endl << aT << std::endl;
	}

	T* Read(std::istream &aIs)
	{
		// Read in the T type, create the appropriate T, read in the T and
		// return same.
		std::string lSType;
		aIs >> lSType;
		T *lT = Create(lSType);
		aIs >> *lT;
		return lT;
	}

private:

	class Duper
	{
	public:
		virtual T* Create() = 0;
	};

	template <class U>
	class UDuper : public Duper
	{
	public:
		virtual T* Create()
		{
			return new U;
		}
		//template <class A>
		//T* Create(const A &aA)
		//{
		//	return new U(aA);
		//}
	};

	template <class U, class A>
	class UADuper : public Duper
	{
	public:
		A *mA;
		virtual T* Create()
		{
			return new U(*mA);
		}
	};

	typedef std::map<std::string, Duper*> TMap;
	TMap mTm;
};

#endif
