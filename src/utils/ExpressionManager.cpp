#include "ExpressionManager.h"
#include <diesel.h>

ExpressionManager::ExpressionManager() {
	_vmCtx = vm_create_context();
	vm_add_variable(_vmCtx, "TIMER", 0.0f);
}

int ExpressionManager::parse(const char * expression) {
	Expression exp;
	exp.num = vm_parse(_vmCtx, expression, exp.tokens, 64);
	_expressions.push_back(exp);
	return _expressions.size() - 1;
}

float ExpressionManager::run(int index) {
	Expression& exp = _expressions[index];
	vm_set_variable(_vmCtx, "TIMER", 2.0f);

	float r = 0.0f;
	int code = vm_run(_vmCtx, exp.tokens, exp.num, &r);
	if (code == 0) {
		DBG_LOG("Result: %g", r);
	}
	else {
		DBG_LOG("Error: %s", vm_get_error(code));
	}

	return r;
}
