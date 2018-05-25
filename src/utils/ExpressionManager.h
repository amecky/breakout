#pragma once
#include <ds_vm.h>
#include <vector>

struct Expression {
	vm_token tokens[64];
	int num;

};
class ExpressionManager {

public:
	ExpressionManager();
	int parse(const char* expression);
	float run(int index);
private:
	std::vector<Expression> _expressions;
	vm_context* _vmCtx;
	uint16_t _timerId;
};
