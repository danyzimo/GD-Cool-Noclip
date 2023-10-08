#pragma once

#include "includes.h"


class b1_layer : public CCLayer {
protected:
	bool init() {
		this->setTouchEnabled(true);
		return true;
	}

public:
	void b1_layer::create2(CCObject*);
	void b1_layer::create3(CCObject*);
};