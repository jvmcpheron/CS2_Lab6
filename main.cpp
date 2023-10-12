

//Jane McPheron


/*
./main
Expression: 1
+
1
;
1 + 1 ; 
Postfix version of expression: + 1 1 ; 
Result is 2

./main
Expression: 2
+
3
*
5
;
2 + 3 * 5 ; 
Postfix version of expression: + 2 * 3 5 ; 
Result is 17

*/


#include <iostream>
#include <string>
#include <list>
#include <math.h>
using namespace std;

    const int NUMBER = 0;
    const int LPAREN = 1;
    const int RPAREN = 2;
    const int ADD = 3;
    const int MINUS = 4;
    const int TIMES = 5;
    const int DIVIDE = 6;
    const int POWER = 7;
    const int SEMI = 8;
    const string OPERATOR_STRINGS[] = {"NUMBER", "(", ")", "+", "-", "*", "/", "^", ";"};
    const int OPERATOR_PRECEDENCE[] = {-1, 0, 0, 1, 1, 2, 2, 3, 0};
    const bool OPERATOR_LEFT_ASSOCIATIVE[] = { false, false, false, true, true, true, true, false, false};

    class ExpressionPart {
    protected:
        int eType;

        ExpressionPart() {}

    public:
        ExpressionPart(int et) { eType = et; }

        int getEType() { return eType; }

        void setEType(int et) { eType = et; }

        virtual void print() {}
    };

    class ExpressionNumber : public ExpressionPart {
    protected:
        double number;

        ExpressionNumber() {}

    public:
        ExpressionNumber(double n) : ExpressionPart(NUMBER) {
            number = n;
        }

        ExpressionNumber(string s) {
            try {
                number = stoi(s);
                setEType(NUMBER);
            }
            catch (exception e) {
                throw "NOT A NUMBER";
            }
        }

        double getNumber() { return number; }
        void print() {
            cout << number << " ";
        }
    };

    class ExpressionOperator : public ExpressionPart {
    protected:

        ExpressionOperator() {}

    public:
        ExpressionOperator(string s) {
            int op;
            for (op = LPAREN; op <= POWER; op++) {
                if (s.compare(OPERATOR_STRINGS[op]) == 0) {
                    break;
                }
            }
            if (op > SEMI) {
                throw ("Unknown operator " + s);
            }
            setEType(op);
        }
        void print() {
            cout << OPERATOR_STRINGS[getEType()] << " ";
        }

    };

const string INFIX_FORMAT_ERROR = "Expression not in infix format";

list<ExpressionPart*> readExpr() {
    cout << "Expression: ";
    list<ExpressionPart*> expressions;
    ExpressionPart *epart = NULL;
    string theString;
    do {
        cin >> theString;
        try {
            double n = stoi(theString);
            epart = new ExpressionNumber(n);
        }
        catch (exception e) {
            try {
                epart = new ExpressionOperator(theString);
            }
            catch (exception e) {
                cerr << "ERROR!  Unknown expression part " << theString << endl;
                break;
            }
        }
        expressions.push_back(epart);
    } while (epart->getEType() != SEMI);
    return expressions;
}

void showExpr(list<ExpressionPart*> expressions) {
    for (auto ep : expressions) {
        ep->print();
    }
    cout << endl;
}

