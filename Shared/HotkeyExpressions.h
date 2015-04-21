#ifndef HOTKEY_EXPRESSIONS_H_INCLUDED
#define HOTKEY_EXPRESSIONS_H_INCLUDED

#include <string>
#include <memory>
#include <functional>

class InputControls;

class InputBinding
{
public:
	virtual ~InputBinding();
	virtual bool IsPressed(InputControls &controls)=0;
	virtual std::string ToString()=0;
};

typedef std::shared_ptr<InputBinding> InputBindingRef;

class UnboundKeyBinding
	: public InputBinding
{
public:
	bool IsPressed(InputControls &controls);
	std::string ToString();
};

class SimpleKeyBinding
	: public InputBinding
{
public:
	SimpleKeyBinding(int key);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	int getKeyIndex() { return keyIndex; }
	
private:
	int keyIndex;
};

class SimpleButtonBinding
	: public InputBinding
{
public:
	SimpleButtonBinding(int button);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	
private:
	int buttonIndex;
};

class InputExpressionBinding
	: public InputBinding
{
public:
	InputExpressionBinding(std::string description, std::function<bool(InputControls&)> func);
	bool IsPressed(InputControls &controls);
	std::string ToString();

private:
	std::string description;
	std::function<bool(InputControls&)> func;
};

class CombinationKeyBinding
	: public InputBinding
{
public:
	CombinationKeyBinding(InputBindingRef first, InputBindingRef second);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

class AlternativesKeyBinding
	: public InputBinding
{
public:
	AlternativesKeyBinding(InputBindingRef first, InputBindingRef second);
	bool IsPressed(InputControls &controls);
	std::string ToString();
	
private:
	InputBindingRef first;
	InputBindingRef second;
};

namespace HotkeyExpressions
{
	InputBindingRef Key(int keyIndex);
	InputBindingRef Button(int buttonIndex);
	InputBindingRef operator+(InputBindingRef lhs, InputBindingRef rhs);
	InputBindingRef operator||(InputBindingRef lhs, InputBindingRef rhs);
	InputBindingRef Unbound();
}

#endif