#ifndef QUAT_TABLE_H
#define QUAT_TABLE_H
#include <vector>
#include <string>
using namespace std;


enum class QuatOpType
{
    addOp,
    minusOp,
    mulOp,
    divOp,
    modOp,
    andOp,
    orOp,
    notOp,
    assignOp,
    jzOp,
    gotoOp,
    equOp,
    neqOp,
    ltOp,
    gtOp,
    noneOp
};

struct Quat
{
    QuatOpType optype;
    int operandIdx1;
    int operandIdx2;
    int resultIdx;
    Quat(QuatOpType optype = QuatOpType::noneOp, int operandId1x = -1, int operandIdx2 = -1, int resultIdx = -1) 
    {
        this->optype = optype;
        this->operandIdx1 = operandId1x;
        this->operandIdx2 = operandIdx2;
        this->resultIdx = resultIdx;
    }
};

class QuatTable
{
public:
    vector<Quat> table;
    void addQuat(Quat q);
    void setQuat(int idx, Quat q);
    int size();
    void print(string filename);
};

#endif