void evalPostfixExpr(list<ExpressionPart*> expressions) {
    list<ExpressionPart*> exprStack;
    for (auto ep : expressions) {
        switch (ep->getEType()) {
            case SEMI:
                if (exprStack.size() == 1) {
                    ExpressionPart* res = exprStack.front();
                    if (res->getEType() == NUMBER) {
                        cout << "Result is " << ((ExpressionNumber*) res)->getNumber() << endl;
                    }
                    else {
                        throw INFIX_FORMAT_ERROR;
                    }
                }
                else {
                    throw INFIX_FORMAT_ERROR;
                }
                break;
            case NUMBER:
                exprStack.push_front(ep);
                break;
            case LPAREN:
            case RPAREN:
                throw INFIX_FORMAT_ERROR;
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                ExpressionPart *rgt = exprStack.front();
                exprStack.pop_front();
                ExpressionPart *lft = exprStack.front();
                exprStack.pop_front();
                if ((lft->getEType() != NUMBER) && (rgt->getEType() != NUMBER)) {
                    throw INFIX_FORMAT_ERROR;
                }
                double res = ((ExpressionNumber*)lft)->getNumber();
                switch (ep->getEType()) {
                    case ADD:
                        res += ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case MINUS:
                        res -= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case TIMES:
                        res *= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case DIVIDE:
                        res /= ((ExpressionNumber *) rgt)->getNumber();
                        break;
                    case POWER:
                        res = pow(res, ((ExpressionNumber *) rgt)->getNumber());
                        break;
                }
                exprStack.push_front(new ExpressionNumber(res));
                break;
        }
    }
}



void evalPrefixExpr(list<ExpressionPart*> expressions) {
    list<ExpressionPart*> exprStack;
    for (auto ep = expressions.rbegin(); ep != expressions.rend(); ++ep) {
        switch ((*ep)->getEType()) {
            case SEMI:
                if (exprStack.size() == 1) {
                    ExpressionPart* res = exprStack.front();
                    if (res->getEType() == NUMBER) {
                        cout << "Result is " << ((ExpressionNumber*) res)->getNumber() << endl;
                    } else {
                        cout << "1";
                        throw INFIX_FORMAT_ERROR;
                    }
                } else {
                    
                }
                break;
            case NUMBER:
                exprStack.push_front(*ep);
                break;
            case LPAREN:
            case RPAREN:
                cout<<"3";
                throw INFIX_FORMAT_ERROR;
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                if (exprStack.size() < 2) {
                    cout<<"4";
                    throw INFIX_FORMAT_ERROR;
                }
                ExpressionPart *lft = exprStack.front();
                exprStack.pop_front();
                ExpressionPart *rgt = exprStack.front();
                exprStack.pop_front();
                if (lft->getEType() != NUMBER || rgt->getEType() != NUMBER) {
                    cout<<"5";
                    throw INFIX_FORMAT_ERROR;
                }
                double res = 0;
                switch ((*ep)->getEType()) {
                    case ADD:
                        res = ((ExpressionNumber*)lft)->getNumber() + ((ExpressionNumber*)rgt)->getNumber();
                        break;
                    case MINUS:
                        res = ((ExpressionNumber*)lft)->getNumber() - ((ExpressionNumber*)rgt)->getNumber();
                        break;
                    case TIMES:
                        res = ((ExpressionNumber*)lft)->getNumber() * ((ExpressionNumber*)rgt)->getNumber();
                        break;
                    case DIVIDE:
                        res = ((ExpressionNumber*)lft)->getNumber() / ((ExpressionNumber*)rgt)->getNumber();
                        break;
                    case POWER:
                        res = pow(((ExpressionNumber*)lft)->getNumber(), ((ExpressionNumber*)rgt)->getNumber());
                        break;
                }
                exprStack.push_front(new ExpressionNumber(res));
                break;
        }
    }

    if (exprStack.size() == 1) {
        ExpressionPart* result = exprStack.front();
        if (result->getEType() == NUMBER) {
            cout << "Result is " << ((ExpressionNumber*)result)->getNumber() << endl;
        } else {
            cout<<"6";
            throw INFIX_FORMAT_ERROR;
        }
    } else {
        cout<<"7";
        throw INFIX_FORMAT_ERROR;
    }
}




