////////////////////////////////////////////////////////////////////
// AutoPtr.h
//
// Copyright 2007 cDc@seacave
// Distributed under the Boost Software License, Version 1.0
// (See http://www.boost.org/LICENSE_1_0.txt)

#ifndef __SEACAVE_AUTOPTR_H__
#define __SEACAVE_AUTOPTR_H__


// I N C L U D E S /////////////////////////////////////////////////


// D E F I N E S ///////////////////////////////////////////////////


namespace SEACAVE {

// S T R U C T S ///////////////////////////////////////////////////

/**************************************************************************************
 * CAutoPtr template
 * ---------------
 * simple smart pointer
 **************************************************************************************/

struct AutoPtrMoveCopy {
	template <typename TYPE>
	static inline void Copy(TYPE*& ptrLeft, TYPE*& ptrRight) {
		ptrLeft = ptrRight;
		ptrRight = NULL;
	}
};

struct AutoPtrDeepCopy {
	template <typename TYPE>
	static inline void Copy(TYPE*& ptrLeft, TYPE*& ptrRight) {
		ptrLeft = (ptrRight != NULL ? new TYPE(*ptrRight) : (TYPE*)NULL);
	}
};

template<typename TYPE, typename CopyPolicy=AutoPtrMoveCopy>
class CAutoPtr
{
private:
	typedef TYPE*			TYPE_PTR;

public:
	inline CAutoPtr() : m_pointer(NULL)
	{	// construct with NULL pointer
	}

	inline explicit CAutoPtr(TYPE_PTR _Ptr) : m_pointer(_Ptr)
	{	// construct from object pointer
	}

	inline CAutoPtr(CAutoPtr& _Right)
	{	// copy-construct by assuming pointer from _Right CAutoPtr
		CopyPolicy::Copy(m_pointer, _Right.m_pointer);
	}

	inline ~CAutoPtr()
	{	// destroy the object
		delete m_pointer;
	}

	void Swap(CAutoPtr& _Right)
	{	// swap compatible _Right (assume pointer)
		const TYPE_PTR tmp(m_pointer);
		m_pointer = _Right.m_pointer;
		_Right.m_pointer = tmp;
	}

	CAutoPtr& operator=(CAutoPtr& _Right)
	{	// assign compatible _Right (assume pointer)
		if (this != &_Right) {
			delete m_pointer;
			CopyPolicy::Copy(m_pointer, _Right.m_pointer);
		}
		return (*this);
	}

	CAutoPtr& operator=(TYPE_PTR _Ptr)
	{	// assign compatible _Right (assume pointer)
		if (m_pointer != _Ptr) {
			delete m_pointer;
			m_pointer = _Ptr;
		}
		return (*this);
	}

	inline TYPE& operator*() const
	{	// return designated value
		ASSERT(m_pointer);
		return (*m_pointer);
	}

	inline TYPE* operator->() const
	{	// return pointer to class object
		ASSERT(m_pointer);
		return m_pointer;
	}

	inline operator TYPE_PTR() const
	{	// return pointer to class object
		return m_pointer;
	}

	inline operator TYPE_PTR&()
	{	// return reference to class object
		return m_pointer;
	}

	inline bool operator==(const CAutoPtr& _Right) const
	{	// return pointer to class object
		return (m_pointer == _Right.m_pointer);
	}

	inline bool operator!=(const CAutoPtr& _Right) const
	{	// return pointer to class object
		return (m_pointer != _Right.m_pointer);
	}

	inline bool operator==(const void* _Right) const
	{	// return pointer to class object
		return (m_pointer == _Right);
	}

	inline bool operator!=(const void* _Right) const
	{	// return pointer to class object
		return (m_pointer != _Right);
	}

	inline void Release()
	{	// release pointer
		delete m_pointer;
		m_pointer = NULL;
	}

	inline void Reset(TYPE_PTR _Ptr = NULL)
	{	// reset pointer
		m_pointer = _Ptr;
	}

private:
	TYPE_PTR	m_pointer;		// the wrapped object pointer

#ifdef _USE_BOOST
protected:
	// implement BOOST serialization
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive& ar, const unsigned int /*version*/) const
	{
		ar & m_pointer;
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int /*version*/)
	{
		TYPE_PTR newPointer;
		ar & newPointer;
		operator=(newPointer);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif
};


template<class TYPE>
class CAutoPtrArr
{
private:
	typedef TYPE*			TYPE_PTR;

public:
	inline CAutoPtrArr() : m_pointer(NULL)
	{	// construct with NULL pointer
	}

	inline explicit CAutoPtrArr(TYPE_PTR _Ptr) : m_pointer(_Ptr)
	{	// construct from object pointer
	}

	inline CAutoPtrArr(CAutoPtrArr& _Right) : m_pointer(_Right.m_pointer)
	{	// construct by assuming pointer from _Right CAutoPtrArr
		_Right.m_pointer = NULL;
	}

	inline ~CAutoPtrArr()
	{	// destroy the object
		delete[] m_pointer;
	}

	CAutoPtrArr& operator=(CAutoPtrArr& _Right)
	{	// assign compatible _Right (assume pointer)
		if (this != &_Right)
		{
			delete[] m_pointer;
			m_pointer = _Right.m_pointer;
			_Right.m_pointer = NULL;
		}
		return (*this);
	}

	CAutoPtrArr& operator=(TYPE_PTR _Ptr)
	{	// assign compatible _Right (assume pointer)
		if (m_pointer != _Ptr)
		{
			delete[] m_pointer;
			m_pointer = _Ptr;
		}
		return (*this);
	}

	inline TYPE& operator*() const
	{	// return designated value
		ASSERT(m_pointer);
		return (*m_pointer);
	}

	inline TYPE* operator->() const
	{	// return pointer to class object
		ASSERT(m_pointer);
		return m_pointer;
	}

	inline operator TYPE_PTR() const
	{	// return pointer to class object
		return m_pointer;
	}

	inline operator TYPE_PTR&()
	{	// return reference to class object
		return m_pointer;
	}

	inline bool operator==(const CAutoPtrArr& _Right) const
	{	// return pointer to class object
		return (m_pointer == _Right.m_pointer);
	}

	inline bool operator!=(const CAutoPtrArr& _Right) const
	{	// return pointer to class object
		return (m_pointer != _Right.m_pointer);
	}

	inline bool operator==(const void* _Right) const
	{	// return pointer to class object
		return (m_pointer == _Right);
	}

	inline bool operator!=(const void* _Right) const
	{	// return pointer to class object
		return (m_pointer != _Right);
	}

	inline void Release()
	{	// release pointer
		delete[] m_pointer;
		m_pointer = NULL;
	}

	inline void Reset(TYPE_PTR _Ptr = NULL)
	{	// reset pointer
		m_pointer = _Ptr;
	}

private:
	TYPE_PTR	m_pointer;		// the wrapped object pointer

#ifdef _USE_BOOST
protected:
	// implement BOOST serialization
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive& ar, const unsigned int /*version*/) const
	{
		ar & m_pointer;
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int /*version*/)
	{
		TYPE_PTR newPointer;
		ar & newPointer;
		operator=(newPointer);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif
};
/*----------------------------------------------------------------*/

} // namespace SEACAVE

#endif // __SEACAVE_AUTOPTR_H__
