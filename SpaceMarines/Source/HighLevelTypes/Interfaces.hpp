#pragma once

namespace SpaceMarines
{

class Dirtiable
{
public:
	bool isDirty() const { return dirty; }
protected:
	void setDirty(bool dirty) { this->dirty = dirty; }
private:
	bool dirty;
};

}
