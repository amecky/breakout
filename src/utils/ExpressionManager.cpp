#include "ExpressionManager.h"
#include <diesel.h>

ExpressionManager::ExpressionManager() {
	_vmCtx = vm_create_context();
	vm_add_variable(_vmCtx, "TIMER", 0.0f);
}

ExpressionManager::~ExpressionManager() {
	vm_destroy_context(_vmCtx);
}

void ExpressionManager::setVariable(const char * name, float value) {
	vm_set_variable(_vmCtx, name, value);
}

int ExpressionManager::parse(const char * expression) {
	Expression exp;
	exp.num = vm_parse(_vmCtx, expression, exp.tokens, 64);
	_expressions.push_back(exp);
	return _expressions.size() - 1;
}

void ExpressionManager::parse(int expressionID, const char * expression) {
	Expression& exp = _expressions[expressionID];
	exp.num = vm_parse(_vmCtx, expression, exp.tokens, 64);
}

float ExpressionManager::run(int index) {
	Expression& exp = _expressions[index];	
	float r = 0.0f;
	int code = vm_run(_vmCtx, exp.tokens, exp.num, &r);
	if (code != 0) {
		DBG_LOG("Error: %s", vm_get_error(code));
	}
	return r;
}
