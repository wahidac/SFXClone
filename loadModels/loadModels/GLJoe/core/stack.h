/**
 * @file GLJoe/core/stack.h
 * 
 * @brief Stack
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */
 
#ifndef DEF_GLJOE_STACK_H
#define DEF_GLJOE_STACK_H

namespace GLJoe
{

template<class T>
class Stack_
{
protected:
	T* _stack; // The stack is an array
	size_t _size; // Capacity of the stack
	unsigned _top; // Current matrix
	
	/**
	 * Stack operations
	 */
	bool full()
	{
		return _top == _size;
	}
	
	bool empty()
	{
		return _top == 0;
	}
	
public:
	
	/**
	 * Constructors
	 */
	Stack_<T>(size_t capacity = 100) :
		_size(capacity), _top(0)
	{
		_stack = new T[_size];
	}
	
	Stack_<T>(const Stack_<T>& ms) :
		_size(ms._size), _top(ms._top)
	{
		_stack = new T[_size];
		for (unsigned i = 0; i < _size; ++i)
		{
			_stack[i] = ms._stack[i];
		}
	}
	
	/**
	 * Destructor
	 */
	~Stack_<T>()
	{
		delete[] _stack;
	}
	
	/**
	 * Operators
	 */
	 
	Stack_<T>& operator=(const Stack_<T>& ms)
	{		
		_size(ms._size);
		_top(ms._top);
		_stack = new T[_size];
		for (unsigned i = 0; i < _size; ++i)
		{
			_stack[i] = ms._stack[i];
		}
		
		return *this;
	}
	
	/**
	 * Stack operations
	 */
	
	// Push
	void push(const T& m)
	{
		if (full())
		{
			Warning("Stack is full");
		}
		else
		{
			_stack[_top++] = m;
		}
	}
	
	// Pop
	void pop()
	{
		if (empty())
		{
			Warning("Stack is empty");
		}
		else
		{
			_top--;
		}
	}
	
	// Top
	const T& top() const
	{
		if (empty())
			Error("Stack is empty");
		
		return _stack[_top - 1];
	}
	
	T& top()
	{
		if (empty())
			Error("Stack is empty");
		
		return _stack[_top - 1];
	}
	
	// Clear
	void clear()
	{
		_top = 0;
	}
	
	// Size
	size_t size() const
	{
		return _size;
	}
	
};

	
} // namespace GLJoe

#endif // DEF_GLJOE_MATRIXSTACK_H