list<ExpressionPart*> convertToPostfix(list<ExpressionPart*> ifExprs) {
    list<ExpressionPart*> pfExprs;
    list<ExpressionPart*> opStack;
    for (auto ep : ifExprs) {
        switch (ep->getEType()) {
            case SEMI:
                while (!opStack.empty()) {
                    ExpressionPart* opp = opStack.front();
                    if (opp->getEType() == LPAREN) {
                        throw INFIX_FORMAT_ERROR;
                    }
                    else {
                        pfExprs.push_back(opp);
                    }
                    opStack.pop_front();
                }
                pfExprs.push_back(ep);
                return pfExprs;
                break;
            case LPAREN:
                opStack.push_front(ep);
                break;
            case RPAREN:
                while ((!opStack.empty()) && (opStack.front()->getEType() != LPAREN)) {
                    pfExprs.push_back(opStack.front());
                    opStack.pop_front();
                }
                if (opStack.empty()) {
                    throw INFIX_FORMAT_ERROR;
                }
                else {
                    opStack.pop_front();
                }
                break;
            case NUMBER:
                pfExprs.push_back(ep);
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                while ((!opStack.empty()) &&
                        (opStack.front()->getEType() != LPAREN) &&
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] > OPERATOR_PRECEDENCE[ep->getEType()]) ||
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] == OPERATOR_PRECEDENCE[ep->getEType()]) && OPERATOR_LEFT_ASSOCIATIVE[ep->getEType()]))) {
                    ExpressionPart *opp = opStack.front();
                    pfExprs.push_back(opp);
                    opStack.pop_front();
                }
                opStack.push_front(ep);
                break;
        }
    }
    pfExprs.clear();
    return pfExprs;
}


list<ExpressionPart*> convertToPrefix(list<ExpressionPart*> ifExprs) {
    list<ExpressionPart*> pfExprs;
    list<ExpressionPart*> opStack;

    ifExprs.reverse();

    for (auto ep : ifExprs) {
        switch (ep->getEType()) {
            case SEMI:
                while (!opStack.empty()) {
                    ExpressionPart* opp = opStack.front();
                    if (opp->getEType() == RPAREN) {
                        throw INFIX_FORMAT_ERROR;
                    }
                    else {
                        pfExprs.push_back(opp);
                    }
                    opStack.pop_front();
                }
                pfExprs.push_back(ep);
                break;
            case LPAREN:
                opStack.push_front(ep);
                break;
            case RPAREN:
                while ((!opStack.empty()) && (opStack.front()->getEType() != RPAREN)) {
                    pfExprs.push_back(opStack.front());
                    opStack.pop_front();
                }
                if (opStack.empty() || opStack.front()->getEType() != LPAREN) {
                    throw INFIX_FORMAT_ERROR;
                }
                else {
                    opStack.pop_front();
                }
                break;
            case NUMBER:
                pfExprs.push_back(ep);
                break;
            case ADD:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case POWER:
                while ((!opStack.empty()) &&
                        (opStack.front()->getEType() != LPAREN) &&
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] > OPERATOR_PRECEDENCE[ep->getEType()]) ||
                        ((OPERATOR_PRECEDENCE[opStack.front()->getEType()] == OPERATOR_PRECEDENCE[ep->getEType()]) && OPERATOR_LEFT_ASSOCIATIVE[ep->getEType()]))) {
                    ExpressionPart *opp = opStack.front();
                    pfExprs.push_back(opp);
                    opStack.pop_front();
                }
                opStack.push_front(ep);
                break;
        }
    }

    while (!opStack.empty()) {
        pfExprs.push_back(opStack.front());
        opStack.pop_front();
    }

    // Reverse the output expression to get the prefix order
    pfExprs.reverse();

    return pfExprs;
}





int main() {
    try {
        list<ExpressionPart *> expressions = readExpr();
        showExpr(expressions);
        list<ExpressionPart *> pfExprs = convertToPrefix(expressions);
        cout << "Postfix version of expression: ";
        showExpr(pfExprs);
        evalPrefixExpr(pfExprs);
    }
    catch (string s) {
        cerr << s << endl;
    }
    return 0;
}